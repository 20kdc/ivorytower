#include <stdint.h>
#include "../b_launch.hxx"

TAPTEST_OBSERVER

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

TAPTEST_EXPORT void test_muldiv64(TapTestEnv * env) {
	test_muldiv64_alldiv(env);
	test_muldiv64_allrem(env);
	test_muldiv64_aulldiv(env);
	test_muldiv64_aullrem(env);
}

TAPTESTSEQ_OPEN
	test_chkstk(env);
	test_muldiv64(env);
TAPTESTSEQ_CLOSE
