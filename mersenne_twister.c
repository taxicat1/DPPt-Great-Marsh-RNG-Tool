#include <stdint.h>

#include "mersenne_twister.h"

#define N 624
#define M 397
#define MATRIX_A 0x9908b0df	
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff

static uint32_t mt[N];
static uint32_t mag01[2] = { 0, MATRIX_A };

static void mt_scramble() {
	uint32_t y;
	unsigned int kk;
	
	for (kk = 0; kk < N-M; kk++) {
		y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
		mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 1];
	}
	
	for (; kk < N-1; kk++) {
		y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
		mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 1];
	}
	
	y = (mt[N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
	mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 1];
}

void mt_srand(uint32_t s) {
	mt[0] = s;
	for (int i = 1; i < N; i++) {
		mt[i] = (1812433253 * (mt[i-1] ^ (mt[i-1] >> 30)) + i); 
	}
	
	mt_scramble();
}

static uint32_t mt_temper(uint32_t x) {
	x ^= (x >> 11);
	x ^= (x << 7) & 0x9d2c5680;
	x ^= (x << 15) & 0xefc60000;
	x ^= (x >> 18);
	return x;
}

uint32_t mt_read(unsigned int i) {
	return mt_temper(mt[i]);
}
