project('ivttest_sdl2', 'c', 'cpp')

if host_machine.system() == 'darwin'
	# use prebuilt SDL2 for Mac (this is so bad...)
	sdl2_dep = dependency('sdl2', fallback : ['sdl2_prebuilt', 'sdl2_dep'])
else
	# This has the useful effect of using:
	# 1. the system SDL2 on platforms where that's supported (so, Linuxes)
	# 2. WrapDB SDL2 otherwise
	sdl2_dep = dependency('sdl2', fallback : ['sdl2', 'sdl2_dep'])
endif

sdl2test_exe = executable('sdl2test', 'sdl2test.cpp', dependencies: [sdl2_dep])
