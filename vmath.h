/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __VMATH_H__
#define __VMATH_H__

#define VMATH_RAND() vmath_random(-1.0, 1.0)

#include "neuron.h"

extern double vmath_dot(neuron_st *n);
extern double vmath_sigmoid(double x);
extern double vmath_random(double min, double max);
extern double vmath_cost(double p, double t); // (prediction - target) ^ 2
extern double vmath_slope(double p, double t);

#endif /* __VMATH_H__ */
