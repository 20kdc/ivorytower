project('ivttest_simple', 'c', 'cpp')

mylibrary_lib = shared_library('mylibrary', 'mylibrary.cpp')
mylibrary_dep = declare_dependency(link_with : mylibrary_lib)

testexe_exe = executable('test_cpp', 'test.cpp', dependencies: [mylibrary_dep])
othertest_exe = executable('test_c', 'test2.c')
