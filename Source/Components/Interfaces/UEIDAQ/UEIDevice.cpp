/**
 * @file UEIDevice.cpp
 * @brief Source file for class UEIDevice
 * @date 22/03/2023
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
 * the class UEIDevice (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIDevice.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIDevice::UEIDevice() : Object() {
    //Initialise the object with "safe" values. This must be overwritten during object initialisation
    //otherwise their faulty value will prompt an error
    name = "";                  //Set to no name 
    deviceId = 0xFF;            //Set the devn to an invalid value in IOM for safety                 
    samplingFrequency = 0.0;    //Set sampling frequency to invalid value (0.0)
    hardwareCorrespondence = false; //Set device as not correspondant to IOM device
    assignedToMap = false;          //Set device as not assigned to any map
}

UEIDevice::~UEIDevice(){
}


bool UEIDevice::Initialise(StructuredDataI &data){
    //Base implementation of general UEIDAQ device parameter checks.
    bool ok = Object::Initialise(data);
    //Initialise helper for retriecing the input parameters
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Check the name of the Object
    if (ok) {
        name = data.GetName();
        ok = (name.Size() != 0ull);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve UEIDevice Name.");
        }
    }
    //Read and validate the Devn parameter (Devicen identifier)
    if (ok){
        ok = helper.Read("Devn", deviceId);
        if (ok){
            ok = deviceId < MAX_IO_SLOTS; 
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Devn out of allowed range for device %s. Maximum Devn : %d.", name.Buffer(), MAX_IO_SLOTS-1);
            }
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrive Devn parameter for device %s.", name.Buffer());
        }
    }
    //Read and validate sampling frequency
    if (ok){
        ok = helper.Read("SamplingFrequency", samplingFrequency);
        if (ok){
            REPORT_ERROR(ErrorManagement::Information, "Sampling frequency set to %f for device %s.", samplingFrequency, name.Buffer());
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrive SamplingFrequency parameter for device %s.", name.Buffer());
        }
    }
    return ok;
}

int32 UEIDevice::GetModel(){
    //Base implmentation returns false by default, this function must be implemented by child class.
    return 0x00;
}

IOLayerType UEIDevice::GetType(){
    //Base implmentation returns false by default, this function must be implemented by child class.
    return NO_LAYER;
}

uint8 UEIDevice::GetSampleSize(){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return 0u;
}

uint32 UEIDevice::GetDeviceChannels(){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return 0u;
}

bool UEIDevice::CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}

bool UEIDevice::CheckChannelListAndDirection(uint32* channelList, uint32 listLength, SignalDirection direction){
    bool ok = (channelList != NULL_PTR(uint32*));
    if (ok){
        for (uint32 i = 0; i < listLength && ok; i++){
            ok &= CheckChannelAndDirection(channelList[i], direction);
        }
    }
    return ok;
}

bool UEIDevice::ConfigureChannel(uint32 channelNumber, uint32 &channelConfiguration){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}
bool UEIDevice::ConfigureChannel(uint32 channelNumber, int32 &channelConfiguration){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}

bool UEIDevice::ConfigureDevice(int32 DAQ_handle){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}

bool UEIDevice::AcceptedSignalType(TypeDescriptor signalType){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}

bool UEIDevice::GetChannelStatus(int32 DAQ_handle, uint32* errorBitField, uint32* pgaStatusArray){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}

bool UEIDevice::ScaleSignal(uint32 channelNumber, uint32 listLength, void* rawData, void* scaledData, TypeDescriptor outputType){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}

bool UEIDevice::ScaleSignal(uint32 channelNumber, uint32 listLength, UEIBufferPointer rawData, void* scaledData, TypeDescriptor outputType){
    //Base implmentation returns false by default, this function must be implemented by child class.    
    return false;
}

uint8 UEIDevice::GetDevN(){
    return deviceId;
}

bool UEIDevice::SetHardwareCorrespondence(){
    bool ok = false;
    //If the hardwareCorrespondence flag is not set, set it and return ok, not ok otherwise
    if (!hardwareCorrespondence){
        ok = true;
        hardwareCorrespondence = true;
    }
    return ok;
}
    
bool UEIDevice::GetHardwareCorrespondence(){
    return hardwareCorrespondence;
}

bool UEIDevice::SetMapAssignment(){
    bool ok = false;
    //If the assignedToMap flag is not set, set it and return ok, not ok otherwise
    if (!assignedToMap){
        ok = true;
        assignedToMap = true;
    }
    return ok;
}

bool UEIDevice::GetMapAssignment(){
    return assignedToMap;
}

float UEIDevice::GetSamplingFrequency(){
    return samplingFrequency;
}

char8* UEIDevice::GetName(){
    return (char8*) name.Buffer();
}

CLASS_REGISTER(UEIDevice, "1.0")
}