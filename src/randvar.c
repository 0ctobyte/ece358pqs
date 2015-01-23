#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float randvar_uniform() {
	static uint8_t is_seeded = 0;
	if(is_seeded == 0) {
		srand((uint32_t)time(NULL));
		is_seeded = 1;
	}
	return (float)rand()/((uint32_t)RAND_MAX+1);
}

float randvar_exp(float lambda) {
	return -(log(1-randvar_uniform())/lambda);
}


