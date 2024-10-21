#!/bin/sh
#export DOCKER_DEFAULT_PLATFORM=linux/amd64
IMAGE_NAME="uvk5"
rm "${PWD}/compiled-firmware/*"
docker build -t $IMAGE_NAME .

docker run --rm -v "${PWD}/compiled-firmware/:/app/compiled-firmware" $IMAGE_NAME /bin/bash -c "rm ./compiled-firmware/*; cd /app && make -s \
ENABLE_SPECTRUM=1 \
ENABLE_FMRADIO=0 \
ENABLE_AIRCOPY=1 \
ENABLE_NOAA=0 \
ENABLE_UART=1 \
TARGET=f4hwn.bandscope \
&& cp f4hwn.bandscope* compiled-firmware/"

docker run --rm -v "${PWD}/compiled-firmware:/app/compiled-firmware" $IMAGE_NAME /bin/bash -c "cd /app && make -s \
ENABLE_SPECTRUM=0 \
ENABLE_FMRADIO=1 \
ENABLE_AIRCOPY=1 \
ENABLE_NOAA=0 \
ENABLE_UART=1 \
TARGET=f4hwn.broadcast \
&& cp f4hwn.broadcast* compiled-firmware/"

docker run --rm -v "${PWD}/compiled-firmware:/app/compiled-firmware" $IMAGE_NAME /bin/bash -c "cd /app && make -s \
ENABLE_SPECTRUM=1 \
ENABLE_FMRADIO=1 \
ENABLE_VOX=0 \
ENABLE_AIRCOPY=0 \
ENABLE_AUDIO_BAR=0 \
ENABLE_FEAT_F4HWN_SPECTRUM=0 \
ENABLE_FEAT_F4HWN_SLEEP=0 \
ENABLE_NOAA=0 \
ENABLE_UART=1 \
TARGET=f4hwn.voxless \
&& cp f4hwn.voxless* compiled-firmware/"

docker run --rm -v "${PWD}/compiled-firmware:/app/compiled-firmware" $IMAGE_NAME /bin/bash -c "cd /app && make -s \
ENABLE_FLASHLIGHT=0 \
ENABLE_SPECTRUM=1 \
ENABLE_FMRADIO=1 \
ENABLE_VOX=0 \
ENABLE_AIRCOPY=0 \
ENABLE_AUDIO_BAR=0 \
ENABLE_FEAT_F4HWN_SPECTRUM=1 \
ENABLE_COPY_CHAN_TO_VFO=0 \
ENABLE_FEAT_F4HWN_SLEEP=0 \
ENABLE_FEAT_F4HWN_RX_TX_TIMER=0 \
ENABLE_FEAT_F4HWN_PMR=1 \
ENABLE_NOAA=0 \
ENABLE_UART=1 \
TARGET=f4hwn.bnb-frills \
&& cp f4hwn.bnb-frills* compiled-firmware/"
