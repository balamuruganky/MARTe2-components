/**
 * @file EventSem.h
 * @brief Header file for class EventSem
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class EventSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTSEM_H_
#define EVENTSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "Errors.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,EventSemOS.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This semaphore is used mostly for thread synchronization.
 *
 * @details After being reset the semaphore is ready to Wait.
 * Once waiting, until a Post arrives all the threads will wait on
 * the semaphore. After the post all tasks are allowed to proceed.
 * A Reset is then required to use the semaphore again.
 *
 * The Lock functions uses an ErrorType object defined in GeneralDefinition.h that could be used by the user to
 * know if an eventual lock error happened because of the timeout or for other reasons.
 *
 * Most of the implementation is delegated to the EventSemOS.h file
 * which is different for each operating system and provides non-portable
 * functions to implement this kind of semaphore.
 */

class EventSem : public EventSemOS {
public:
    /**
     * No operation.
     */
    EventSem();

    /**
     * No operation.
     */
    virtual ~EventSem();

    /**
     * @copydetails IEventSem::Wait
     * @details Provides default parameter to wait with an infinite timeout.
     */
    bool Wait(FlagsType &error, const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @copydetails IEventSem::ResetWait
     * @details Provides default parameter to reset and wait with an infinite timeout.
     */
    bool ResetWait(FlagsType &error, const TimeoutType &timeout = TTInfiniteWait);

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEM_H_ */

