/*******************************************************************************
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

#ifndef APP_BITFLAGS_H
#define APP_BITFLAGS_H

extern volatile uint8_t bitflags; // variable instanced in main.c

#define BF_CROSS_BAND  0x01
#define BT_MONITOR_FN  0x02
#define BF_DUAL_WATCH  0x04
#define BF_FLASHLIGHT  0x08
#define BF_MODE_RX_DW  0x10
#define BF_DS_WAKE_UP  0x20

#if 0

inline static void bitset(uint8_t m, bool x) {
    bitflags = x ? (bitflags | m) : (bitflags & ~m);
}
inline static bool bitchk(uint8_t m) { return bitflags & m; }
inline static void bitflp(uint8_t m) { bitflags ^= m; }

#else

#define bitchk(m)    ( bitflags & m )
#define bitset(m, x) { bitflags = x ? (bitflags | m) : (bitflags & ~m); }
#define bitflp(m)    { bitflags ^= m; }

#endif

#endif //APP_BITFLAGS_H
