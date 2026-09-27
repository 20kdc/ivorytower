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
for arch in x86 x64 arm64; do
	echo "compiling false MSVC for: $arch"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch"
	mkdir -p "${SDK_MSVCPFX}/lib/$arch"
	# Pure imports are managed here.
	ivtw_import_defs vc14_redist "$arch" "${SDK_MSVCPFX}/lib/$arch"
	# oldnames.lib gets included by default, so we need to have it.
	ivtw_import_defs oldnames "$arch" "${SDK_MSVCPFX}/lib/$arch"
	# Compile CRT0.
	mkdir -p "${SDK_MSVCPFX}/obj/$arch/libcmt"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch/libcmtd"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch/msvcrt"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch/msvcrtd"
	for object in security_cookie crt0_con_w crt0_con_a crt0_gui_w crt0_gui_a crt0_dll; do
		# Versions are mapped here from flags to lib names.
		# Trust me, it's better this way.
		"${IVTW_CL}-$arch" /c "/MT" "/Fo${SDK_MSVCPFX}/obj/$arch/libcmt/$object.obj" "${SDK_MSVCPFX}/src/$object.c"
		"${IVTW_CL}-$arch" /c "/MTd" "/Fo${SDK_MSVCPFX}/obj/$arch/libcmtd/$object.obj" "${SDK_MSVCPFX}/src/$object.c"
		"${IVTW_CL}-$arch" /c "/MD" "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrt/$object.obj" "${SDK_MSVCPFX}/src/$object.c"
		"${IVTW_CL}-$arch" /c "/MDd" "/Fo${SDK_MSVCPFX}/obj/$arch/msvcrtd/$object.obj" "${SDK_MSVCPFX}/src/$object.c"
	done
	# echo "chk do version"
	for version in libcmt libcmtd msvcrt msvcrtd; do
		"${IVTW_LIB}" "/machine:$arch" "${SDK_MSVCPFX}/obj/$arch/$version/"* "/out:${SDK_MSVCPFX}/lib/$arch/$version.lib"
	done
done

# -- STL --

cp -r "${IVTW_DLPFX}stl16/stl/inc/"* "${SDK_MSVCPFX}/include/"
