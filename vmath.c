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

double vmath_dot(double *va, double *vb, size_t size) 
{
    double r = 0;
    for(size_t i = 0; i < size; i++) {
        r += va[i] * vb[i];
    }

    return r;
}

double vmath_sigmoid(double x) 
{
    return 1 / (1 + exp(-x));
}

double vmath_relu(double x)
{
    // max(0, x);
    return x < 0 ? 0 : x;
}

double vmath_randomf(double min, double max)
{
    static uint32_t s = 0;
    if(s == 0) {
        srand(time(NULL));
        s++;
    }
    return min + (rand() / (RAND_MAX / (max - min)));
}

int vmath_randomi(int min, int max)
{
    static uint32_t s = 0;
    if(s == 0) {
        srand(time(NULL));
        s++;
    }
    return rand() % (max + 1 - min) + min;
}

/**
 * squared error
 * prediction and target
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

double vmath_lr_cost(double *models, double *targets, size_t size)
{
    double r = 0;
    for(size_t i = 0; i < size; i++) {
        r += (models[i] - targets[i]) * (models[i] - targets[i]);
    }
    return r;
}

double vmath_avg_lr_cost(double *models, double *targets, size_t size)
{
    return (1/size) * vmath_lr_cost(models, targets, size);
}

/**
 * 
 * b = b - 0.1 * vmath_slope();
 */