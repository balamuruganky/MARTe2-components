/**
 * @file UEIAI217_803.cpp
 * @brief Source file for class UEIAI217_803
 * @date 09/02/2023
 * @author Xavier Ruche
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class UEIAI217_803 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "UEIAI217_803.h"
#include "UEIDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

UEIAI217_803::UEIAI217_803() : Object() {
    deviceId = 0u;
    samplingFrequency = 0.0;
    hardwareCorrespondence = false;
    assignedToMap = false;
}

UEIAI217_803::~UEIAI217_803(){
    
    //Destroy the FIRBanks strcutures
}

bool UEIAI217_803::Initialise(StructuredDataI &data){ 
    bool ok = Object::Initialise(data);
    //Initialise helper for retriecing the input parameters
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Check the name of the Object
    if (ok) {
        name = data.GetName();
        ok = (name.Size() != 0ull);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                "Could not retrieve DAQ Master Object Name.");
        }
    }
    //Read and validate the Devn parameter (Devicen identifier)
    if (ok){
        ok = helper.Read("Devn", deviceId);
        if (ok){
            ok = deviceId < MAX_IO_SLOTS; 
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                "Devn out of allowed range for device %s. Maximum Devn : %d.", name.Buffer(), MAX_IO_SLOTS-1);
            }
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
            "Could not retrive Devn parameter for device %s.", name.Buffer());
        }
    }
    //Read and validate sampling frequency
    if (ok){
        ok = helper.Read("Sampling_ferquency", samplingFrequency);
        if (ok){
            REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
            "Sampling frequency set to %f for device %s.", samplingFrequency, name.Buffer());
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
            "Could not retrive Sampling_frequency parameter for device %s.", name.Buffer());
        }
    }
    //Read and validate the gains
    if(ok){
        uint32 numberOfGains;
        bool found = helper.ReadArray("Gains", gains, numberOfGains);
        ok = (numberOfGains <= CHANNEL_NUMBER);
        if (ok){
            if (numberOfGains == 0u || !found){
                gains = (uint16*)malloc(2*CHANNEL_NUMBER);
                for (uint8 i = 0u; i < CHANNEL_NUMBER; i++){
                    gains[i] = 1u;
                }
                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                "No gain information was supplied for channels at device %s. All channels configured with gain 1.", name.Buffer());
            }else{
                ok = (numberOfGains == CHANNEL_NUMBER);
                if (ok){
                    for (uint8 i = 0u; i < CHANNEL_NUMBER && ok; i++){
                        if (!(  gains[i] == 1 || gains[i] == 2 ||
                                gains[i] == 4 || gains[i] == 8 ||
                                gains[i] == 16|| gains[i] == 32||
                                gains[i] == 64))
                        {
                            ok = false;
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                            "Invalid gain supplied for device %s. (Gain in position %d)", name.Buffer(), i);                               
                        }
                    }   
                }else{
                    REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                    "Incomplete gains information for device %s.", name.Buffer());
                }
            }
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
            "Too many gains defined for channels at device %s. Maximum channel number = %d.", name.Buffer(), CHANNEL_NUMBER);
        }
    }
    //Read and choose ADC Mode (DEFAULT mode by default)
    if (ok){
        StreamString ADC_mode_string;
        helper.Read("ADC_mode", ADC_mode_string, "DEFAULT");
        if (ADC_mode_string == StreamString("ENHANCED")){
            ADCMode = DQ_AI217_SET_ADC_ENH;
        }else if (ADC_mode_string == StreamString("DEFAULT")){
            ADCMode = DQ_AI217_SET_ADC_DEFAULT;
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
            "Invalid ADC_mode provided for device %s.", name.Buffer());
            ok = false;  
        }
    }
    //Read and validate FIR taps (optional parameters)
    if (ok){
        bool requestedFilters = data.MoveRelative("Filters");
        if (!requestedFilters){
            REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
            "No FIR filters specified for device %s, banks A,B,C and D deactivated.", name.Buffer());
            //Set the FIR bank configuration structures to not enabled
            for (uint32 i = 0; i < FIR_BANK_NUMBER; i++){
                FIRBanks[i].bankState = BANK_NOT_ENABLED;
            }      
        }else{
            //Filters structure is found, go through the different banks settings
            StreamString banks [FIR_BANK_NUMBER] = {"A","B","C","D"};
            //loop over the four different FIR banks structure
            for (uint32 i = 0; i < FIR_BANK_NUMBER && ok; i++){
                //Loop to setup a single FIR bank (A,B,C or D)
                bool bankDefined = data.MoveRelative(banks[i].Buffer());
                if (bankDefined){
                    //This bank is defined, search the parameters to configure the bank
                    //Check wether we wish to configure it as default or custom configuration
                    bool defaultConfiguration = data.Read("Default_filter", FIRBanks[i].defaultBankSetting);
                    bool customConfiguration = helper.ReadArray("Taps", FIRBanks[i].taps, FIRBanks[i].nTaps);
                    ok = (defaultConfiguration != customConfiguration);
                    if (ok){
                        if (customConfiguration){
                            FIRBanks[i].bankState = CUSTOM_FIR_SETTING;
                            ok = FIRBanks[i].nTaps <= MAX_FIR_TAPS_803;
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                                "Invalid FIR bank %s configuration for device %s, the number of taps must be equal or lower than %d.", banks[i].Buffer() ,name.Buffer(), MAX_FIR_TAPS_803);     
                            }else{
                                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                                "FIR bank %s for device %s will be enabled as custom FIR filter.", banks[i].Buffer() ,name.Buffer());
                            }
                        }else if (defaultConfiguration){
                            FIRBanks[i].bankState = DEFAULT_FIR_SETTING;
                            ok = (FIRBanks[i].defaultBankSetting <= 9);     //Possible Default FIR settings as defined in documentation for this layer
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                                "Invalid FIR bank %s configuration for device %s, the default FIR configuration index must be in 0-9 range.", banks[i].Buffer() ,name.Buffer());     
                            }else{
                                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                                "FIR bank %s for device %s will be enabled as default FIR filter (setting %d).", banks[i].Buffer() ,name.Buffer(), FIRBanks[i].defaultBankSetting);
                            }                      
                        }
                    }else{
                       REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                        "Invalid FIR bank %s configuration for device %s, a bank must contain either a custom or default configuration.", banks[i].Buffer() ,name.Buffer()); 
                    }
                    ok &= data.MoveToAncestor(1u);
                }else{
                    FIRBanks[i].bankState = BANK_NOT_ENABLED;
                    REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                    "FIR bank %s not defined for device %s, this bank will be disabled.", banks[i].Buffer() ,name.Buffer());
                }
            }
            //Return to the device node
            ok &= data.MoveToAncestor(1u);
        }
    }
    return ok;
}

bool UEIAI217_803::CheckChannelAndDirection(uint32 channelNumber, uint8 direction){
    //For AI-217-803 all the channels (0 to 15 as only 16 channels are available) are inputs
    bool validChannel = false;
    //Channel only valid if it is configured as an output
    if (direction == OUTPUT_CHANNEL){
        //Channel only valid if within range [0,15]
        if (channelNumber>=0u && channelNumber<16){
            validChannel = true;
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "DAQMasterObject::Initialise - "
            "AI-217-803 layer only accepts channels 0 to 15.");
        }
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "DAQMasterObject::Initialise - "
        "AI-217-803 layer only accepts output signals!");
    }
    return validChannel;
}

bool UEIAI217_803::ConfigureDevice(int32 DAQ_handle){
    bool ok = true;
    //Configure all the channels in the layer at the same time (for now no custom channel configuration is allowed)
    /*ok = (DqAdv217SetCfgLayer(DAQ_handle, deviceId, DQ_AI217_SETCFG_ALL_CHAN, DQ_SET_CFG_LAYER_ADC, ADCMode) >= 0);
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "DAQMasterObject::Initialise - "
        "Unable to configure ADC CfgLayer for Device %s.", name.Buffer());
    }
    if (ok){

    }*/
    //Configure the FIR filters as required
    if (ok){
        //As described in the API, set all the FIR banks to default state prior to custom initialisation
        ok = (DqAdv217SetFIR(DAQ_handle, deviceId, 0x0f, 0x08,0,0,NULL,NULL) >= 0);
        //All the FIR banks are initialised to their default value and enabled, no need to re enable them
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "UEIAI217_803::ConfigureDevice - "
            "Unable to set default FIR parameters for device %s.", name.Buffer());  
        }
    }
    if (ok){
        //Set custom configuration for ach FIR filter
        StreamString banks [FIR_BANK_NUMBER] = {"A","B","C","D"};
        for (uint8 i = 0u; i < FIR_BANK_NUMBER && ok; i++){
            int32 bank = (0x01<<i);
            int32 action = 0;
            int32 decrat = 0;
            int32 tapsize = 0;
            float64* taps = NULL;
            switch(FIRBanks[i].bankState){
                case BANK_NOT_ENABLED:
                    //Disable the FIR filter
                    action = 0x00;                  //FIR Disable flag
                break;
                case DEFAULT_FIR_SETTING:
                    //Change the FIR filter setting to a new setting
                    action = 0x01 | 0x80;           // FIR Enable flag + Set Index flag
                    tapsize = FIRBanks[i].defaultBankSetting;
                break;
                case CUSTOM_FIR_SETTING:
                    action = 0x01 | 0x04;           // FIR Enable flag + Load Coeffs flag
                    tapsize = FIRBanks[i].nTaps;
                    taps = FIRBanks[i].taps;
                break;
                default:
                    ok = false;
                break;
            }
            int32 totalFilterResult;
            ok = (DqAdv217SetFIR(DAQ_handle, deviceId, bank, action, decrat, tapsize,&totalFilterResult,  taps) >= 0);
            if (!ok){
                REPORT_ERROR(ErrorManagement::CommunicationError, "UEIAI217_803::ConfigureDevice - "
                "Error trying to program FIR configuration for bank %s for device %s.", banks[i].Buffer() ,name.Buffer()); 
            }
            if (ok){
                if (totalFilterResult != 8388608){
                    REPORT_ERROR(ErrorManagement::Warning, "UEIAI217_803::ConfigureDevice - "
                    "Inaccurate return value for bank %s for device %s (expected 8388608, got %d).",banks[i].Buffer() ,name.Buffer(),totalFilterResult);  
                }
            }
        }
    }
    return ok;
}


uint8 UEIAI217_803::GetDevN(){
    return deviceId;
}

int32 UEIAI217_803::GetModel(){
    return 535; //217 in hexadecimal
}
void UEIAI217_803::SetHardwareCorrespondence(){
    hardwareCorrespondence = true;
}

bool UEIAI217_803::GetHardwareCorrespondence(){
    return hardwareCorrespondence;
}

void UEIAI217_803::SetMapAssignment(){
    assignedToMap = true;
}

bool UEIAI217_803::GetMapAssignment(){
    return assignedToMap;
}

uint32 UEIAI217_803::GetDeviceChannels(){
    return CHANNEL_NUMBER;
}
bool UEIAI217_803::ConfigureChannel(uint32* channel){
    uint32 gain = 0u;
    switch(gains[*channel]){
        case 1:
            gain = DQ_AI217_GAIN_1;
            break;
        case 2:
            gain = DQ_AI217_GAIN_2;
            break;
        case 4:
            gain = DQ_AI217_GAIN_4;
            break;
        case 8:
            gain = DQ_AI217_GAIN_8;
            break;
        case 16:
            gain = DQ_AI217_GAIN_16;
            break;
        case 32:
            gain = DQ_AI217_GAIN_32;
            break;
        case 64:
            gain = DQ_AI217_GAIN_64;
            break;
        default:
            return false;
            break;
    }
    *channel = (*channel | DQ_LNCL_GAIN(gain) | DQ_LNCL_DIFF); //AI-217 can only operate in differential mode
    return true;
}

bool UEIAI217_803::AcceptedSignalType(TypeDescriptor signalType){
    //Check the datatypes this device can output. Return true for the datatypes accepted as a valid output of this device
    bool accepted = false;
    /*if (signalType == UnsignedInteger32Bit || signalType == Float64Bit){
        accepted = true;
    }*/
    if (signalType == Float64Bit){
        accepted = true;
    }
    return accepted;
}

float UEIAI217_803::GetSamplingFrequency(){
    return samplingFrequency;
}
CLASS_REGISTER(UEIAI217_803, "1.0")
}