/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include "crt0_common.h"

static char tls_emulator_active = 0;

void __stdcall __NOTVCRUNTIME_tlsemu(HMODULE * module, int reason, void * reserved) {
	PIMAGE_DOS_HEADER dosheader = (PIMAGE_DOS_HEADER) module;
	PIMAGE_NT_HEADERS ntheader = (((void *) module) + (size_t) dosheader->e_lfanew);
	PIMAGE_DATA_DIRECTORY tlsdatdir = ntheader->OptionalHeader.DataDirectory + 9;
	if (ntheader->FileHeader.SizeOfOptionalHeader < offsetof(IMAGE_OPTIONAL_HEADER, DataDirectory[10]))
		return;
	if (!tlsdatdir->Size)
		return;

	if (reason == DLL_PROCESS_ATTACH) {
		DEBUGF("TLS emulator: TLS directory present, va = %lx ib = %p\n", tlsdatdir->VirtualAddress, (void *) ntheader->OptionalHeader.ImageBase);
	}

	PIMAGE_TLS_DIRECTORY tlsdir = (PIMAGE_TLS_DIRECTORY) (ntheader->OptionalHeader.ImageBase + tlsdatdir->VirtualAddress);

	PDWORD pindex = (PDWORD) tlsdir->AddressOfIndex;

	/*
	 * alright, now into the TLS emulator proper
	 * this code is less than perfectly tested, but the alternative is certain doom
	 * need to get the XP VM back up.
	 * virtualbox, the one actually good VM for it, isn't packaged for deb13?
	 */
	DWORD index = *pindex;
	/* setup */
	if (reason == DLL_PROCESS_ATTACH || reason == DLL_THREAD_ATTACH) {
		if (index == TLS_OUT_OF_INDEXES) {
			tls_emulator_active = 1;
			DEBUGF("TLS emulator: Required\n");
			/* TLS emulation required */
			index = *pindex = TlsAlloc();
			if (index == TLS_OUT_OF_INDEXES || !index)
				return;
			DWORD isize = tlsdir->EndAddressOfRawData - tlsdir->StartAddressOfRawData;
			DWORD total = (tlsdir->EndAddressOfRawData - tlsdir->StartAddressOfRawData) + tlsdir->SizeOfZeroFill;
			void * mem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, total);
			memcpy(mem, (void *) tlsdir->StartAddressOfRawData, isize);
			TlsSetValue(index, mem);
		}
	}
	/* run callbacks */
	if (tls_emulator_active) {
		PIMAGE_TLS_CALLBACK * callbacks = (PIMAGE_TLS_CALLBACK *) tlsdir->AddressOfCallBacks;
		while (*callbacks) {
			(*callbacks)(module, reason, reserved);
			callbacks++;
		}
	}
	/* teardown */
	if (reason == DLL_PROCESS_DETACH || reason == DLL_THREAD_DETACH) {
		if (tls_emulator_active && (index != 0 && index != TLS_OUT_OF_INDEXES)) {
			*pindex = TLS_OUT_OF_INDEXES;
			TlsFree(index);
		}
	}
}
