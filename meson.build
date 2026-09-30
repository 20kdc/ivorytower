project('ivorytower-driver', 'cpp')

version_str = run_command(['git', 'describe', '--all', '--long'], capture : true, check : true).stdout().splitlines()[0]

version_cpp = configure_file(
  input : 'src/common/version.cpp.in',
  output : 'version.cpp',
  configuration : {'PLACEHOLDER' : version_str}
)

iblis_inc = include_directories('src')
iblis_lib = shared_library('iblis',
	'src/common/cvar.cpp',
	'src/common/util.cpp',
	'src/common/main.cpp',
	version_cpp,
	include_directories: iblis_inc
)
iblis_dep = declare_dependency(link_with: iblis_lib, include_directories: iblis_inc)

setupcore = executable('setupcore',
	'src/setupcore/install.cpp',
	'src/setupcore/main.cpp',
	'src/setupcore/meson.cpp',
	'src/setupcore/names.cpp',
	'src/setupcore/y_boxenrunner.cpp',
	'src/setupcore/y_osxcross.cpp',
	'src/setupcore/y_w32cross.cpp',
	'src/setupcore/z_box.cpp',
	'src/setupcore/z_mingw.cpp',
	'src/setupcore/z_osxcross.cpp',
	'src/setupcore/z_w32cross.cpp',
	dependencies: iblis_dep
)

test('test_realpath', executable('test_realpath', 'src/tests/realpath.cpp', dependencies: iblis_dep))
