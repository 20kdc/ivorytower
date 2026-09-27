# Supplementary `.def` files

These `.def` files are (mostly) the result of passing various DLLs through `gendef`.

As the mechanical output of `gendef`, they do not carry the copyright of it. I believe, under the same principles as Wine spec-files, they do not carry any copyright from their source DLLs either.

They are also absolutely necessary for interoperability purposes.

Ultimately, as unreviewed `gendef` output, they are open to tweaking if it Makes Thing Work Better.

Any such changes are, of course, in the public domain.

These files are organized by 'source package'. Specific SDK scripts pick up these and try to make them look like a real MSVC tree.

* `vc14_redist`: `https://aka.ms/vc14/vc_redist.*.exe` for `*`: `x86`, `x64`, `arm64`.
	* `vcruntime140_1` is _missing_ on x86 and it's unclear why. However, the only really scary function it supplies is `__CxxFrameHandler4`. LLVM only knows `__CxxFrameHandler3` from `vcruntime140`, doesn't know the NLG functions _at all,_ and the NLG functions are also duplicate symbols.
* `oldnames`: `oldnames.lib` content. Entirely custom, but also completely empty for now.
