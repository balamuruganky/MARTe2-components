/**
 * @file Object.cpp
 * @brief Source file for class Object
 * @date 04/08/2015
 * @author André Neto
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
 * the class Object (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "Memory.h"
#include "ClassRegistryItem.h"
#include "FastPollingMutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
Object::Object() {
    referenceCounter = 0;
}

Object::~Object() {
}

static FastPollingMutexSem refMux;

uint32 Object::DecrementReferences() {
    int32 ret;
    refMux.FastLock();
    ret = --referenceCounter;
    refMux.FastUnLock();
    return ret;
}

uint32 Object::IncrementReferences() {
    int32 ret;
    refMux.FastLock();
    ret = ++referenceCounter;
    refMux.FastUnLock();
    return ret;
}

Object *Object::Clone() const {
    return NULL;
}

uint32 Object::NumberOfReferences() const {
    return referenceCounter;
}

bool Object::Initialise(const StructuredData &data) {
    return false;
}

void *Object::operator new(size_t size) throw () {
    return NULL;
}

void Object::GetIntrospectionCopy(Introspection &destination) const {
    destination = introspection;
}

CLASS_REGISTER(Object, "1.0")
