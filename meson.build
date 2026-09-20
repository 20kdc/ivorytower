project('ivorytower-driver', 'cpp')

version_str = run_command(['git', 'describe', '--all', '--long'], capture : true, check : true).stdout().splitlines()[0]

version_cpp = configure_file(
  input : 'src/version.cpp.in',
  output : 'version.cpp',
  configuration : {'PLACEHOLDER' : version_str}
)

setupcore = executable('setupcore',
	'src/cvar.cpp',
	'src/main.cpp',
	'src/meson.cpp',
	'src/names.cpp',
	'src/util.cpp',
	'src/z_distrobox.cpp',
	'src/z_mingw.cpp',
	'src/z_osxcross.cpp',
	version_cpp
)
