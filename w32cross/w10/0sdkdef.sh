# Characterization for Windows SDK 10.0.19041.0, downloadable as a single ISO file.

ISO_PATH="${IVTW_DLPFX}19041.5609.250311-1926.vb_release_svc_im_WindowsSDK.iso"
ISO_URL="https://go.microsoft.com/fwlink/?linkid=2312004"

# For safety reasons, this is duplicated in 4msvc.sh for the rm -rf
SDK_MSVCPFX="${IVTW_SDKPFX}VC/Tools/MSVC"

SDK_CL_ARGS="/winsysroot \"\$W32CROSS_SDKROOT\""

# Run the isoextract stage in isolation.
# Usually this is run during msiextract and then 'cleaned up'.
sdk_isoextract() {
	rm -rf "${IVTW_SDKPFX}build/isoextract"
	mkdir -p "${IVTW_SDKPFX}build/isoextract"
	7z x "-o${IVTW_SDKPFX}build/isoextract" "$ISO_PATH"
}

sdk_build() {
	# These stages prepare the Windows SDK itself.
	ivtw_stage 1download
	ivtw_stage 2msiextract
	ivtw_stage 3treecasefix
	# -- The version of the SDK prepared HERE can be used to build code which doesn't end up involving vcruntime etc. --
	# This stage prepares the 'fake MSVC'.
	ivtw_stage 4msvc
}
