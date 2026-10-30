#include <windows.h>
#include "../b_launch.hxx"
#include "../test_inspector.hxx"

__declspec(thread) Inspector myInspector("example");
__declspec(thread) int counter = 1;

int decideSubcounterInitValue() {
	return 1;
}

void pokeGuard(void * env) {
	((TapTestEnv *) env)->observe(&myInspector.isAlive);
}

void guardable(void * ign) {
	// Clang uses super special logic for this which inits it on use.
	__declspec(thread) static int subcounter = decideSubcounterInitValue();
	counter++;
	subcounter++;
	// This access here (i.e. the one place we can ACTUALLY observe that Inspector was never setup) is what triggers the guard logic to be written.
	// If we observe alive 0 here, we just observed an unconstructed object!
	printf("\tabout to check inspector\n");
	printf("\tinspector alive %i == 1\n", myInspector.isAlive);
	printf("\ttls counter %x == 2\n", counter);
	printf("\ttls interior subcounter %x == 2\n", subcounter);

	// Notably, *strange things* happen if a thread both *precedes* and *outlives* a library but also interacts with it.
	// Namely, the OT myInspector will never be deconstructed; nor can it be, since that'd require somehow halting the thread to go do that side-quest.
}

WINAPI unsigned long myThreadFn(void *) {
	guardable(NULL);
	return 0;
}

TAPTESTSEQ_OPEN
	// printf("\t_MSC_VER = %i\n", _MSC_VER);
	puts("\tcreating new thread");
	// on new thread
	HANDLE myThread = CreateThread(NULL, 0, myThreadFn, NULL, 0, NULL);
	WaitForSingleObject(myThread, INFINITE);
	CloseHandle(myThread);
	puts("\trunning guard test on background thread");
	// on background thread
	env->callBkgThreadAndWait(guardable, NULL);
	puts("\thalting the bkg thread");
	env->setBkgThread(false);
	puts("\thalted the bkg thread, restarting it");
	env->setBkgThread(true);
	puts("\tmaking bkg thread poke guard");
	env->callBkgThreadAndWait(pokeGuard, env);
	puts("\tduring free only main inspector should die, leaving a 'leaked' inspector");
TAPTESTSEQ_CLOSE
