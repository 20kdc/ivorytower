# Characterization for empty dummy SDK.

SDK_VCARCHS="x86 x64 arm64"
SDK_PACKAGES=""

sdk_disclaimer() {
	echo "This SDK downloads NOTHING BECAUSE IT IS A DUMMY SDK."
}

sdk_download() {
	true
}

sdk_build() {
	true
}
