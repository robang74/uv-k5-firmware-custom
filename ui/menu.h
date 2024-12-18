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

#ifndef UI_MENU_H
#define UI_MENU_H

//#include <stdbool.h>
//#include <stdint.h>

#include "audio.h"     // VOICE_ID_t
#include "settings.h"

// RAF: menu display area only has room for 6 characters + the end-of-string
//      zero. Avoid to store that zero, will save some space in .data, when
//      as more menu voice as more space is strongly requires, expecially.

#ifdef ENABLE_SIXTH_CHARS_MENU
#define MENU_VOICE_LENGHT 6
#else
#define MENU_VOICE_LENGHT 7
#endif

#ifdef ENABLE_ROBANG74_UI_MENU /////////////////////////////////////////////////

extern const char        gSubMenu_TXP[8][4];
extern const char        gSubMenu_SFT_D[3][2];
extern const char        gSubMenu_BAT_TXT[3][2];
extern const char        gSubMenu_W_N[2][2];
extern const char        gSubMenu_OFF_ON[2][2];

extern const char        gSubMenu_ROGER[3][4];
extern const char        gSubMenu_RESET[2][4];
extern const char        gSubMenu_RX_TX[4][5];

extern char              edit_original[17];
extern char              edit[17];

extern const char        gSubMenu_BATTYP[3][5];

#ifdef ENABLE_VOICE
    extern const char    gSubMenu_VOICE[4][3];
#endif

#ifdef ENABLE_ALARM
    extern const char    gSubMenu_AL_MOD[2][4];
#endif

#ifdef ENABLE_DTMF_CALLING
    extern const char    gSubMenu_D_RSP[4][4];
#endif

#ifdef ENABLE_SCRAMBLER
    extern const char    gSubMenu_SCRAMBLER[11][4];
#endif

#ifdef ENABLE_FEAT_F4HWN
    extern const char    gSubMenu_SET_PWR[8][4];
    extern const char    gSubMenu_SET_TOT[4][4];
    extern const char    gSubMenu_SET_PTT[2][6];
    extern const char    gSubMenu_SET_LCK[2][5];
    extern const char    gSubMenu_SET_MET[2][5];
#ifdef ENABLE_FEAT_F4HWN_NARROWER
    extern const char    gSubMenu_SET_NFM[2][5];
#endif
#endif

#ifdef ENABLE_FEAT_F4HWN
    extern const char    gSubMenu_PONMSG[5][5];
#else
    extern const char    gSubMenu_PONMSG[4][5];
#endif

//RAF: equivalent to the original but explicit
typedef struct __attribute__((__aligned__(1))) {
#else
typedef struct {
#endif //ENABLE_ROBANG74_UI_MENU ///////////////////////////////////////////////
    const char     name[MENU_VOICE_LENGHT];
    const uint8_t  menu_id;
} __attribute__((__packed__)) t_menu_item;

extern const t_menu_item MenuList[];

/** ORIGINAL PART *************************************************************/

enum menu_id_enum : uint8_t
{
    MENU_SQL = 0,
    MENU_STEP,
    MENU_TXP,
    MENU_R_DCS,
    MENU_R_CTCS,
    MENU_T_DCS,
    MENU_T_CTCS,
    MENU_SFT_D,
    MENU_OFFSET,
    MENU_TOT,
    MENU_W_N,
#ifdef ENABLE_SCRAMBLER
    MENU_SCR,
#endif
    MENU_BCL,
#ifdef ENABLE_FEAT_F4HWN
    MENU_TX_LOCK, 
#endif
    MENU_MEM_CH,
    MENU_DEL_CH,
    MENU_MEM_NAME,
    MENU_MDF,
    MENU_SAVE,
#ifdef ENABLE_VOX
    MENU_VOX,
#endif
    MENU_ABR,
    MENU_ABR_ON_TX_RX,
    MENU_ABR_MIN,
    MENU_ABR_MAX,
    MENU_TDR,
    MENU_BEEP,
#ifdef ENABLE_VOICE
    MENU_VOICE,
#endif
    MENU_SC_REV,
    MENU_AUTOLK,
    MENU_S_ADD1,
    MENU_S_ADD2,
    MENU_S_ADD3,
    MENU_STE,
    MENU_RP_STE,
    MENU_MIC,
#ifdef ENABLE_AUDIO_BAR
    MENU_MIC_BAR,
#endif
    MENU_COMPAND,
    MENU_1_CALL,
    MENU_S_LIST,
    MENU_SLIST1,
    MENU_SLIST2,
    MENU_SLIST3,
#ifdef ENABLE_ALARM
    MENU_AL_MOD,
#endif
#ifdef ENABLE_DTMF_CALLING
    MENU_ANI_ID,
#endif
    MENU_UPCODE,
    MENU_DWCODE,
    MENU_PTT_ID,
    MENU_D_ST,
#ifdef ENABLE_DTMF_CALLING
    MENU_D_RSP,
    MENU_D_HOLD,
#endif
    MENU_D_PRE,
#ifdef ENABLE_DTMF_CALLING  
    MENU_D_DCD,
    MENU_D_LIST,
#endif
    MENU_D_LIVE_DEC,
    MENU_PONMSG,
    MENU_ROGER,
    MENU_VOL,
    MENU_BAT_TXT,
    MENU_AM,
#ifdef ENABLE_AM_FIX
    MENU_AM_FIX,
#endif
#ifdef ENABLE_NOAA
    MENU_NOAA_S,
#endif
    MENU_RESET,
    MENU_F_LOCK,
#ifndef ENABLE_FEAT_F4HWN
    MENU_200TX,
    MENU_350TX,
    MENU_500TX,
#endif
    MENU_350EN,
#ifndef ENABLE_FEAT_F4HWN
    MENU_SCREN,
#endif
#ifdef ENABLE_F_CAL_MENU
    MENU_F_CALI,  // reference xtal calibration
#endif
#ifdef ENABLE_FEAT_F4HWN_SLEEP
    MENU_SET_OFF,
#endif
#ifdef ENABLE_FEAT_F4HWN
    MENU_SET_PWR,
    MENU_SET_PTT,
    MENU_SET_TOT,
    MENU_SET_EOT,
    MENU_SET_CTR,
    MENU_SET_INV,
    MENU_SET_LCK,
    MENU_SET_MET,
    MENU_SET_GUI,
    MENU_SET_TMR,
    #ifdef ENABLE_FEAT_F4HWN_NARROWER
        MENU_SET_NFM,
    #endif
    #ifdef ENABLE_FEAT_F4HWN_VOL
        MENU_SET_VOL,
    #endif
#endif
    MENU_BATCAL,  // battery voltage calibration
    MENU_F1SHRT,
    MENU_F1LONG,
    MENU_F2SHRT,
    MENU_F2LONG,
    MENU_MLONG,
    MENU_BATTYP
};

extern const uint8_t FIRST_HIDDEN_MENU_ITEM;

#ifndef ENABLE_ROBANG74_UI_MENU ////////////////////////////////////////////////

extern const char        gSubMenu_TXP[8][6];
extern const char        gSubMenu_SFT_D[3][4];
extern const char        gSubMenu_W_N[2][7];
extern const char        gSubMenu_OFF_ON[2][4];
extern const char        gSubMenu_TOT[11][7];

#ifdef ENABLE_VOICE
    extern const char    gSubMenu_VOICE[3][4];
#endif

#ifdef ENABLE_ALARM
    extern const char    gSubMenu_AL_MOD[2][5];
#endif

#ifdef ENABLE_DTMF_CALLING
    extern const char    gSubMenu_D_RSP[4][11];
#endif

#ifdef ENABLE_FEAT_F4HWN
    extern const char    gSubMenu_SET_PWR[7][6];
    extern const char    gSubMenu_SET_PTT[2][8];
    extern const char    gSubMenu_SET_TOT[4][7];
    extern const char    gSubMenu_SET_LCK[2][9];
    extern const char    gSubMenu_SET_MET[2][8];
#ifdef ENABLE_FEAT_F4HWN_NARROWER
    extern const char    gSubMenu_SET_NFM[2][8];
#endif
#endif

#ifdef ENABLE_FEAT_F4HWN
    extern const char    gSubMenu_PONMSG[5][8];
#else
    extern const char    gSubMenu_PONMSG[4][8];
#endif
extern const char        gSubMenu_ROGER[3][6];
extern const char        gSubMenu_RESET[2][4];
extern const char        gSubMenu_RX_TX[4][6];
extern const char        gSubMenu_BAT_TXT[3][8];
extern const char        gSubMenu_BATTYP[3][9];

#ifdef ENABLE_SCRAMBLER
    extern const char    gSubMenu_SCRAMBLER[][7];
#endif

extern char              edit_original[17];
extern char              edit[17];

#endif //ENABLE_ROBANG74_UI_MENU ///////////////////////////////////////////////

extern const char* const gSubMenu_RXMode[4];
extern const char* const gSubMenu_MDF[4];
extern const char* const gSubMenu_PTT_ID[5];
extern const char* const gSubMenu_F_LOCK[F_LOCK_LEN];

typedef struct {
    char* name;
    uint8_t id;
} t_sidefunction;

extern const t_sidefunction gSubMenu_SIDEFUNCTIONS[];
extern const uint8_t        gSubMenu_SIDEFUNCTIONS_size;

extern int32_t              gSubMenuSelection;
extern uint8_t              gMenuCursor;
extern bool                 gIsInSubMenu;
extern int                  edit_index;

void UI_DisplayMenu(void);
int UI_MENU_GetCurrentMenuId();
uint8_t UI_MENU_GetMenuIdx(uint8_t id);
uint8_t UI_MENU_GetMenuSize(bool lock);

#endif //UI_MENU_H
