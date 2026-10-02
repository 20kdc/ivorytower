/*
 * recreate issue seen w/ godot 3.5.3 modules/fbx/fbx_parser/FBXCommon.h
 *
 * ...turns out this was happening on *MinGW* and Godot was ignoring my compiler request.
 * as it turns out Godot does NOT want to be compiled on MSVC, or even a quasi-MSVC.
 * it wants MinGW-w64. which, to be fair, makes it very easy to compile.
 * but it does mean this test is merely a sanity check rather than useful.
 */
#include <string>
namespace FBXCommon {
	const uint64_t EXAMPLE = 1234;
}
