# Why always install the STL?

Other setups in the `ivorytower` project support STL switch-off.

Here, though, you will always get the STL no matter what.

The answer to this is that there really isn't a good way to turn off the STL without making a separate set of compiler-wrapper scripts, or for the -cl compilers, a separate `winsysroot`.

The compiler-wrapper scripts are pretty what makes `w32cross` 'work out of the box', so keeping them in good shape is vital.

Besides, if you don't care about C++ ABI, you probably don't need `w32cross` in the first place and may find MinGW preferrable.

Or maybe just have it as a separate SDK compile?

I will say that a `zero` configuration (no dependence on the C++ standard library _whatsoever_) is impossible, as `notvcrt` and `vcruntime140` are basically a non-negotiable part of the VC++-emulating arrangement.

I would argue that if you are not interested in using `notvcrt`, `w32cross` is not for you; which is fine, to be clear!
But at that point you are trying to assemble your own toolchain doing something out of scope for `w32cross`.
