/*******************************************************************************
 *
 * Copyright 2023 Dual Tachyon - https://github.com/DualTachyon
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

#ifdef ENABLE_FMRADIO
    #include "app/fm.h"
#endif
#include "audio.h"
#include "bsp/dp32g030/gpio.h"
#ifdef ENABLE_FMRADIO
    #include "driver/bk1080.h"
#endif
#include "driver/bk4819.h"
#include "driver/gpio.h"
#include "driver/system.h"
#include "driver/systick.h"
#include "functions.h"
#include "misc.h"
#include "settings.h"
#include "ui/ui.h"


BEEP_Type_t gBeepToPlay = BEEP_NONE;

void AUDIO_PlayBeep(BEEP_Type_t Beep)
{

    if (Beep != BEEP_880HZ_60MS_DOUBLE_BEEP &&
        Beep != BEEP_500HZ_60MS_DOUBLE_BEEP &&
        Beep != BEEP_440HZ_500MS &&
#ifdef ENABLE_DTMF_CALLING
        Beep != BEEP_880HZ_200MS &&
        Beep != BEEP_880HZ_500MS &&
#endif
#ifdef ENABLE_FEAT_F4HWN
        Beep != BEEP_400HZ_30MS &&
        Beep != BEEP_500HZ_30MS &&
        Beep != BEEP_600HZ_30MS &&
#endif
       !gpEeprom->BEEP_CONTROL)
        return;

#ifdef ENABLE_AIRCOPY
    if (gScreenToDisplay == DISPLAY_AIRCOPY)
        return;
#endif

    if (gCurrentFunction == FUNCTION_RECEIVE)
        return;

    if (gCurrentFunction == FUNCTION_MONITOR)
        return;

#ifdef ENABLE_FMRADIO
    if (gFmRadioMode)
        BK1080_Mute(true);
#endif

    AUDIO_AudioPathOff();

    if (gCurrentFunction == FUNCTION_POWER_SAVE && gRxIdleMode)
        BK4819_RX_TurnOn();

    SYSTEM_DelayMs(20);

    uint16_t ToneConfig = BK4819_ReadRegister(BK4819_REG_71);

    uint16_t ToneFrequency;
    switch (Beep)
    {
        default:
        case BEEP_NONE:
            ToneFrequency = 220;
            break;
        case BEEP_1KHZ_60MS_OPTIONAL:
            ToneFrequency = 1000;
            break;
        case BEEP_500HZ_60MS_DOUBLE_BEEP_OPTIONAL:
        case BEEP_500HZ_60MS_DOUBLE_BEEP:
            ToneFrequency = 500;
            break;
        case BEEP_440HZ_500MS:
            ToneFrequency = 440;
            break;
        case BEEP_880HZ_60MS_DOUBLE_BEEP:
#ifndef ENABLE_FEAT_F4HWN
        case BEEP_880HZ_200MS:
        case BEEP_880HZ_500MS:
#endif
            ToneFrequency = 880;
            break;
#ifdef ENABLE_FEAT_F4HWN
        case BEEP_400HZ_30MS:
            ToneFrequency = 400;
            break;
        case BEEP_500HZ_30MS:
            ToneFrequency = 500;
            break;
        case BEEP_600HZ_30MS:
            ToneFrequency = 600;
            break;
#endif
    }

    if(Beep == BEEP_400HZ_30MS || Beep == BEEP_500HZ_30MS || Beep == BEEP_600HZ_30MS)
    {
        BK4819_WriteRegister(BK4819_REG_70, BK4819_REG_70_ENABLE_TONE1 | ((1 & 0x7f) << BK4819_REG_70_SHIFT_TONE1_TUNING_GAIN));
    }

    BK4819_PlayTone(ToneFrequency, true);

    SYSTEM_DelayMs(2);

    AUDIO_AudioPathOn();

    SYSTEM_DelayMs(60);

    uint16_t Duration;
    switch (Beep)
    {
        case BEEP_880HZ_60MS_DOUBLE_BEEP:
            BK4819_ExitTxMute();
            SYSTEM_DelayMs(60);
            BK4819_EnterTxMute();
            SYSTEM_DelayMs(20);
            [[fallthrough]];
        case BEEP_500HZ_60MS_DOUBLE_BEEP_OPTIONAL:
        case BEEP_500HZ_60MS_DOUBLE_BEEP:
            BK4819_ExitTxMute();
            SYSTEM_DelayMs(60);
            BK4819_EnterTxMute();
            SYSTEM_DelayMs(20);
            [[fallthrough]];
        case BEEP_1KHZ_60MS_OPTIONAL:
            BK4819_ExitTxMute();
            Duration = 60;
            break;
#ifdef ENABLE_FEAT_F4HWN
        case BEEP_400HZ_30MS:
        case BEEP_500HZ_30MS:
        case BEEP_600HZ_30MS:
            BK4819_ExitTxMute();
            Duration = 30;
            break;
#endif
        case BEEP_440HZ_500MS:
#ifndef ENABLE_FEAT_F4HWN
        case BEEP_880HZ_200MS:
            BK4819_ExitTxMute();
            Duration = 200;
            break;
        case BEEP_880HZ_500MS:
#endif
        default:
            BK4819_ExitTxMute();
            Duration = 500;
            break;
    }

    SYSTEM_DelayMs(Duration);
    BK4819_EnterTxMute();
    SYSTEM_DelayMs(20);

    AUDIO_AudioPathOff();

    SYSTEM_DelayMs(5);
    BK4819_TurnsOffTones_TurnsOnRX();
    SYSTEM_DelayMs(5);
    BK4819_WriteRegister(BK4819_REG_71, ToneConfig);

    if (gEnableSpeaker)
        AUDIO_AudioPathOn();

#ifdef ENABLE_FMRADIO
    if (gFmRadioMode)
        BK1080_Mute(false);
#endif

    if (gCurrentFunction == FUNCTION_POWER_SAVE && gRxIdleMode)
        BK4819_Sleep();

#ifdef ENABLE_VOX
    gVoxResumeCountdown = 80;
#endif

}



#ifdef ENABLE_VOICE /***********************************************************/

uint8_t           gVoiceReadIndex;
uint8_t           gVoiceWriteIndex;
VOICE_ID_t        gVoiceID[8];
VOICE_ID_t        gAnotherVoiceID = VOICE_ID_INVALID;
volatile uint16_t gCountdownToPlayNextVoice_10ms;
volatile bool     gFlagPlayQueuedVoice;

static void AUDIO_PlayVoice(uint8_t VoiceID)
{
    unsigned int i;

    GPIO_SetBit(&GPIOA->DATA, GPIOA_PIN_VOICE_0);
    SYSTEM_DelayMs(20);
    GPIO_ClearBit(&GPIOA->DATA, GPIOA_PIN_VOICE_0);

    for (i = 0; i < 8; i++)
    {
        if ((VoiceID & 0x80U) == 0)
            GPIO_ClearBit(&GPIOA->DATA, GPIOA_PIN_VOICE_1);
        else
            GPIO_SetBit(&GPIOA->DATA, GPIOA_PIN_VOICE_1);

        SYSTICK_DelayUs(1000);
        GPIO_SetBit(&GPIOA->DATA, GPIOA_PIN_VOICE_0);
        SYSTICK_DelayUs(1200);
        GPIO_ClearBit(&GPIOA->DATA, GPIOA_PIN_VOICE_0);
        VoiceID <<= 1;
        SYSTICK_DelayUs(200);
    }
}

/*******************************************************************************
*
* Copyright 2024 Roberto A. Foglietta <roberto.foglietta@gmail.com>
*
*     https://github.com/robang74
*
* Licensed under the Apache License, Version 2.0 (the "License");
*
**START(C)**********************************************************************
*
* RAF: looking at the void delay tables we can see that all the values have a
*      a limited range and in particular:
*
*        1. their minimum can be subtracted to them, say 45 the common one
*
*        2. plus the values can be divided by 5 without any reamin
*
*      after these two operations, the Chinese delay values can be stored into
*      4 bits each, reducing the table size by half. While the English values
*      can be stored in 5 bits and this complicates a little the things.
*
*      Storing 5 bits can be achieved with 76 half-bytes + 10 bytes in which
*      storing the sequence of the 76 most significant bit of each of them.
*
*      The 2 tables encoded in this way occupy 57% of the original size.
*      Unfortunately they require a more elaborated code for retrieving the
*      values encoded and this can increase the total footprint even more.
*
*      Because of all these complication, even when both the CH/EN voices are
*      enabled this encoding enlarge the firmware size of 28 bytes.
*
*      However this attempt can be interesting when those tables would be longer
*      or in case the values can be modified a little and/or rearranged to suite
*      a more efficient encoding. For example using value x8 coded in 4bits will
*      make this approach increase the firmware size by 16 bytes only.
*
*      For example in the Chinese table there are values that are near to these:
*
*         112:1 (14), 104:41 (13), 96:4 (12), 88:3 (11), 72:3 (9), 56:12 (7)
*
*      Considering that these values are expressed in 10ms they might be approx
*      into four values only. This will allows to encoding them in 2 bits and
*      being able to be decoded in a simple way.
*
********************************************************************************
*
* RAF: in the second attempt has been dedicated to compress the table using an
*      approximation that allows to quickly decode the values:
*
*        CH: enc(x) ((x - 50) >> 2); max(abs(err)) = 2, avg(abs(err)) = 1.09
*            min(x) = 50; avg(x) = 88.25; max(x) = 110; avg(abs(err)) = 2.3%
*        EN: dec(x) ((x - 45) >> 3); max(abs(err)) = 4, avg(abs(err)) = 1.84 
*            min(x) = 45; avg(x) = 89.57; max(x) = 155; avg(abs(err)) = 4.5%
*
*      Related times might vary between 450 and 1550 ms which can be single
*      numbers [0-9, aka digit] or short words like "exit" or "point". In this
*      case the human users might not notice (or not too much) a variation in
*      speaking shorter or longer by 5% in English or 2% in Chinese.
*
*      Under these assumptions, it seems reasonable provide an encoding/decoding
"      which allows to store these times into 4 bits instead of 8 bits memcell.
*
*      Current implementation enlarge the firmware with both CH/EN for 4 bytes.
*      Hence, it does not make sense complicate the code for nothing or less.
*
*      However with an variance of {EN9%,CH5%} the tables size 134 -> 48 bytes.
*      In this case the size shrink would be something like 15 bytes or less.
*/

#if 0 //RAF: just for evaluating the impact of a more efficient data encoding

#ifdef ENABLE_VOICE_CHINESE
#define DLYID_MAX_CHINESE 58
static const uint8_t VoiceClipLengthChinese[DLYID_MAX_CHINESE >> 1] =  { 0 };
#endif

#ifdef ENABLE_VOICE_ENGLISH
#define DLYID_MAX_ENGLISH 76
static const uint8_t VoiceClipLengthEnglish[DLYID_MAX_ENGLISH >> 1] = { 0 };
#endif

#define DLY_MIN_ENGLISH 45
#define DLY_MIN_CHINESE 50
#define DLY_DEC(d, id, en) (((id & 1) ? d >> 4 : d & 0x0F) << (en ? 3 : 2))
static uint8_t get_delay_by_id(const uint8_t *p, uint8_t id, bool en)
{
    const uint8_t d = p[id >> 1];
    return DLY_DEC(d, id, en) + (en ? DLY_MIN_ENGLISH : DLY_MIN_CHINESE);
}

#else

#ifdef ENABLE_VOICE_CHINESE
#define DLYID_MAX_CHINESE 58
static const uint8_t VoiceClipLengthChinese[DLYID_MAX_CHINESE] =
{
    0x32, 0x32, 0x32, 0x37, 0x37, 0x32, 0x32, 0x32,
    0x32, 0x37, 0x37, 0x32, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x69, 0x64, 0x69, 0x5A, 0x5F, 0x5F, 0x64,
    0x64, 0x69, 0x64, 0x64, 0x69, 0x69, 0x69, 0x64,
    0x64, 0x6E, 0x69, 0x5F, 0x64, 0x64, 0x64, 0x69,
    0x69, 0x69, 0x64, 0x69, 0x64, 0x64, 0x55, 0x5F,
    0x5A, 0x4B, 0x4B, 0x46, 0x46, 0x69, 0x64, 0x6E,
    0x5A, 0x64,
};
#endif

#ifdef ENABLE_VOICE_ENGLISH
#define DLYID_MAX_ENGLISH 76
static const uint8_t VoiceClipLengthEnglish[DLYID_MAX_ENGLISH] =
{
    0x50, 0x32, 0x2D, 0x2D, 0x2D, 0x37, 0x37, 0x37,
    0x32, 0x32, 0x3C, 0x37, 0x46, 0x46, 0x4B, 0x82,
    0x82, 0x6E, 0x82, 0x46, 0x96, 0x64, 0x46, 0x6E,
    0x78, 0x6E, 0x87, 0x64, 0x96, 0x96, 0x46, 0x9B,
    0x91, 0x82, 0x82, 0x73, 0x78, 0x64, 0x82, 0x6E,
    0x78, 0x82, 0x87, 0x6E, 0x55, 0x78, 0x64, 0x69,
    0x9B, 0x5A, 0x50, 0x3C, 0x32, 0x55, 0x64, 0x64,
    0x50, 0x46, 0x46, 0x46, 0x4B, 0x4B, 0x50, 0x50,
    0x55, 0x4B, 0x4B, 0x32, 0x32, 0x32, 0x32, 0x37,
    0x41, 0x32, 0x3C, 0x37,
};
#endif

#define get_delay_by_id(p, id, en) (p[id])

#endif //RAF: just for evaluating the impact of a more efficient data encoding

/*
 **********************************************************************END(C)**/

void AUDIO_PlaySingleVoice(bool bFlag)
{
    uint8_t VoiceID;
    uint8_t Delay;

    VoiceID = gVoiceID[0];

    if (gpEeprom->VOICE_PROMPT != VOICE_PROMPT_OFF && gVoiceWriteIndex > 0)
    {
#ifdef ENABLE_VOICE_CHINESE
        if (gpEeprom->VOICE_PROMPT == VOICE_PROMPT_CHINESE)
        {
            if (VoiceID < DLYID_MAX_CHINESE)
            {
                Delay = get_delay_by_id(VoiceClipLengthChinese, VoiceID, 0); //RAF
                VoiceID += VOICE_ID_CHI_BASE;
            }
            else
                goto Bailout;
        }
#endif
#ifdef ENABLE_VOICE_ENGLISH
        if (gpEeprom->VOICE_PROMPT == VOICE_PROMPT_ENGLISH)
        {
            if (VoiceID < DLYID_MAX_ENGLISH)
            {
                Delay = get_delay_by_id(VoiceClipLengthEnglish, VoiceID, 1); //RAF
                VoiceID += VOICE_ID_ENG_BASE;
            }
            else
                goto Bailout;
        }
#endif

        if (FUNCTION_IsRx())   // 1of11
            BK4819_SetAF(BK4819_AF_MUTE);

        #ifdef ENABLE_FMRADIO
            if (gFmRadioMode)
                BK1080_Mute(true);
        #endif

        AUDIO_AudioPathOn();

        #ifdef ENABLE_VOX
            gVoxResumeCountdown = 2000;
        #endif

        SYSTEM_DelayMs(5);
        AUDIO_PlayVoice(VoiceID);

        if (gVoiceWriteIndex == 1)
            Delay += 3;

        if (bFlag)
        {
            SYSTEM_DelayMs(Delay * 10);

            if (FUNCTION_IsRx())    // 1of11
                RADIO_SetModulation(gRxVfo->Modulation);

            #ifdef ENABLE_FMRADIO
                if (gFmRadioMode)
                    BK1080_Mute(false);
            #endif

            if (!gEnableSpeaker)
                AUDIO_AudioPathOff();

            gVoiceWriteIndex    = 0;
            gVoiceReadIndex     = 0;

            #ifdef ENABLE_VOX
                gVoxResumeCountdown = 80;
            #endif

            return;
        }

        gVoiceReadIndex                = 1;
        gCountdownToPlayNextVoice_10ms = Delay;
        gFlagPlayQueuedVoice           = false;

        return;
    }

Bailout:
    gVoiceReadIndex  = 0;
    gVoiceWriteIndex = 0;
}

void AUDIO_SetVoiceID(uint8_t Index, VOICE_ID_t VoiceID)
{
    if (Index >= ARRAY_SIZE(gVoiceID))
        return;

    if (Index == 0)
    {
        gVoiceWriteIndex = 0;
        gVoiceReadIndex  = 0;
    }

    gVoiceID[Index] = VoiceID;

    gVoiceWriteIndex++;
}

uint8_t AUDIO_SetDigitVoice(uint8_t Index, uint16_t Value)
{
    uint16_t Remainder;
    uint8_t  Result;
    uint8_t  Count;

    if (Index == 0)
    {
        gVoiceWriteIndex = 0;
        gVoiceReadIndex  = 0;
    }

    Count     = 0;
    Result    = Value / 1000U;
    Remainder = Value % 1000U;
    if (Remainder < 100U)
    {
        if (Remainder < 10U)
            goto Skip;
    }
    else
    {
        Result = Remainder / 100U;
        gVoiceID[gVoiceWriteIndex++] = (VOICE_ID_t)Result;
        Count++;
        Remainder -= Result * 100U;
    }
    Result = Remainder / 10U;
    gVoiceID[gVoiceWriteIndex++] = (VOICE_ID_t)Result;
    Count++;
    Remainder -= Result * 10U;

Skip:
    gVoiceID[gVoiceWriteIndex++] = (VOICE_ID_t)Remainder;

    return Count + 1U;
}

void AUDIO_PlayQueuedVoice(void)
{
    uint8_t VoiceID;
    uint8_t Delay;
    bool    Skip;

    Skip = false;

    if (gVoiceReadIndex != gVoiceWriteIndex && gpEeprom->VOICE_PROMPT != VOICE_PROMPT_OFF)
    {
        VoiceID = gVoiceID[gVoiceReadIndex];
#ifdef ENABLE_VOICE_CHINESE
        if (gpEeprom->VOICE_PROMPT == VOICE_PROMPT_CHINESE)
        {
            if (VoiceID < DLYID_MAX_CHINESE)
            {
                Delay = get_delay_by_id(VoiceClipLengthChinese, VoiceID, 0); //RAF
                VoiceID += VOICE_ID_CHI_BASE;
            }
            else
                Skip = true;
        }
#endif
#ifdef ENABLE_VOICE_ENGLISH
        if (gpEeprom->VOICE_PROMPT == VOICE_PROMPT_ENGLISH)
        {
            if (VoiceID < DLYID_MAX_ENGLISH)
            {
                Delay = get_delay_by_id(VoiceClipLengthEnglish, VoiceID, 1); //RAF
                VoiceID += VOICE_ID_ENG_BASE;
            }
            else
                Skip = true;
        }
#endif
        gVoiceReadIndex++;

        if (!Skip)
        {
            if (gVoiceReadIndex == gVoiceWriteIndex)
                Delay += 3;

            AUDIO_PlayVoice(VoiceID);

            gCountdownToPlayNextVoice_10ms = Delay;
            gFlagPlayQueuedVoice           = false;

            #ifdef ENABLE_VOX
                gVoxResumeCountdown = 2000;
            #endif

            return;
        }
    }

    if (FUNCTION_IsRx())
    {
        RADIO_SetModulation(gRxVfo->Modulation); // 1of11
    }

    #ifdef ENABLE_FMRADIO
        if (gFmRadioMode)
            BK1080_Mute(false);
    #endif

    if (!gEnableSpeaker)
        AUDIO_AudioPathOff();

    #ifdef ENABLE_VOX
        gVoxResumeCountdown = 80;
    #endif

    gVoiceWriteIndex    = 0;
    gVoiceReadIndex     = 0;
}

#endif //ENABLE_VOICE *********************************************************/
