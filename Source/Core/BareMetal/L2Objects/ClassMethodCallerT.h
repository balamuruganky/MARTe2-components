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

//TODO
//TODO
//TODO
//TODO   Separate inline into inline section
//TODO
//TODO
//TODO

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with four arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * @param argType2 is the type of the input parameter to be passed to the target method 2.
 * @param argType3 is the type of the input parameter to be passed to the target method 3.
 * @param argType4 is the type of the input parameter to be passed to the target method 4.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className, typename MethodPointer, typename argType1,typename argType2,typename argType3,typename argType4>
class ClassMethodCallerT: public ClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            parameters.Read("param1",param1);
            argType2 param2;
            parameters.Read("param2",param2);
            argType3 param3;
            parameters.Read("param3",param3);
            argType4 param4;
            parameters.Read("param4",param4);
            err =  (actual->*pFun)(param1,param2,param3,param4);
        }
        return err;
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){

        // no need to check validity of
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, *(param.operator->()));
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};


/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with three arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * @param argType2 is the type of the input parameter to be passed to the target method 2.
 * @param argType3 is the type of the input parameter to be passed to the target method 3.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className,typename MethodPointer, typename argType1,typename argType2,typename argType3>
class ClassMethodCallerT<className,MethodPointer,argType1,argType2,argType3,void>:public ClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3);


    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            argType2 param2;
            argType3 param3;
            parameters.Read("param1",param1);
            parameters.Read("param2",param2);
            parameters.Read("param3",param3);
            err =  (actual->*pFun)(param1,param2,param3);
        }
        return err;
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, *(param.operator->()));
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with two arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * @param argType2 is the type of the input parameter to be passed to the target method 2.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className, typename MethodPointer, typename argType1,typename argType2>
class ClassMethodCallerT<className,MethodPointer,argType1,argType2,void,void>:public ClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2);


    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            argType2 param2;
            parameters.Read("param1",param1);
            parameters.Read("param2",param2);
            err =  (actual->*pFun)(param1,param2);
        }
        return err;
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, *(param.operator->()));
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className, typename MethodPointer, typename argType1>
class ClassMethodCallerT<className,MethodPointer,argType1,void,void,void>:public ClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1);

    /**
     * @brief Constructor from a class method with one i            argType4 param4;
     * nput parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            parameters.Read("param1",param1);
            err =  (actual->*pFun)(param1);
        }
        return err;
    }

    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, *(param.operator->()));
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with no arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className,typename MethodPointer>
class ClassMethodCallerT<className,MethodPointer,void,void,void,void>:public ClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)();

private:

    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
public:

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){
        return Call(object);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        return Call(object);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StreamI &stream){
        return Call(object);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object){
        className* actual = dynamic_cast<className *>(object);
        return (actual->*pFun)();
    }

};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one StructuredDataI *argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 */
template <class className,typename MethodPointer>
class ClassMethodCallerT<className,MethodPointer,StructuredDataI,void,void,void>:public ClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(StructuredDataI *parameters);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){
        className* actual = dynamic_cast<className *>(object);
        return (actual->*pFun)(parameters);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ErrorManagement::ErrorType err(true);

        ReferenceT<StructuredDataI> sI = parameters.Get(0);

        err.parametersError = !sI.IsValid();
        if (err.ErrorsCleared()){
            err = Call(object,*(sI.operator->()));
        }

        return err;
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};


/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one ReferenceContainer &argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 */
template <class className,typename MethodPointer>
class ClassMethodCallerT<className,MethodPointer,ReferenceContainer,void,void,void>:public ClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(ReferenceContainer &parameters);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){

        ReferenceContainer param1;

        Object * o = dynamic_cast<Object *>(&parameters);

        Reference ref(o);

        param1.Insert(ref);

        return Call (object ,param1);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        className* actual = dynamic_cast<className *>(object);
        return (actual->*pFun)(parameters);
    }

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one StreamI *argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + ClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className,typename MethodPointer>
class ClassMethodCallerT<className,MethodPointer,StreamI,void,void,void>:public ClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(StreamI *);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    ClassMethodCallerT(MethodPointer method,uint32 mask){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters){
        return ErrorManagement::parametersError;
    }

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        className* actual = dynamic_cast<className *>(object);

        Reference par = parameters.Get(0);

        ReferenceT<StreamI> param = par;

        return (actual->*pFun)(param.operator->());
    }

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StreamI &stream){
        className* actual = dynamic_cast<className *>(object);

        return (actual->*pFun)(stream);
    }

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};


}

#endif /* CLASSMETHODCALLERT_H_ */

