#include <SDL.h>

#ifdef main
#undef main
#endif

struct test {
	void * a = 0;
};

int main(int argc, char ** argv) {
	return 0;
}
