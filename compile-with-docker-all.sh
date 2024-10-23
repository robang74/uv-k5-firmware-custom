#!/bin/bash
#
# Copyright 2024 Roberto A. Foglietta <roberto.foglietta@gmail.com>
#
#     https://github.com/robang74
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#     Unless required by applicable law or agreed to in writing, software
#     distributed under the License is distributed on an "AS IS" BASIS,
#     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#     See the License for the specific language governing permissions and
#     limitations under the License.
#
#export DOCKER_DEFAULT_PLATFORM=linux/amd64

IMAGE_NAME="uvk5"
DEST_DIR="compiled-firmware/"
LOCL_DIR="${PWD}/${DEST_DIR}/"

#sudo rm  -f ${LOCL_DIR}/*
docker build -t $IMAGE_NAME .

function exec_in_docker() {
    docker run --rm -v "${LOCL_DIR}:/app/${DEST_DIR}/" \
	    $IMAGE_NAME /bin/bash -c "$@"
}

function make_in_docker() {
    target="$1"; shift
    exec_in_docker "cd /app && make -s TARGET=${target} $@ \
        && mv -f ./${target}* ./${DEST_DIR}/"
}

exec_in_docker "rm -f ./${DEST_DIR}/*"

TVOXLESS="ENABLE_SPECTRUM=1 ENABLE_FMRADIO=1 \
    ENABLE_VOX=0 \
    ENABLE_AIRCOPY=0 \
    ENABLE_AUDIO_BAR=0 \
    ENABLE_FEAT_F4HWN_SLEEP=0 \
    ENABLE_FEAT_F4HWN_SPECTRUM=0"

# REDUCE_LOW_MID_TX_POWER   0b
# FLASHLIGHT               64b
# FEAT_F4HWN_CA            64b
# SHOW_CHARGE_LEVEL       104b
# F4HWN_RX_TX_TIMER       148b
# COPY_CHAN_TO_VFO        192b
# AUDIO_BAR               386b
# FEAT_F4HWN_SLEEP        512b

make_in_docker "f4hwn.fullflash" "${TVOXLESS} \
    ENABLE_VOX=0 \
    ENABLE_AIRCOPY=0 \
    ENABLE_AUDIO_BAR=0 \
    ENABLE_FEAT_F4HWN_SLEEP=0 \
    ENABLE_FEAT_F4HWN_SPECTRUM=1 \
    ENABLE_REDUCE_LOW_MID_TX_POWER=1 \
    ENABLE_FEAT_F4HWN_RX_TX_TIMER=0 \
    ENABLE_SHOW_CHARGE_LEVEL=0 \
    ENABLE_COPY_CHAN_TO_VFO=1 \
    ENABLE_ROBANG74_UI_MENU=1 \
    ENABLE_FLOCK_SHORT_MENU=1 \
    ENABLE_SIXTH_CHARS_MENU=1 \
    ENABLE_FLASHLIGHT=0"

# RAF: to test the new code is compiling
ret=$?; test "$1" == "x" && exit $ret

make_in_docker "f4hwn.voxless" "${TVOXLESS}"
make_in_docker "f4hwn.bandscope" "ENABLE_SPECTRUM=1 ENABLE_FMRADIO=0"
make_in_docker "f4hwn.broadcast" "ENABLE_SPECTRUM=0 ENABLE_FMRADIO=1"

