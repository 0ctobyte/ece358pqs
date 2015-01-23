#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "randvar.h"

#define NUM_SAMPLES 1000

void test_randvar_exp(float lambda) {
	float e = 0.0f, sum = 0.0f, sum_squared = 0.0f, expected_mean = 1.0f/lambda, expected_variance = 1.0f/pow(lambda, 2);
	for(uint32_t i = 0; i < NUM_SAMPLES; i++, sum += e, sum_squared += pow(e, 2)) {
		e = randvar_exp(lambda);
	}

	float mean = sum/NUM_SAMPLES;
	float variance = (sum_squared/NUM_SAMPLES) - pow(mean, 2);
	
	float mean_error = (fabs(mean - expected_mean)/expected_mean)*100.0f;
	float variance_error = (fabs(variance - expected_variance)/expected_variance)*100.0f;

	printf("\nEXPONENTIAL RANDVAR TEST\nmean: %f\nvariance: %f\nmean error: %f%%\nvariance error: %f%%\n", mean, variance, mean_error, variance_error);
}

void test_randvar_uniform() {
	float u = 0.0f, sum = 0.0f, sum_squared = 0.0f, expected_mean = 0.5f, expected_variance = 1.0f/12.0f;
	for(uint32_t i = 0; i < NUM_SAMPLES; i++, sum += u, sum_squared += pow(u, 2)) {
		u = randvar_uniform();
	}

	float mean = sum/NUM_SAMPLES;
	float variance = (sum_squared/NUM_SAMPLES) - pow(mean, 2);
	
	float mean_error = (fabs(mean - expected_mean)/expected_mean)*100.0f;
	float variance_error = (fabs(variance - expected_variance)/expected_variance)*100.0f;

	printf("\nUNIFORM RANDVAR TEST\nmean: %f\nvariance: %f\nmean error: %f%%\nvariance: %f%%\n", mean, variance, mean_error, variance_error);
}

