#include <setjmp.h>
// seems to have no effect. good? bad? idk, you decide!
// #include <setjmpex.h>
#include <stdio.h>
#include <exception>
#include "../test_inspector.hxx"
#include "../b_launch.hxx"

Inspector global("global");

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

TAPTESTSEQ_OPEN
	jmp_buf jb;
	if (setjmp(jb)) {
		puts("\ta");
	} else {
		puts("\tb");
		lj_outer(jb);
	}
	// hmm, now with that done
	try {
		ex_outer();
	} catch (...) {
		// done!
	}
TAPTESTSEQ_CLOSE
