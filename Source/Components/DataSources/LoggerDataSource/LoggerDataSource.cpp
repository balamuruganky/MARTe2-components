/**
 * @file LoggerDataSource.cpp
 * @brief Source file for class LoggerDataSource
 * @date Nov 9, 2016 TODO Verify the value and format of the date
 * @author aneto TODO Verify the name and format of the author
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
 * the class LoggerDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LoggerBroker.h"
#include "LoggerDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

LoggerDataSource::LoggerDataSource() :
        DataSourceI() {
}

LoggerDataSource::~LoggerDataSource() {
}

bool LoggerDataSource::Synchronise() {
    return false;
}

bool LoggerDataSource::AllocateMemory() {
    return true;
}

uint32 LoggerDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool LoggerDataSource::GetSignalMemoryBuffer(const uint32 signalIdx,
                                             const uint32 bufferIdx,
                                             void*& signalAddress) {
    return true;
}

const char8* LoggerDataSource::GetBrokerName(StructuredDataI& data,
                                             const SignalDirection direction) {
    return "LoggerBroker";
}

bool LoggerDataSource::GetInputBrokers(ReferenceContainer& inputBrokers,
                                       const char8* const functionName,
                                       void* const gamMemPtr) {
    return false;
}

bool LoggerDataSource::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                        const char8* const functionName,
                                        void* const gamMemPtr) {
    ReferenceT<BrokerI> broker("LoggerBroker");
    bool ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = outputBrokers.Insert(broker);
    }
    return ok;
}

bool LoggerDataSource::PrepareNextState(const char8 * const currentStateName,
                                        const char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(LoggerDataSource, "1.0")
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

