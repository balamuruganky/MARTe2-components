/**
 * @file UEIWritter.h
 * @brief Header file for class UEIWritter
 * @date 20/03/2023
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

 * @details This header file contains the declaration of the class UEIWritter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIWritter_H_
#define UEIWritter_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "MemoryDataSourceI.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "StreamString.h"
#include "ObjectRegistryDatabase.h"

#include "UEIMasterObject.h"
#include "UEIMapContainer.h"
#include "UEIDataSourceI.h"



/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
* @brief DataSource implementation to be used with the UEIDAQ interface and UEIDAQ devices within MARTe.
* @details This DataSource implements the needed methods and mechanics to retrieve data from a/various UEIDAQ hardware layer/s directly within MARTe.
* The DataSource needs to be linked to an existing UEIDAQ interface within the same application describing the desired configuration for the device
* and its different layers.
*
* The DataSource configuration must provide the Device paramter, which contains a string referencing the name of the UEIMasterObject object instantiated
* within the application which will serve as the interface to the UEIDAQ device. The DataSource must retrieve the data through a Map object (see UEIRtDMap and
* UEIRtVMap) which defines the acquisition method and parameters. The configuration must also contain a Map parameter with a string referencing the name of the
* Map object associated to the acquisition scheme.
*
* Since the available methods to retrieve data from the IOM are based on periodical polling requests to the IOM, the DataSource performs such requests
* within a loop upon call of the Synchronise method. These system calls must happen in a short enough period to ensure the FIFOs on the hardware layers
* do not overflow if operating on a VMap mode. Since the refresh requests happen in a loop, the period is defined by an sleep within the loop, which can be
* configured through the PollSleepPeriod parameter (in ms). This parameter is optional and assumed 0 if not provided, nonetheless, since the UEIDAQ models
* availalble are fitted with single core cpus running Linux, the use of loops with no sleep (default configuration) mean the system becomes starved of resources
* and the operating system may block, therefore adjustment of this parameter is recommended.
*
* The DataSource can only be used as an input DataSource providing information to MARTe (output signals to hardware are not yet available).
* The DataSource must have the following input signals if configured as input DataSource. All the signals except the first signal must have the same ammount of
* samples and NumberOfElements and NumberOfDimensions == 1. For RtDMaps only 1 sample is allowed for each signal, while for RtVMap a higher than 1 sample
* is allowed for the signals.
* - Status signal : uint32 signal with a single sample, providing status of the DataSource. the first 16 bits of the signal present a counter of the number
* of errors detected during new packet acquisition (number of FIFO overflows or general errors). The 16th bit signals wether the data provided presents a gap
* from the previous sampled data (1 == data gap detected), the 17th bit signals if a FIFO overflow has been detected during the last packet acquisition and the 18th
* bit signals if another kind of error (general error) was detected during last packet retrieving.
* - Timestamp : uint64 signal with the selected ammount of samples (same for all signals except Status) providing the local timestamp of the UEIDAQ hardware 
* (relative to the map start time).
* - Signal0 to SignalN : Signal with the selected ammount of samples providing the read value of such channel in the map (signals are provided in the 
* same order as configured within the map object associated to the DataSource). The acceptance or not of a given signal type depends only on the device channel
* configured; in order to give the user the maximum flexibility posible each signal is checked for type validity with the associated physical channel.
*
* See the example below for a configuration of the DataSource:
* <pre>
*   +UEIWritter1 = {
*        Class = UEIDataSourceI::UEIWritter
*        Device = "UEIDevice"
*        Map = "Map1"
*        PollSleepPeriod = 1
*        Signals = {
*            Status = {
*                Type = uint32
*            }
*            Timestamp = {
*                Type = uint64
*            }
*            Output0 = {
*                Type = float32
*            }
*            Output1 = {
*                Type = float64
*            }
*        }
*    }
* </pre>
*
*/
class UEIWritter : public UEIDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    UEIWritter();

    /**
     * @brief Destructor. Stops the Embedded thread which reads from the CRIOUARTSerial.
     */
    virtual ~UEIWritter();

    /**
     * @brief Verifies that the configuration described above is correctly set.
     * @return true if the configuration respects all the rules described in the class description.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see Checks that the signal properties are as described in the class description.
     * @return true if the signal properties respect all the rules described in the class description.
     */
    bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @see DataSourceI::TerminateInputCopy
     */
    virtual bool TerminateInputCopy(const uint32 signalIdx,  const uint32 offset, const uint32 numberOfSamples);

    /**
     * @brief Assigns the correct broker for the connected signals. Checks that only input signals are provided, this
     * datasource does only support input signals.
     * @return "MemoryMapSychronisedInputBroker" if the signal is an input signal.
     */
    const char8* GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief Sycnhronise method for the DataSource.
     * @details This method performs the synchronisation to the latest IOM data. The method calls the PollForNewPacket method of the assigned Map
     * to retrieve new data from the IOM.
     * @return true.
     */
    bool Synchronise();

    /**
     * @see DataSourceI::PrepareNextState
     */
    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName);

    bool AllocateMemory();
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIWritter_H_ */
