/**
 * @file EmbeddedService.h
 * @brief Header file for class EmbeddedService
 * @date Sep 1, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class EmbeddedService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDSERVICE_H_
#define EMBEDDEDSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BitRange.h"
#include "BitBoolean.h"
#include "ErrorType.h"
#include "ExecutionInfo.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Interface to a family of objects that allow interfacing a class method to a thread or a pool of threads.
 * @details The method interface is specified in EmbeddedServiceI::MethodBinderT<class>::MethodPointer.
 * It returns an ErrorType and has one parameter of type EmbeddedServiceI::ExecutionInfo&
 */
class EmbeddedServiceI {
public:

    /**
     * Interface to the Method Binders.
     * Not usable
     */
    class MethodBinderI {

    public:
        /**
         * TODO
         */
        MethodBinderI() {
        }
        ;

        /**
         * TODO
         */
        virtual ~MethodBinderI() {
        }
        ;

        /**
         *
         */
        virtual ErrorManagement::ErrorType Execute(ExecutionInfo info)=0;

    };

    /**
     * TODO
     */
    template<typename className>
    class MethodBinderT: public MethodBinderI {

    public:

        /**
         * @brief Type definition for the method pointer prototype
         */
        typedef ErrorManagement::ErrorType (className::*MethodPointer)(ExecutionInfo &info);

        /**
         * TODO
         */
        MethodBinderT(className &o,
                      MethodPointer f);

        /**
         * TODO
         */
        virtual ~MethodBinderT();

        /**
         * TODO
         */
        virtual ErrorManagement::ErrorType Execute(ExecutionInfo info);

    private:

        /**
         * TODO
         */
        className &object;

        /**
         * TODO
         */
        MethodPointer function;
    };

    /**
     * allocated by the user.
     * memory managed by object
     */
    EmbeddedServiceI(MethodBinderI &binder) :
            method(binder) {
    }

    /**
     * allocated by the user.
     * memory managed by object
     */
    template<typename className>
    EmbeddedServiceI(MethodBinderT<className> &binder) :
            method(binder) {
    }

    /**
     *
     */
    virtual ~EmbeddedServiceI() {
    }

    /**
     * TODO
     * same as object interface
     */
    virtual bool Initialise(StructuredDataI &data)=0;

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Start()=0;

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Stop()=0;

protected:
    /**
     * TODO
     */
    inline ErrorManagement::ErrorType Execute(ExecutionInfo information) {
        return method.Execute(information);
    }

    MethodBinderI &method;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename className>
EmbeddedServiceI::MethodBinderT<className>::MethodBinderT(className &o,
                                                          MethodBinderT<className>::MethodPointer f) :
        object(o),
        function(f) {
}
;

template<typename className>
EmbeddedServiceI::MethodBinderT<className>::~MethodBinderT() {
}

template<typename className>
ErrorManagement::ErrorType EmbeddedServiceI::MethodBinderT<className>::Execute(ExecutionInfo info) {
    return (object.*function)(info);
}

}

#endif /* EMBEDDEDSERVICE_H_ */

