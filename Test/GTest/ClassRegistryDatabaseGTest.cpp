/**
 * @file ClassRegistryDatabaseGTest.cpp
 * @brief Source file for class ClassRegistryDatabaseGTest
 * @date 06/08/2015
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
 * the class ClassRegistryDatabaseGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

#include "ClassRegistryDatabaseTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ClassRegistryDatabaseGTest,TestInstance) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestInstance());
}

TEST(ClassRegistryDatabaseGTest,TestDeleteTrue) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestDeleteTrue());
}

TEST(ClassRegistryDatabaseGTest,TestDeleteFalse) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestDeleteFalse());
}

TEST(ClassRegistryDatabaseGTest,TestAdd) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestAdd());
}

TEST(ClassRegistryDatabaseGTest,TestAddTheSameName) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestAddTheSameName());
}

TEST(ClassRegistryDatabaseGTest,TestFindTrue) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFind("abcdefg", true));
}

TEST(ClassRegistryDatabaseGTest,TestFindFalse) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFind("abcd", false));
}

TEST(ClassRegistryDatabaseGTest,TestFindDLLOnlyClassNameTrue) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFindDLL("boh","IntegerObject",true));
}

TEST(ClassRegistryDatabaseGTest,TestFindDLLOnlyClassNameFalse) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFindDLL("boh","boh",false));
}

TEST(ClassRegistryDatabaseGTest,TestList) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestList());
}

TEST(ClassRegistryDatabaseGTest,TestSize) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestSize());
}

TEST(ClassRegistryDatabaseGTest,TestElementAt) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestElementAt());
}

TEST(ClassRegistryDatabaseGTest,TestCreateInstances) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestCreateInstances());
}

TEST(ClassRegistryDatabaseGTest,TestPolimorphismChild2Father) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestPolimorphismChild2Father());
}

TEST(ClassRegistryDatabaseGTest,TestPolimorphismFather2Child) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestPolimorphismFather2Child());
}
