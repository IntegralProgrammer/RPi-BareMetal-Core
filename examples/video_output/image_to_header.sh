#!/bin/bash

INPUT_FILE=$1

[ -z $INPUT_FILE ] && { echo "Please specify an input image file: ./image_to_header.sh input.png"; exit 1; }

convert $INPUT_FILE rgb:image.rgb
xxd -i image.rgb > image.h
rm image.rgb
