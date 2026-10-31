#!/bin/sh -e

setupcore_setup() {
	meson setup --reconfigure "$ITSETUP_DIR/build" "$ITSETUP_DIR"
}

setupcore_compile() {
	meson compile -C "$ITSETUP_DIR/build"
}

setupcore_build() {
	setupcore_setup && setupcore_compile
}
