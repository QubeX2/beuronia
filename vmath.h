/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __VMATH_H__
#define __VMATH_H__

#define VMATH_RANDF() vmath_randomf(-1.0, 1.0)

#include "neuron.h"
#include <stdint.h>

extern double vmath_dot(double* va, double* vb, size_t size);

// derivate of activation functions
extern double vmath_d_afunc(double x, double (*f)(double));

// activation functions
extern double vmath_sigmoid(double x);
extern double vmath_relu(double x);
extern double vmath_tanh(double x);
extern double vmath_softplus(double x);
extern double vmath_gaussian(double x);
extern double vmath_linear(double x);

// random
extern double vmath_randomf(double min, double max);
extern int vmath_randomi(int min, int max);

extern double vmath_error(double p, double t); // (prediction - target) ^ 2
extern double vmath_d_error(double p, double t);
extern double vmath_lr_cost(double* models, double* targets, size_t size);
extern double vmath_avg_lr_cost(double* models, double* targets, size_t size);

extern uint32_t vmath_bytes2uint32(char bytes[4]);

extern char* vmath_create_id();
#endif /* __VMATH_H__ */
