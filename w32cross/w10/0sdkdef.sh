# Characterization for Windows SDK 10.0.19041.0, downloadable as a single ISO file.

ISO_PATH="${IVTW_DLPFX}19041.5609.250311-1926.vb_release_svc_im_WindowsSDK.iso"
ISO_URL="https://go.microsoft.com/fwlink/?linkid=2312004"

STL16_PATH="${IVTW_DLPFX}stl16"
STL16_URL="https://github.com/microsoft/STL/"
STL16_BRANCH="vs-2019-16.10"

# For safety reasons, this is duplicated in 4msvc.sh for the rm -rf
SDK_MSVCPFX="${IVTW_SDKPFX}VC/Tools/MSVC/"

SDK_CL_ARGS="/winsysroot \"\$W32CROSS_SDKROOT\""

sdk_disclaimer() {
	echo "This SDK downloads:"
	echo " $ISO_PATH : $ISO_URL"
	echo " $STL16_PATH : $STL16_URL ($STL16_BRANCH)"
	echo "It requires, among other things:"
	echo " clang-cl lld-link llvm-lib"
	echo " 7z msiextract patch"
}

sdk_download() {
	# Get the ISO if we haven't got it already.
	if [ ! -e "$ISO_PATH" ]; then
		mkdir -p "$IVTW_DLPFX"
		wget "$ISO_URL" -O "$ISO_PATH"
	fi

	# Get the STL if we haven't got it already.
	if [ ! -e "$STL16_PATH" ]; then
		git clone --depth=1 --branch "$STL16_BRANCH" "$STL16_URL" "$STL16_PATH"
	fi
}

# Run the isoextract stage in isolation.
# Usually this is run during msiextract and then 'cleaned up'.
sdk_isoextract() {
	rm -rf "${IVTW_SDKPFX}build/isoextract"
	mkdir -p "${IVTW_SDKPFX}build/isoextract"
	7z x "-o${IVTW_SDKPFX}build/isoextract" "$ISO_PATH"
}

sdk_build() {
	# These stages prepare the Windows SDK itself.
	ivtw_stage 2msiextract
	ivtw_stage 3treecasefix
	# -- The version of the SDK prepared HERE can be used to build code which doesn't end up involving vcruntime etc. --
	# This stage prepares the 'fake MSVC'.
	ivtw_stage 4msvc
}
