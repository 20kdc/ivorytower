#include <ppltasks.h>
#include <stdio.h>

// NOLINTNEXTLINE
__declspec(dllexport) void it_concrt140() {
	Concurrency::create_task([] {
		puts("Narwhal swim");
	});
}
