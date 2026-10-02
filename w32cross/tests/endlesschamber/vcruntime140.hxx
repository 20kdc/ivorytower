// contains useful 'dummy' functions
#include <cstdlib>
#include <windows.h>
#include <setjmp.h>
#include <eh.h>
#include <exception>
#include <typeinfo>

class Test_VCRT140_MySecretClassBase : public std::exception {
public:
	// generate a pure virtual
	virtual bool is_conclusive() = 0;
};

// we use this to generate various dynamic_cast situations
class Test_VCRT140_MySecretClass : public Test_VCRT140_MySecretClassBase {
public:
	virtual bool is_conclusive() override { return true; }
};

// _CreateFrameInfo: not used

// _CxxThrowException
// NOLINTNEXTLINE
__declspec(dllexport) void it_vcruntime140_cxxthrowexception() {
	throw Test_VCRT140_MySecretClass();
}

// _FindAndUnlinkFrame: CRT-private
// _IsExceptionObjectToBeDestroyed: CRT-private
// _SetWinRTOutOfMemoryExceptionCallback: CRT-private
// __AdjustPointer: sdk_w10/stl/src/excptptr.cpp
//                  libcxx/src/support/runtime/exception_pointer_msvc.ipp
//                  this is an interface between vcruntime and the STL
//                  we'd need to implement ehdata.h (ourselves, it has no MinGW-w64 representation) to get this
// __BuildCatchObject: CRT-private
// __BuildCatchObjectHelper: CRT-private
// __C_specific_handler: EH personality! not sure how to figure this one
// __C_specific_handler_noexcept: similar
// __CxxDetectRethrow: CRT-private
// __CxxExceptionFilter: CRT-private
// __CxxFrameHandler*: covered by other tests I believe
// __CxxQueryExceptionSize: CRT-private
// __CxxRegisterExceptionObject: CRT-private
// __CxxUnregisterExceptionObject: just going to assume CRT-private
// __DestructExceptionObject: CRT-private
// __FrameUnwindFilter: CRT-private
// __GetPlatformExceptionInfo: sdk_w10/stl/src/pplerror.cpp
// __NLG_Dispatch2: CRT-private
// __NLG_Return2: CRT-private

// __RTCastToVoid
// NOLINTNEXTLINE
__declspec(dllexport) void * it_vcruntime140_RTCastToVoid(std::exception * ex) {
	return dynamic_cast<void *>(ex);
}

// __RTDynamicCast
// NOLINTNEXTLINE
__declspec(dllexport) void * it_vcruntime140_RTDynamicCast(std::exception * ex) {
	return dynamic_cast<Test_VCRT140_MySecretClass *>(ex);
}

// __RTtypeid
// NOLINTNEXTLINE
__declspec(dllexport) const std::type_info * it_vcruntime140_RTtypeid(std::exception * ex) {
	// TODO: This doesn't appear to actually call __RTtypeid???
	return &typeid(ex);
}

// __TypeMatch: CRT-private?
// __current_exception: libc++ says this returns EHExceptionRecord**
//                      looks like STL business
// __current_exception_context: assume this is like __current_exception

// __intrinsic_setjmp / __intrinsic_setjmpex
// NOLINTNEXTLINE
__declspec(dllexport) void it_vcruntime140_setjmp() {
	jmp_buf buf;
	if (!setjmp(buf)) {
		Sleep(0);
		longjmp(buf, 1);
	}
}

// __processing_throw: CRT-internal

/*
 * __report_gsfailure: this is worth carefully documenting
 * in short, the GS stuff looks supposed to happen by merging in BufferOverflow.lib or BufferOverflowU.lib
 * KB894573 is mentioned, but no luck finding it
 * but then also people mention that this isn't ACTUALLY what should happen, instead there's a dedicated GS object?
 * for now, I'm leaving the NotVCRT as it is on this. can always change it later
 */

// __std_exception_copy: TODO if std::exception ABI doesn't match up, START HERE
// __std_exception_destroy: looks similarly suspicious

// __std_terminate: emitted in Clang CGException.cpp
// 'direct' terminate() is done through the UCRT.
// NOLINTNEXTLINE
__declspec(dllexport) void it_vcruntime140_std_terminate(void (*narwhal)()) throw(Test_VCRT140_MySecretClass) {
	// if the narwhal throws an unexpected exception, we should go through unexpected or terminate handling
	narwhal();
}

// __std_type_info_compare: TODO
// __std_type_info_destroy_list: SHOULD already be called by notvcrt
// __std_type_info_hash: TODO
// __std_type_info_name: TODO
// __telemetry_main_invoke_trigger: unnecessary
// __telemetry_main_return_trigger: unnecessary
// __unDName / __unDNameEx: These are C functions that are in defs and are not header-exposed. They'll be fine.

// __vcrt_GetModuleFileNameW: what...?
// __vcrt_GetModuleHandleW: what...?
// __vcrt_InitializeCriticalSectionEx: what...?
// __vcrt_LoadLibraryExW: what...?

// _get_purecall_handler: exposed by stdlib.h
// NOLINTNEXTLINE
__declspec(dllexport) void * it_vcruntime140_get_purecall_handler() {
	return (void *) _get_purecall_handler();
}

// _get_unexpected: stl/include/exception
// NOLINTNEXTLINE
__declspec(dllexport) void * it_vcruntime140_get_unexpected() {
	return (void *) std::get_unexpected();
}

// set_unexpected: stl/include/exception
// NOLINTNEXTLINE
__declspec(dllexport) void * it_vcruntime140_set_unexpected(void * v) {
	return (void *) std::set_unexpected((unexpected_function) v);
}

// _is_exception_typeof: exposed by eh.h
// NOLINTNEXTLINE
__declspec(dllexport) int it_vcruntime140_is_exception_typeof(void * a, void * b) {
	return _is_exception_typeof(*(std::type_info *) a, (EXCEPTION_POINTERS *) b);
}

// unexpected/_set_se_translator/__uncaught_exception/__uncaught_exceptions/_set_purecall_handler
// NOLINTNEXTLINE
__declspec(dllexport) void * it_vcruntime140_unexpected_series() {
	std::unexpected();
	_set_se_translator(nullptr);
	__uncaught_exception();
	__uncaught_exceptions();
	_set_purecall_handler(nullptr);
	return nullptr;
}

// _local_unwind: CRT-private
// _purecall: see Test_VCRT140_MySecretClassBase::is_conclusive

// longjmp: see setjmp

// memchr/memcmp/memcpy/memmove/memset
// NOLINTNEXTLINE
__declspec(dllexport) int it_vcruntime140_memseries(void * a, void * b, int c, int d) {
	if (memchr(a, 'b', d)) {
		return memcmp(b, a, d);
	} else if (c == 0) {
		memcpy(a, b, d);
	} else if (c == 1) {
		memmove(a, b, d);
	} else if (c == 2) {
		memset(a, 'x', d);
	}
	return 0;
}

// strchr/strrchr/strstr
// NOLINTNEXTLINE
__declspec(dllexport) char * it_vcruntime140_strseries(char *, char * x) {
	char * send = strchr(x, 0);
	char * mid = strrchr(x, '.');
	return strstr(send, mid);
}

// wcschr/wcsrchr/wcsstr
// NOLINTNEXTLINE
__declspec(dllexport) wchar_t * it_vcruntime140_wcsseries(wchar_t *, wchar_t * x) {
	wchar_t * send = wcschr(x, 0);
	wchar_t * mid = wcsrchr(x, '.');
	return wcsstr(send, mid);
}
