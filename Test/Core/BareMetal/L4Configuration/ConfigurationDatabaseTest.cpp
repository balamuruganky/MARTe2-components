/**
 * @file ConfigurationDatabase.cpp
 * @brief Source file for class ConfigurationDatabase
 * @date 19/11/2015
 * @author Andre Neto
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
#include "ConfigurationDatabaseTest.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
bool ConfigurationDatabaseTest::TestDefaultConstructor() {
    ConfigurationDatabase cdb;
    bool ok = cdb.Lock(TTInfiniteWait);
    cdb.Unlock();
    return ok;
}

bool ConfigurationDatabaseTest::TestCreateAbsolute_ValidPath() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A");
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = cdb.MoveAbsolute("A");
    }

    ok = cdb.CreateAbsolute("A.B");
    if (ok) {
        ok = cdb.MoveAbsolute("A.B");
    }

    ok = cdb.CreateAbsolute("A.B.C");

    ok = cdb.CreateAbsolute("D.E.F");

    if (ok) {
        ok = cdb.MoveAbsolute("D.E.F");
    }
    if (ok) {
        ok = cdb.MoveAbsolute("A.B.C");
    }

    return ok;
}

bool ConfigurationDatabaseTest::TestCreateAbsolute_InvalidPath() {
    ConfigurationDatabase cdb;
    bool ok = !cdb.MoveAbsolute("A.B");

    if (ok) {
        ok = cdb.CreateAbsolute("A.B");
    }
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = cdb.MoveAbsolute("A.B");
    }

    ok = !cdb.CreateAbsolute("A.B");

    return ok;
}

bool ConfigurationDatabaseTest::TestCreateRelative_ValidPath() {
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateRelative("A");
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = cdb.MoveAbsolute("A");
    }

    ok = cdb.CreateRelative("A.B");
    if (ok) {
        ok = cdb.MoveAbsolute("A.A.B");
    }
    if (ok) {
        ok = !cdb.MoveAbsolute("A.B");
    }
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    ok = cdb.CreateRelative("D.E.F");

    if (ok) {
        ok = cdb.CreateRelative("D.E.F");
    }
    if (ok) {
        ok = cdb.MoveAbsolute("D.E.F.D.E.F");
    }

    return ok;
}

bool ConfigurationDatabaseTest::TestCreateRelative_InvalidPath() {
    ConfigurationDatabase cdb;
    bool ok = !cdb.MoveRelative("A.B");

    if (ok) {
        ok = cdb.CreateRelative("A.B");
    }
    if (ok) {
        ok = cdb.CreateRelative("C");
    }
    ok = cdb.MoveToAncestor(1);
    if (ok) {
        ok = !cdb.CreateRelative("C");
    }
    if (ok) {
        ok = cdb.CreateRelative("B");
    }
    if (ok) {
        ok = cdb.MoveAbsolute("A.B.B");
    }
    cdb.MoveToRoot();
    ok = !cdb.CreateRelative("A.B");

    return ok;
}

/*bool ConfigurationDatabaseTest::TestMoveToRoot() {
    ConfigurationDatabase cdb;
    bool ok = !cdb.MoveRelative("A.B");

    if (ok) {
        ok = cdb.CreateRelative("A.B");
    }
    if (ok) {
        ok = cdb.CreateRelative("C");
    }
    ok = cdb.MoveToAncestor(1);
    if (ok) {
        ok = !cdb.CreateRelative("C");
    }
    if (ok) {
        ok = cdb.CreateRelative("B");
    }
    if (ok) {
        ok = cdb.MoveAbsolute("A.B.B");
    }
    cdb.MoveToRoot();
    ok = !cdb.CreateRelative("A.B");

    return ok;
}*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

