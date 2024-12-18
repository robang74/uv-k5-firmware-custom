///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2017, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and snprintf implementation, optimized for speed
//        on embedded systems with a very limited resources. These routines are
//        thread safe and reentrant. Use this instead of the bloated standard/
//        newlib printf cause these use malloc for printf.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRINTF_H_
#define _PRINTF_H_

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Tiny snprintf implementation
 */
int _vsnprintf(char *str, size_t size, const char *format, va_list ap);
#define vsnprintf _vsnprintf

#ifndef ENABLE_ROBANG74_SPRINTF_FUNC
/**
 * Output a character to a custom device like UART.
 * This function is declared here only. You have to write your custom implementation somewhere.
 * \param character to output
 */
void _putchar(char character);

/**
 * Tiny printf implementation
 * You have to implement _putchar if you use printf()
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the array, not counting the terminating null character
 */
int printf(const char* format, ...);
#endif

/**
 * Tiny sprintf implementation
 * Due to security reasons YOU SHOULD CONSIDER USING SNPRINTF INSTEAD!
 * \param buffer A pointer to the buffer where to store the formatted string
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the array, not counting the terminating null character
 */
int sprintf(char* buffer, const char* format, ...);

#ifndef ENABLE_ROBANG74_SPRINTF_FUNC
/**
 * Tiny snprintf implementation
 * \param buffer A pointer to the buffer where to store the formatted string
 * \param count The maximum number of characters to store in the buffer, including a terminating null character
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the array, not counting the terminating null character
 */
int snprintf(char* buffer, size_t count, const char* format, ...);
#endif

#ifdef __cplusplus
}
#endif

#endif  // _PRINTF_H_
