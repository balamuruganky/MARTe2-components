/**
 * @file StringHelper.h
 * @brief Header file for class StringHelper
 * @date 30/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class StringHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGHELPER_H_
#define STRINGHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Implementation of functions to manipulate strings.
 * @details These methods allows the most used and basic functions to manipulate char8 *strings and
 * are particularly useful in the implementation of higher levels string and stream implementations.
 */
class StringHelper {

public:

    /**
     * @brief Concatenates two strings.
     * @param[in,out] destination the string where the source will be appended.
     * @param[in] source the string to be appended to the destination.
     * @return destination.
     */
    static char8 *Concatenate(char8 *destination,
                              const char8 *source);

    /**
     * @brief Concatenates two strings until 'size' chars.
     * @param[in,out] destination the string where the source will be appended.
     * @param[in] source the string to be appended to the destination.
     * @param[in] size is the maximum number of bytes to append.
     * @return destination.
     */
    static char8 *ConcatenateN(char8 *destination,
                               const char8 *source,
                               uint32 size);

    /**
     * @brief Searches for a character in a string.
     * @param[in] string is the source string.
     * @param[in] c is the character to find in string.
     * @return a pointer to the first occurrence of c in the string, NULL if c is not found.
     */
    static const char8 *SearchChar(const char8 *string,
                                   char8 c);

    /**
     * @brief Compares two strings.
     * @param[in] string1 the first string.
     * @param[in] string2 the second string.
     * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
     */
    static int32 Compare(const char8 *string1,
                         const char8 *string2);

    /**
     * @brief Compares two strings for a maximum of 'size' characters.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @param[in] size is the maximum number of characters to compare.
     * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
     */
    static int32 CompareN(const char8 *string1,
                          const char8 *string2,
                          uint32 size);

    /**
     * @brief Copies the source into the destination.
     * @param[out] destination the destination string.
     * @param[in] source the string to copy into the destination.
     * @return true if the source is copied to the destination.
     */
    static bool Copy(char8 *destination,
                     const char8 *source);

    /**
     * @brief Copies the source into the destination for a maximum of 'size' chars.
     * @param[in,out] destination the destination string.
     * @param[in] source the string to copy into the destination.
     * @param[in] size the maximum number of bytes to copy.
     * @return true if the source is copied to the destination.
     */
    static bool CopyN(char8 *destination,
                      const char8 *source,
                      uint32 size);

    /**
     * @brief Returns the index position of the first character in string2 founded in string1 (e.g. "abcde" "12d" returns 3).
     * @param[in] string1 is the source string.
     * @param[in] string2 contains the characters which must be searched in string1.
     * @return the index at the first occurrence of the first character from string2 found in string1.
     * If the string1 or string2 is NULL returns -1.
     * If the string2 is not found in the string1 returns the length of the string1 (the last Index + 1).
     */
    static int32 SearchIndex(const char8 *string1,
                             const char8 *string2);

    /**
     * @brief Returns the length of a string.
     * @param[in] string the source string.
     * @return the length of the string.
     */
    static uint32 Length(const char8 *string);

    /**
     * @brief Returns the pointer of the first character in string1 matched with one of chars in string2.
     * @param[in] string1 is the source string.
     * @param[in] string2 contains the characters which must be searched in string1.
     * @return a pointer to the first occurrence of the first character from string2 contained in string1.
     */
    static const char8 *SearchChars(const char8 *string1,
                                    const char8 *string2);

    /**
     * @brief Returns a pointer to the last occurrence of the character c in the string.
     * @param[in] string the source string.
     * @param[in] c the character to search.
     * @return a pointer to the last occurrence of c in string.
     */
    static const char8 *SearchLastChar(const char8 *string,
                                       char8 c);

    /**
     * @brief Returns a pointer to the first occurrence of substring in string.
     * @param[in] string the source string.
     * @param[in] substring the string which must be searched in string.
     * @return a pointer to the first occurrence of substring in string.
     */
    static const char8 *SearchString(const char8 *string,
                                     const char8 *substring);


    /**
     * @brief Get the token using characters as delimiters.
     * @param[in] string is the string to tokenize.
     * @param[in] delimiter contains character delimiters.
     * @param[out] result is the substring between delimiters.
     * @return a pointer to the next position after the delimiter for a successive tokenize operation or
     * the end of the string if terminator is not found. It returns NULL if at least one of the
     * arguments is NULL.
     */
    static const char8* TokenizeByChars(const char8* const string,
                                        const char8* const delimiter,
                                        char8* const result);

    /**
     * @brief Get the token using a string as delimiter.
     * @param[in] string is the string to tokenize.
     * @param[in] terminator is the string delimiter.
     * @param[out] result is the next position for a successive operation (NULL if terminator is not found).
     * @return a pointer to the next position after the substring for a successive tokenize operation or NULL if the substring is not found.
     */
    static const char8* TokenizeByString(const char8* const string,
                                         const char8* const terminator,
                                         char8* const result);

    /**
     * @brief Gets the substring between two indexes.
     * @param[in] begin is the first index.
     * @param[in] end is the second index.
     * @param[in] string is the string where to search the indexes
     * @param[out] result is the substring.
     * @return true if indexes are valid and the strings are not NULL.
     */
    static bool Substr(const uint32 begin,
                       const uint32 end,
                       const char8* const string,
                       char8* const result);

    /**
     * @brief Fills the string with a constant char.
     * @param[in,out] string to be filled with the char.
     * @param[in] size length of the string in bytes.
     * @param[in] c the char to fill the string.
     * @return true if the string is not null and the size > 0 and the operation succeeds.
     */
    static bool SetChar(char8 *string,
                        const uint32 &size,
                        const char8 &c);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGHELPER_H_ */

