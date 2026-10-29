# NOTVCRUNTIME
# This is not VCRuntime.
# This is free and unencumbered software released into the public domain.
# For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.

.text

# See 'Libcalls' in reference material

# So the main thing to keep in mind here is that the compiler-rt functions are cdecl.
# And the MSVC functions are stdcall.

# In the interest of not wasting a LOT of time,
# I wrote this out in C, realized there was no good way to fix the identifiers,
#  and then just compiled it to assembly and cleaned it up.

.globl __alldiv
__alldiv:
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	calll ___divdi3
	addl $16, %esp
	retl $16

.globl __aulldiv
__aulldiv:
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	calll ___udivdi3
	addl $16, %esp
	retl $16

.globl __allrem
__allrem:
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	calll ___moddi3
	addl $16, %esp
	retl $16

.globl __aullrem
__aullrem:
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	calll ___umoddi3
	addl $16, %esp
	retl $16

.globl __allmul
__allmul:
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	pushl 16(%esp)
	calll ___muldi3
	addl $16, %esp
	retl $16
