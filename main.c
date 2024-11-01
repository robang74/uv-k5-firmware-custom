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

#include <stdint.h>
#include <string.h>

#ifdef ENABLE_AM_FIX
    #include "am_fix.h"
#endif

#include "audio.h"
#include "board.h"
#include "misc.h"
#include "radio.h"
#include "settings.h"
#include "version.h"

#ifdef ENABLE_FEAT_F4HWN
    #ifdef ENABLE_FMRADIO
        #include "app/fm.h"
    #endif
    #ifdef ENABLE_SPECTRUM
        #include "app/spectrum.h"
    #endif
    #include "app/chFrScanner.h"
#endif

#include "app/app.h"
#include "app/dtmf.h"
#include "bsp/dp32g030/gpio.h"
#include "bsp/dp32g030/syscon.h"

#include "driver/backlight.h"
#include "driver/bk4819.h"
#include "driver/gpio.h"
#include "driver/system.h"
#include "driver/systick.h"
#include "driver/eeprom.h"
#ifdef ENABLE_UART
    #include "driver/uart.h"
#endif

#include "helper/battery.h"
#include "helper/boot.h"

#include "ui/lock.h"
#include "ui/welcome.h"
#include "ui/menu.h"

void _putchar(__attribute__((unused)) char c)
{
#ifdef ENABLE_UART
    UART_Send((uint8_t *)&c, 1);
#endif
}

/*******************************************************************************
 *
 * Copyright 2024 Roberto A. Foglietta <roberto.foglietta@gmail.com>
 *
 *     https://github.com/robang74
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 **START(C)**/

#ifdef ENABLE_FEAT_F4HWN
#include "app/bitflags.h"
// RAF: defined extern uint8_t in app/bitflags.h
//      + initialised in main() with gDW and gCB
//      volatile is possibly superflous but ARM.
volatile uint8_t bitflags;
#endif

#ifdef ENABLE_RUNDATA_MEMORY
//RAF: defined in firmware.ld
extern uint32_t __rundata_start;
extern uint32_t __rundata_end;

void __attribute__((section(".preinit_array"))) init_rundata_ramseg(void) {
    uint8_t *start = (uint8_t *)__rundata_start;
    uint8_t *end = (uint8_t *)__rundata_end;

    memset(start, 0, end-start);
    memcpy(gpEeprom, &start, 4);

    #ifdef ENABLE_DTMF_CALLING
    strcpy(gpEeprom->ANI_DTMF_ID, "123");
    strcpy(gpEeprom->KILL_CODE, "ABCD9");
    strcpy(gpEeprom->REVIVE_CODE, "9DCBA");
    #endif
    strcpy(gpEeprom->DTMF_UP_CODE, "12345");
    strcpy(gpEeprom->DTMF_DOWN_CODE, "54321");
}
#endif

/*
 **********************************************************************END(C)**/

void Main(void)
{
    // Enable clock gating of blocks we need
    SYSCON_DEV_CLK_GATE = 0
        | SYSCON_DEV_CLK_GATE_GPIOA_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_GPIOB_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_GPIOC_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_UART1_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_SPI0_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_SARADC_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_CRC_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_AES_BITS_ENABLE
        | SYSCON_DEV_CLK_GATE_PWM_PLUS0_BITS_ENABLE;


    SYSTICK_Init();
    BOARD_Init();

    boot_counter_10ms = 250;   // 2.5 sec

#ifdef ENABLE_UART
    UART_Init();
    UART_Send(UART_Version, strlen(UART_Version));
#endif

    // Not implementing authentic device checks

    memset(gDTMF_String, '-', sizeof(gDTMF_String));
    gDTMF_String[sizeof(gDTMF_String) - 1] = 0;

    BK4819_Init();

    BOARD_ADC_GetBatteryInfo(&gBatteryCurrentVoltage, &gBatteryCurrent);

    SETTINGS_InitEEPROM();

#ifdef ENABLE_FEAT_F4HWN
#ifdef ENABLE_FEAT_F4HWN_SLEEP
    gSetting_set_off = 1;
    gSleepModeCountdown_500ms = 0; //RAF: initialisation in the main? Right!
#endif
    bitflags = 0; //RAF: not nessarly zero, but it is zero by now.
    gDW = gpEeprom->DUAL_WATCH;
    gCB = gpEeprom->CROSS_BAND_RX_TX;
#endif

    SETTINGS_WriteBuildOptions();
    SETTINGS_LoadCalibration();

    RADIO_ConfigureChannel(0, VFO_CONFIGURE_RELOAD);
    RADIO_ConfigureChannel(1, VFO_CONFIGURE_RELOAD);

    RADIO_SelectVfos();

    RADIO_SetupRegisters(true);

    for (unsigned int i = 0; i < ARRAY_SIZE(gBatteryVoltages); i++)
        BOARD_ADC_GetBatteryInfo(&gBatteryVoltages[i], &gBatteryCurrent);

    BATTERY_GetReadings(false);

#ifdef ENABLE_AM_FIX
    AM_fix_init();
#endif

    const BOOT_Mode_t  BootMode = BOOT_GetMode();

    if (BootMode == BOOT_MODE_F_LOCK)
    {

        gF_LOCK = true;            // flag to say include the hidden menu items
        #ifdef ENABLE_FEAT_F4HWN
            gpEeprom->KEY_LOCK = 0;
            SETTINGS_SaveSettings();
            #ifndef ENABLE_VOX
                gMenuCursor = 64; // move to hidden section, fix me if change... !!! Remove VOX and Mic Bar
            #else
                #ifdef ENABLE_FEAT_F4HWN_SLEEP
                    gMenuCursor = 67; // move to hidden section, fix me if change... !!!
                #else
                    gMenuCursor = 66; // move to hidden section, fix me if change... !!!
                #endif
            #endif
            gSubMenuSelection = gSetting_F_LOCK;
        #endif
    }

    // count the number of menu items
    gMenuListCount = UI_MENU_GetMenuSize(gF_LOCK);

    // wait for user to release all butts before moving on
    if (!GPIO_CheckBit(&GPIOC->DATA, GPIOC_PIN_PTT) ||
         KEYBOARD_Poll() != KEY_INVALID ||
         BootMode != BOOT_MODE_NORMAL)
    {   // keys are pressed
        UI_DisplayReleaseKeys();
        BACKLIGHT_TurnOn();

        // 500ms
        for (int i = 0; i < 50;)
        {
            //RAF: if the first check fail stop checking, compiler should do
            //     as code optimisation in speed but not necessarly in space.
            i = GPIO_CheckBit(&GPIOC->DATA, GPIOC_PIN_PTT) ? \
                (KEYBOARD_Poll() == KEY_INVALID ? i + 1 : 0) : 0;
            SYSTEM_DelayMs(10);
        }
        gKeyReading0 = KEY_INVALID;
        gKeyReading1 = KEY_INVALID;
        gDebounceCounter = 0;
    }

    if (!gChargingWithTypeC && gBatteryDisplayLevel == 0)
    {
        FUNCTION_Select(FUNCTION_POWER_SAVE);

        if (gpEeprom->BACKLIGHT_TIME < 61) // backlight is not set to be always on
            BACKLIGHT_TurnOff();    // turn the backlight OFF
        else
            BACKLIGHT_TurnOn();     // turn the backlight ON

        gReducedService = true;
    }
    else
    {
        UI_DisplayWelcome();

        BACKLIGHT_TurnOn();

#ifdef ENABLE_FEAT_F4HWN
        if (gpEeprom->POWER_ON_DISPLAY_MODE != POWER_ON_DISPLAY_MODE_NONE && gpEeprom->POWER_ON_DISPLAY_MODE != POWER_ON_DISPLAY_MODE_SOUND)
#else
        if (gpEeprom->POWER_ON_DISPLAY_MODE != POWER_ON_DISPLAY_MODE_NONE)
#endif
        {   // 2.55 second boot-up screen
            while (boot_counter_10ms > 0)
            {
                if (KEYBOARD_Poll() != KEY_INVALID)
                {   // halt boot beeps
                    boot_counter_10ms = 0;
                    break;
                }
            }
            RADIO_SetupRegisters(true);
        }

#ifdef ENABLE_PWRON_PASSWORD
        if (gpEeprom->POWER_ON_PASSWORD < 1000000)
        {
            bIsInLockScreen = true;
            UI_DisplayLock();
            bIsInLockScreen = false;
        }
#endif

        BOOT_ProcessMode(BootMode);

        GPIO_ClearBit(&GPIOA->DATA, GPIOA_PIN_VOICE_0);

        gUpdateStatus = true;

#ifdef ENABLE_VOICE
        {
            AUDIO_SetVoiceID(0, VOICE_ID_WELCOME);

            //RAF, TODO: can this change during the code execution?
            //           No, then use a #define. Anyway const type.
            const uint8_t Channel = gpEeprom->ScreenChannel[gpEeprom->TX_VFO];
            if (IS_MR_CHANNEL(Channel))
            {
                AUDIO_SetVoiceID(1, VOICE_ID_CHANNEL_MODE);
                AUDIO_SetDigitVoice(2, Channel + 1);
            }
            else if (IS_FREQ_CHANNEL(Channel))
                AUDIO_SetVoiceID(1, VOICE_ID_FREQUENCY_MODE);

            AUDIO_PlaySingleVoice(0);
        }
#endif

#ifdef ENABLE_NOAA
        RADIO_ConfigureNOAA();
#endif
    }

#ifdef ENABLE_FEAT_F4HWN_RESTORE_SCAN
    switch (gpEeprom->CURRENT_STATE) {
        case 1:
            //gScanRangeStart = 0;
            //ACTION_Scan(false);
            gpEeprom->SCAN_LIST_DEFAULT = gpEeprom->CURRENT_LIST;
            CHFRSCANNER_Start(true, SCAN_FWD);
            break;

        case 2:
            gScanRangeStart = gScanRangeStart ? 0 : gTxVfo->pRX->Frequency;
            gScanRangeStop = gpEeprom->VfoInfo[!(gpEeprom->TX_VFO)].freq_config_RX.Frequency;
            if(gScanRangeStart > gScanRangeStop)
            {
                SWAP(gScanRangeStart, gScanRangeStop);
            }
            //ACTION_Scan(false);
            CHFRSCANNER_Start(true, SCAN_FWD);
            break;

        #ifdef ENABLE_FMRADIO
        case 3:
            FM_Start(); // For compiler alignments and paddings...
            break;
        #endif

        #ifdef ENABLE_SPECTRUM
        case 4:
            APP_RunSpectrum(); // For compiler alignments and paddings...
            break;
        #endif

        default:
            // No action for CURRENT_STATE == 0 or other unexpected values
            break;
    }
#endif //ENABLE_FEAT_F4HWN_RESTORE_SCAN

    while (true) {
        APP_Update();

        if (gNextTimeslice) {

            APP_TimeSlice10ms();

            if (gNextTimeslice_500ms) {
                APP_TimeSlice500ms();
            }
        }
    }
}
