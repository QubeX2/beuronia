/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __NODE_H__
#define __NODE_H__

#include <stddef.h>
#include <stdint.h>

typedef struct node_st node_st;

struct node_st {
    size_t size;
    double output;
    double bias;
    double *inputs;
    double *weights;
    double prediction;
    double target;
    double dot;
    double cost;
    // derivatives
    double d_cost;
    double d_prediction;
    double (*func)(double);
};

extern void node_init(node_st *n, size_t s, double (*f)(double));
extern double node_predict(node_st *n, double learning_rate);
extern double node_output(node_st *n, double (*f)(double));
extern void node_set_inputs(node_st *n, ...);
extern void node_set_weights(node_st *n, ...);


#endif /* __NODE_H__ */