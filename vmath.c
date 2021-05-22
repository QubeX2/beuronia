/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include "vmath.h"
#include "memory.h"
#include "neuron.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double vmath_dot(double* va, double* vb, size_t size)
{
    double r = 0;
    for (size_t i = 0; i < size; i++) {
        r += va[i] * vb[i];
    }

    return r;
}

/**
 * activation functions
 *
 * softplus
 * sigmoid
 * relu
 * tanh
 * gaussian
 */
double vmath_softplus(double x) { return log(1 + exp(x)); }

double vmath_sigmoid(double x) { return 1 / (1 + exp(-x)); }

double vmath_relu(double x)
{
    // max(0, x);
    return x < 0 ? 0 : x;
}

double vmath_tanh(double x) { return tanh(x); }

double vmath_gaussian(double x) { return exp(-x * x); }

double vmath_linear(double x) { return x; }
/* end activation functions */

double vmath_randomf(double min, double max)
{
    static uint32_t s = 0;
    if (s == 0) {
        srand(time(NULL));
        s++;
    }
    return min + (rand() / (RAND_MAX / (max - min)));
}

int vmath_randomi(int min, int max)
{
    static uint32_t s = 0;
    if (s == 0) {
        srand(time(NULL));
        s++;
    }
    return rand() % (max + 1 - min) + min;
}

/**
 * squared error
 * prediction and target
 */
double vmath_error(double p, double t) { return (p - t) * (p - t); }

/**
 *
 * power rule - if f(x) = x^y, then derivative of f(x) = yx^y-1
 * example: f(x) = x^3 , dx of f(x) = 3x^2
 */
double vmath_d_error(double p, double t)
{
    // double h = 0.01;
    // return (vmath_cost(p + h, t) - vmath_cost(p, t))/h;
    return 2 * (p - t);
}

/**
 * linear regression cost
 */
double vmath_lr_cost(double* models, double* targets, size_t size)
{
    double r = 0;
    for (size_t i = 0; i < size; i++) {
        r += (models[i] - targets[i]) * (models[i] - targets[i]);
    }
    return r;
}

/**
 * average linear regression cost
 */
double vmath_avg_lr_cost(double* models, double* targets, size_t size)
{
    return (1 / size) * vmath_lr_cost(models, targets, size);
}

/**
 * derivate of activation function
 */
double vmath_d_afunc(double x, double (*f)(double))
{
    if (f == vmath_sigmoid) {
        // f(x)(1 - f(x))
        return vmath_sigmoid(x) * (1 - vmath_sigmoid(x));
    } else if (f == vmath_relu) {
        // if x < 0 : 0
        // if x > 0 : 1
        // if x == 0 : undefined
        return x < 0 ? 0 : 1;
    } else if (f == vmath_tanh) {
        // 1 - f(x)^2
        return 1 - pow(vmath_tanh(x), 2);
    } else if (f == vmath_softplus) {
        // sigmoid
        return vmath_sigmoid(x);
    } else if (f == vmath_gaussian) {
        // -2xe^-x^2
        return -2 * x * exp(-x * x);
    } else if (f == vmath_linear) {
        return 1;
    }

    return 0;
}

char* vmath_create_id()
{
    char* id;
    MALLOC(id, 12);
    static uint32_t c = 0;
    c += 1;
    c &= 0xffffffff;

    time_t t = time(NULL);
    id[0] = (t >> 24) & 0xff;
    id[1] = (t >> 16) & 0xff;
    id[2] = (t >> 8) & 0xff;
    id[3] = t & 0xff;

    int r = vmath_randomi(0, 0x7fffffff);

    id[4] = (r >> 24) & 0xff;
    id[5] = (r >> 16) & 0xff;
    id[6] = (r >> 8) & 0xff;
    id[7] = r & 0xff;

    id[11] = (c >> 24) & 0xff;
    id[10] = (c >> 16) & 0xff;
    id[9] = (c >> 8) & 0xff;
    id[8] = c & 0xff;
    return id;
}

uint32_t vmath_bytes2uint32(char bytes[4])
{
    return (uint32_t)((uint8_t)bytes[0] << 24) | ((uint8_t)bytes[1] << 16) | ((uint8_t)bytes[2] << 8) | (uint8_t)bytes[3];
}
