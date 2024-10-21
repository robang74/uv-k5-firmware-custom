#!/bin/bash
#export DOCKER_DEFAULT_PLATFORM=linux/amd64

IMAGE_NAME="uvk5"
DEST_DIR="compiled-firmware/"
LOCL_DIR="${PWD}/${DEST_DIR}/"
ALWAYSIN="ENABLE_NOAA=0 ENABLE_UART=1"

#sudo rm  -f ${LOCL_DIR}/*
docker build -t $IMAGE_NAME .

function exec_in_docker() {
    docker run --rm -v "${LOCL_DIR}:/app/${DEST_DIR}/" \
	    $IMAGE_NAME /bin/bash -c "$@"
}

function make_in_docker() {
    exec_in_docker "cd /app && DESTDIR=${DEST_DIR} \
	    make -s ${ALWAYSIN} $@ && mv -f ./f4hwn.* ./${DEST_DIR}/"
}

exec_in_docker "rm -f ./${DEST_DIR}/*"

make_in_docker "\
ENABLE_REDUCE_LOW_MID_TX_POWER=1 \
ENABLE_FLASHLIGHT=1 \
ENABLE_SPECTRUM=1 \
ENABLE_FMRADIO=1 \
ENABLE_VOX=0 \
ENABLE_TX1750=1 \
ENABLE_AIRCOPY=0 \
ENABLE_AUDIO_BAR=0 \
ENABLE_FEAT_F4HWN=1 \
ENABLE_FEAT_F4HWN_SPECTRUM=1 \
ENABLE_FEAT_F4HWN_RX_TX_TIMER=0 \
ENABLE_COPY_CHAN_TO_VFO=0 \
ENABLE_FEAT_F4HWN_SLEEP=0 \
ENABLE_FEAT_F4HWN_PMR=0 \
TARGET=f4hwn.fullflash"

make_in_docker "\
ENABLE_SPECTRUM=1 \
ENABLE_FMRADIO=0 \
ENABLE_AIRCOPY=1 \
TARGET=f4hwn.bandscope"

make_in_docker "\
ENABLE_SPECTRUM=0 \
ENABLE_FMRADIO=1 \
ENABLE_AIRCOPY=1 \
TARGET=f4hwn.broadcast"

make_in_docker "\
ENABLE_SPECTRUM=1 \
ENABLE_FMRADIO=1 \
ENABLE_VOX=0 \
ENABLE_AIRCOPY=0 \
ENABLE_AUDIO_BAR=0 \
ENABLE_FEAT_F4HWN_SPECTRUM=0 \
ENABLE_FEAT_F4HWN_SLEEP=0 \
TARGET=f4hwn.voxless"

