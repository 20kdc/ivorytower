/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#ifndef _MSC_VER
#error "You may need to pass -fms-compatibility-version, Clang isn't taking you seriously"
#endif

#include <stdint.h>
#include <stddef.h>
/* So we need to get Clang to use Clang's vadefs.h */
#include <vadefs.h>

#ifdef __cplusplus
#define _CRT_BEGIN_C_HEADER extern "C" {
#define _CRT_END_C_HEADER }
#else
#define _CRT_BEGIN_C_HEADER
#define _CRT_END_C_HEADER
#endif

#define _CRTIMP __declspec(dllimport)
#define _CRTIMP2 _CRTIMP
#define _VCRTIMP _CRTIMP
#define _MRTIMP2 _CRTIMP

#define _HAS_CXX17 0

#define _In_opt_z_
#define _Field_range_(s, e)
#define _Inout_
#define _In_
#define _Check_return_
#define _Ret_notnull_
#define _Ret_maybenull_
#define _Success_(c)
#define __CRTDECL
#define _Out_writes_z_(c)
#define _In_z_
#define _Outptr_result_maybenull_
#define _CRT_INSECURE_DEPRECATE(f)
#define _Pre_maybenull_
#define _Always_(c)
#define _Printf_format_string_params_(c)
#define _Printf_format_string_
#define _Scanf_format_string_params_(c)
#define _Scanf_format_string_
#define _Scanf_s_format_string_params_(c)
#define _Scanf_s_format_string_
#define _In_opt_
#define _Out_writes_opt_z_(c)
#define _Out_writes_opt_(c)
#define _Post_maybez_
#define _Pre_notnull_
#define _Out_writes_(c)
#define _In_reads_(c)
#define _Pre_z_
#define _Out_opt_
#define _Outptr_result_nullonfailure_
#define _Out_writes_bytes_to_(p, c)
#define _In_range_(s, e)
#define _Deref_post_valid_
#define _Inout_opt_
#define _Out_
#define _Out_writes_bytes_(c)
#define _In_reads_bytes_(c)
#define _In_reads_bytes_opt_(c)
#define _CRT_DEPRECATE_TEXT(p)
#define _Inout_updates_(s)
#define _Inout_updates_opt_(s)
#define _Inout_updates_bytes_(s)
#define _Inout_updates_z_(s)
#define _Post_readable_size_(s)
#define _Post_writable_byte_size_(s)
#define _Post_invalid_
#define _Deref_post_z_
#define _Post_z_
#define _Post_satisfies_(c)
#define _Out_writes_to_(p, c)
#define _Out_writes_to_opt_(p, c)
#define _Out_writes_bytes_to_opt_(p, c)
#define _Out_writes_bytes_opt_(s)
#define _In_reads_or_z_(s)
#define _In_reads_or_z_opt_(s)
#define _Outptr_result_z_
#define _Pre_opt_z_
#define _NODISCARD
#define _Inout_opt_z_
#define _Deref_prepost_opt_z_
#define _When_(c, r)
#define _Prepost_z_
#define _Ret_z_
#define _Inout_z_
#define _Ret_writes_z_(s)
#define _In_reads_opt_(s)
#define _Deref_prepost_z_
#define _Deref_pre_opt_z_
#define _Deref_prepost_opt_valid_z
#define _Post_equal_to_(s)
#define _CRT_INSECURE_DEPRECATE_MEMORY(lies)
#define _CRT_INSECURE_DEPRECATE_GLOBALS(bad)
#define _Out_writes_all_(s)
#define _Out_writes_all_opt_(s)
#define _Deref_prepost_opt_valid_
#define _At_buffer_(a, b, c, d)
#define _Outptr_result_buffer_maybenull_(c)
#define _Outptr_result_maybenull_z_
#define _Ret_maybenull_z_
#define _Inout_bytecount_(s)
#define _Deref_post_opt_valid_
#define _Analysis_assume_(c)
#define _Pre_satisfies_(c)
#define _Deref_ret_z_

#define __CLR_OR_THIS_CALL __thiscall
#define __CLRCALL_PURE_OR_CDECL __cdecl
#define __CLRCALL_OR_CDECL __cdecl

#define _CRT_SATELLITE_CODECVT_IDS_NOIMPORT
