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

//#include <assert.h>
//#include <string.h>

#include "app/action.h"
#include "app/app.h"
#include "app/chFrScanner.h"
#include "app/common.h"
#include "app/dtmf.h"
#ifdef ENABLE_FLASHLIGHT
    #include "app/flashlight.h"
#endif
#ifdef ENABLE_FMRADIO
    #include "app/fm.h"
#endif
#include "app/scanner.h"
#include "audio.h"
#include "bsp/dp32g030/gpio.h"
#ifdef ENABLE_FMRADIO
    #include "driver/bk1080.h"
#endif
#include "driver/bk4819.h"
#include "driver/gpio.h"
#include "driver/backlight.h"
#include "functions.h"
#include "misc.h"
#include "settings.h"
#include "ui/inputbox.h"
#include "ui/ui.h"

#ifdef ENABLE_FEAT_F4HWN_SCREENSHOT
  #include "screenshot.h"
#endif

#if defined(ENABLE_FMRADIO)
static void ACTION_Scan_FM(bool bRestart);
#endif

#if defined(ENABLE_ALARM) || defined(ENABLE_TX1750)
static void ACTION_AlarmOr1750(bool b1750);
inline static void ACTION_Alarm() { ACTION_AlarmOr1750(false); }
inline static void ACTION_1750() { ACTION_AlarmOr1750(true); };
#endif

inline static void ACTION_ScanRestart() { ACTION_Scan(true); };

void (*action_opt_table[])(void) = {
    [ACTION_OPT_NONE] = &FUNCTION_NOP,
    [ACTION_OPT_POWER] = &ACTION_Power,
    [ACTION_OPT_MONITOR] = &ACTION_Monitor,
    [ACTION_OPT_SCAN] = &ACTION_ScanRestart,
    [ACTION_OPT_KEYLOCK] = &COMMON_KeypadLockToggle,
    [ACTION_OPT_A_B] = &COMMON_SwitchVFOs,
    [ACTION_OPT_VFO_MR] = &COMMON_SwitchVFOMode,
    [ACTION_OPT_SWITCH_DEMODUL] = &ACTION_SwitchDemodul,

#ifdef ENABLE_FLASHLIGHT
    [ACTION_OPT_FLASHLIGHT] = &ACTION_FlashLight,
#else
    [ACTION_OPT_FLASHLIGHT] = &FUNCTION_NOP,
#endif

#ifdef ENABLE_VOX
    [ACTION_OPT_VOX] = &ACTION_Vox,
#else
    [ACTION_OPT_VOX] = &FUNCTION_NOP,
#endif

#ifdef ENABLE_FMRADIO
    [ACTION_OPT_FM] = &ACTION_FM,
#else
    [ACTION_OPT_FM] = &FUNCTION_NOP,
#endif

#ifdef ENABLE_ALARM
    [ACTION_OPT_ALARM] = &ACTION_Alarm,
#else
    [ACTION_OPT_ALARM] = &FUNCTION_NOP,
#endif

#ifdef ENABLE_TX1750
    [ACTION_OPT_1750] = &ACTION_1750,
#else
    [ACTION_OPT_1750] = &FUNCTION_NOP,
#endif

#ifdef ENABLE_BLMIN_TMP_OFF
    [ACTION_OPT_BLMIN_TMP_OFF] = &ACTION_BlminTmpOff,
#else
    [ACTION_OPT_BLMIN_TMP_OFF] = &FUNCTION_NOP,
#endif

#ifdef ENABLE_FEAT_F4HWN
    [ACTION_OPT_RXMODE] = &ACTION_RxMode,
    [ACTION_OPT_MAINONLY] = &ACTION_MainOnly,
    [ACTION_OPT_PTT] = &ACTION_Ptt,
    [ACTION_OPT_WN] = &ACTION_Wn,
    [ACTION_OPT_BACKLIGHT] = &ACTION_BackLight,
#else
    [ACTION_OPT_RXMODE] = &FUNCTION_NOP,
#endif
};

static_assert(ARRAY_SIZE(action_opt_table) == ACTION_OPT_LEN);

void ACTION_Power(void)
{
    if (++gTxVfo->OUTPUT_POWER > OUTPUT_POWER_HIGH)
        gTxVfo->OUTPUT_POWER = OUTPUT_POWER_LOW1;

    gRequestSaveChannel = 1;

    gRequestDisplayScreen = gScreenToDisplay;

#ifdef ENABLE_VOICE
    gAnotherVoiceID   = VOICE_ID_POWER;
#endif

}

void ACTION_Monitor(void)
{
    if (gCurrentFunction != FUNCTION_MONITOR) { // enable the monitor
        RADIO_SelectVfos();
#ifdef ENABLE_NOAA
        if (IS_NOAA_CHANNEL(gRxVfo->CHANNEL_SAVE) && gIsNoaaMode)
            gNoaaChannel = gRxVfo->CHANNEL_SAVE - NOAA_CHANNEL_FIRST;
#endif
        RADIO_SetupRegisters(true);
        APP_StartListening(FUNCTION_MONITOR);
        return;
    }

    gMonitor = false;

    if (gScanStateDir != SCAN_OFF) {
        gScanPauseDelayIn_10ms = scan_pause_delay_in_1_10ms;
        gScheduleScanListen    = false;
        gScanPauseMode         = true;
    }

#ifdef ENABLE_NOAA
    if (gpEeprom->DUAL_WATCH == DUAL_WATCH_OFF && gIsNoaaMode) {
        gNOAA_Countdown_10ms = NOAA_countdown_10ms;
        gScheduleNOAA        = false;
    }
#endif

    RADIO_SetupRegisters(true);

#ifdef ENABLE_FMRADIO
    if (gFmRadioMode) {
        FM_Start();
        gRequestDisplayScreen = DISPLAY_FM;
    }
    else
#endif
        gRequestDisplayScreen = gScreenToDisplay;
}

void ACTION_Scan(bool bRestart)
{
    (void)bRestart;

#ifdef ENABLE_FMRADIO
    if (gFmRadioMode) {
        ACTION_Scan_FM(bRestart);
        return;
    }
#endif

    if (SCANNER_IsScanning()) {
        return;
    }

    // not scanning
    gMonitor = false;

#ifdef ENABLE_DTMF_CALLING
    DTMF_clear_RX();
#endif
    gDTMF_RX_live_timeout = 0;
    memset(gDTMF_RX_live, 0, sizeof(gDTMF_RX_live));

    RADIO_SelectVfos();

#ifdef ENABLE_NOAA
    if (IS_NOAA_CHANNEL(gRxVfo->CHANNEL_SAVE)) {
        return;
    }
#endif

    GUI_SelectNextDisplay(DISPLAY_MAIN);

    if (gScanStateDir != SCAN_OFF) {
        // already scanning

        if (!IS_MR_CHANNEL(gNextMrChannel)) {
            CHFRSCANNER_Stop();
#ifdef ENABLE_VOICE
            gAnotherVoiceID = VOICE_ID_SCANNING_STOP;
#endif
            return;
        }

        // channel mode. Keep scanning but toggle between scan lists
        gpEeprom->SCAN_LIST_DEFAULT = (gpEeprom->SCAN_LIST_DEFAULT + 1) % 6;
#ifdef ENABLE_FEAT_F4HWN_RESTORE_SCAN
        SETTINGS_WriteCurrentState();
#endif

        // jump to the next channel
        CHFRSCANNER_Start(false, gScanStateDir);
        gScanPauseDelayIn_10ms = 1;
        gScheduleScanListen    = false;
    } else {
        // start scanning
        CHFRSCANNER_Start(true, SCAN_FWD);

#ifdef ENABLE_VOICE
        AUDIO_SetVoiceID(0, VOICE_ID_SCANNING_BEGIN);
        AUDIO_PlaySingleVoice(true);
#endif

        // clear the other vfo's rssi level (to hide the antenna symbol)
        gVFO_RSSI_bar_level[(gpEeprom->RX_VFO + 1) & 1U] = 0;

        // let the user see DW is not active
        gDualWatchActive = false;
    }

    gUpdateStatus = true;
}


void ACTION_SwitchDemodul(void)
{
    gRequestSaveChannel = 1;

    gTxVfo->Modulation++;

    if(gTxVfo->Modulation == MODULATION_UKNOWN)
        gTxVfo->Modulation = MODULATION_FM;
}

/*******************************************************************************
 *
 * Copyright 2023 Dual Tachyon - https://github.com/DualTachyon
 * Copyright 2024 Roberto A. Foglietta <roberto.foglietta@gmail.com>
 *
 *     https://github.com/robang74
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 **START(C)**/

void ACTION_Handle(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld)
{
    if (gScreenToDisplay == DISPLAY_MAIN && gDTMF_InputMode)
    {
        // entering DTMF code
        gPttWasReleased = true;

        if (Key != KEY_SIDE1 || bKeyHeld || !bKeyPressed)
            return;

        // side1 btn pressed
        gBeepToPlay = BEEP_1KHZ_60MS_OPTIONAL;
        gRequestDisplayScreen = DISPLAY_MAIN;

        if (gDTMF_InputBox_Index <= 0) {
            // turn off DTMF input box if no codes left
            gDTMF_InputMode = false;
            return;
        }

        // DTMF codes are in the input box
        gDTMF_InputBox[--gDTMF_InputBox_Index] = '-'; // delete one code

#ifdef ENABLE_VOICE
        gAnotherVoiceID = VOICE_ID_CANCEL;
#endif
        return;
    }

    if (!bKeyHeld && bKeyPressed) // button pushed
        return;

    // held or released beyond this point

    if(!(bKeyHeld && !bKeyPressed)) // don't beep on released after hold
        gBeepToPlay = BEEP_1KHZ_60MS_OPTIONAL;


    //RAF: using an array, saved 4 bytes.
    enum ACTION_OPT_t func[2] = { ACTION_OPT_NONE };
    switch(Key) {
        case KEY_SIDE1:
        //RAF,TODO: these variables are not const, hence not in capital
            func[0]  = gpEeprom->KEY_1_SHORT_PRESS_ACTION;
            func[1]  = gpEeprom->KEY_1_LONG_PRESS_ACTION;
            break;
        case KEY_SIDE2:
            func[0]  = gpEeprom->KEY_2_SHORT_PRESS_ACTION;
            func[1]  = gpEeprom->KEY_2_LONG_PRESS_ACTION;
            break;
        case KEY_MENU:
            func[1]  = gpEeprom->KEY_M_LONG_PRESS_ACTION;
            break;
        default:
            break;
    }

    if(bKeyHeld || bKeyPressed) // held
    {
#ifdef ENABLE_FEAT_F4HWN_SCREENSHOT
        getScreenShot();
        if (!bKeyPressed) //ignore release if held
            return;
#endif
        func[0] = func[1];
    }

    action_opt_table[func[0]]();
}

/*
 **********************************************************************END(C)**/

#ifdef ENABLE_FMRADIO

void ACTION_FM(void)
{
    if (gCurrentFunction != FUNCTION_TRANSMIT && gCurrentFunction != FUNCTION_MONITOR)
    {
        gInputBoxIndex = 0;

        if (gFmRadioMode) {
            FM_TurnOff();
            gFlagReconfigureVfos  = true;
            gRequestDisplayScreen = DISPLAY_MAIN;

#ifdef ENABLE_VOX
            gVoxResumeCountdown = 80;
#endif
            return;
        }

        gMonitor = false;

        RADIO_SelectVfos();
        RADIO_SetupRegisters(true);

        FM_Start();

        gRequestDisplayScreen = DISPLAY_FM;
    }
}

static void ACTION_Scan_FM(bool bRestart)
{
    if (FUNCTION_IsRx())
        return;

    GUI_SelectNextDisplay(DISPLAY_FM);

    gMonitor = false;

    if (gFM_ScanState != FM_SCAN_OFF) {
        FM_PlayAndUpdate();

#ifdef ENABLE_VOICE
        gAnotherVoiceID = VOICE_ID_SCANNING_STOP;
#endif
        return;
    }

    uint16_t freq;

    if (bRestart) {
        gFM_AutoScan = true;
        gFM_ChannelPosition = 0;
        FM_EraseChannels();
        freq = BK1080_GetFreqLoLimit(gpEeprom->FM_Band);
    } else {
        gFM_AutoScan = false;
        gFM_ChannelPosition = 0;
        freq = gpEeprom->FM_FrequencyPlaying;
    }

    BK1080_GetFrequencyDeviation(freq);
    FM_Tune(freq, 1, bRestart);

#ifdef ENABLE_VOICE
    gAnotherVoiceID = VOICE_ID_SCANNING_BEGIN;
#endif

}

#endif

#if defined(ENABLE_ALARM) || defined(ENABLE_TX1750)

static void ACTION_AlarmOr1750(const bool b1750)
{

    if(gpEeprom->KEY_LOCK && gpEeprom->KEY_LOCK_PTT)
        return;

    #if defined(ENABLE_ALARM)
        const AlarmState_t alarm_mode = (gpEeprom->ALARM_MODE == ALARM_MODE_TONE) ? ALARM_STATE_TXALARM : ALARM_STATE_SITE_ALARM;
        gAlarmRunningCounter = 0;
    #endif

    #if defined(ENABLE_ALARM) && defined(ENABLE_TX1750)
        gAlarmState = b1750 ? ALARM_STATE_TX1750 : alarm_mode;
    #elif defined(ENABLE_ALARM)
        gAlarmState = alarm_mode;
    #else
        gAlarmState = ALARM_STATE_TX1750;
    #endif

    (void)b1750;
    gInputBoxIndex = 0;

    gFlagPrepareTX = gAlarmState != ALARM_STATE_OFF;

    if (gScreenToDisplay != DISPLAY_MENU)     // 1of11 .. don't close the menu
        gRequestDisplayScreen = DISPLAY_MAIN;
}

#endif

#ifdef ENABLE_VOX

void ACTION_Vox(void)
{
    gpEeprom->VOX_SWITCH = !gpEeprom->VOX_SWITCH;
    gRequestSaveSettings = true;
    gFlagReconfigureVfos = true;
    gUpdateStatus        = true;

    #ifdef ENABLE_VOICE
        gAnotherVoiceID  = VOICE_ID_VOX;
    #endif
}

#endif

#ifdef ENABLE_BLMIN_TMP_OFF

void ACTION_BlminTmpOff(void)
{
    if(++gpEeprom->BACKLIGHT_MIN_STAT == BLMIN_STAT_UNKNOWN) {
        gpEeprom->BACKLIGHT_MIN_STAT = BLMIN_STAT_ON;
        BACKLIGHT_SetBrightness(gpEeprom->BACKLIGHT_MIN);
    } else {
        BACKLIGHT_SetBrightness(0);
    }
}

#endif

#ifdef ENABLE_FEAT_F4HWN 

void ACTION_Update(void)
{
    gSaveRxMode          = false;
    gFlagReconfigureVfos = true;
    gUpdateStatus        = true;
}

void ACTION_Ptt(void)
{
    gSetting_set_ptt_session = !gSetting_set_ptt_session;
}

void ACTION_Wn(void)
{
    bool chbw;

    if(FUNCTION_IsRx()) {
        gRxVfo->CHANNEL_BANDWIDTH = !gRxVfo->CHANNEL_BANDWIDTH;
        chbw = gRxVfo->CHANNEL_BANDWIDTH;
    } else {
        gTxVfo->CHANNEL_BANDWIDTH = !gTxVfo->CHANNEL_BANDWIDTH;
        chbw = gTxVfo->CHANNEL_BANDWIDTH;
    }

    BK4819_SetFilterBandwidth(chbw,
#ifdef ENABLE_AM_FIX
            true
#else
            false
#endif
        );
}

void ACTION_BackLight(void)
{
    if(gBackLight)
    {
        gpEeprom->BACKLIGHT_TIME = gBacklightTimeOriginal;
    }
    gBackLight = false;
    BACKLIGHT_TurnOn();
}

void ACTION_BackLightOnDemand(void)
{
    if(gBackLight == false)
    {
        gBacklightTimeOriginal = gpEeprom->BACKLIGHT_TIME;
        gpEeprom->BACKLIGHT_TIME = 61;
        gBackLight = true;
    }
    else
    if(gBacklightBrightnessOld == gpEeprom->BACKLIGHT_MAX)
    {
        gpEeprom->BACKLIGHT_TIME = 0;
    }
    else
    {
        gpEeprom->BACKLIGHT_TIME = 61;
    }
    
    BACKLIGHT_TurnOn();
}

/*******************************************************************************
 *
 * Copyright 2023 Dual Tachyon - https://github.com/DualTachyon
 * Copyright 2024 Roberto A. Foglietta <roberto.foglietta@gmail.com>
 *
 *     https://github.com/robang74
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 **START(C)**/
 
#include "bitflags.h"

void ACTION_RxMode(void)
{
    if(bitchk(BF_MODE_RX_DW))
        gpEeprom->CROSS_BAND_RX_TX = !gpEeprom->CROSS_BAND_RX_TX;
    else
        gpEeprom->DUAL_WATCH = !gpEeprom->DUAL_WATCH;
    bitflp(BF_MODE_RX_DW);

    ACTION_Update();
}

void ACTION_MainOnly(void)
{
    if(bitchk(BT_MONITOR_FN)) {
        uint8_t txvfo;
        //RAF: can we use here the value gpEeprom->TX_VFO + 1?
        //     if this works, then we saved a whole bit! ;-)
        txvfo = (gpEeprom->TX_VFO & ~3) ? 0 : gpEeprom->TX_VFO + 1;
        gpEeprom->DUAL_WATCH = bitchk(BF_DUAL_WATCH) ? txvfo : 0;
        //RAF: can we use here the value gpEeprom->TX_VFO + 1?
        //     if this works, then we saved a whole bit! ;-)
        gpEeprom->CROSS_BAND_RX_TX = bitchk(BF_CROSS_BAND) ? txvfo : 0;
        //RAF: at this point these two variable have the same
        //     if this is (almost) true, a tristate is enough.
    } else {
        //RAF: gpEeprom->CROSS_BAND_RX_TX ?= _A _B or (TX_VFO + 1)
        //     allowed values: 0 (OFF), 1 (A), 2 (B) only.
        //     if gpEeprom->TX_VFO remains informative then
        //     the following instruction is superfluous.
        bitset(BF_CROSS_BAND, gpEeprom->CROSS_BAND_RX_TX);
        //RAF: gpEeprom->DUAL_WATCH ?= _A _B or (TX_VFO + 1)
        //     allowed values: 0 (OFF), 1 (A), 2 (B) only.
        //     if gpEeprom->TX_VFO remains informative then
        //     the following instruction is superfluous.
        bitset(BF_DUAL_WATCH, gpEeprom->DUAL_WATCH);
        gpEeprom->CROSS_BAND_RX_TX = 0;
        gpEeprom->DUAL_WATCH = 0;
    }
    bitflp(BT_MONITOR_FN);

    ACTION_Update();
}

/*
 **********************************************************************END(C)**/

#endif
