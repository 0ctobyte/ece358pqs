#include <stdint.h>

#include "test.h"

int32_t main(int32_t argc, char **argv) {
		test_randvar_uniform();
		test_randvar_exp(75);
		return 0;
}
