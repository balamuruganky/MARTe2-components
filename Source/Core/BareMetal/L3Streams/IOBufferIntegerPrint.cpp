/**
 * @file IOBufferIntegerPrint.cpp
 * @brief Source file for class IOBufferIntegerPrint
 * @date 02/10/2015
 * @author Giuseppe Ferrò
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
 * the class IOBufferIntegerPrint (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "FormatDescriptor.h"
#include "BitSetToInteger.h"
#include "IOBuffer.h"
#include "Shift.h"

namespace MARTe {

/*lint -e568 [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
// returns the exponent
// positiveNumber is the abs (number)
/** @brief Calculates the order of a number, namely its number of digits minus one.
 * @param positiveNumber is the number argument which must be positive.
 * @return the number of digits minus one.
 *
 * The function operates by comparing with 10**N with converging by bisection to the correct value. */
template<typename T>
static inline uint16 GetOrderOfMagnitude(const T positiveNumber) {
    T tenToExponent = static_cast<T>(1);
    uint8 nDigits = 0u;
    // check whether exponent greater than 10
    if (sizeof(T) >= 8u) { // max 19
        /*lint -e{734} */
        T temp = tenToExponent * static_cast<T>(10000000000); // 10 zeros
        if (positiveNumber >= temp) {
            tenToExponent = temp;
            nDigits += 10u;
        }
    }

    // check whether exponent greater than 5
    if (sizeof(T) >= 4u) { // max 9
        /*lint -e{734} */
        T temp = tenToExponent * static_cast<T>(100000); // 5 zeros
        if (positiveNumber >= temp) {
            tenToExponent = temp;
            nDigits += 5u;
        }
    }

    // check whether exponent greater than 2
    if (sizeof(T) >= 2u) { // max 4 zeros
        /*lint -e{734} */
        T temp = tenToExponent * static_cast<T>(100); // 2 zeros
        if (positiveNumber >= temp) {
            tenToExponent = temp;
            nDigits += 2u;
        }
    }

    // check whether exponent greater than 1
    /*lint -e{734} */
    T temp = tenToExponent * static_cast<T>(10); // 1
    if (positiveNumber >= temp) {
        tenToExponent = temp;
        nDigits++;
    }

    // check whether exponent greater than 1
    /*lint -e{734} */
    temp = tenToExponent * static_cast<T>(10);  // 1
    // avoid overflowing in case of signed number
    if (temp > tenToExponent) {
        if (positiveNumber >= temp) {
            nDigits++;
        }
    }
    return nDigits;
}

// returns the number of digits necessary to represent this number -1
/** @brief Calculates the number of digits for a hexadecimal representation.
 * @param number is the number argument.
 * @return the minimum number of digits for an hexadecimal print.
 *
 * This function operates comparing the number with 16**N numbers with N=1,2,4,8 converging by
 * bisection to the correct value. */

template<typename T>
static inline uint16 GetNumberOfDigitsHexNotation(T number) {
    uint8 nDigits = 1u;

    // negative numbers are 2 complements and have therefore all bits
    if (number >= static_cast<T>(0)) {

        uint8 shift = 0u;
        // check if larger than 2**32
        if (sizeof(T) == 8u) {
            if (number >= static_cast<T>(0x100000000u)) {
                nDigits += 8u;
                shift = 32u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**16
        if (sizeof(T) >= 4u) {
            if (number >= static_cast<T>(0x10000u)) {
                nDigits += 4u;
                shift = 16u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**8
        if (sizeof(T) >= 2u) {
            if (number >= static_cast<T>(0x100u)) {
                nDigits += 2u;
                shift = 8u;
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**4
        if (number >= static_cast<T>(0x10u)) {
            nDigits += 1u;
        }
    }
    else {
        nDigits = static_cast<uint8>(sizeof(T) * 2u);
    }

    return nDigits;
}

// returns the number of digits necessary to represent this number -1
/** @brief Calculates the number of digits for an octal representation.
 * @param number is the number argument.
 * @return the minimum number of digits for an octal print.
 *
 * This function operates comparing the number with 8**N numbers with N=1,2,4,8,16 converging by
 * bisection to the correct value. */

template<typename T>
static inline uint16 GetNumberOfDigitsOctalNotation(T number) {
    // negative numbers are 2 complements and have therefore all bits
    uint8 nDigits = 1u;

    if (number >= static_cast<T>(0)) {

        uint8 shift = 0u;
        if (sizeof(T) == 8u) {
            if (number >= static_cast<T>(0x1000000000000u)) {
                nDigits += 16u;
                shift = 48u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        if (sizeof(T) >= 4u) {
            if (number >= static_cast<T>(0x1000000u)) {
                nDigits += 8u;
                shift = 24u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**12
        if (sizeof(T) >= 2u) {
            if (number >= static_cast<T>(0x1000)) {
                nDigits += 4u;
                shift = 12u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**6
        if (number >= static_cast<T>(0x40u)) {
            nDigits += 2u;
            shift = 6u;
            number = Shift::LogicalRightSafeShift(number, shift);
        }

        // check if larger than 2**2
        if (number >= static_cast<T>(0x8u)) {
            nDigits += 1u;
        }
    }
    else {
        nDigits = static_cast<uint8>(((sizeof(T) * 8u) + 2u) / 3u);
    }

    return nDigits;
}

// returns the number of digits necessary to represent this number -1
/** @brief Calculates the number of digits for a binary representation.
 * @param number is the number argument.
 * @return the minimum number of digits for a binary print.
 *
 * This function operates comparing the number with 2**N numbers with N=1,2,4,8,16,32 converging by
 * bisection to the correct value. */

template<typename T>
static inline uint16 GetNumberOfDigitsBinaryNotation(T number) {
    uint8 nDigits = 1u;

    // negative numbers are 2 complements and have therefore all bits
    if (number >= static_cast<T>(0)) {

        uint8 shift = 0u;
        // check if larger than 2**32
        // if so shift
        if (sizeof(T) == 8u) {
            if (number >= static_cast<T>(0x100000000u)) {
                nDigits += 32u;
                shift = 32u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**16
        if (sizeof(T) >= 4u) {
            if (number >= static_cast<T>(0x10000u)) {
                nDigits += 16u;
                shift = 16u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**8
        if (sizeof(T) >= 2u) {
            if (number >= static_cast<T>(0x100u)) {
                nDigits += 8u;
                shift = 8u;
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**4
        if (number >= static_cast<T>(0x10u)) {
            nDigits += 4u;
            shift = 4u;
            number = Shift::LogicalRightSafeShift(number, shift);
        }

        // check if larger than 2**2
        if (number >= static_cast<T>(0x4u)) {
            nDigits += 2u;
            shift = 2u;
            number = Shift::LogicalRightSafeShift(number, shift);
        }

        // check if larger than 2**1
        if (number >= static_cast<T>(0x2u)) {
            nDigits += 1u;
        }
    }
    else {
        nDigits = static_cast<uint8>(sizeof(T) * 8u);
    }

    return nDigits;
}

/** @brief Prints a string on a generic stream.
 * @param stream is a generic stream class which implements a PutC() function.
 * @param s is the string to be printed. */
template<class streamer>
static inline void PutS(streamer & stream,
                        const char8 *s) {
    while (s[0] != '\0') {
        if (!stream.PutC(s[0])) {
            //TODO
        }
        s = &s[1];
    }
}

/** @brief Prints an integer number on a general stream in decimal notation.
 * @param stream is a general stream class which implements a PutC() function.
 * @param maximumSize is the maximum requested space for the number print.
 * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
 * @param leftAligned specifies if the number must be print with left or right alignment.
 * @param addPositiveSign specifies if we want print the '+' before positive numbers.
 * @return true.
 *
 * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
 * Respects maximumSize and number integrity.
 * If not possible (maximum size minor than the minimum space for the number print) outputs is ? */
/*lint -e{9143} [MISRA C++ Rule 5-3-2]. Justification: application of sign - is applied only in case of negative number (then signed numbers).*/
template<typename T>
bool IntegerToStreamDecimalNotation(IOBuffer &stream,                     // must have a GetC(c) function where c is of a type that can be obtained from chars
                                    const T number,
                                    uint16 maximumSize = 0u,       // 0 means that the number is printed in its entirety
                                    bool padded = false,   // if maximumSize!=0 & align towards the right or the left
                                    const bool leftAligned = false,   // if padded and maximumSize!=0 align towards the left
                                    const bool addPositiveSign = false)   // prepend with + not just with - for negative numbers
                                    {

    bool ret = false;

    // if no limits set the numberSize as big enough for the largest integer
    if (maximumSize == 0u) {
        maximumSize = 20u;
        padded = false;
    }

    // put here the unsigned version of the number
    T positiveNumber;
    // put here the total space needed for the number
    // 1 always for the one figure to print
    uint16 numberSize = 1u;

    // if negative invert it and account for the '-' in the size
    if (number < static_cast<T>(0)) {
        /*lint -e{9134} -e{501} -e{732} the number is signed */
        positiveNumber = -number;
        numberSize++;
    }
    else {
        // if positive copy it and account for the '+' in the size if addPositiveSign set
        positiveNumber = number;
        if (addPositiveSign) {
            numberSize++;
        }
    }

    // 0x800000....
    if (positiveNumber < static_cast<T>(0)) {
        if ((sizeof(T) == 8u) && (maximumSize >= 20u)) {
            PutS(stream, "-9223372036854775808");
            ret = true;
        }
        if ((sizeof(T) == 4u) && (maximumSize >= 10u)) {
            PutS(stream, "-2147483648");
            ret = true;
        }
        if ((sizeof(T) == 2u) && (maximumSize >= 6u)) {
            PutS(stream, "-32768");
            ret = true;
        }
        if ((sizeof(T) == 1u) && (maximumSize >= 4u)) {
            PutS(stream, "-128");
            ret = true;
        }

        // does not fit
        numberSize = maximumSize + 1u;
    }

    else {

        // add the number of figures beyond the first
        numberSize += GetOrderOfMagnitude(positiveNumber);
    }

    if (!ret) {

        // is there enough space for the number?
        if (maximumSize < numberSize) {
            // if no than we shall print '?' so the size is 1 now
            numberSize = 1u;

            // fill up to from 1 maximumSize with ' '
            if ((padded) && (!leftAligned)) {
                for (uint32 i = 1u; i < maximumSize; i++) {
                    if (!stream.PutC(' ')) {
                        //TODO
                    }
                }
            }

            // put the ?
            if (!stream.PutC('?')) {
                //TODO
            }

        }
        else { // enough space

            // fill up from numberSize to maximumSize with ' '
            if ((padded) && (!leftAligned)) {
                for (uint32 i = numberSize; i < maximumSize; i++) {
                    if (!stream.PutC(' ')) {
                        //TODO
                    }
                }
            }

            // add sign
            if (number < static_cast<T>(0)) {
                if (!stream.PutC('-')) {
                    //TODO
                }
            }
            else {
                if (addPositiveSign) {
                    if (!stream.PutC('+')) {
                        //TODO
                    }
                }
            }

            // put number
            Number2StreamDecimalNotationPrivate(stream, positiveNumber);
        }

        // fill up from numberSize to maximumSize with ' '
        if ((padded) && (leftAligned)) {
            for (uint32 i = numberSize; i < maximumSize; i++) {
                if (!stream.PutC(' ')) {
                    //TODO
                }
            }
        }
    }

    return true;
}

/** @brief Prints an integer number on a general stream in hexadecimal notation.
 * @param stream is a general stream class which implements a PutC() function.
 * @param maximumSize is the maximum requested space for the number print.
 * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
 * @param leftAligned specifies if the number must be print with left or right alignment.
 * @param zeroPaddedBitSize specifies the actual number of bits in format. Set to sizeof(T) means to add trailing zeros, any smaller, non zero value means determines how many bits to print
 * @param addHeader specifies if we want to add the hex header '0x' before the number.
 * @return true.
 *
 * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
 * Uses hexadecimal notation.
 * Respects maximumSize and number integrity.
 * If not possible (maximum size minor than the minimum space for the number print) output is ? */
template<typename T>
bool IntegerToStreamExadecimalNotation(IOBuffer &stream,
                                       const T number,
                                       uint16 maximumSize = 0u,       // 0 means that the number is printed in its entirety
                                       bool padded = false,   // if maximumSize!=0 & align towards the right or the left
                                       const bool leftAligned = false,   // if padded and maximumSize!=0 align towards the left
                                       const uint16 zeroPaddedBitSize = 0u,       // if not 0 is used to determine how many trailing zeroes to print
                                       const bool addHeader = false)   // prepend with 0x
                                       {
    // put here size of number
    uint16 headerSize = 0u;

    // adding two chars 0x header
    if (addHeader) {
        headerSize = 2u;
    }

    // actual meaningful digits
    uint16 numberOfDigits = GetNumberOfDigitsHexNotation(number);

    // add header for total size if padded
    uint16 numberSize = headerSize + numberOfDigits;

    // 1000 = no limits
    if (maximumSize == 0u) {
        maximumSize = 1000u;
        padded = false;
    }

    // cannot fit the number even without trailing zeroes
    if (maximumSize < numberSize) {
        numberSize = 1u; // just the '?'

        // pad on the left
        if ((padded) && (!leftAligned)) {
            for (uint32 i = 1u; i < maximumSize; i++) {
                if (!stream.PutC(' ')) {
                    //TODO
                }
            }
        }
        // put the ?
        if (!stream.PutC('?')) {
            //TODO
        }

    }
    else {
        // work out what it means in terms of hex digits
        uint16 zeroPaddedDigits = (zeroPaddedBitSize + 3u) / 4u;

        //In case of trailing zeros the digits are the maximum possible or equal to maximum size (-2 if there is header)
        if (zeroPaddedDigits > numberOfDigits) {

            // add header for total size if padded
            uint16 fullNumberSize = headerSize + zeroPaddedDigits;

            // check if adding all zeros number will not fit
            if (fullNumberSize > maximumSize) {
                // how much is exceeding?
                uint16 excess = fullNumberSize - maximumSize;
                // number is made to fit the available space
                numberSize = maximumSize;
                // we cannot print all the zeros, remove excess
                numberOfDigits = zeroPaddedDigits - excess;
            }
            else {
                // we will use the full number size
                numberSize = fullNumberSize;
                // we will print all digits
                numberOfDigits = zeroPaddedDigits;
            }
        }

        // in case of left alignment
        if ((padded) && (!leftAligned)) {
            for (uint32 i = numberSize; i < maximumSize; i++) {
                if (!stream.PutC(' ')) {
                    //TODO
                }
            }
        }

        // add header
        if (addHeader) {
            if (!stream.PutC('0')) {
                //TODO
            }
            if (!stream.PutC('x')) {
                //TODO
            }
        }

        // work out how much to shift number to extract most significative hex
        // we use the above calculate number size
        uint8 bits = static_cast<uint8>((numberOfDigits - 1u) * 4u);

        // loop backwards stepping each nibble
        for (uint8 i = bits; static_cast<int8>(i) >= 0; i -= 4u) {
            //to get the digit, shift the number and by masking select only the 4 LSB bits
            uint8 digit = (static_cast<uint8>(Shift::LogicalRightSafeShift(number, i)) & (0xFu));

            // skips trailing zeros until we encounter the first non zero, or if putTrailingZeros was already set
            //if ((digit != 0) || (putTrailingZeros)){
            //putTrailingZeros = true;
            if (digit < 10u) {
                /*lint -e(9125) -e(9117) */
                if (!stream.PutC(static_cast<char8>('0' + digit))) {
                    //TODO
                }
            }
            else {
                /*lint -e(9125) -e(9117) */
                if (!stream.PutC(static_cast<char8>(('A' + digit) - 10u))) {
                    //TODO
                }
            }

        }
    }

    // in case of right alignment
    if ((padded) && (leftAligned)) {
        for (uint16 i = numberSize; i < maximumSize; i++) {
            if (!stream.PutC(' ')) {
                //TODO
            }
        }
    }
    return true;

}

/** @brief Prints an integer number on a general stream in octal notation.
 * @param stream is a general stream class which implements a PutC() function.
 * @param maximumSize is the maximum requested space for the number print.
 * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
 * @param leftAligned specifies if the number must be print with left or right alignment.
 * @param zeroPaddedBitSize specifies the actual number of bits in format. Set to sizeof(T) means to add trailing zeros, any smaller, non zero value means determines how many bits to print
 * @param addHeader specifies if we want to add the oct header '0o' before the number.
 * @return true.
 *
 * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
 * Uses octal notation.
 * Respects maximumSize and number integrity.
 * If not possible (maximum size minor than the minimum space for the number print) output is ?  */
template<typename T>
bool IntegerToStreamOctalNotation(IOBuffer &stream,
                                  const T number,
                                  uint16 maximumSize = 0u,       // 0 means that the number is printed in its entirety
                                  bool padded = false,   // if maximumSize!=0 & align towards the right or the left
                                  const bool leftAligned = false,   // if padded and maximumSize!=0 align towards the left
                                  const uint16 zeroPaddedBitSize = 0u,       // if not 0 is used to determine how many trailing zeroes to print
                                  bool addHeader = false)   // prepend with 0o
                                  {

    // put here size of number
    uint16 headerSize = 0u;

    // adding two chars 0x header
    if (addHeader) {
        headerSize = 2u;
    }

    // actual meaningful digits
    uint16 numberOfDigits = GetNumberOfDigitsOctalNotation(number);

    // add header for total size if padded
    uint16 numberSize = headerSize + numberOfDigits;

    // 1000 = no limits
    if (maximumSize == 0u) {
        maximumSize = 1000u;
        padded = false;
    }
    // cannot fit the number even without trailing zeroes
    if (maximumSize < numberSize) {
        numberSize = 1u; // just the '?'

        // pad on the left
        if ((padded) && (!leftAligned)) {
            for (uint32 i = 1u; i < maximumSize; i++) {
                if (!stream.PutC(' ')) {
                    //TODO
                }
            }
        }
        // put the ?
        if (!stream.PutC('?')) {
            //TODO
        }

    }
    else {
        // work out what it means in terms of octal digits
        uint16 zeroPaddedDigits = (zeroPaddedBitSize + 2u) / 3u;

        // if the zero padded number needs to be bigger
        // work out how many zeroes we can actually afford
        if (zeroPaddedDigits > numberOfDigits) {

            // add header for total size if padded
            uint16 fullNumberSize = headerSize + zeroPaddedDigits;

            // check if adding all zeros number will not fit
            if (fullNumberSize > maximumSize) {
                // how much is exceeding?
                uint16 excess = fullNumberSize - maximumSize;
                // number is made to fit the available space
                numberSize = maximumSize;
                // we cannot print all the zeros, remove excess
                numberOfDigits = zeroPaddedDigits - excess;
            }
            else {
                // we will use the full number size
                numberSize = fullNumberSize;
                // we will print all digits
                numberOfDigits = zeroPaddedDigits;
            }
        }

        // in case of left alignment
        if ((padded) && (!leftAligned)) {
            for (uint32 i = numberSize; i < maximumSize; i++) {
                if (!stream.PutC(' ')) {
                    //TODO
                }
            }
        }

        // add header
        if (addHeader) {
            if (!stream.PutC('0')) {
                //TODO
            }
            if (!stream.PutC('o')) {
                //TODO
            }
        }

        // work out how much to shift number to extract most significative hex
        // we use the above calculate number size
        uint8 bits = static_cast<uint8>((numberOfDigits - 1u) * 3u);

        // loop backwards stepping each nibble
        for (uint8 i = bits; static_cast<int8>(i) >= 0; i -= 3u) {

            //right shift of the number
            uint8 digit = static_cast<uint8>(static_cast<uint8>(Shift::LogicalRightSafeShift(number, i)) & 0x7u);
            /*lint -e(9125) -e(9117) */
            if (!stream.PutC(static_cast<char8>('0' + digit))) {
                //TODO
            }
        }
    }

// in case of right alignment
    if ((padded) && (leftAligned)) {
        for (uint16 i = numberSize; i < maximumSize; i++) {
            if (!stream.PutC(' ')) {
                //TODO
            }
        }
    }
    return true;
}

/** @brief Prints an integer number on a general stream in binary notation.
 * @param stream is a general stream class which implements a PutC() function.
 * @param maximumSize is the maximum requested space for the number print.
 * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
 * @param leftAligned specifies if the number must be print with left or right alignment.
 * @param zeroPaddedBitSize specifies the actual number of bits in format. Set to sizeof(T) means to add trailing zeros, any smaller, non zero value means determines how many bits to print
 * @param addHeader specifies if we want to add the bin header '0b' before the number.
 * @return true.
 *
 * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
 * Uses binary notation.
 * Respects maximumSize and number integrity.
 * If not possible (maximum size minor than the minimum space for the number print) output is ?  */
template<typename T>
bool IntegerToStreamBinaryNotation(IOBuffer &stream,
                                   const T number,
                                   uint16 maximumSize = 0u,       // 0 means that the number is printed in its entirety
                                   bool padded = false,   // if maximumSize!=0 & align towards the right or the left
                                   const bool leftAligned = false,   // if padded and maximumSize!=0 align towards the left
                                   const uint16 zeroPaddedBitSize = 0u,       // if not 0 is used to determine how many trailing zeroes to print
                                   const bool addHeader = false)   // prepend with 0b
                                   {

// 1000 = no limits
    if (maximumSize == 0u) {
        maximumSize = 1000u;
        padded = false;
    }

    // put here size of number
    uint16 headerSize = 0u;

    // adding two chars 0x header
    if (addHeader) {
        headerSize = 2u;
    }

    // actual meaningful digits
    uint16 numberOfDigits = GetNumberOfDigitsBinaryNotation(number);

    // add header for total size if padded
    uint16 numberSize = headerSize + numberOfDigits;

    // cannot fit the number even without trailing zeroes
    if (maximumSize < numberSize) {
        numberSize = 1u; // just the '?'

        // pad on the left
        if ((padded) && (!leftAligned)) {
            for (uint32 i = 1u; i < maximumSize; i++) {
                if (!stream.PutC(' ')) {
                    //TODO
                }
            }
        }
        // put the ?
        if (!stream.PutC('?')) {
            //TODO
        }

    }
    else {

        // if the zero padded number needs to be bigger
        // work out how many zeroes we can actually afford
        if (zeroPaddedBitSize > numberOfDigits) {

            // add header for total size if padded
            uint16 fullNumberSize = headerSize + zeroPaddedBitSize;

            // check if adding all zeros number will not fit
            if (fullNumberSize > maximumSize) {
                // how much is exceeding?
                uint16 excess = fullNumberSize - maximumSize;
                // number is made to fit the available space
                numberSize = maximumSize;
                // we cannot print all the zeros, remove excess
                numberOfDigits = zeroPaddedBitSize - excess;
            }
            else {
                // we will use the full number size
                numberSize = fullNumberSize;
                // we will print all digits
                numberOfDigits = zeroPaddedBitSize;
            }
        }

        // in case of left alignment
        if ((padded) && (!leftAligned)) {
            for (uint32 i = numberSize; i < maximumSize; i++) {
                if (!stream.PutC(' ')) {
                    //TODO
                }
            }
        }

        // add header
        if (addHeader) {
            if (!stream.PutC('0')) {
                //TODO
            }
            if (!stream.PutC('b')) {
                //TODO
            }
        }

        // work out how much to shift number to extract most significative hex
        // we use the above calculate number size
        uint8 bits = static_cast<uint8>(numberOfDigits - 1u);

        // loop backwards stepping each nibble
        for (uint8 i = bits; static_cast<int8>(i) >= 0; i--) {
            //to get the digit, shift the number and by masking select only the 4 LSB bits
            uint8 digit = (static_cast<uint8>(Shift::LogicalRightSafeShift(number, i)) & 0x1u);

            // skips trailing zeros until we encounter the first non zero, or if putTrailingZeros was already set
            /*lint -e(9125) -e(9117) */
            if (!stream.PutC(static_cast<char8>('0' + digit))) {
                //TODO
            }

        }
    }

    // in case of right alignment
    if (padded && leftAligned) {
        for (uint16 i = 0u; i < (maximumSize - numberSize); i++) {
            if (!stream.PutC(' ')) {
                //TODO
            }
        }
    }
    return true;
}

/** @brief Print on a general stream using a specific format.
 * @param stream is a general stream class which implements a PutC() function.
 * @param number is the integer to print.
 * @param format is the desired format.
 * @return true if the format is correct, false otherwise.
 *
 * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
 * Uses notation specified in format.
 * Also respects all relevant format parameters.
 * Respects format.size and number integrity.
 * If not possible output is ? */
template<typename T>
bool IntegerToStreamPrivate(IOBuffer &stream,
                            const T number,
                            const FormatDescriptor &format,
                            uint16 actualBitSize = static_cast<uint16>(sizeof(T) * 8u)) {

    bool ret = false;
// do not use actual Bit Size if binaryPadded is not set
    if (!format.binaryPadded) {
        actualBitSize = 0u;
    }

    if (format.binaryPadded == DecimalNotation) {
        ret = IntegerToStreamDecimalNotation(stream, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, format.fullNotation);
    }
    if (format.binaryPadded == HexNotation) {
        ret = IntegerToStreamExadecimalNotation(stream, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, actualBitSize,
                                                format.fullNotation);
    }
    if (format.binaryPadded == OctalNotation) {
        ret = IntegerToStreamOctalNotation(stream, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, actualBitSize,
                                           format.fullNotation);
    }
    if (format.binaryPadded == BitNotation) {
        ret = IntegerToStreamBinaryNotation(stream, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, actualBitSize,
                                            format.fullNotation);
    }

    return ret;
}

/** @brief Print on a general stream using a specific format.
 * @param stream is a general stream class which implements a PutC() function.
 * @param number is the integer to print.
 * @param format is the desired format.
 * @return true if the format is correct, false otherwise.
 *
 * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
 * Uses notation specified in format.
 * Also respects all relevant format parameters.
 * Respects format.size and number integrity.
 * If not possible output is ? */

static
bool BitSetToStreamPrivate(IOBuffer &stream,
                           uint32 *numberAddress,
                           uint8 numberBitShift,
                           const uint8 numberBitSize,
                           const bool numberIsSigned,
                           const FormatDescriptor &format) {

    bool ret = true;

    // smaller than 16 bit
    if (numberBitSize <= 16u) {

        if (numberBitSize <= 8u) {
            if (numberIsSigned) {
                int8 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }
            }
            else {
                uint8 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }
            }

        }
        else {
            if (numberIsSigned) {
                int16 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }

            }
            else {
                uint16 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }

            }
        }

    }
    else {

        if (numberBitSize <= 32u) {
            if (numberIsSigned) {
                int32 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }
            }
            else {
                uint32 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }
            }

        }
        else {
            if (numberIsSigned) {
                int64 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }
            }
            else {
                uint64 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(stream, destination, format, numberBitSize);
                }
            }
        }
    }

    return ret;
}

bool IntegerToStream(IOBuffer &stream,
                     const uint8 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}
bool IntegerToStream(IOBuffer &stream,
                     const int8 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}
bool IntegerToStream(IOBuffer &stream,
                     const uint16 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}
bool IntegerToStream(IOBuffer &stream,
                     const int16 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}
bool IntegerToStream(IOBuffer &stream,
                     const uint32 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}
bool IntegerToStream(IOBuffer &stream,
                     const int32 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}
bool IntegerToStream(IOBuffer &stream,
                     const uint64 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}
bool IntegerToStream(IOBuffer &stream,
                     const int64 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(stream, number, format);
}

bool BitSetToStream(IOBuffer &stream,
                    uint32 * const numberAddress,
                    const uint8 numberBitShift,
                    const uint8 numberBitSize,
                    const bool numberIsSigned,
                    const FormatDescriptor &format) {
    return BitSetToStreamPrivate(stream, numberAddress, numberBitShift, numberBitSize, numberIsSigned, format);

}

}
