/*******************************************************************************
 *
 * Copyright 2024 Armel F4HWN - https://github.com/armel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 *
 * Copyright 2024 Roberto A. Foglietta <roberto.foglietta@gmail.com>
 *
 *     See below in the code the part that has been reworked
 */

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#ifdef ENABLE_UART
#ifdef ENABLE_FEAT_F4HWN_SCREENSHOT 

#include "debugging.h"
#include "driver/st7565.h"

static inline void getScreenShot(void)
{
    char str[4] = "P1\n"; //RAF: the string is here

    LogUart(str);         //RAF: and sent here -4b
    LogUart("128 64\n");  //RAF: w32 alignied +8ch 

    for(uint8_t b = 0; b < 8; b++)
    {
        for(uint8_t i = 0; i < 128; i++)
        {
            sprintf(str, "%d ", ((gStatusLine[i] >> b)  & 0x01));
            LogUart(str);
        }
        LogUart("\n");
    }

    for(uint8_t l = 0; l < 7; l++)
    {
        for(uint8_t b = 0; b < 8; b++)
        {
            for(uint8_t i = 0; i < 128; i++)
            {
                sprintf(str, "%d ", ((gFrameBuffer[l][i] >> b)  & 0x01));
                LogUart(str);
            }
        }
        LogUart("\n");
    }

    LogUart("\n----------------\n");
}

#endif //ENABLE_FEAT_F4HWN_SCREENSHOT
#endif //ENABLE_UART
#endif //SCREENSHOT_H
