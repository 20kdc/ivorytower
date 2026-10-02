#include <setjmp.h>
// seems to have no effect. good? bad? idk, you decide!
// #include <setjmpex.h>
#include <stdio.h>
#include <exception>

class Inspector {
public:
	const char * name;
	Inspector(const char * name) : name(name) {
		printf("+ Inspector(%s)\n", name);
	}
	~Inspector() {
		printf("- Inspector(%s)\n", name);
	}
};

void lj_inner(jmp_buf jb) {
	longjmp(jb, 1);
}

void lj_outer(jmp_buf jb) {
	Inspector i("longjmp");
	lj_inner(jb);
}

void ex_inner() {
	throw std::exception();
}

void ex_outer() {
	Inspector i("exception");
	ex_inner();
}

int main(int argc, char ** argv) {
	jmp_buf jb;
	if (setjmp(jb)) {
		puts("a");
	} else {
		puts("b");
		lj_outer(jb);
	}
	// hmm, now with that done
	try {
		ex_outer();
	} catch (...) {
		// done!
	}
	return 0;
}
