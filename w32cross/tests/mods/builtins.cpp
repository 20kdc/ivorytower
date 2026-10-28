#include <stdint.h>
#include "../b_launch.hxx"

TAPTEST_OBSERVER

// These tests cover the MSVC runtime libcalls we have to fake.
// See-also:
// https://github.com/llvm/llvm-project/blob/e9a93baac2f8aad3a2114ec25f6fdb9342a03cd4/llvm/include/llvm/IR/RuntimeLibcalls.td#L3412
// https://github.com/llvm/llvm-project/blob/fa5e5fcfc594f86297d0f3bfd3f15c37cfcb87c4/llvm/test/CodeGen/X86/win32-int-runtime-libcalls.ll#L26

TAPTEST_EXPORT void test_chkstk(TapTestEnv * env) {
	int data[0x20000];
	for (int i = 0; i < 0x20000; i++)
		data[i] = i;
	taptest_observe(env, data);
	printf("\t%i %i\n", data[0], data[0x1FFFF]);
	taptest_observe(env, _alloca(0x10000));
}

TAPTEST_EXPORT void test_muldiv64_alldiv(TapTestEnv * env) {
	int64_t arr[] = {1, 1};
	taptest_observe(env, arr);
	arr[0] = arr[0] / arr[1];
	taptest_observe(env, arr);
}

TAPTEST_EXPORT void test_muldiv64_allrem(TapTestEnv * env) {
	int64_t arr[] = {1, 1};
	taptest_observe(env, arr);
	arr[0] = arr[0] % arr[1];
	taptest_observe(env, arr);
}

TAPTEST_EXPORT void test_muldiv64_aulldiv(TapTestEnv * env) {
	uint64_t arr[] = {1, 1};
	taptest_observe(env, arr);
	arr[0] = arr[0] / arr[1];
	taptest_observe(env, arr);
}

TAPTEST_EXPORT void test_muldiv64_aullrem(TapTestEnv * env) {
	uint64_t arr[] = {1, 1};
	taptest_observe(env, arr);
	arr[0] = arr[0] % arr[1];
	taptest_observe(env, arr);
}

// This likely won't actually call allmul.
// But if it would, then we know.
TAPTEST_EXPORT void test_muldiv64_allmul(TapTestEnv * env) {
	int64_t arr[] = {1, 1};
	taptest_observe(env, arr);
	arr[0] = arr[0] * arr[1];
	taptest_observe(env, arr);
}

TAPTEST_EXPORT void test_muldiv64(TapTestEnv * env) {
	test_muldiv64_alldiv(env);
	test_muldiv64_allrem(env);
	test_muldiv64_aulldiv(env);
	test_muldiv64_aullrem(env);
	test_muldiv64_allmul(env);
}

TAPTESTSEQ_OPEN
	test_chkstk(env);
	test_muldiv64(env);
TAPTESTSEQ_CLOSE
