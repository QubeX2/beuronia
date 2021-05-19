/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>
#include "neuron.h"

extern double vector_dot(neuron_st n, int size);
extern double vector_sigmoid(double x);

#endif /* __VECTOR_H__ */
