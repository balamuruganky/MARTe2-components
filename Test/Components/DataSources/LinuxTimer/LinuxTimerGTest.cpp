/**
 * @file LinuxTimerGTest.cpp
 * @brief Source file for class LinuxTimerGTest
 * @date 25/10/2016
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
 * the class LinuxTimerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LinuxTimerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(LinuxTimerGTest,TestConstructor) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(LinuxTimerGTest,TestAllocateMemory) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(LinuxTimerGTest,TestGetNumberOfMemoryBuffers) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(LinuxTimerGTest,TestGetSignalMemoryBuffer) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(LinuxTimerGTest,TestGetSignalMemoryBuffer_False) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}

TEST(LinuxTimerGTest,TestGetBrokerName) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(LinuxTimerGTest, TestGetInputBrokers) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(LinuxTimerGTest, TestGetOutputBrokers) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(LinuxTimerGTest, TestSynchronise) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(LinuxTimerGTest, TestExecute) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(LinuxTimerGTest, TestExecute_Busy) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestExecute_Busy());
}

TEST(LinuxTimerGTest, TestPrepareNextState) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(LinuxTimerGTest, TestInitialise_Default) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(LinuxTimerGTest, TestInitialise_Busy) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestInitialise_Busy());
}

TEST(LinuxTimerGTest, TestInitialise_Empty) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(LinuxTimerGTest, TestInitialise_False) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestInitialise_False());
}

TEST(LinuxTimerGTest, TestSetConfiguredDatabase) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(LinuxTimerGTest, TestSetConfiguredDatabase_False_MoreThan2Signals) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThan2Signals());
}

TEST(LinuxTimerGTest, TestSetConfiguredDatabase_False_No32BitsSignal1) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_No32BitsSignal1());
}

TEST(LinuxTimerGTest, TestSetConfiguredDatabase_False_No32BitsSignal2) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_No32BitsSignal2());
}

TEST(LinuxTimerGTest, TestSetConfiguredDatabase_NoFrequencySet) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_NoFrequencySet());
}

TEST(LinuxTimerGTest, TestSetConfiguredDatabase_One_Signal_Per_GAM) {
    LinuxTimerTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_One_Signal_Per_GAM());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

