/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vmath.h"
#include "neuron.h"

double vmath_dot(neuron_st *n) 
{
    double r = 0;

    for(size_t i = 0; i < n->size; i++) {
        r += n->inputs[i] * n->weights[i];
    }

    return r + n->bias;
}

double vmath_sigmoid(double x) 
{
    return 1 / (1 + exp(-x));
}

double vmath_random(double min, double max)
{
    static uint32_t s = 0;
    if(s == 0) {
        srand(time(NULL));
        s++;
    }
    return min + (rand() / (RAND_MAX / (max - min)));
}

/**
 * squared error
 */
double vmath_cost(double p, double t)
{
    return (p - t) * (p - t);
}

/**
 *
 * power rule - if f(x) = x^y, then derivative of f(x) = yx^y-1 
 * example: f(x) = x^3 , dx of f(x) = 3x^2
 */ 
double vmath_slope(double p, double t)
{
    //double h = 0.01;
    //return (vmath_cost(p + h, t) - vmath_cost(p, t))/h;
    return 2 * (p - t);
}

/**
 * 
 * b = b - 0.1 * vmath_slope();
 */