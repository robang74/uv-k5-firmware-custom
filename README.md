# Status

This repository contains **UNTESTED** code on two serving branches:

- **main**: cherry picking from devel, it compiles but it is untested

- **devel**: seeking for insparation, might not even compile (WARNING)

I have managed to fill the flash at 99.96% and to reduce the size of it, enough to start being interesting.

There is still space for improvements but the mass of untested code is skyrocketing and going to be overwhelming for almost everyone else who wishes leverage it.

Time to share this challenge with other people and spawning more branches like stable, testing, etc. with two small commits to test per week? Wanna join?

I have a **JucJet UV-K5**, it works well with `CHIRP`, `CSP` and [k5prog](https://github.com/robang74/k5prog) but not tried to flash it, yet

Those are interested in this development, please drop an e-mail `roberto.foglietta@gmail.com` 

or get in touch by the Zello channel `UV-K5-mods` - [zello.com/uv-k5-mods](zello.com/uv-k5-mods)

<img src="https://zello.com/channels/qr?name=UV-K5-mods" alt="zello.com/uv-k5-mods" style="float:center; width:145px; heigth:145px;"/>

## Fork scope

This github repository and related ativities are part of a broader project named **Guy Fawkes Starting Pack** started with a [LinkedIn article](https://www.linkedin.com/pulse/guy-fawkes-starting-pack-roberto-a-foglietta-jm9kf) on 15th January 2023 and continously updated after its 1st draft pubblication. It consultation is public but because LinkedIn is a proprietary medium platform that usually prevents people printing their own PDF copy of the article, then it is also available in [PDF](https://drive.google.com/file/d/1DveGE1CbTdQ8xOOhGyEOS_hilnxMM_XK/view) format by a Google Driver link.

## Fork goals

Which is the reason for this fork from the well-known **armel** repo?

1. I do things for me myself and in doing that I am using cloud-based tools

2. which are giving more free-of-charge resources to people who share their work.

3. sharing information is a well-assessed attitude rooted in my 20 year-long private teacher experience

4. and it fits with my idea that Open Source and Bazaar principles are sounding valid,

5. plus I felt that Mr. Armel is more interested in donations and its own business,

6. rather than collecting, organizing and integrating attributes for UV-K5 firmware.

I wish to be clear about the last two points: nothing personal and everyone does what s/he wants. In fact, here I am, forking a githup repo and carrying on my way.

In case you wish to contribute with a commit or a technical information or an insightful grasp or with fixing or highlighting a shortcoming or a constructive critic, you are welcome whoever you are or doing in your own life.

## On my own way

For everybody else, silence is gold plus quick middle fingering is my way to go (cfr. the [bullsh*t asymmetry principle](https://statmodeling.stat.columbia.edu/2019/01/28/bullshit-asymmetry-principle/)) and you are invited to do something the same: take *the bright side of the life* (cit.), and skip the rest.

> I don't have a short temper, I just have a quick reaction to bullshit -- Elizabeth Taylor

<img src="colazione-da-tiffany.jpg" width="420" heigh="153" alt="Audrey Hepburn in Colazione da Tiffany">

Audrey Hepburn in the picture, not Elizabeth Taylor, Ada Lovelace, Hedwig Eva Maria Kiesler (aka Hedy Lamarr) or any other lovely nerdishly Morticia Addams. LOL

Hackers, we will never ask for your permission to do things that we know or learning how to do. Keep it stick in the mind and write it down in a post-it and attach to your PC screen nearby the one you are using for your login password.

## Links

* [UV-K5 Wiki (repo)](https://github.com/ludwich66/Quansheng_UV-K5_Wiki)

* [UV-K5 Wiki (home)](https://github.com/ludwich66/Quansheng_UV-K5_Wiki/wiki)

* [amnemonic (tools)](https://github.com/amnemonic/Quansheng_UV-K5_Firmware)

* [losehu (mod)](https://github.com/losehu/uv-k5-firmware-custom)

* [fagci reborn (FW)](https://github.com/fagci/uvk5-fagci-reborn)

* [fagci modder (tool)](https://github.com/fagci/qs-uvk5-firmware-modder)

* [selevo WebUSBserial (tool)](https://github.com/selevo/WebUsbSerialTerminal)

* [piotr022 playground (mesg)](https://github.com/piotr022/UV_K5_playground)

* [tunas1337 18-to-1300 (fork)](https://github.com/fagci/UV-K5-Modded-Firmwares)

***
***

# Quansheng UV-K5/K6/5R v2.1.27 firmware

**Open Source re-implementation**

This repository is 

* a fork of [Egzumer custom firmware](https://github.com/egzumer/uv-k5-firmware-custom), 

* who was a merge of [OneOfEleven custom firmware](https://github.com/OneOfEleven/uv-k5-firmware-custom) 

* with [fagci spectrum analizer](https://github.com/fagci/uv-k5-firmware-fagci-mod/tree/refactor) 

All is a cloned and customized version of DualTachyon's 

* open firmware found [here](https://github.com/DualTachyon/uv-k5-firmware)

plus some more changes.

## Preamble

> [!NOTE]
> About Chirp, as many others firmwares, you need to use a dedicated driver available on [this repository](https://github.com/armel/uv-k5-chirp-driver).

> [!WARNING]
> THIS FIRMWARE HAS NO BRAIN. PLEASE USE YOUR OWN.
> 
> Use this firmware at your own risk (entirely). There is absolutely no guarantee that it will work in any way shape or form on your radio(s), it may even brick your radio(s), in which case, you'd need to buy another radio.
Anyway, have fun.

> [!CAUTION]
> I recommend to backup your eeprom with [k5prog](https://github.com/sq5bpf/k5prog) before playing with alternative firmwares. It's a good reflex to have.

## Table of Contents

* [My Features](#main-features)
* [Main Features from Egzumer](#main-features-from-egzumer)
* [Manual](#manual)
* [Radio Performance](#radio-performance)
* [Compiler](#compiler)
* [Building](#building)
* [Credits](#credits)
* [Other sources of information](#other-sources-of-information)
* [License](#license)
* [Example changes/updates](#example-changesupdates)

### Main features:

* improve default power settings level: 
    * Low1 to Low5 (<~20mW, ~125mW, ~250mW, ~500mW, ~1W), 
    * Mid ~2W, 
    * High ~5W,
    * User (see SetPwr),
* improve s-meter (IARU recommandations),
* improve bandscope (Spectrum Analyser):
    * add channel name,
    * add save of some spectrum parameters,
* improve UI: 
    * menu index is always visible, even if a menu is selected,
    * s-meter new design (Classic or Tiny), 
    * MAIN ONLY screen mode, 
    * DUAL and CROSS screen mode, 
    * RX blink on VFO RX, 
    * RX LED blink, 
    * Squelch level and Monitor,
    * Step value,
    * CTCSS or DCS value,
    * KeyLock message,
    * last RX,
    * move BatTxt menu from 34/63 to 30/63 (just after BatSave menu 29/63),
    * rename BackLt to BLTime,
    * rename BltTRX to BLTxRx,
    * improve memory channel input,
    * add percent and gauge to Air Copy,
    * improve audio bar,
    * and more...
* new menu entries and changes:
    * add SetPwr menu to set User power (<20mW, 125mW, 250mW, 500mW, 1W, 2W or 5W),
    * add SetPtt menu to set PTT mode (Classic or OnePush),
    * add SetTot menu to set TOT alert (Off, Sound, Visual, All),
    * add SetCtr menu to set contrast (0 to 15),
    * add SetInv menu to set screen in invert mode (Off or On),
    * add SetEot menu to set EOT (End Of Transmission) alert (Off, Sound, Visual, All),
    * add SetMet menu to set s-meter style (Classic or Tiny),
    * add SetLck menu to set what is locked (Keys or Keys + PTT),
    * add SetGui menu to set font size on the VFO baseline (Classic or Tiny),
    * add TXLock menu to open TX on channel,
    * add SetTmr menu to set RX and TX timers (Off or On),
    * add SetOff menu to set the delay before the transceiver goes into deep sleep (Off or 1 minute to 2 hours),
    * rename BatVol menu (52/63) to SysInf, which displays the firmware version in addition to the battery status,
    * improve PonMsg menu,
    * improve BackLt menu,
    * improve TxTOut menu,
    * improve ScnRev menu (CARRIER FAST, CARRIER SLOW, STOP, TIMEOUT from 5 secondes to 2 minutes)
    * add HAM CA F Lock band (for Canadian zone),
    * remove blink and SOS functionality, 
    * remove AM Fix menu (AM Fix is ENABLED by default),
    * add support of 3500mAh battery,
* improve status bar:
    * add SetPtt mode in status bar,
    * change font and bitmaps,
    * move USB icon to left of battery information,
    * add RX and TX timers,
* improve lists and scan lists options:
    * add new list 3,
    * add new list 0 (channel without list...),
    * add new scan lists options,
        * scan list 0 (all channels without list),
        * scan list 1,
        * scan list 2,
        * scan list 3,
        * scan lists [1, 2, 3],
        * scan all (all channels with ou without list),
    * add scan list shortcuts,
* new actions:
    * RX MODE,
    * MAIN ONLY,
    * PTT, 
    * WIDE NARROW,
    * 1750Hz,
* new key combinations:
    * add the F + UP or F + DOWN key combination to dynamically change the Squelch level,
    * add the F + F1 or F + F2 key combination to dynamically change the Step,
    * add F + 8 to quickly switch backlight between BLMin and BLMax on demand (this bypass BackLt strategy),
    * add F + 9 to return to BackLt strategy,
    * add long press on MENU, in * SCAN mode, to temporarily exclude a memory channel,
    * add short press on [0, 1, 2, 3, 4 or 5], in * SCAN mode, to dynamically change scan list.
* many fix:
    * squelch, 
    * s-meter,
    * DTMF overlaying, 
    * scan list 2 ignored, 
    * scan range limit,
    * clean display on startup,
    * no more PWM noise,
    * and more...
* enabled AIR COPY
* disabled ENABLE_DTMF_CALLING,
* disabled SCRAMBLER,
* remove 200Tx, 350Tx and 500Tx,
* unlock TX on all bands needs only to be repeat 3 times,
* code refactoring and many memory optimization,
* and more...

### Main features from Egzumer:
* many of OneOfEleven mods:
   * AM fix, huge improvement in reception quality
   * long press buttons functions replicating F+ action
   * fast scanning
   * channel name editing in the menu
   * channel name + frequency display option
   * shortcut for scan-list assignment (long press `5 NOAA`)
   * scan-list toggle (long press `* Scan` while scanning)
   * configurable button function selectable from menu
   * battery percentage/voltage on status bar, selectable from menu
   * longer backlight times
   * mic bar
   * RSSI s-meter
   * more frequency steps
   * squelch more sensitive
* fagci spectrum analyzer (**F+5** to turn on)
* some other mods introduced by me:
   * SSB demodulation (adopted from fagci)
   * backlight dimming
   * battery voltage calibration from menu
   * better battery percentage calculation, selectable for 1600mAh or 2200mAh
   * more configurable button functions
   * long press MENU as another configurable button
   * better DCS/CTCSS scanning in the menu (`* SCAN` while in RX DCS/CTCSS menu item)
   * Piotr022 style s-meter
   * restore initial freq/channel when scanning stopped with EXIT, remember last found transmission with MENU button
   * reordered and renamed menu entries
   * LCD interference crash fix
   * many others...

## Manual

Up to date manual is available in the [Wiki section](https://github.com/armel/uv-k5-firmware-custom/wiki)

## Radio performance

Please note that the Quansheng UV-Kx radios are not professional quality transceivers, their
performance is strictly limited. The RX front end has no track-tuned band pass filtering
at all, and so are wide band/wide open to any and all signals over a large frequency range.

Using the radio in high intensity RF environments will most likely make reception anything but
easy (AM mode will suffer far more than FM ever will), the receiver simply doesn't have a
great dynamic range, which results in distorted AM audio with stronger RX'ed signals.
There is nothing more anyone can do in firmware/software to improve that, once the RX gain
adjustment I do (AM fix) reaches the hardwares limit, your AM RX audio will be all but
non-existent (just like Quansheng's firmware).
On the other hand, FM RX audio will/should be fine.

But, they are nice toys for the price, fun to play with.

## Compiler

arm-none-eabi GCC version 10.3.1 is recommended, which is the current version on Ubuntu 22.04.03 LTS.
Other versions may generate a flash file that is too big.
You can get an appropriate version from: https://developer.arm.com/downloads/-/gnu-rm

clang may be used but isn't fully supported. Resulting binaries may also be bigger.
You can get it from: https://releases.llvm.org/download.html

## Building

### Github Codespace build method

This is the least demanding option as you don't have to install enything on your computer. All you need is Github account.

1. Go to https://github.com/armel/uv-k5-firmware-custom
1. Click green `Code` button
1. Change tab from `Local` to `Codespace`
1. Click green `Create codespace on main` button

<img src="images/Code_Space_1.png" width=700 />

5. Open `Makefile`
1. Edit build options, save `Makefile` changes
1. Run `./compile-with-docker.sh` in terminal window
1. Open folder `compiled-firmware`
1. Right click `firmware.packed.bin`
1. Click `Download`, now you should have a firmware on your computer that you can proceed to flash on your radio. You can use [online flasher](https://egzumer.github.io/uvtools)

<img src="images/Code_Space_2.png" width=700 />

### Docker build method

If you have docker installed you can use [compile-with-docker.bat](./compile-with-docker.bat) (Windows) or [compile-with-docker.sh](./compile-with-docker.sh) (Linux/Mac), the output files are created in `compiled-firmware` folder. This method gives significantly smaller binaries, I've seen differences up to 1kb, so it can fit more functionalities this way. The challenge can be (or not) installing docker itself.

### Linux Docker installation

1. follow this instrctions [page](https://docs.docker.com/engine/install/ubuntu/) from docker.io

2. run the script `compile-with-docker.sh` or the script `compile-with-docker-all.sh` with `sudo`

Note that the root provilegs are needed just the first time for the first installation (download images) 

or even better `sudo docker build -t ukv5 .` which build the uvk5:latest on the local system.

### Windows environment build method

1. Open windows command line and run:
    ```
    winget install -e -h git.git Python.Python.3.8 GnuWin32.Make
    winget install -e -h Arm.GnuArmEmbeddedToolchain -v "10 2021.10"
    ```
2. Close command line, open a new one and run:
    ```
    pip install --user --upgrade pip
    pip install crcmod
    mkdir c:\projects & cd /D c:/projects
    git clone https://github.com/armel/uv-k5-firmware-custom.git
    ```
3. From now on you can build the firmware by going to `c:\projects\uv-k5-firmware-custom` and running `win_make.bat` or by running a command line:
    ```
    cd /D c:\projects\uv-k5-firmware-custom
    win_make.bat
    ```
4. To reset the repository and pull new changes run (!!! it will delete all your changes !!!):
    ```
    cd /D c:\projects\uv-k5-firmware-custom
    git reset --hard & git clean -fd & git pull
    ```

I've left some notes in the win_make.bat file to maybe help with stuff.

## Credits

Many thanks to various people:

* [Egzumer](https://github.com/egzumer)
* [OneOfEleven](https://github.com/OneOfEleven)
* [DualTachyon](https://github.com/DualTachyon)
* [Mikhail](https://github.com/fagci)
* [Andrej](https://github.com/Tunas1337)
* [Manuel](https://github.com/manujedi)
* @wagner
* @Lohtse Shar
* [@Matoz](https://github.com/spm81)
* @Davide
* @Ismo OH2FTG
* [OneOfEleven](https://github.com/OneOfEleven)
* @d1ced95
* and others I forget

## Other sources of information

[ludwich66 - Quansheng UV-K5 Wiki](https://github.com/ludwich66/Quansheng_UV-K5_Wiki/wiki)<br>
[amnemonic - tools and sources of information](https://github.com/amnemonic/Quansheng_UV-K5_Firmware)

## License

Copyright 2023 Dual Tachyon
https://github.com/DualTachyon

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

## Example changes/updates

Here are a few photos.

|![Main Only and Dual RX Respond](https://github.com/armel/uv-k5-firmware-custom-feat-F4HWN/blob/main/photos/IMG_3291.png)|
|:--:|
| Main Only and Dual RX Respond |


|![Main Only and Dual RX Respond (invert mode)](https://github.com/armel/uv-k5-firmware-custom-feat-F4HWN/blob/main/photos/IMG_3290.png)|
|:--:|
| Main Only and Dual RX Respond (invert mode) |


|![Some new menu entries](https://github.com/armel/uv-k5-firmware-custom-feat-F4HWN/blob/main/photos/IMG_3292.png)|
|:--:|
| Some new menu entries |


|![Main Only and Spectrum Analyzer](https://github.com/armel/uv-k5-firmware-custom-feat-F4HWN/blob/main/photos/IMG_3293.png)|
|:--:|
| Main Only and Spectrum Analyzer |
