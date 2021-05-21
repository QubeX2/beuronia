/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 * 
 * TESTS NEURAL NETWORK WITH LAYERS
 * TEST DATA FLOWER PETALS LENGHT, WIDHT AND COLOR
 * TRAIN NETWORK TO PREDICT COLOR
 */
#include <stdio.h>
#include "network.h"
#include "layer.h"
#include "neuron.h"
#include "vmath.h"

int main(void)
{
    // length, width, color 0, 1
    double t_dataset[8][3] = {
        { 2, 1, 0 },
        { 3, 1, 0 },
        { 2, .5, 0 },
        { 1, 1, 0 },

        { 3, 1.5, 1, },
        { 3.5, .5, 1, },
        { 4, 1.5, 1 },
        { 5.5, 1, 1},
    };
    double learning_rate = 0.2;
    size_t num_iterations = 500000;

    network_st netw;
    network_init(&netw, "network");

    layer_st lin, lout;
    layer_init(&lin, "layer in");
    layer_init(&lout, "layer out");

    network_push_layer(&netw, &lin);
    network_push_layer(&netw, &lout);

    neuron_st ni1, ni2, no;
    neuron_init(&ni1, "nn in 1", vmath_sigmoid);
    neuron_init(&ni2, "nn in 2", vmath_sigmoid);
    neuron_init(&no, "nn out", vmath_sigmoid);    

    layer_push_neuron(&lin, &ni1);
    layer_push_neuron(&lin, &ni2);
    layer_push_neuron(&lout, &no);
    
    link_st l1, l2;
    link_init(&l1, "link 1.1", &ni1, &no);
    link_init(&l2, "link 1.2", &ni2, &no);

    double targets[1];
    for(size_t iter = 1; iter <= num_iterations; iter++) {
        int w = vmath_randomi(0, 7);
        targets[0] = t_dataset[w][2];
        double *inputs = t_dataset[w];
        network_forward(&netw, inputs, 2);
        network_backward(&netw, targets, 1);
        network_update_weights(&netw, learning_rate);
        network_print_training(&netw, targets, 1, iter);
    }
    return 0;
}