/**
 * @file ConfigurationDatabase.cpp
 * @brief Source file for class ConfigurationDatabase
 * @date 27/10/2015
 * @author Andre' Neto
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
 * the class ConfigurationDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"
#include "ConfigurationDatabase.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"
#include "String.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ConfigurationDatabase::ConfigurationDatabase() {

}

ConfigurationDatabase::~ConfigurationDatabase() {

}

bool ConfigurationDatabase::Write(const char * const name,
                                  const AnyType &value) {
    ReferenceT<AnyObject> objToWrite(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = objToWrite.IsValid();
    if (ok) {
        ok = objToWrite->Load(value);
        if (ok) {
            objToWrite->SetName(name);
            ok = currentNode.IsValid();
            if (ok) {
                ok = currentNode->Insert(objToWrite);
            }
        }
    }
    return ok;
}

AnyType ConfigurationDatabase::GetType(const char * const name) {
    bool found = false;
    Reference foundReference;
    uint32 i;
    for (i = 0; (!found) && (i < currentNode->Size()); i++) {
        foundReference = currentNode->Get(i);
        found = (StringHelper::Compare(foundReference->GetName(), name) == 0);
    }

    AnyType retType;
    if (found) {
        ReferenceT<AnyObject> objToRead = foundReference;
        if (objToRead.IsValid()) {
            retType = objToRead->GetType();
        }
    }

    return retType;
}

bool ConfigurationDatabase::Copy(StructuredDataI &destination) {
    return destination.AddToCurrentNode(currentNode);
}

bool ConfigurationDatabase::MoveToRoot() {
    bool ok = rootNode.IsValid();
    if (ok) {
        currentNode = rootNode;
    }
    return ok;
}

bool ConfigurationDatabase::Read(const char * const name,
                                 const AnyType &value) {

    //Could have used the ReferenceContainerFilterObjectName but this way is faster given that no complex paths are involved
    bool found = false;
    Reference foundReference;
    uint32 i;
    for (i = 0; (!found) && (i < currentNode->Size()); i++) {
        foundReference = currentNode->Get(i);
        found = (StringHelper::Compare(foundReference->GetName(), name) == 0);
    }

    bool ok = found;
    if (ok) {
        ReferenceT<AnyObject> objToRead = foundReference;
        ok = objToRead.IsValid();
        if (ok) {
            ok = TypeConvert(value, objToRead->GetType());
        }
    }

    return ok;
}

bool ConfigurationDatabase::MoveAbsolute(const char * const path) {

    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, path);
    ReferenceContainer resultSingle;
    rootNode->Find(resultSingle, filter);

    bool ok = (resultSingle.Size() > 0);
    if (ok) {
        //Invalidate move to leafs
        ReferenceT<ReferenceContainer> container = resultSingle.Get(0);
        if (container.IsValid()) {
            currentNode = container;
        }
    }

    return ok;
}

bool ConfigurationDatabase::MoveRelative(const char * const path) {

    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, path);
    ReferenceContainer resultSingle;
    currentNode->Find(resultSingle, filter);

    bool ok = (resultSingle.Size() > 0);
    if (ok) {
        //Invalidate move to leafs
        ReferenceT<ReferenceContainer> container = resultSingle.Get(0);
        if (container.IsValid()) {
            currentNode = container;
        }
    }

    return ok;
}

bool ConfigurationDatabase::MoveToAncestor(uint32 generations) {
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH, currentNode);
    ReferenceContainer resultPath;
    currentNode->Find(resultPath, filter);
    bool ok = (resultPath.Size() > 0);
    if (ok) {
        int32 newPositionIdx = resultPath.Size() - generations;
        ok = (newPositionIdx >= 0);
        if (ok) {
            currentNode = resultPath.Get(newPositionIdx);
        }
    }
    return ok;
}

bool ConfigurationDatabase::CreateNodes(const char * const path) {
    String pathStr = path;
    pathStr.Seek(0u);
    bool ok = (pathStr.Size() > 0u);
    String token;
    char c;

    while (ok && (pathStr.GetToken(token, ".", c))) {
        ok = (token.Size() > 0);
        if (ok) {
            if (!rootNode.IsValid()) {
                ReferenceT<ReferenceContainer> rootContainer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                rootNode = rootContainer;
                rootNode->SetName(token.Buffer());
                currentNode = rootNode;
            }
            else {
                ReferenceT<ReferenceContainer> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                container->SetName(token.Buffer());
                ok = currentNode->Insert(container);
                if (ok) {
                    currentNode = container;
                }
            }

            if (ok) {
                ok = token.Seek(0u);
                if (ok) {
                    ok = token.SetSize(0u);
                }
            }
        }
    }
    return ok;
}

bool ConfigurationDatabase::CreateNodesAbsolute(const char * const path) {
    currentNode = rootNode;
    return CreateNodes(path);
}

bool ConfigurationDatabase::CreateNodesRelative(const char * const path) {
    return CreateNodes(path);
}

bool ConfigurationDatabase::AddToCurrentNode(Reference node) {
    ReferenceT<ReferenceContainer> nodeToAdd = node;
    bool ok = nodeToAdd.IsValid();
    if (ok) {
        ok = currentNode->Insert(nodeToAdd);
    }
    return ok;
}

}
