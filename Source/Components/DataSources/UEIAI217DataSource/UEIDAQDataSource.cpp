/**
 * @file UEIDAQDataSource.cpp
 * @brief Source file for class UEIDAQDataSource
 * @date 07/02/2023
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
 * the class UEIDAQDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#include "UEIDAQDataSource.h"

namespace MARTe {

UEIDAQDataSource::UEIDAQDataSource() : MemoryDataSourceI() {
        poll_sleep_period = 100;
        deviceName = StreamString("");
        mapName = StreamString("");

}

UEIDAQDataSource::~UEIDAQDataSource() {
}

bool UEIDAQDataSource::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Get the name of the DataSource
    name = data.GetName();
    ok = data.Read("Device", deviceName);
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "No device specified for UEIDAQDataSource %s", name.Buffer());        
    }
    if (ok){
        ok = data.Read("Map", mapName);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "No map specified for UEIDAQDataSource %s", name.Buffer());        
        }
    }
    //Retrieve PollSleepPeriod (optional)
    if (ok){
        helper.Read("PollSleepPeriod", poll_sleep_period, 100);
    }
    //Retrieve the reference to the UEIDAQ device
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok){
        device = ord->Find(deviceName.Buffer());
        ok = device.IsValid();
        if (ok){
           REPORT_ERROR(ErrorManagement::Information, "Device %s found (at UEIDAQDataSource %s)", deviceName.Buffer(), name.Buffer());
        }else{
           REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find device %s (at UEIDAQDataSource %s)", deviceName.Buffer(), name.Buffer());
        }
    }
    //Retrieve the reference to the selected map within the UEIDAQ device
    if (ok){
        StreamString mapPath = StreamString("");
        mapPath += deviceName;
        mapPath += ".Maps.";
        mapPath += mapName;
        map = ord->Find(mapPath.Buffer());
        ok = map.IsValid();
        if (ok){
            REPORT_ERROR(ErrorManagement::Information, "Map %s found (at UEIDAQDataSource %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find map %s (at UEIDAQDataSource %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }
    }
    //Only one DataSource can be associated with a map, check that the map is not being used by another DataSource
    if (ok){
        ok = (!map->RegisterDS(OutputSignals));     //Check if the Map has already been assigned to a DS (and assign it if not)
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "The map %s requested by DataSource %s is already assigned to another DataSource!", mapName.Buffer(), name.Buffer());
        }
    }
    return ok;
}

bool UEIDAQDataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    //Check the number of signals into the datasource
    if (ok){
        ok = (numberOfSignals > 0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "At least one signal must be provided for DataSource %s", name.Buffer());
        }
        //The number of signals cannot be larger than the number of physical channels defined in the map
        if (ok){
            uint32 nChannels;
            ok = map->GetNumberOfChannels(OUTPUT_CHANNEL, nChannels); //TODO, check for Input channels
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not get number of Input channels for Map %s", map->GetName());
            }else{
                ok = (numberOfSignals == nChannels+1);  //+1 as we must supply a Timestamp signal
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Number of signals in DataSource %s must match signal number in its map (%s)", name.Buffer(), map->GetName());
                }
            }
        }
    }
    //First Signal from this DataSource must be a Timestamp uint64 signal
    if (ok){
        StreamString timestampSignalName;
        ok = (GetSignalName(0, timestampSignalName));
        if (ok){
            ok = (timestampSignalName == StreamString("Timestamp"));
            ok &= (GetSignalType(0) == UnsignedInteger64Bit);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Signal 0 for this DataSource must be a \"Timestamp\" uint64 signal");
            }
        }
    }
    //All signals within a DataSource must be of the same type (simplification condition). Therefore, no mismatch in the 
    // type of signals is allowed
    if (ok){
        //Get the type of the first signal (we know we do have at least 1 signal) (skip timestamp signal)
        signalType = GetSignalType(1u);
        ok = (signalType == Float64Bit); //TODO, for now we only support scaled float64 data, we'll update on this later
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Signal type inconsistency in DataSource %s (all signals must be float64 bit for now)", name.Buffer());        
        }
    }
    //Check that all the signals are of the same type
    if(ok){
        //Traverse all the signals defined for this DataSource
        for (uint32 i = 1u; i < numberOfSignals && ok; i++){
            //Check that all the signals do have the same type as the selected one (the first signal)
            ok = (signalType == GetSignalType(i));
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Signal type inconsistency in DataSource %s (all signals must be of same type)", name.Buffer());
        }
    }
    //Check that all the devices of the map assigned to this DS support the selected signal Type
    if (ok){
        //Check if the map does support the signal type specified
        ok = map->IsSignalAllowed(signalType,OUTPUT_CHANNEL); //TODO as the current DS only supports OUTPUT signals
        if(!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Signal type provided for DataSource %s is incompatible with specified map", name.Buffer());
        }
    }
    //Check that all the signals have NumberOfElements 1 (1 signal = 1 physical channel) and register the number of signals
    if (ok){
        for (uint32 i = 0; i < numberOfSignals && ok; i++){
            uint32 nOfElements = 0u;
            ok = (GetSignalNumberOfElements(i, nOfElements));
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve number of elements for signal %d in DataSource %s", i, name.Buffer());
            }
            if (ok){
                ok = (nOfElements == 1u);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Signals in DataSource %s must have only 1 element", name.Buffer());
                }
            }
        }
    }
    //DESIGN DECISION : One DataSource can only interact with a single GAM, multiple gams would cause the map to re-refresh on the same cycle
    //the operation of the DS would be unpredictable. Therefore only one GAM is supposed to interact with this DS
    if (ok){
        ok = (GetNumberOfFunctions() <= 1u);  //0 GAMS is also ok, you can leave the DataSource "unconnected"
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "This DataSource can only interact to a single GAM, currently %d GAMs are configured to interface this DataSource", GetNumberOfFunctions());
        }
    }
    //Check NumberOfSamples for all the signals to be equal and 1 if the Map is XDMap or equal for all signals in XVMap.
    if (ok){
        switch (map->GetType()){
            case RTVMAP:
            {
                //if it is an RtDMap check that the sample number is equal for all the signals
                bool firstSignalVisited = false;
                for (uint32 i = 0; i < numberOfSignals && ok; i++){
                    uint32 samples = 0;
                    bool signalFound = (GetFunctionSignalSamples(InputSignals,0,i,samples));
                    if (signalFound && !firstSignalVisited){
                        nSamples = samples;
                        ok = (samples > 0);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Sample number for signals in DataSource %s must be at least 1", name.Buffer());
                            
                        }
                        firstSignalVisited = true;
                    }
                    if (signalFound){
                        ok = (samples == nSamples);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "All signals for DataSource %s must have the same number of samples", name.Buffer());
                        }
                    }
                }
            }
            break;
            case RTDMAP:
            {
                //if it is an RtDMap check that the sample number is 1, other is not supported
                for (uint32 i = 0; i < numberOfSignals && ok; i++){
                    uint32 samples = 0;
                    bool signalFound = (GetFunctionSignalSamples(InputSignals,0,i,samples));
                    if (signalFound){
                        ok = (samples == 1u);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Datasource %s only supports Sample number equal to 1 for RtDMap", name.Buffer());
                        }
                    }
                }
            }
            break;
        }
    }
    //TODO more checks
    return ok;
}

bool UEIDAQDataSource::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples){
    return true;
}

bool UEIDAQDataSource::Synchronise() {
    //Start to poll for next packet to the Map. The memory access is handled by the Map Container
/*    bool ok = false;
    uint32 counter = 0;
    while(!ok){
        ok = (map->PollForNewPacket(reinterpret_cast<float64*>(memory), nOutputChannels));
        if (!ok){
            counter ++;
            Sleep::MSec(poll_sleep_period);    //To change
        }
        if (counter > 100) break;
    }
    */
    //printf("NumberOfBuffers : %d\n", GetNumberOfMemoryBuffers());
    //void* address;
    //bool ok = (GetSignalMemoryBuffer(0,0,address));
    //printf("Ok : %d\n", ok);
    //float64* mypointer = reinterpret_cast<float64*>(address);
    Sleep::MSec(500);
    (reinterpret_cast<uint64*>(memory))[0] = 101u;
    (reinterpret_cast<uint64*>(memory))[1] = 102u;
    (reinterpret_cast<uint64*>(memory))[2] = 103u;
    (reinterpret_cast<uint64*>(memory))[3] = 104u;
    (reinterpret_cast<uint64*>(memory))[4] = 105u;
    (reinterpret_cast<float64*>(memory))[5] = 2.01;	
    (reinterpret_cast<float64*>(memory))[6] = 3.01;
    (reinterpret_cast<float64*>(memory))[7] = 4.01;
    (reinterpret_cast<float64*>(memory))[8] = 5.01;
    (reinterpret_cast<float64*>(memory))[9] = 6.01;
    (reinterpret_cast<float64*>(memory))[10] = 7.01;
    printf("totalMemorySize : %d\n", totalMemorySize);		
    REPORT_ERROR(ErrorManagement::Information,"Size of the memory area : %d", (uint32)(totalMemorySize/sizeof(float64)));
    return true;
}

bool UEIDAQDataSource::AllocateMemory(){
    uint32 nOfSignals = GetNumberOfSignals();
    bool ret = (memory == NULL_PTR(uint8 *));
    if (ret) {
        if (nOfSignals > 0u) {
            signalOffsets = new uint32[nOfSignals];
            ret = (signalOffsets != NULL_PTR(uint32*));
            if (ret) {
                signalSize = new uint32[nOfSignals];
                ret = (signalSize != NULL_PTR(uint32*));
            }
        }
    }

    stateMemorySize = 0u;
    for (uint32 s = 0u; (s < nOfSignals) && (ret); s++) {
        uint32 thisSignalMemorySize;
        ret = GetSignalByteSize(s, thisSignalMemorySize);

        if (ret) {
            if (signalOffsets != NULL_PTR(uint32 *)) {
                signalOffsets[s] = stateMemorySize;
            }
        }
        if (ret) {
            ret = (thisSignalMemorySize > 0u);
        }
        if (ret) {
            stateMemorySize += (thisSignalMemorySize * numberOfBuffers * nSamples);
            signalSize[s] = thisSignalMemorySize * nSamples;
        }
    }
    uint32 numberOfStateBuffers = GetNumberOfStatefulMemoryBuffers();
    if (ret) {
        ret = (numberOfStateBuffers > 0u);
    }
    if (ret) {
        totalMemorySize = stateMemorySize * numberOfStateBuffers;
        if (memoryHeap != NULL_PTR(HeapI *)) {
            memory = reinterpret_cast<uint8 *>(memoryHeap->Malloc(totalMemorySize));
        }
        ret = MemoryOperationsHelper::Set(memory, '\0', totalMemorySize);
    }
    return ret;
}


const char8* UEIDAQDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    //The Datasource is synchronous to the Reception of data from UEIDAQ device
    if (direction == InputSignals){
        //if (map->GetType() == RTDMAP){
            return "MemoryMapSynchronisedInputBroker";
        //}else if (map->GetType() == RTVMAP){
         //   return "MemoryMapSynchronisedMultiBufferInputBroker";
        //}else{
            //Only input signals are supported for this DataSource
           // REPORT_ERROR(ErrorManagement::InternalSetupError, "UEIDAQDataSource::GetBrokerName - DataSource %s only supports input signals", name.Buffer());
            //return "";
        //}
    }else{
        //Only input signals are supported for this DataSource
        REPORT_ERROR(ErrorManagement::InternalSetupError, "UEIDAQDataSource::GetBrokerName - DataSource %s only supports input signals", name.Buffer());
        return "";
    }
}

bool UEIDAQDataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    return true;
}

CLASS_REGISTER(UEIDAQDataSource, "1.0")
}
