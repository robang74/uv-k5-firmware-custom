/*******************************************************************************
 *
 * Copyright 2023 Dual Tachyon - https://github.com/DualTachyon
 * Copyright 2024 Roberto A. Foglietta <roberto.foglietta@gmail.com>
 *
 *     https://github.com/robang74
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
 */

#ifndef UI_UI_H
#define UI_UI_H

//#include <stdbool.h>
//#include <stdint.h>
//#include <string.h>

#include "misc.h"

void UI_GenerateChannelString(char *pString, const uint8_t Channel);
void UI_GenerateChannelStringEx(char *pString, const bool bShowPrefix, const uint8_t ChannelNumber);
void UI_PrintStringSmallBold(const char *pString, uint8_t Start, uint8_t End, uint8_t Line);
void UI_PrintStringSmallBufferBold(const char *pString, uint8_t * buffer);
void UI_DisplayFrequency(const char *string, uint8_t X, uint8_t Y, bool center);
void UI_DisplayPopup(const char *string);
void UI_DrawPixelBuffer(uint8_t (*buffer)[128], uint8_t x, uint8_t y, bool black);

#ifdef ENABLE_FEAT_F4HWN
void PutPixel(uint8_t x, uint8_t y, bool fill);
void PutPixelStatus(uint8_t x, uint8_t y, bool fill);
void GUI_DisplaySmallest(const char *pString, uint8_t x, uint8_t y, bool statusbar, bool fill);
#endif

void UI_DrawLineBuffer(uint8_t (*buffer)[128], int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool black);
void UI_DrawRectangleBuffer(uint8_t (*buffer)[128], int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool black);

/* RAF: inline functions ******************************************************/

#include "font.h"

void UI_PrintStringSmallNormal(const char *pString, uint8_t Start, uint8_t End, uint8_t Line);
void UI_PrintStringBuffer(const char *pString, uint8_t * buffer, uint32_t char_width, const uint8_t *font);
void UI_PrintStrLenBuffer(const char *pString, uint8_t * buffer, uint32_t char_width, const uint8_t *font, const size_t length);
void UI_PrintMenuString(const char *pString, uint8_t Start, uint8_t End, uint8_t Line, uint8_t Width);
void UI_PrintString(const char *pString, uint8_t Start, uint8_t End, uint8_t Line, uint8_t Width);
void UI_PrintStringSmallBufferNormal(const char *pString, uint8_t * buffer);

void UI_DisplayClear();

#endif
