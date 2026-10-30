#include <windows.h>
#include "b_launch.hxx"

#define BKGMSGSLOTS 3
#define HoverMsg_Nop 0
#define HoverMsg_Quit 1
#define HoverMsg_Squeak 2
#define HoverMsg_Run 3

class BkgMsg {
public:
	BkgMsg(int code) : code(code) {}
	BkgMsg(int code, void * p1) : code(code), p1(p1) {}
	BkgMsg(int code, void * p1, void * p2) : code(code), p1(p1), p2(p2) {}
	int code = HoverMsg_Nop;
	void * p1, * p2;
};

class HoverMsgslot {
public:
	HANDLE mutex;
	BkgMsg msg;
	HoverMsgslot(BOOL owned) : msg(HoverMsg_Nop) {
		mutex = CreateMutex(NULL, owned, NULL);
	}
	~HoverMsgslot() {
		CloseHandle(mutex);
	}
};

class BkgThread {
public:
	// it's important that always hold at least one mutex.
	// this keeps the threads in lockstep, else a thread WILL just keep freewheeling forever.
	HoverMsgslot msg[BKGMSGSLOTS] = {
		HoverMsgslot(TRUE),
		HoverMsgslot(FALSE),
		HoverMsgslot(FALSE)
	};
	HANDLE hoverThread;
	int hoverThreadIsAlive = 0;
	int nextMsgSlot = 0;

	static WINAPI ULONG hoverThreadFn(void * val) {
		BkgThread * envImpl = (BkgThread *) val;
		// slot we are currently holding
		int holdingSlot = -1;
		int waitOnSlot = 1;
		while (1) {
			WaitForSingleObject(envImpl->msg[waitOnSlot].mutex, INFINITE);
			envImpl->hoverThreadIsAlive = 1;
			// got slot! release former slot if any
			if (holdingSlot != -1)
				ReleaseMutex(envImpl->msg[holdingSlot].mutex);
			holdingSlot = waitOnSlot;
			// set next slot to wait on
			waitOnSlot = (waitOnSlot + 1) % BKGMSGSLOTS;
			// now actually execute command
			BkgMsg msg = envImpl->msg[holdingSlot].msg;
			switch (msg.code) {
				case HoverMsg_Nop:
					break;
				case HoverMsg_Quit:
					return 0;
				case HoverMsg_Squeak:
					puts((const char *) msg.p1);
					break;
				case HoverMsg_Run:
					((void (*)(void *)) msg.p1)(msg.p2);
					break;
			}
		}
		return 0;
	}

	BkgThread() {
		msg[1].msg.code = HoverMsg_Nop; // HoverMsg_Squeak;
		msg[1].msg.p1 = (void *) "+ hover thread";
		hoverThread = CreateThread(NULL, 0, hoverThreadFn, this, 0, NULL);
		// not a memory barrier, but hover thread promises to have mutex
		volatile int * hoverThreadOkInt = &hoverThreadIsAlive;
		while (!*hoverThreadOkInt);
	}

	~BkgThread() {
		// send quit msg
		// msg[nextMsgSlot].code = HoverMsg_Squeak;
		// msg[nextMsgSlot].p1 = (void *) "- hover thread";
		// submitMsg();
		submitMsg(BkgMsg(HoverMsg_Quit));
		// done, close hover
		WaitForSingleObject(hoverThread, INFINITE);
		CloseHandle(hoverThread);
	}

	void submitMsg(BkgMsg newMsg) {
		msg[nextMsgSlot].msg = newMsg;
		int curMsgSlot = nextMsgSlot;
		nextMsgSlot = (nextMsgSlot + 1) % BKGMSGSLOTS;
		// if these two statements were reversed, we'd be holding no mutexes.
		// this would mean hover thread would just keep spinning through the message queue
		WaitForSingleObject(msg[nextMsgSlot].mutex, INFINITE);
		ReleaseMutex(msg[curMsgSlot].mutex);
	}

	void ensureMessagesCompleted() {
		// We're waiting for the last message slot to complete.
		// We know it completed if we can complete a cycle of NOPs.
		int slotToWaitFor = nextMsgSlot;
		while (1) {
			submitMsg(BkgMsg(HoverMsg_Nop));
			if (nextMsgSlot == slotToWaitFor)
				break;
		}
	}
};

class TapTestEnvImpl : public TapTestEnv {
public:
	BkgThread * bkgThread = new BkgThread();

	TapTestEnvImpl() {
		bkgThread = new BkgThread();
	}

	virtual void setBkgThread(bool state) override {
		if (!state) {
			if (bkgThread) {
				delete bkgThread;
				bkgThread = NULL;
			}
		} else {
			if (!bkgThread)
				bkgThread = new BkgThread();
		}
	}

	~TapTestEnvImpl() {
		setBkgThread(false);
	}

	virtual void observe(void * ptr) override {
		// do nothing, but the test doesn't know that
	}

	virtual void callBkgThreadAndWait(void (*fn)(void * data), void * data) override {
		if (!bkgThread) {
			puts("TEST BUG: bkgThread dead, can't call it");
			return;
		}
		bkgThread->submitMsg(BkgMsg(HoverMsg_Run, (void *) fn, data));
		bkgThread->ensureMessagesCompleted();
	}
};

void launch_test(const char * testid) {
	printf("\n%s [load] {\n", testid);
	{
		TapTestEnvImpl testEnvironment;
		HMODULE lib = LoadLibraryA(testid);
		if (!lib) {
			// might have been intentionally omitted
			// for instance exception testing kinda messes with OllyDbg
			// can use 'pass to program' but easier to just not have them
			printf("} MISSING\n");
			return;
		}
		TapTestSeq ** testptr = (TapTestSeq **) GetProcAddress(lib, "X_4AE");
		if (!testptr) {
			printf("MISSING TESTPTR: %s\n", testid);
			exit(1);
		}
		printf("} [invoke] {\n");
		(*testptr)->invoke(&testEnvironment);
		printf("} [free] {\n");
		FreeLibrary(lib);
	}
	printf("}\n\n");
}

int main(int argc, char ** argv) {
	launch_test("mod_exceptions_vs_destructors");
	launch_test("mod_cout");
	launch_test("mod_builtins");
	launch_test("mod_declspec_thread");
}
