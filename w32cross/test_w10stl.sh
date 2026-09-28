#!/bin/sh -e

# This script attempts to build the STL.
# In theory, doing this is useless.
# However, it does actually have a purpose.
export W32CROSS_SDKID=w10

. common/cbase.sh

stlbase=downloaded/stl16/stl/src

rm -rf tests/bin/stl16obj
mkdir -p tests/bin/stl16obj

do_obj() {
	echo "$1"
	"$IVTW_CL-x64" /c /EHs /MD /DCRTDLL2 /D_CRTBLD /Itests/stl16inc /otests/bin/stl16obj/ "$stlbase/$1"
}

do_obj atomic.cpp
# do_obj atomic_wait.cpp
# do_obj awint.hpp
do_obj cerr.cpp
do_obj cin.cpp
do_obj clog.cpp
# do_obj cond.cpp
do_obj cout.cpp
do_obj cthread.cpp
do_obj dllmain.cpp
do_obj dllmain_satellite.cpp
# do_obj excptptr.cpp
do_obj filesys.cpp
# do_obj filesystem.cpp
do_obj fiopen.cpp
# do_obj format.cpp
# do_obj future.cpp
do_obj instances.cpp
do_obj iomanip.cpp
do_obj ios.cpp
do_obj iosptrs.cpp
do_obj iostream.cpp
do_obj locale0.cpp
do_obj locale0_implib.cpp
# do_obj locale.cpp
# do_obj memory_resource.cpp
# do_obj mexcptptr.cpp
# do_obj mpiostream.cpp
# do_obj msvcp_atomic_wait.src
do_obj multprec.cpp
# do_obj mutex.cpp
do_obj nothrow.cpp
# do_obj parallel_algorithms.cpp
# do_obj pplerror.cpp
# do_obj ppltasks.cpp
# do_obj primitives.hpp
# do_obj raisehan.cpp
do_obj sharedmutex.cpp
# do_obj special_math.cpp
do_obj stdhndlr.cpp
do_obj stdthrow.cpp
# do_obj StlCompareStringA.cpp
# do_obj StlCompareStringW.cpp
# do_obj StlLCMapStringA.cpp
# do_obj StlLCMapStringW.cpp
# do_obj syncstream.cpp
do_obj syserror.cpp
do_obj syserror_import_lib.cpp
# do_obj taskscheduler.cpp
do_obj thread0.cpp
# do_obj _tolower.cpp
# do_obj _toupper.cpp
# do_obj tzdb.cpp
do_obj ulocale.cpp
do_obj uncaught_exception.cpp
do_obj uncaught_exceptions.cpp
do_obj ushcerr.cpp
do_obj ushcin.cpp
do_obj ushclog.cpp
do_obj ushcout.cpp
do_obj ushiostr.cpp
# do_obj vector_algorithms.cpp
do_obj wcerr.cpp
do_obj wcin.cpp
do_obj wclog.cpp
do_obj wcout.cpp
do_obj winapinls.cpp
do_obj winapisupp.cpp
do_obj wiostrea.cpp
# do_obj wlocale.cpp
do_obj xalloc.cpp
do_obj xcosh.cpp
do_obj xdateord.cpp
do_obj xdint.cpp
do_obj xdnorm.cpp
do_obj xdscale.cpp
do_obj xdtento.cpp
do_obj xdtest.cpp
do_obj xdunscal.cpp
do_obj xexp.cpp
do_obj xfcosh.cpp
do_obj xfdint.cpp
do_obj xfdnorm.cpp
do_obj xfdscale.cpp
do_obj xfdtento.cpp
do_obj xfdtest.cpp
do_obj xfdunsca.cpp
do_obj xferaise.cpp
do_obj xfexp.cpp
do_obj xfprec.cpp
do_obj xfsinh.cpp
do_obj xfvalues.cpp
do_obj xgetwctype.cpp
do_obj xlcosh.cpp
do_obj xldint.cpp
do_obj xldscale.cpp
do_obj xldtento.cpp
do_obj xldtest.cpp
do_obj xldunsca.cpp
do_obj xlexp.cpp
do_obj xlgamma.cpp
do_obj xlocale.cpp
do_obj xlock.cpp
do_obj xlpoly.cpp
do_obj xlprec.cpp
do_obj xlsinh.cpp
do_obj xlvalues.cpp
# do_obj xmath.hpp
do_obj xmbtowc.cpp
do_obj xmtx.cpp
# do_obj xmtx.hpp
do_obj xnotify.cpp
do_obj xonce2.cpp
do_obj xonce.cpp
do_obj xpoly.cpp
do_obj xprec.cpp
do_obj xrngabort.cpp
do_obj xrngdev.cpp
do_obj xsinh.cpp
do_obj xstod.cpp
do_obj xstof.cpp
do_obj xstoflt.cpp
do_obj xstol.cpp
do_obj xstold.cpp
do_obj xstoll.cpp
do_obj xstopfx.cpp
do_obj xstoul.cpp
do_obj xstoull.cpp
do_obj xstoxflt.cpp
do_obj xstrcoll.cpp
do_obj xstrxfrm.cpp
do_obj xthrow.cpp
do_obj xtime.cpp
do_obj xtowlower.cpp
do_obj xtowupper.cpp
do_obj xvalues.cpp
do_obj xwcscoll.cpp
# do_obj xwcsxfrm.cpp
do_obj xwctomb.cpp
do_obj xwstod.cpp
do_obj xwstof.cpp
do_obj xwstoflt.cpp
do_obj xwstold.cpp
do_obj xwstopfx.cpp
do_obj xwstoxfl.cpp
# do_obj xxcctype.hpp
# do_obj xxdftype.hpp
# do_obj xxfftype.hpp
# do_obj xxlftype.hpp
# do_obj xxstod.hpp
# do_obj xxwctype.hpp
# do_obj xxxdtent.hpp
# do_obj xxxprec.hpp
