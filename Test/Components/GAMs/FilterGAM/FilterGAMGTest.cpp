/**
 * @file FilterGAMGTest.cpp
 * @brief Source file for class FilterGAMGTest
 * @date Jan 30, 2017 TODO Verify the value and format of the date
 * @author Llorenc Capella
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
 * the class FilterGAMGTest (public, protected, and private). Be aware that some 
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

#include "FilterGAM.h"
#include "FilterGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FilterGAMGTest,TestConstructor) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FilterGAMGTest,TestInitialise) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FilterGAMGTest,TestInitialiseNum0) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNum0());
}

TEST(FilterGAMGTest,TestInitialiseDen0) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseDen0());
}

TEST(FilterGAMGTest,TestStaticGainFIR) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestStaticGainFIR());
}

TEST(FilterGAMGTest,TestStaticGainIIR) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestStaticGainIIR());
}

TEST(FilterGAMGTest,TestFailNormalise) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestFailNormalise());
}

TEST(FilterGAMGTest,TestSetup) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(FilterGAMGTest,TestSetupNoInputSignal) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSignal());
}

TEST(FilterGAMGTest,TestSetupNoOutputSignal) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputSignal());
}

TEST(FilterGAMGTest,TestSetupNoInputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSamples());
}

TEST(FilterGAMGTest,TestSetupNoOutputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputSamples());
}

TEST(FilterGAMGTest,TestSetupNoNumberOfElementsInput) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoNumberOfElementsInput());
}

TEST(FilterGAMGTest,TestSetupNoNumberOfElementsOutput) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoNumberOfElementsOutput());
}

TEST(FilterGAMGTest,TestSetupNumberOfSamplesOutput2) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNumberOfSamplesOutput2());
}
TEST(FilterGAMGTest,TestSetup0NumberOfElements) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetup0NumberOfElements());
}

TEST(FilterGAMGTest,TestSetupDifferentInputOutputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupDifferentInputOutputSamples());
}


TEST(FilterGAMGTest,TestSetupFailNumberOfSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupFailNumberOfSamples());
}

TEST(FilterGAMGTest,TestSetupNoInputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputDimension());
}

TEST(FilterGAMGTest,TestSetupWrongInputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputDimension());
}

TEST(FilterGAMGTest,TestSetupNoOutputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputDimension());
}

TEST(FilterGAMGTest,TestSetupWrongOutputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputDimension());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInput) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInput2) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput2());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2());
}

TEST(FilterGAMGTest,TestExecuteIIRConstant) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant());
}

TEST(FilterGAMGTest,TestExecuteIIRConstant2) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant2());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantDimArray1) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstantDimArray1());
}

TEST(FilterGAMGTest,TestExecuteElements1Samples10) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteElements1Samples10());
}

TEST(FilterGAMGTest,TestSetupSeveralSignals) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignals());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffNumberOfInputElements) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffNumberOfInputElements());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffNumberOfOutputElements) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffNumberOfOutputElements());
}


TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffInputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffInputSamples());
}

TEST(FilterGAMGTest,TestExecuteSeveralSignalsFIR) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteSeveralSignalsFIR());
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

