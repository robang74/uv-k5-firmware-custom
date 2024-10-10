#!/bin/sh
#export DOCKER_DEFAULT_PLATFORM=linux/amd64
IMAGE_NAME="uvk5"
docker build -t $IMAGE_NAME .
docker run --rm -v "${PWD}/compiled-firmware:/app/compiled-firmware" $IMAGE_NAME /bin/bash -c "rm ./compiled-firmware/*; cd /app && make && cp f4hwn* compiled-firmware/"