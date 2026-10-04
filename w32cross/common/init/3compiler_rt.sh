#!/bin/sh -e

. common/cbase.sh

# -- compiler-rt extract --
# I know, I know, it's 'better' to build from source, but LLVM haven't done compiler-rt tarballs since 2020.
# compiler-rt packages in mingw-w64 are also now only for 64-bit platforms, so I guess we better hope LLVM don't find new things to use builtins for.
# I don't know. There's only so much I can do here, okay? These build scripts are a convoluted *mess* as it is.
# And if I compile it myself, one binary is as good as another anyway.
# We could maybe do something with Debian `libclang-rt-21-dev-win`, assuming that's portable (or can be reliably sourced)?
# Grasping at straws doesn't even vaguely cover where I'm at at this point.

rm -rf "${IVTW_SDKPFX}build/compiler-rt"
mkdir -p "${IVTW_SDKPFX}build/compiler-rt"
if [ -e "thirdparty/mingw-w64-cross-compiler-rt/libclang_rt.builtins-i386.a" ]; then
	# If someone goes out of their way to try this, take the hint.
	# In future, this might instead be used as an intentional caching mechanic, so we only unzstd once.
	# Either way, it lets the user override the compiler-rt with minimal effort.
	cp "thirdparty/mingw-w64-cross-compiler-rt/libclang_rt.builtins-"*".a" "${IVTW_SDKPFX}build/compiler-rt/"
else
	unzstd < thirdparty/mingw-w64-cross-compiler-rt/mingw-w64-cross-compiler-rt-21.1.1-1-x86_64.pkg.tar.zst | tar -x -C "${IVTW_SDKPFX}build/compiler-rt"
	# Try to be at least a little clever here regarding version updates.
	mv "${IVTW_SDKPFX}build/compiler-rt/usr/lib/clang/"*"/lib/windows/"* "${IVTW_SDKPFX}build/compiler-rt/"
fi
