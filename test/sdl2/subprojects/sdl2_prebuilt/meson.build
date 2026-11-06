# SDL2 is downloaded by ./prepare
# So we just need to link it

project('sdl2', 'c')

cc = meson.get_compiler('c')

if host_machine.system() == 'darwin'
	# TODO: None of this actually works at present because the Mac packaging is borked.
	# SDL2 headers refer to themselves with the SDL2/ prefix.
	# Now, that's not a problem in-and-of-itself, so long as that prefix is consistent with the FS.
	# Problem: It isn't.
	sdl2_dep = declare_dependency(
		link_args : [
			'-L' + meson.current_source_dir() + '/mac/lib',
			'-lSDL2'
		],
		include_directories : include_directories('mac/inc', 'mac/inc/SDL2')
	)
else
	error('This subproject is not usable on', host_machine.system(), '. Please use another SDL2 source.')
endif
