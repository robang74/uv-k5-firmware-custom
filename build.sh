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
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

trap "printf '\n>>> Ctrl-C catched, exit\n\n' >&2; exit 1" SIGINT

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

name=${0##*/};
issh=${name%.*};
issh=${issh/*sh/SHELL}
if [ "$issh" == "SHELL" ]; then
    echo
    echo "ERROR: this scrip cannot run as source, it needs bash, abort."
    echo
    exit 1
fi >&2

alive=0
if [ "x${1:-1}" == "x-h" -o "x${1:-1}" == "x--help" ]; then
    echo
    echo "$name [-h|-a] [all]"
    echo "  -h: this help"
    echo "  -a: keep alive the container"
    echo
    exit 0
elif [ "x${1:-1}" == "x-a" ]; then
    echo
    echo "WARNING: (-a) container started will remain alive"
    echo '`->access with docker exec -ti uvk5-$id /bin/bash'
    shift
    alive=1
fi

cwd=${0%/*}
cwd=${cwd:-.}
cd $cwd

if [ "$xtimex" == "" ]; then
    function show_git_branch_info() {
        a=$(git branch -vv | sed -ne "s,\* .*\[\([^ ]*\)\] .*,\\1,p")
        set -- $(echo ${a%/*} ${a##*/})
        a=$(git remote -v | sed -ne "s,^$1 *\([^ ]*\) .*(fetch),\\1,p")
        b=${a##*/} ; c=${a/\/$b/}; d=${c##*/}; echo "$d [ ${b/.git/} ] $2"
    }
    {
        date -u +"######## %s (UNIX) - %F %T (UTC) ########"
        which git >/dev/null\
         && echo "######## $(show_git_branch_info 2>/dev/null)"
    } >> build.log
    xtimex=1 time -p $0 "$@" | tee -a build.log
    exit $?
fi

trap "exit 1" SIGINT

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

WRKG_DIR="/app"
DEST_DIR="compiled-firmware/"
LOCL_DIR="${PWD}/${DEST_DIR}/"

DOCK_IMG="uvk5"
DOCK_NME=$DOCK_IMG-$$
FILE_IMG="docker.image"
DOCK_RMT=$(cat $FILE_IMG | cut -d# -f1 | grep .)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

function exec_in_docker() {
    docker exec -w "$WRKG_DIR" $DOCK_NME /bin/bash -c "$@"
}

function make_in_docker() {
    target="$1"; shift
    cmd="
find . -type f -name \*.o -delete;
make -j -s TARGET=${target} $@ && \
mv -f ./${target}* ./${DEST_DIR}/"
    exec_in_docker "$cmd"
}

function dockimgchk() {
    docker images | sed -ne "s,^$1 *\([^ ]*\) .*,$1:\\1,p"
}

function show_firmware_filesize() {
    echo
    cd compiled-firmware/
    files=$(ls -1 *.bin 2>/dev/null | grep -v packed)
    test -n "$files" || return
    echo "Binary firmware files sorted per byte size:"
    echo
    sz=$[60*1024]
    for f in $files; do
        eval $(du -b $f | sed -e "s,\([0-9]*\)\t\(.*\)\.bin,bs=\\1; nm=\\2,")
        ps=$[bs*100]
        pa=$[ps/sz]
        pb=$[((ps*100)/sz)%100]
        test $pb -le 9 && pb="0$pb"
        printf "  $bs  $pa.$pb%%  $nm\n"
    done | sort -k1 -n
    cd - >/dev/null
    echo
}

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

if ! dockimgchk $DOCK_IMG | grep -qe ":latest$"; then
    if [ "$DOCK_RMT" == "" ]; then
        echo
        echo "ERROR: \$DOCK_RMT is null, check $FILE_IMG text file, abort."
        echo
        exit 1 
    fi >&2
    echo
    echo "docker copying $DOCK_RMT in $DOCK_IMG:latest ..."
    echo
    echo "FROM $DOCK_RMT" | docker build -t $DOCK_IMG:latest -
    echo
    if ! dockimgchk $DOCK_IMG | grep -qe ":latest$"; then
        exit 1
    fi
    docker images | grep -we "^uvk5" | tr -s ' '
    echo
fi

echo
echo "docker session $DOCK_NME starting ..."
echo
if [ "$alive" != "0" ]; then
    trap "docker stop $DOCK_NME >/dev/null &" EXIT
fi
if ! docker run --name "$DOCK_NME" -d --rm -w "$WRKG_DIR" \
    -v "${LOCL_DIR}:/app/${DEST_DIR}/" -ti uvk5 cat; then
    exit 1
fi
make distclean >/dev/null 2>&1 #RAF: just in case
if true; then #RAF: there are few but good reasons to use tar here
    tar c -X docker-cptar-exclude.list . | dd bs=1M \
        |  docker cp -a - $DOCK_NME:$WRKG_DIR
else
    docker cp . $DOCK_NME:$WRKG_DIR
fi 2>&1 | grep -v " records "
echo "Building ${1:+ALL }firmware ..."
echo

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

DEFAULT="" # "ENABLE_VOICE=1"

default_first=0

if [ $default_first -ne 0 ]; then
    make_in_docker "f4hwn.default" $DEFAULT

    # RAF: to test the new code is compiling
    ret=$?;
    if [ "$1" != "all" ]; then
        show_firmware_filesize
        exit $ret
    fi
fi

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# no torch & no charge lvl,  need  (bytes)  (*next)  (rundata)
# REDUCE_LOW_MID_TX_POWER      0b  (incl.)     *         *
# FLASHLIGHT                  64b  (avail)     *         *
# FEAT_F4HWN_CA               64b  (excl.)     .         .
# SHOW_CHARGE_LEVEL          104b  (avail)     *         *
# F4HWN_RX_TX_TIMER          148b  (avail)  ( -128)      *
# COPY_CHAN_TO_VFO           192b  (incl.)     *         *
# AUDIO_BAR                  386b  ( -184)               *
# FEAT_F4HWN_SLEEP           512b  ( -440)               *
# VOX                              (-1032)            (avail)
# AIRCOPY                          (-1996)            (-1028)
# -------------------------------------------------------------
# ALL THE OPTIONS                  (-3900)               ?
#
# FEAT_F4HWN_SCREENSHOT            (avail)               *
# FEAT_F4HWN_RESTORE_SCAN          (avail)               *
# FEAT_F4HWN_RESET_CHANNEL         (avail)               *
# -------------------------------------------------------------
# ALL THE OPTIONS                     ?               (-2896)
#
#   text  data   bss    dec  filename
#  58204    20  2748  60972  f4hwn.broadcast
#  59736    52  3116  62904  f4hwn.bandscope
#  59736    52  3116  62904  f4hwn.default
#  60400    52  3060  63512  f4hwn.voxless
#  61336    52  3068  64456  f4hwn.fullflash
#
# Binary firmware files sorted per byte size:
#
#  58224  94.76%  f4hwn.broadcast
#  59788  97.31%  f4hwn.bandscope
#  59788  97.31%  f4hwn.default
#  60452  98.39%  f4hwn.voxless
#  61388  99.91%  f4hwn.fullflash

TVOXLESS="ENABLE_SPECTRUM=1 ENABLE_FMRADIO=1 \
    ENABLE_VOX=0 \
    ENABLE_AIRCOPY=0 \
    ENABLE_AUDIO_BAR=0 \
    ENABLE_FEAT_F4HWN_SLEEP=1 \
    ENABLE_FEAT_F4HWN_SPECTRUM=0 \
    ENABLE_FEAT_F4HWN_RESTORE_SCAN=0 \
    ENABLE_NOAA=0"

make_in_docker "f4hwn.fullflash" "${TVOXLESS} ENABLE_RUNDATA_MEMORY=1 \
    ENABLE_VOX=0 \
    ENABLE_VOICE=0 \
    ENABLE_AIRCOPY=0 \
    ENABLE_AUDIO_BAR=1 \
    ENABLE_FEAT_F4HWN_SLEEP=1 \
    ENABLE_FEAT_F4HWN_SPECTRUM=1 \
    ENABLE_FEAT_F4HWN_RESTORE_SCAN=1 \
    +ENABLE_FEAT_F4HWN_RESET_CHANNEL=1 \
    ENABLE_REDUCE_LOW_MID_TX_POWER=1 \
    ENABLE_FEAT_F4HWN_RX_TX_TIMER=1 \
    ENABLE_FEAT_F4HWN_SCREENSHOT=1 \
    ENABLE_SHOW_CHARGE_LEVEL=1 \
    ENABLE_COPY_CHAN_TO_VFO=1 \
    ENABLE_ROBANG74_UI_MENU=1 \
    ENABLE_FLOCK_SHORT_MENU=1 \
    ENABLE_SIXTH_CHARS_MENU=1 \
    ENABLE_FLASHLIGHT=1"

if [ $default_first -eq 0 ]; then
    # RAF: to test the new code is compiling
    ret=$?;
    if [ "$1" != "all" ]; then
        show_firmware_filesize
        exit $ret
    fi
    make_in_docker "f4hwn.default" $DEFAULT
fi

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

BCOMMON="ENABLE_AIRCOPY=1 ENABLE_NOAA=0"

make_in_docker "f4hwn.bandscope" "ENABLE_SPECTRUM=1 ENABLE_FMRADIO=0 $BCOMMON"
make_in_docker "f4hwn.broadcast" "ENABLE_SPECTRUM=0 ENABLE_FMRADIO=1 $BCOMMON"
make_in_docker "f4hwn.voxless" "${TVOXLESS}"

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

show_firmware_filesize
