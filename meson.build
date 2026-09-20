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
	'src/util.cpp',
	version_cpp
)
