#!/bin/sh -e

if [ "$1" != "" ]; then
	if [ "$1" != "-v" ]; then
		echo "chkccl parameter can be be -v or nothing"
		exit 1
	fi
fi

W32CROSS_SDKID=w10

. common/cbase.sh

# We need a VCRuntime, and the SDK won't give us a real one.
# Luckily, the VCRuntime is basically libgcc but for VC. It's not even the STL.
# We *can* just write our own, more or less.

notvcrt/ext/sync.sh

rm -rf "${IVTW_SDKPFX}ucrt/src/notvcrt"
mkdir -p "${IVTW_SDKPFX}ucrt/src"
mkdir -p "${IVTW_SDKPFX}ucrt/include"
mkdir -p "${IVTW_SDKPFX}wsdk/include"

cp -r notvcrt/src "${IVTW_SDKPFX}ucrt/src/notvcrt"

# Note that different headers end up different places.
# Specifically, eh.h and excpt.h are obviously Windows headers in disguise.
# While setjmp/etc. are tied up with the CRT or compiler.
cp notvcrt/include_ext/eh.h "${IVTW_SDKPFX}wsdk/include"
cp notvcrt/include_ext/excpt.h "${IVTW_SDKPFX}wsdk/include"
cp notvcrt/include_ext/setjmp.h "${IVTW_SDKPFX}ucrt/include"
cp notvcrt/include_ext/setjmpex.h "${IVTW_SDKPFX}ucrt/include"
cp notvcrt/include/crtdefs.h "${IVTW_SDKPFX}ucrt/include"
cp notvcrt/include/intrin0.h "${IVTW_SDKPFX}ucrt/include"
cp notvcrt/include/isa_availability.h "${IVTW_SDKPFX}ucrt/include"
cp notvcrt/include/vadefs.h "${IVTW_SDKPFX}ucrt/include"
# All VCRuntime headers go in with the UCRT.
cp notvcrt/include/vcruntime*.h "${IVTW_SDKPFX}ucrt/include"
# This MUST go here since otherwise it clobbers WSDK.
cp notvcrt/include/winnt.h "${IVTW_SDKPFX}ucrt/include"

# -- NotVCRT compilation and lib creation begins here --

rm -rf "${IVTW_SDKPFX}build/notvcrt_obj"
NOTVCRT_OBJDIR="${IVTW_SDKPFX}build/notvcrt_obj"
NOTVCRT_LIBDIR="${IVTW_SDKPFX}ucrt/lib"
NOTVCRT_SRCDIR="${IVTW_SDKPFX}ucrt/src/notvcrt"

# Compile for all supported architectures.
# Note that we can only compile DLL versions.
# We do compile debug versions, but they're half-hearted.
for arch in x86 x64 arm64; do
	echo "compiling: $arch"
	mkdir -p "$NOTVCRT_OBJDIR/$arch"
	mkdir -p "$NOTVCRT_LIBDIR/$arch"

	# Pure imports are managed here.
	ivtw_import_defs vc14_redist "$arch" "$NOTVCRT_LIBDIR/$arch"
	# oldnames.lib gets included by default, so we need to have it.
	ivtw_import_defs oldnames "$arch" "$NOTVCRT_LIBDIR/$arch"

	# Compile CRT0.
	mkdir -p "$NOTVCRT_OBJDIR/$arch/vcruntime"

	# Build 'simpler' objects
	# Notably, this builds the 'ANSI' versions of crt0_exe.
	# The Unicode versions are built in the next pass.
	for object in \
	crt0_common crt0_gs crt0_fltused \
	crt0_dll crt0_dll_nomain \
	crt0_exe_con crt0_exe_gui \
	; do
		# Versions are mapped here from flags to lib names.
		# Trust me, it's better this way.
		"${IVTW_CL}-$arch" "$1" /c /O1 /MD \
		"/Fo$NOTVCRT_OBJDIR/$arch/vcruntime/${object}.obj" \
			"$NOTVCRT_SRCDIR/${object}.c"
	done

	# Unicode versions where relevant.
	for object in crt0_exe_con crt0_exe_gui; do
		"${IVTW_CL}-$arch" /c /O1 /MD /D_UNICODE \
		"/Fo$NOTVCRT_OBJDIR/$arch/vcruntime/${object}_w.obj" \
			"$NOTVCRT_SRCDIR/${object}.c"
	done

	# Build VCRuntime supplement.
	# Note: DO NOT USE /FA1 for actual build it breaks the compile :<
	for object in \
	vcr_typeinfo vcr_new_nothrow \
	vcr_new1 vcr_new2 vcr_new3 vcr_new4 vcr_new5 vcr_new6 vcr_new7 vcr_new8 \
	vcr_del01 vcr_del02 vcr_del03 vcr_del04 vcr_del05 vcr_del06 vcr_del07 vcr_del08 vcr_del09 vcr_del10 vcr_del11 vcr_del12 \
	; do
		# Versions are mapped here from flags to lib names.
		# Trust me, it's better this way.
		# We use /EHs here because std::bad_alloc exception has to be caught in vrc_new.
		"${IVTW_CL}-$arch" /c /O1 /MD /EHs \
		"/Fo$NOTVCRT_OBJDIR/$arch/vcruntime/${object}.obj" \
		   "$NOTVCRT_SRCDIR/${object}.cpp"
	done

	if [ "$arch" = "x86" ]; then
		ARCH_COMPILERRT="${IVTW_SDKPFX}build/compiler-rt/libclang_rt.builtins-i386.a"
	elif [ "$arch" = "x64" ]; then
		ARCH_COMPILERRT="${IVTW_SDKPFX}build/compiler-rt/libclang_rt.builtins-x86_64.a"
	elif [ "$arch" = "arm64" ]; then
		ARCH_COMPILERRT="${IVTW_SDKPFX}build/compiler-rt/libclang_rt.builtins-aarch64.a"
	elif [ "$arch" = "arm" ]; then
		ARCH_COMPILERRT="${IVTW_SDKPFX}build/compiler-rt/libclang_rt.builtins-arm.a"
	fi

	# Merge all libraries to create the One True Library.
	# This merging strategy solves issues with defaultlib not propagating right.
	# There's no real reason not to do this, since we don't have a static vcruntime anyway.
	# We ignore vcruntime140_1 for now, we may never actually end up using it.
	"${IVTW_LIB}" "/machine:$arch" \
	"$NOTVCRT_LIBDIR/$arch/ucrt.lib" \
	"$NOTVCRT_LIBDIR/$arch/vcruntime140.lib" \
	"$NOTVCRT_LIBDIR/$arch/vcruntime140_threads.lib" \
	"$ARCH_COMPILERRT" \
	"$NOTVCRT_OBJDIR/$arch/vcruntime/"*.obj \
	"/out:$NOTVCRT_LIBDIR/$arch/msvcrt.lib"
done
