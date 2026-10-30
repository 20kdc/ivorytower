#pragma once

#include <stdio.h>

/*
 * Test environment.
 * This contains 'observe' (a function that 'could theoretically do anything with ptr' for deoptimization purposes).
 * The test environment also has a 'background thread'.
 * This thread was started before the test is even loaded, which makes it useful to test things like TLS which behave interestingly in that case.
 * It also can outlive the test, which is interesting for similar reasons.
 */
class TapTestEnv {
public:
	virtual void observe(void * ptr) = 0;
	virtual void callBkgThreadAndWait(void (*fn)(void * data), void * data) = 0;
	virtual void setBkgThread(bool active) = 0;
};

class TapTestSeq {
public:
	virtual void invoke(TapTestEnv * env) = 0;
};

// Exports are useful as markers for binary analysis
// Also for exporting things
#define TAPTEST_EXPORT extern "C" __declspec(dllexport)

// Declares an observer proxy that is _fastcall
// This reduces the effect it has on the output code.
#define TAPTEST_OBSERVER \
_fastcall TAPTEST_EXPORT void taptest_observe(TapTestEnv * env, void * ptr) { \
	env->observe(ptr); \
}

#define TAPTESTSEQ_OPEN \
class TestClass : public TapTestSeq { \
public: \
	virtual void invoke(TapTestEnv * env) override {

#define TAPTESTSEQ_CLOSE \
	} \
}; \
TestClass g_myTest; \
TAPTEST_EXPORT TapTestSeq * X_4AE = (TapTestSeq *) &g_myTest;
