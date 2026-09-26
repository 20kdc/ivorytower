# Characterization for Windows SDK 10.0.19041.0, downloadable as a single ISO file.

ISO_PATH="downloaded/19041.5609.250311-1926.vb_release_svc_im_WindowsSDK.iso"
ISO_URL="https://go.microsoft.com/fwlink/?linkid=2312004"

ADDARGS_CL="/winsysroot \"`readlink -f target`\""

sdk_msiextract() {
	# MSI extraction and mangling are a single stage.
	# Basically this is just one big block of stuff which is per-SDK 'characterized.

	# needed for windows.h/etc.
	msiextract "build/isoextract/Installers/Windows SDK for Windows Store Apps Headers-x86_en-us.msi" -C build/msiextract
	msiextract "build/isoextract/Installers/Windows SDK for Windows Store Apps Libs-x86_en-us.msi" -C build/msiextract
	# not strictly necessary, but also, d3dcompiler_47.dll (not for ARM though), fxc.exe, dxc.exe, etc...
	# someone WILL want these.
	msiextract "Windows SDK for Windows Store Apps Tools-x86_en-us.msi" -C build/msiextract

	msiextract "build/isoextract/Installers/Windows SDK Desktop Headers arm-x86_en-us.msi" -C build/msiextract
	msiextract "build/isoextract/Installers/Windows SDK Desktop Headers arm64-x86_en-us.msi" -C build/msiextract
	msiextract "build/isoextract/Installers/Windows SDK Desktop Headers x64-x86_en-us.msi" -C build/msiextract
	msiextract "build/isoextract/Installers/Windows SDK Desktop Headers x86-x86_en-us.msi" -C build/msiextract

	msiextract "build/isoextract/Installers/Windows SDK Desktop Libs arm64-x86_en-us.msi" -C build/msiextract
	msiextract "build/isoextract/Installers/Windows SDK Desktop Libs x64-x86_en-us.msi" -C build/msiextract
	msiextract "build/isoextract/Installers/Windows SDK Desktop Libs x86-x86_en-us.msi" -C build/msiextract

	msiextract "build/isoextract/Installers/Universal CRT Headers Libraries and Sources-x86_en-us.msi" -C build/msiextract
	msiextract "build/isoextract/Installers/Universal CRT Redistributable-x86_en-us.msi" -C build/msiextract

	# So a problem here is SDK layout.
	# In short, LLVM really, REALLY wants us to give it an SDK with the 'proper layout'.
	# /winsysroot is set to Program Files, which means we get the space-containing "Windows Kits" directory involved.
	# We don't want to encourage a (potentially brittle in the Unix world) space-filled layout.
	# But we definitely need to tell LLVM we are using UCRT...
	# We will have to live with what LLVM wants us to do here, as much as I hate it, but we'll provide alternate arrangements.
	mkdir -p target
	mv -T "build/msiextract/Program Files/Windows Kits" "target/Windows Kits"
	# setup an arrangement vaguely like DevDivInternal as this is spaceless
	ln -s "Windows Kits/10/Lib/10.0.19041.0" "target/lib"
	ln -s "Windows Kits/10/Include/10.0.19041.0" "target/inc"
	ln -s "Windows Kits/10/Redist/10.0.19041.0" "target/redist"
}
