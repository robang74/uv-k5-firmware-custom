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

const t_menu_item MenuList[] =
{
/******************************************
 *   text 6 chars,  menu ID
 *****************************************/
    {"SQL",         MENU_SQL           },
    {"W/N",         MENU_W_N           },
    {"STP",         MENU_STEP          },
    {"RxMode",      MENU_TDR           },
    {"Mic",         MENU_MIC           },
#ifdef ENABLE_AUDIO_BAR
    {"MicBar",      MENU_MIC_BAR       },
#endif
#ifdef ENABLE_VOX
    {"VOX",         MENU_VOX           },
#endif
    {"Beep",        MENU_BEEP          },
#ifdef ENABLE_VOICE
    {"Voice",       MENU_VOICE         },
#endif
#ifdef ENABLE_FEAT_F4HWN
    {"TxLCK",       MENU_TX_LOCK       },
#endif
    {"TxPWR",       MENU_TXP           }, // was "TXP"
    {"RxDCS",       MENU_R_DCS         }, // was "R_DCS"
    {"RxCTCS",      MENU_R_CTCS        }, // was "R_CTCS"
    {"TxDCS",       MENU_T_DCS         }, // was "T_DCS"
    {"TxCTCS",      MENU_T_CTCS        }, // was "T_CTCS"
    {"FqDir",       MENU_SFT_D         }, // was "SFT_D"
    {"FqOff",       MENU_OFFSET        }, // was "OFFSET"
#ifndef ENABLE_FEAT_F4HWN
    {"SCR",         MENU_SCR           }, // was "SCR"
#endif
    {"BCL",         MENU_BCL           }, // was "BCL"
    {"CxEx",        MENU_COMPAND       },
    {"AM",          MENU_AM            }, // was "AM"
    {"SCN+1",       MENU_S_ADD1        },
    {"SCN+2",       MENU_S_ADD2        },
    {"SCN+3",       MENU_S_ADD3        },
    {"MEM-CH",      MENU_MEM_CH        }, // was "MEM-CH"
    {"DEL-CH",      MENU_DEL_CH        }, // was "DEL-CH"
    {"NAM-CH",      MENU_MEM_NAME      },

    {"S-LIST",      MENU_S_LIST        },
    {"SCN-L1",      MENU_SLIST1        },
    {"SCN-L2",      MENU_SLIST2        },
    {"SCN-L3",      MENU_SLIST3        },
    {"S-RESM",      MENU_SC_REV        },
#ifdef ENABLE_NOAA
    {"S-NOAA",      MENU_NOAA_S        },
#endif
    {"F1-SHR",      MENU_F1SHRT        },
    {"F1-LNG",      MENU_F1LONG        },
    {"F2-SHR",      MENU_F2SHRT        },
    {"F2-LNG",      MENU_F2LONG        },
    {"M-Long",      MENU_MLONG         },
    {"AUTOLK",      MENU_AUTOLK        }, // was "AUTOLk"
    {"TOT",         MENU_TOT           }, // was "TOT"
    {"BatSav",      MENU_SAVE          }, // was "SAVE"
    {"BatTxt",      MENU_BAT_TXT       },
    {"BatClb",      MENU_BATCAL        }, // battery voltage calibration
    {"BatTyp",      MENU_BATTYP        }, // battery type 1600/2200mAh
    {"Menu",        MENU_MDF           }, // was "MDF"
    {"POnMsg",      MENU_PONMSG        },
    {"ABR",         MENU_ABR           }, // was "ABR"
    {"ABR-",        MENU_ABR_MIN       },
    {"ABR+",        MENU_ABR_MAX       },
    {"ABRx",        MENU_ABR_ON_TX_RX  },
    {"Roger",       MENU_ROGER         },
    {"STE",         MENU_STE           },
    {"RP-STE",      MENU_RP_STE        },
    {"1-Call",      MENU_1_CALL        },
#ifdef ENABLE_ALARM
    {"AlarmT",      MENU_AL_MOD        },
#endif
#ifdef ENABLE_DTMF_CALLING
    {"ANI-ID",      MENU_ANI_ID        },
#endif
    {"PTT-ID",      MENU_PTT_ID        },
    {"UpCode",      MENU_UPCODE        },
    {"DwCode",      MENU_DWCODE        },
    {"D-2Stn",      MENU_D_ST          },
#ifdef ENABLE_DTMF_CALLING
    {"D-Resp",      MENU_D_RSP         },
    {"D-Hold",      MENU_D_HOLD        },
#endif
    {"D-Prel",      MENU_D_PRE         },
#ifdef ENABLE_DTMF_CALLING
    {"D-Decd",      MENU_D_DCD         },
    {"D-List",      MENU_D_LIST        },
#endif
    {"D-Live",      MENU_D_LIVE_DEC    }, // live DTMF decoder
#ifndef ENABLE_FEAT_F4HWN
#ifdef ENABLE_AM_FIX
    {"AM-Fix",      MENU_AM_FIX        }, //RAF,TODO: to move below MENU_AM ?
#endif
#endif
#ifdef ENABLE_FEAT_F4HWN
    {"SysInf",      MENU_VOL           }, // was "VOL"
#else
    {"BatVol",      MENU_VOL           }, // was "VOL"
#endif
#ifdef ENABLE_FEAT_F4HWN
    {"s-PWR",       MENU_SET_PWR       },
    {"s-PTT",       MENU_SET_PTT       },
    {"s-TOT",       MENU_SET_TOT       },
    {"s-EOT",       MENU_SET_EOT       },
#ifdef ENABLE_FEAT_F4HWN_CONTRAST
    {"s-CTR",       MENU_SET_CTR       },
#endif
    {"s-INV",       MENU_SET_INV       },
    {"s-LCK",       MENU_SET_LCK       },
    {"s-MET",       MENU_SET_MET       },
    {"s-GUI",       MENU_SET_GUI       },
    {"s-TMR",       MENU_SET_TMR       },
#ifdef ENABLE_FEAT_F4HWN_SLEEP
    {"s-OFF",       MENU_SET_OFF       },
#endif
#ifdef ENABLE_FEAT_F4HWN_NARROWER
    {"s-NFM",       MENU_SET_NFM       },
#endif
#ifdef ENABLE_FEAT_F4HWN_VOL
    {"s-VOL",       MENU_SET_VOL       },
#endif
#endif
    {"Reset",       MENU_RESET         },
/******************************************
 * hidden menu voices, those below
 * enable pressing PTT + KEY1 + ON
 *****************************************/
    {"FqLck",       MENU_F_LOCK        },
#ifdef ENABLE_F_CAL_MENU
    {"FqClb",       MENU_F_CALI        }, // reference xtal calibration
#endif
#ifndef ENABLE_FEAT_F4HWN
    {"200TX",       MENU_200TX         }, // was "200TX"
    {"350TX",       MENU_350TX         }, // was "350TX"
    {"500TX",       MENU_500TX         }, // was "500TX"
#endif
    {"350EN",       MENU_350EN         }, // was "350EN"
#ifndef ENABLE_FEAT_F4HWN
    {"SCREN",       MENU_SCREN         }, // was "SCREN"
#endif
/******************************************
 * end of list - do NOT delete or move
 *****************************************/
    {"",            0xff               }
};

const uint8_t FIRST_HIDDEN_MENU_ITEM = MENU_F_LOCK;

const char gSubMenu_TXP[8][4] =
{
    "USR",
    "MIN",
    "LW2",
    "LW3",
    "LW4",
    "LW5",
    "MID",
    "MAX"
};

const char gSubMenu_SFT_D[3][2] =
{
    "o"
    "+",
    "-"
};

const char gSubMenu_BAT_TXT[3][2] =
{
    "-",
    "V",
    "%"
};

const char gSubMenu_W_N[2][2] =
{
    "W",
    "N"
};

const char gSubMenu_OFF_ON[2][2] =
{
    "0",
    "1"
};


const char gSubMenu_ROGER[3][4] =
{
    "OFF",
    "RGR",
    "MDC"
};

const char gSubMenu_RESET[2][4] =
{
    "VFO",
    "ALL"
};

// a copy of the text before editing so that
// we can easily test for changes/difference
char    edit_original[17];
char    edit[17];

const char gSubMenu_RX_TX[4][5] =
{
    "NONE",
    "TX",
    "RX",
    "BOTH"
};

const char gSubMenu_BATTYP[3][5] =
{
    "1600",
    "2200",
    "3500"
};

/////

#ifdef ENABLE_VOICE
const char gSubMenu_VOICE[4][3] =
{
    "--",
    "CH",
    "EN"
};
#endif

#ifdef ENABLE_ALARM
const char gSubMenu_AL_MOD[2][5] =
{
    "SITE",
    "TONE"
};
#endif

#ifdef ENABLE_DTMF_CALLING
const char gSubMenu_D_RSP[4][5] =
{
    "NONE",
    "RING",
    "RPLY",
    "BOTH"
};
#endif

#ifdef ENABLE_SCRAMBLER
const char gSubMenu_SCRAMBLER[11][4] =
{
    "OFF",
    "2.6",
    "2.7",
    "2.8",
    "2.9",
    "3.0",
    "3.1",
    "3.2",
    "3.3",
    "3.4",
    "3.5"
};
#endif

#ifdef ENABLE_FEAT_F4HWN

const char gSubMenu_SET_PWR[8][4] = //RAF: 8 instead of 4 saves a byte (!!)
{
    "<20",
    "125",
    "250",
    "500",
    "1W",
    "2W",
    "5W"
};

const char gSubMenu_SET_TOT[4][4] =  // Use by SET_EOT too
{
    "OFF",
    "SND",
    "SEE",
    "ALL"
};

const char gSubMenu_SET_PTT[2][6] =
{
    "PRESS",
    "1PUSH"
};

const char gSubMenu_SET_LCK[2][5] =
{
    "KEYS",
    "+PTT"
};

const char gSubMenu_SET_MET[2][5] =
{
    "TINY",
    "NRML"
};

#ifdef ENABLE_FEAT_F4HWN_NARROWER
const char gSubMenu_SET_NFM[2][5] =
{
   "12.5",
   "6.25"
};
#endif

const char gSubMenu_PONMSG[][5] =
{
#ifdef ENABLE_FEAT_F4HWN
    "ALL",
    "SND",
#else
    "FULL",
#endif
    "MESG",
    "VOLT",
    "NONE"
};

#endif //ENABLE_FEAT_F4HWN

////

const char* const gSubMenu_RXMode[] =
{
    "RX-1\nTX-1",       // TX and RX on main only
    "RX12\nRESP",       // Watch both and respond
    "RX-2\nTX-1",       // TX on main, RX on secondary
    "RX12\nTX-1"        // always TX on main, but RX on both
};

const char* const gSubMenu_MDF[] =
{
    "FREQ",
    "CHNL",
    "NAME",
    "N+FQ"
};

const char* const gSubMenu_PTT_ID[] =
{
    "OFF",
    "UP",
    "DOWN",
    "UP+DOWN",
    "APOLLO\nQUINDAR"
};

const char * const gSubMenu_F_LOCK[] =
{
    "HAM+\n137-174\n400-470",

#ifdef ENABLE_FLOCK_SHORT_MENU
    "HAM-\n144-146\n430-440",
#else
    "FCC\n144-148\n420-450",
#endif

#ifdef ENABLE_FEAT_F4HWN_CA
    "CA\n144-148\n430-450",
#endif

#ifndef ENABLE_FLOCK_SHORT_MENU
    "CE\n144-146\n430-440",
    "GB\n144-148\n430-440",
    "137-174\n400-430",
    "137-174\n400-438",
#endif

#ifdef ENABLE_FEAT_F4HWN_PMR
    "PMR-446",
#endif

#ifdef ENABLE_FEAT_F4HWN_GMRS_FRS_MURS
    "GMRS\nFRS\nMURS",
#endif

    "NONE",
    "ALL",
};

const t_sidefunction gSubMenu_SIDEFUNCTIONS[] =
{
    {"NONE",            ACTION_OPT_NONE},
#ifdef ENABLE_FLASHLIGHT
    {"TORCH",           ACTION_OPT_FLASHLIGHT},
#endif
    {"POWER",           ACTION_OPT_POWER},
    {"MONITOR",         ACTION_OPT_MONITOR},
    {"SCAN",            ACTION_OPT_SCAN},
#ifdef ENABLE_VOX
    {"VOX",             ACTION_OPT_VOX},
#endif
#ifdef ENABLE_ALARM
    {"ALARM",           ACTION_OPT_ALARM},
#endif
#ifdef ENABLE_FMRADIO
    {"FM RX",           ACTION_OPT_FM},
#endif
#ifdef ENABLE_TX1750
    {"1750Hz",          ACTION_OPT_1750},
#endif
    {"LOCK\nKEYPAD",    ACTION_OPT_KEYLOCK},
    {"VFO A/B",         ACTION_OPT_A_B},
    {"VFO MEM",         ACTION_OPT_VFO_MR},
    {"DM MODE",         ACTION_OPT_SWITCH_DEMODUL},
#ifdef ENABLE_BLMIN_TMP_OFF
    {"BL MIN\nTMP OFF",  ACTION_OPT_BLMIN_TMP_OFF},      //BackLight Minimum Temporay OFF
#endif
#ifdef ENABLE_FEAT_F4HWN
    {"RX MODE",         ACTION_OPT_RXMODE},
    {"RX W/N CH",       ACTION_OPT_WN},
    {"MAIN ONLY",       ACTION_OPT_MAINONLY},
    {"PTT",             ACTION_OPT_PTT},
#endif
};
