// #ifndef DRAND48_H
// #define DRAND48_H

// #include <stdlib.h>

// #define drandm 0x100000000LL
// #define drandc 0xB16
// #define dranda 0x5DEECE66DLL

// static unsigned long long seed = 1;

// double drand48(void)
// {
//     seed = (dranda * seed + drandc) & 0xFFFFFFFFFFFFLL;
//     unsigned int x = seed >> 16;
//     return ((double)x / (double)drandm);
// }

// void srand48(unsigned int i)
// {
//     seed = (((long long int)i) << 16) | rand();
// }

// #endif

#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#define MAXFLOAT FLT_MAX

#include <random>


static double drand48()
{
	static std::random_device seed_gen;
	static std::mt19937 engine(seed_gen());
	static std::uniform_real_distribution<> dist(0.0, 1.0);
	return dist(engine);
}