#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "iblis.h"

char * iblis::strdup_checked(const char * src) {
	char * res = strdup(src);
	if (!res) {
		puts("failed strdup");
		exit(1);
	}
	return res;
}

iblis::CString::CString(const iblis::CStr & a, const iblis::CStr & b) : CStr((char *) malloc(a.len() + b.len() + 1)) {
	if (!ptr) {
		puts("failed strcatdup");
		exit(1);
	}
	strcpy((char *) ptr, a.ptr);
	strcat((char *) ptr, b.ptr);
}
