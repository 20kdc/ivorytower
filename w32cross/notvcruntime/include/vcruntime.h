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

#define _CRT_BEGIN_C_HEADER
#define _CRT_END_C_HEADER
#define _In_opt_z_
#define _Field_range_(s, e)
#define _Inout_
#define _In_
#define _Check_return_
#define _Ret_notnull_
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
#define _CRT_DEPRECATE_TEXT(p)
#define _Inout_updates_(s)
#define _Inout_updates_opt_(s)
#define _Post_readable_size_(s)
