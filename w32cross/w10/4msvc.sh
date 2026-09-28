#!/bin/sh -e

. common/cbase.sh

# Always regenerate this directory to prevent clobbering.
rm -rf "${IVTW_SDKPFX}VC/Tools/MSVC"
mkdir -p "${SDK_MSVCPFX}"

# We need a VCRuntime, and the SDK won't give us a real one.
# Luckily, the VCRuntime is basically libgcc but for VC. It's not even the STL.
# We *can* just write our own.
cp -r notvcruntime/* "${SDK_MSVCPFX}"

# -- FALSE MSVC COMPILATION STARTS HERE --

# Compile for all supported architectures.
# Note that we can only compile DLL versions.
# We do compile debug versions, but they're half-hearted.
for arch in x86 x64 arm64; do
	echo "compiling false MSVC for: $arch"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch"
	mkdir -p "${SDK_MSVCPFX}/lib/$arch"
	# Pure imports are managed here.
	ivtw_import_defs vc14_redist "$arch" "${SDK_MSVCPFX}/lib/$arch"
	# oldnames.lib gets included by default, so we need to have it.
	ivtw_import_defs oldnames "$arch" "${SDK_MSVCPFX}/lib/$arch"

	# Compile CRT0.
	mkdir -p "${SDK_MSVCPFX}/obj/$arch/msvcrt"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch/msvcrtd"
	# Build 'simpler' objects
	for object in crt0_common crt0_gs crt0_app_atexit crt0_dll crt0_dll_nomain; do
		# Versions are mapped here from flags to lib names.
		# Trust me, it's better this way.
		"${IVTW_CL}-$arch" /c /MD "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrt/${object}.obj" "${SDK_MSVCPFX}/src/${object}.c"
		"${IVTW_CL}-$arch" /c /MDd "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrtd/${object}.obj" "${SDK_MSVCPFX}/src/${object}.c"
	done
	for object in crt0_exe_con crt0_exe_gui; do
		"${IVTW_CL}-$arch" /c /MD "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrt/${object}a.obj" "${SDK_MSVCPFX}/src/${object}.c"
		"${IVTW_CL}-$arch" /c /MD /D_UNICODE "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrt/${object}w.obj" "${SDK_MSVCPFX}/src/${object}.c"
		"${IVTW_CL}-$arch" /c /MDd "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrtd/${object}a.obj" "${SDK_MSVCPFX}/src/${object}.c"
		"${IVTW_CL}-$arch" /c /MDd /D_UNICODE "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrtd/${object}w.obj" "${SDK_MSVCPFX}/src/${object}.c"
	done

	# Build C link libraries.
	for version in msvcrt msvcrtd; do
		"${IVTW_LIB}" "/machine:$arch" "${SDK_MSVCPFX}/obj/$arch/$version/"* "/out:${SDK_MSVCPFX}/lib/$arch/$version.lib"
	done

	# Merge VCRuntime libraries to create the one that's needed.
	# We ignore vcruntime140_1 for now, we may never actually end up using it.
	"${IVTW_LIB}" "/machine:$arch" \
	"${SDK_MSVCPFX}/lib/$arch/vcruntime140.lib" \
	"${SDK_MSVCPFX}/lib/$arch/vcruntime140_threads.lib" \
	"/out:${SDK_MSVCPFX}/lib/$arch/vcruntime.lib"

	# Build the C++ link library. Note we can't build debug libs, as we don't have the defs for those STLs.
	"${IVTW_LIB}" "/machine:$arch" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140.lib" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140_1.lib" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140_atomic_wait.lib" \
	"${SDK_MSVCPFX}/lib/$arch/msvcp140_codecvt_ids.lib" \
	"/out:${SDK_MSVCPFX}/lib/$arch/msvcprt.lib"
done

# -- STL --

cp -r "${IVTW_DLPFX}stl16/stl/inc/"* "${SDK_MSVCPFX}/include/"
