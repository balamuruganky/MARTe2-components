/**
 * @file ClassMethodCallerT.h
 * @brief Header file for class ClassMethodCallerT
 * @date 13/06/2016
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * argTypeou may not use this work except in compliance with the Licence.
 * argTypeou may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANargType KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.
 *
 * @details This header file contains the declaration of the class ClassMethodCallerT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODCALLERT_H_
#define CLASSMETHODCALLERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassMethodCaller.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one argument.
 *
 * @details This specialisation of the template generates a pair of "Call"
 * methods for target methods which accept exactly one argument. One is the
 * version for by reference arguments and the other for by copy arguments.
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the argument to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, ReferenceContainer&> mc(&ClassA::MethodX);
 * + ClassMethodCallerT<ClassA, int&> mc(&ClassA::MethodY);
 *
 * @tparam className is the class owning the target method.
 * @tparam argType1 is the type of the input parameter to be passed to the
 * target method.
 */
template<typename className, typename argType1 = void>
class ClassMethodCallerT: public ClassMethodCaller {

public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef bool (className::*MethodPointer)(argType1);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer f);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief Calls the class method with one argument passed by reference.
     * @param[in] context is the pointer to the object owning the method.
     * @param[in] ref is the class method argument.
     * @return
     * + ErrorManagement::FatalError if the class method returns false
     * + ErrorManagement::NoError if it returns true
     */
    virtual ErrorManagement::ErrorType Call(Object * context,
                                            argType1 ref);

    /**
     * @brief Calls the class method with one argument passed by copy.
     * @param[in] context is the pointer to the object owning the method.
     * @param[in] ref is the class method argument.
     * @param[in] byCopy states if the x argument is actually passed by copy
     * (meaningful when used from templates, helping on overloading resolution)
     * @return
     * + ErrorManagement::FatalError if the class method returns false
     * + ErrorManagement::NoError if it returns true
     */
    virtual ErrorManagement::ErrorType Call(Object * context,
                                            argType1 ref,
                                            bool ByCopy);

    /**
     * @brief Gets the pointer to the method
     */
    MethodPointer GetMethodPointer();

private:

    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods without arguments.
 *
 * @details This specialisation of the template generates a "Call" method
 * for target methods which do not accept arguments.
 *
 * This specialisation expects only the name of the target class in the list
 * of class template parameters, because the target method will not receive
 * any argument.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA> mc(&ClassA::MethodZ);
 *
 * @tparam className is the class owning the target method.
 */
template<typename className>
class ClassMethodCallerT<className> : public ClassMethodCaller {

public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef bool (className::*MethodPointer)();

    /**
     * @brief Constructor from a class method without arguments.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer f);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief Calls the class method.
     * @param[in] context is the pointer to the object owning the method.
     * @return
     * + ErrorManagement::FatalError if the class method returns false
     * + ErrorManagement::NoError if it returns true
     */
    virtual ErrorManagement::ErrorType Call(Object * context);

    /**
     * @brief Gets the pointer to the method
     */
    MethodPointer GetMethodPointer();

private:

    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/***************
 * 0 arguments *
 ***************/
template<typename className>
ClassMethodCallerT<className, void>::ClassMethodCallerT(MethodPointer f) {
    pFun = f;
}

template<typename className>
ClassMethodCallerT<className, void>::~ClassMethodCallerT() {
}

template<typename className>
ErrorManagement::ErrorType ClassMethodCallerT<className, void>::Call(Object * context) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    className *actualContext = dynamic_cast<className *>(context);
    if (actualContext == NULL_PTR(className *)) {
        err = ErrorManagement::UnsupportedFeature;
    }
    else {
        (actualContext->*pFun)() ? (err = ErrorManagement::NoError) : (err = ErrorManagement::FatalError);
    }
    return err;
}

template<typename className>
typename ClassMethodCallerT<className, void>::MethodPointer ClassMethodCallerT<className, void>::GetMethodPointer() {
    return pFun;
}

/***************
 * 1 argument  *
 ***************/
template<typename className, typename argType1>
ClassMethodCallerT<className, argType1>::ClassMethodCallerT(MethodPointer f) {
    pFun = f;
}

template<typename className, typename argType1>
ClassMethodCallerT<className, argType1>::~ClassMethodCallerT() {
}

template<typename className, typename argType1>
ErrorManagement::ErrorType ClassMethodCallerT<className, argType1>::Call(Object * context,
                                                                         argType1 ref) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    className *actualContext = dynamic_cast<className *>(context);
    if (actualContext == NULL_PTR(className *)) {
        err = ErrorManagement::UnsupportedFeature;
    }
    else {
        (actualContext->*pFun)(ref) ? (err = ErrorManagement::NoError) : (err = ErrorManagement::FatalError);
    }
    return err;
}

template<typename className, typename argType1>
ErrorManagement::ErrorType ClassMethodCallerT<className, argType1>::Call(Object * context,
                                                                         argType1 ref,
                                                                         bool ByCopy) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    className *actualContext = dynamic_cast<className *>(context);
    if (actualContext == NULL_PTR(className *)) {
        err = ErrorManagement::UnsupportedFeature;
    }
    else {
        (actualContext->*pFun)(ref) ? (err = ErrorManagement::NoError) : (err = ErrorManagement::FatalError);
    }
    return err;
}

template<typename className, typename argType1>
typename ClassMethodCallerT<className, argType1>::MethodPointer ClassMethodCallerT<className, argType1>::GetMethodPointer() {
    return pFun;
}

}

#endif /* CLASSMETHODCALLERT_H_ */

