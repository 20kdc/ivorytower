#!/bin/sh -e

. common/cbase.sh

# Get the ISO if we haven't got it already.
if [ ! -e "$ISO_PATH" ]; then
	mkdir -p "$IVTW_DLPFX"
	wget "$ISO_URL" -O "$ISO_PATH"
fi

# Get the STL if we haven't got it already.
if [ ! -e "${IVTW_DLPFX}stl16" ]; then
	git clone --depth=1 --branch vs-2019-16.10 https://github.com/microsoft/STL/ "${IVTW_DLPFX}stl16"
fi
