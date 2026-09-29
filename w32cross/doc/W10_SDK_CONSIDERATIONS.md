**I am not a lawyer and this document is not legal advice.**

This document is a review of how the licensing of the downloaded prerequisites (Windows 10 SDK disc and the STL includes) may apply to programs built with the W32Cross W10 SDK.

## STL

The STL is the simpler one.

The Apache+LLVM exception license should make copyright issues caused by the STL non-existent unless redistributing a built SDK (don't do that, there are other reasons not to).

## Windows SDK

This is the awkward bit. In short, everything after 2.b. is either irrelevant or a bunch of massive overreaches.

Of what is actually relevant (this is **not** a complete description, it is an attempt to cover what is relevant to building and linking programs using this SDK):

* `1.a`: Technically, only the programs being developed have to run on a Microsoft operating system. Since that's the goal (otherwise why bother with the SDK), this is probably fine.
* `1.b`: This is really weird. I think it's focusing on 'third party machines', so the developer downloading it to their dev machine may be fine? But also _the utilities list covers the UCRT._
	* **However,** `REDIST.TXT` (<https://learn.microsoft.com/en-us/legal/windows-sdk/redist>) goes on to basically say everything's fine UCRT-wise. \
	  I feel like it would be hard to argue given that 2.a.i explicitly gives permission. (And if Microsoft wants to sue someone, it doesn't actually matter what they did, anyway.)
* `1.c`: Implies permission to use in a private CI.
* `1.d`: Unsure if relevant.
* `1.e`: If you use the alljoyn headers (and by extension `windows.devices.alljoyn.interop.h`) then you probably need to include the notice.
	* There are other notices in case you, for example, embed DXC into your program. Heed them if relevant.
* `2.a.i`: _At least_ the following are safe to redistribute:
	* The dynamic UCRT
	* `dxil`/`d3dcompiler_47`/`dxcompiler` (likely subject to third party license notices)
	* `microsoft.mbn.dll`; this is something obscure that even MSDN seems to have more or less forgotten about. Documentation seems sparse. But it's here, .lib files 'n' all.
* `2.a.ii`: Any `.lib` files may only be redistributed in a compiled program.
	* It's my guess that this is the rule that MinGW-w64 decided they needed to engineer around. \
	  We need whatever MSVC compatibility we can get, so this isn't an option for us.
* `2.a.iii`: Some of these are really sketchy, but I'd also say they are not enforced; Valve knowingly redistribute `REDIST.TXT` code all the time to Steam Decks.
	* Since we don't have any intention of modifying i.e. the UCRT _period_ (it'd be impractical) we're not going to modify any notices.
	* The last point about an 'Excluded License' is pretty obviously a spiteful jab at copyleft licenses. \
	  The rest of the conditions were already enough to make the licensing dangerously close to incompatible with the GPL. This is _just_ for spite.
		* I'll circle back to this in the next section.
* `2.b`: Entirely irrelevant boilerplate.
* `3`: Irrelevant boilerplate.
* `4`: Irrelevant boilerplate, except perhaps 4.d ('modern cryptography methods'?)
* `5`: Concerning in the sense that redistributing the ISO directly could be seen as infringement.
* `6`: Irrelevant.
* `7`: _Incredibly_ weird.
	* The anti-benchmarking rule is kind of just plain crazy.
	* This is a bit concerning in the _long term_ in the sense that you're not allowed to, say, host the SDK ISO somewhere.
* `8`: Useless stuff from the cryptography ban era. We could learn something from that era, like not giving a shit and redistributing libdvdcss to every corner of the planet.
* `9`: Boilerplate.
* `10`: Weird.
* `11`: Boilerplate.
* `12`: Scummy boilerplate.
* `13`: Boilerplate.
* `14`: Boilerplate.
* `15`: Very weird given 14.

## `2.a.iii` analysis

So, the effects of `2.a.iii` are the biggest 'potential showstopper'.

However, I'd like to point out some things. In increasing order of importance:

1. Don't do static UCRT link.
2. This **only** matters for GPLv2 compliance. The value of having this SDK be working but not suitable for GPLv2 programs is high.
3. GPL programs are built with MSVC **all the time.** They're not doing anything we aren't.
4. Most importantly, the code has to make the `.lib` file code _subject to_ the license.
5. _The GPL was not written by idiots._

Let's break this down by GPL version:

* The GPLv1 (yes we're going back this far) is pretty clear about what isn't covered. `.lib` imports are obviously 'definitions files'. Even the header files are covered.
* The GPLv2 has a pretty loose clause which _could_ technically be interpreted as making the import definitions _maybe_ subject to the license, insofaras the surviving mapping from 'symbol name' to 'DLL' _accompanies the executable._
	* Given this is a successor to the GPLv1's clause, I don't think it was _intended_ to cover import libraries. It could obviously be maliciously read this way if you were a lawyer with a perverse hatred for common sense.
	* Also, given point 3, I think this is a triviality and nobody cares.
* It seems pretty clear that the GPLv3's wording would classify _any_ import library in the Windows SDK under _System Libraries_ and thus not subject to the GPLv3.

Since we've supplied our own crt0, it really comes down to if the GPLv2 covers an import library.

(If you _really_ want to go for this at any cost, you can always use an 'any later version' clause and just say you're distributing under the GPLv3, subject to if that is possible.)
