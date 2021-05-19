/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __NEURON_H__
#define __NEURON_H__

typedef struct neuron_st neuron_st;

struct neuron_st {
    int num_inputs;
    double output;
    double bias;
    double *inputs;
    double *weights;
};

#endif /* __NEURON_H__ */