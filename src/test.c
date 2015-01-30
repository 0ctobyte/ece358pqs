#include <stdio.h>
#include <math.h>

#include "randvar.h"
#include "es.h"

#define NUM_SAMPLES 1000

void test_randvar_exp(float lambda) {
	randvar_seed();

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
	randvar_seed();

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

void _test_es_pq_dot(es_pq_t *pq, char *filename) {
	FILE *f;
	f = fopen(filename, "w");
	fprintf(f, "digraph pq {\n");

	for(uint32_t index = 1; (index<<1) <= es_pq_size(pq); index++) {
		es_event_t p = es_pq_at(pq, index);
		es_event_t l = es_pq_at(pq, (index<<1));
		char c_p = ((p.event_type == 0) ? 'A' : ((p.event_type == 1) ? 'D' : 'O'));
		char c_l = ((l.event_type == 0) ? 'A' : ((l.event_type == 1) ? 'D' : 'O'));
		fprintf(f, "\t%c_%d -> %c_%d;\n", c_p, p.time, c_l, l.time);
		if(((index<<1)+1) <= es_pq_size(pq)) {
			es_event_t r = es_pq_at(pq, (index<<1)+1);
			char c_r = ((r.event_type == 0) ? 'A' : ((r.event_type == 1) ? 'D' : 'O'));
			fprintf(f, "\t%c_%d -> %c_%d;\n", c_p, p.time, c_r, r.time);
		}
	}

	if(es_pq_size(pq) == 1) {
		es_event_t e = es_pq_at(pq, 1);
		char c_e = ((e.event_type == 0) ? 'A' : ((e.event_type == 1) ? 'D' : 'O'));
		fprintf(f, "\t%c_%d\n", c_e, e.time); 
	}

	fprintf(f, "}\n");
}

void _test_es_pq_print(es_pq_t *pq) {
	for(uint32_t index = 1; index <= es_pq_size(pq); index++) {
		es_event_t e = es_pq_at(pq, index);
		char c_e = ((e.event_type == 0) ? 'A' : ((e.event_type == 1) ? 'D' : 'O'));
		printf("(%c, %d), ", c_e, e.time);
	}
	
	printf("\n");
}

void test_es_pq() {
	randvar_seed();

	es_pq_t *pq = es_pq_create(64);

	for(float u = randvar_uniform(); es_pq_size(pq) < 10; u = randvar_uniform()) {
		es_event_t e = {.event_type = (int)(u*3), .time = (int)(u*1000000)};
		es_pq_enqueue(pq, e);
	}
	
	_test_es_pq_dot(pq, "pq0.dot");
	_test_es_pq_print(pq);

	for(uint32_t i = 1; es_pq_size(pq) > 0; i++) {
		char filename[16];
		sprintf(filename, "pq%d.dot", i);
		es_event_t e = es_pq_dequeue(pq);
		printf("%c_%d\n", e.event_type+0x41, e.time);
		_test_es_pq_dot(pq, filename);
		_test_es_pq_print(pq);
	}

	es_pq_delete(pq);
}

