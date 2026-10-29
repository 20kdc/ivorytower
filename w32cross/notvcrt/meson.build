# Meson build file for notvcrt.
# This lets notvcrt be cleanly built using the bootstrap compilation environment.

project('notvcrt', 'c', 'cpp')

notvcrt_src = [
	'src/crt0/common.c',
	'src/crt0/dll.c',
	'src/crt0/dll_nomain.c',
	'src/crt0/dll_tlsemu.c',
	'src/crt0/exe_con.c',
	'src/crt0/exe_gui.c',
	'src/crt0/exe_con_w.c',
	'src/crt0/exe_gui_w.c',
	'src/crt0/fltused.c',
	'src/crt0/gs.c',
	'src/crt0/tls.c',
	'src/vcr/del01.cpp',
	'src/vcr/del02.cpp',
	'src/vcr/del03.cpp',
	'src/vcr/del04.cpp',
	'src/vcr/del05.cpp',
	'src/vcr/del06.cpp',
	'src/vcr/del07.cpp',
	'src/vcr/del08.cpp',
	'src/vcr/del09.cpp',
	'src/vcr/del10.cpp',
	'src/vcr/del11.cpp',
	'src/vcr/del12.cpp',
	'src/vcr/new1.cpp',
	'src/vcr/new2.cpp',
	'src/vcr/new3.cpp',
	'src/vcr/new4.cpp',
	'src/vcr/new5.cpp',
	'src/vcr/new6.cpp',
	'src/vcr/new7.cpp',
	'src/vcr/new8.cpp',
	'src/vcr/new_nothrow.cpp',
	'src/vcr/typeinfo.cpp'
]

# If needed in future.

if host_machine.cpu_family() == 'x86'
	notvcrt_src += ['src/crt0/msvc_int64_thunks.s']
endif
#elif host_machine.cpu_family() == 'x86_64'
#notvcrt_src += ['src/crtx/chkstk_x64.s']
#endif
#assert(false, host_machine.cpu_family())

notvcrt_lib = static_library('notvcrt', notvcrt_src)
