#!/bin/sh -e

. common/cbase.sh

# Always regenerate this directory to prevent clobbering.
rm -rf "${IVTW_SDKPFX}VC/Tools/MSVC"
mkdir -p "${SDK_MSVCPFX}"

# We need a VCRuntime, and the SDK won't give us a real one.
# Luckily, the VCRuntime is basically libgcc but for VC. It's not even the STL.
# We *can* just write our own.
notvcrt/ext/sync.sh
cp -r notvcrt/include notvcrt/src "${SDK_MSVCPFX}"
cp -r notvcrt/include_ext/* "${SDK_MSVCPFX}include"

# The trick we use for vcruntime_exception.h is contingent on us using the STL headers.
cp -r "${IVTW_DLPFX}stl16/stl/inc/"* "${SDK_MSVCPFX}/include/"

# -- FALSE MSVC COMPILATION STARTS HERE --

# Compile for all supported architectures.
# Note that we can only compile DLL versions.
# We do compile debug versions, but they're half-hearted.
for arch in x86 x64 arm64; do
	echo "compiling: $arch"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch"
	mkdir -p "${SDK_MSVCPFX}/lib/$arch"
	# Pure imports are managed here.
	ivtw_import_defs vc14_redist "$arch" "${SDK_MSVCPFX}/lib/$arch"
	# oldnames.lib gets included by default, so we need to have it.
	ivtw_import_defs oldnames "$arch" "${SDK_MSVCPFX}/lib/$arch"

	# Compile CRT0.
	for debug in "" "d"; do
		# echo "dbg $debug"
		mkdir -p "${SDK_MSVCPFX}/obj/$arch/msvcrt${debug}"
		# Build 'simpler' objects
		for object in crt0_common crt0_gs crt0_app_atexit crt0_dll crt0_dll_nomain; do
			# Versions are mapped here from flags to lib names.
			# Trust me, it's better this way.
			"${IVTW_CL}-$arch" /c /O1 "/MD${debug}" \
			"/Fo${SDK_MSVCPFX}/obj/$arch/msvcrt${debug}/${object}.obj" \
			   "${SDK_MSVCPFX}/src/${object}.c"
		done
		# Dual-mode objects (Unicode and not)
		for object in crt0_exe_con crt0_exe_gui; do
			"${IVTW_CL}-$arch" /c /O1 "/MD${debug}" \
			"/Fo${SDK_MSVCPFX}/obj/$arch/msvcrt${debug}/${object}a.obj" \
			   "${SDK_MSVCPFX}/src/${object}.c"
			"${IVTW_CL}-$arch" /c /O1 "/MD${debug}" /D_UNICODE \
			"/Fo${SDK_MSVCPFX}/obj/$arch/msvcrt${debug}/${object}w.obj" \
			   "${SDK_MSVCPFX}/src/${object}.c"
		done
		"${IVTW_LIB}" "/machine:$arch" \
			 "${SDK_MSVCPFX}/obj/$arch/msvcrt${debug}/"*.obj \
		"/out:${SDK_MSVCPFX}/lib/$arch/msvcrt${debug}.lib"
	done

	# Build VCRuntime supplement.
	# This is NOT CRT0, this is C++ stuff.
	# Note: DO NOT USE /FA1 for actual build it breaks the compile :<
	mkdir -p "${SDK_MSVCPFX}/obj/$arch/vcruntime"
	for object in \
	vcr_typeinfo vcr_new_nothrow \
	vcr_new1 vcr_new2 vcr_new3 vcr_new4 vcr_new5 vcr_new6 vcr_new7 vcr_new8 \
	vcr_del01 vcr_del02 vcr_del03 vcr_del04 vcr_del05 vcr_del06 vcr_del07 vcr_del08 vcr_del09 vcr_del10 vcr_del11 vcr_del12 \
	; do
		# Versions are mapped here from flags to lib names.
		# Trust me, it's better this way.
		# We use /EHs here because std::bad_alloc exception has to be caught in vrc_new.
		"${IVTW_CL}-$arch" /c /O1 /MD /EHs \
		"/Fo${SDK_MSVCPFX}/obj/$arch/vcruntime/${object}.obj" \
		   "${SDK_MSVCPFX}/src/${object}.cpp"
	done

	# Merge VCRuntime libraries to create the one that's needed.
	# We ignore vcruntime140_1 for now, we may never actually end up using it.
	"${IVTW_LIB}" "/machine:$arch" \
	"${SDK_MSVCPFX}/lib/$arch/vcruntime140.lib" \
	"${SDK_MSVCPFX}/lib/$arch/vcruntime140_threads.lib" \
	"${SDK_MSVCPFX}/obj/$arch/vcruntime/"*.obj \
	"/out:${SDK_MSVCPFX}/lib/$arch/vcruntime.lib"

	# Build the C++ link library. Note we can't build debug libs, as we don't have the defs for those STLs.
	"${IVTW_LIB}" "/machine:$arch" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140.lib" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140_1.lib" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140_atomic_wait.lib" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140_codecvt_ids.lib" \
	"/out:${SDK_MSVCPFX}/lib/$arch/msvcprt.lib"
done
