#!/bin/sh -e

. stages/sdk.sh

# Get the ISO if we haven't got it already.
if [ ! -e "$ISO_PATH" ]; then
	mkdir -p downloaded
	wget "$ISO_URL" -O "$ISO_PATH"
fi

# Get the STL if we haven't got it already.
if [ ! -e "stl16" ]; then
	git clone --depth=1 --branch vs-2019-16.10 https://github.com/microsoft/STL/ stl16
fi
