project('ivttest_sdl2', 'c')

# This has the useful effect of using the system SDL2 on platforms where that's supports (so, Linuxes) while also using WrapDB SDL2 everywhere else.
sdl2_dep = dependency('sdl2', fallback : ['sdl2', 'sdl2_dep'])
