/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stdio.h>
#include <string.h>
#include "network.h"
#include "layer.h"
#include "neuron.h"
#include "link.h"
#include "memory.h"
#include "system.h"
#include "vmath.h"

void network_print_training(network_st *nw, double *targets, size_t size, size_t iter)
{
    layer_st *olayer = nw->layers[nw->num_layers - 1];

    if(size != olayer->num_neurons) {
        system_die("the number of targets doesn't match the number of predictions");
    }

    for(size_t t = 0; t < olayer->num_neurons; t++) {
        neuron_st *nn = olayer->neurons[t];
        bool guess = (int)(nn->output > 0.5 ? 1 : 0) == (int)(targets[t]);
        printf("ITER: %zu, NN: %s, PREDICTION: %f, TARGET: %f, GUESS: %s\n", 
            iter,
            nn->name,
            nn->output,targets[t],
            guess == true ? "RIGHT" : "WRONG"
        );
    }
}

void network_print(network_st *nw)
{
    printf("Network: { .name=%s, .num_layers=%zu }\n", nw->name, nw->num_layers);
    for(size_t y = 0; y < nw->num_layers; y++) {
        
        // layers
        layer_st *layer = nw->layers[y];
        layer_print(layer);
        
        for(size_t n = 0; n < layer->num_neurons; n++) {
            // neurons
            neuron_st *nn = layer->neurons[n];
            neuron_print(nn);

            if(nn->num_inputs > 0) {
                printf("\t\t\tINPUTS:\n");
                for(size_t i = 0; i < nn->num_inputs; i++) {
                    link_st *link = nn->inputs[i];
                    link_print(link);
                }
                printf("\n");
            }

            if(nn->num_outputs > 0) {
                printf("\t\t\tOUTPUTS:\n");
                for(size_t i = 0; i < nn->num_outputs; i++) {
                    link_st *link = nn->outputs[i];
                    link_print(link);
                }
                printf("\n");
            }
        }
    }
}

void network_forward(network_st *nw, double *inputs, size_t size)
{
    if(nw->num_layers == 0) {
        system_die("no layers");
    }

    layer_st *ilayer = nw->layers[0];
    if(size != ilayer->num_neurons) {
        system_die("the number of inputs doesn't match number of neurons");
    }

    // assign inputs to neuron outputs
    for(size_t i = 0; i < ilayer->num_neurons; i++) {
        ilayer->neurons[i]->output = inputs[i];
    }

    // forward propagation
    for(size_t a = 1; a < nw->num_layers; a++) {
        layer_st *layer = nw->layers[a];
        for(size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st *nn = layer->neurons[n];
            neuron_update_output(nn);
        }
    }
}

void network_backward(network_st *nw, double *targets, size_t size)
{
    if(nw->num_layers == 0) {
        system_die("no layers");
    }

    // output layer
    layer_st *olayer = nw->layers[nw->num_layers - 1];

    if(size != olayer->num_neurons) {
        system_die("the number of targets doesn't match the number of outputs");
    }

    // derivate of squared error cost in output layer
    for(size_t o = 0; o < olayer->num_neurons; o++) {
        neuron_st *nn = olayer->neurons[o];
        nn->output_derivate = vmath_d_error(nn->output, targets[o]);
    }

    for(size_t t = nw->num_layers - 1; t >= 1; t--) {
        layer_st *layer = nw->layers[t];
    
        // derivate of its total input regard to each of its input weights
        for(size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st *nn = layer->neurons[n];
            nn->input_derivate = nn->output_derivate * vmath_d_afunc(nn->total_input, nn->func);
            nn->acc_input_derivate += nn->input_derivate;
            nn->num_acc_derivates++;
        }

        // derivate of each weight coming into the node
        for(size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st *nn = layer->neurons[n];
            for(size_t i = 0; i < nn->num_inputs; i++) {
                link_st *link = nn->inputs[i];
                link->error_derivate = nn->input_derivate * link->source->output;
                link->acc_error_derivate += link->error_derivate;
                link->num_acc_derivates++;
            }
        }
        if(t == 1) {
            continue;
        }

        // previous layer
        layer_st *player = nw->layers[t - 1];
        for(size_t n = 0; n < player->num_neurons; n++) {
            // error derivate of outputs
            neuron_st *nn = player->neurons[n];
            nn->output_derivate = 0;
            for(size_t i = 0; i < nn->num_outputs; i++) {
                link_st *link = nn->outputs[i];
                nn->output_derivate += link->weight * link->dest->input_derivate;
            }
        }
    }
}

void network_update_weights(network_st *nw, double learningRate)
{
    for(size_t a = 1; a < nw->num_layers; a++) {
        layer_st *layer = nw->layers[a];
        for(size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st *nn = layer->neurons[n];

            // update node's bias
            if(nn->num_acc_derivates > 0) {
                nn->bias -= learningRate * (nn->acc_input_derivate / nn->num_acc_derivates);
                nn->acc_input_derivate = 0;
                nn->num_acc_derivates = 0;
            }

            // update weights
            for(size_t i = 0; i < nn->num_inputs; i++) {
                link_st *link = nn->inputs[i];
                if(link->num_acc_derivates > 0) {
                    link->weight -= learningRate * (link->acc_error_derivate / link->num_acc_derivates);
                    link->acc_error_derivate = 0;
                    link->num_acc_derivates = 0;
                }

            }
        }
    }
}

void network_init(network_st *nw, char *name)
{
    nw->name = strdup(name);
    nw->layers = NULL;
    nw->num_layers = 0;
}

void network_push_layer(network_st *nw, layer_st *layer)
{
    REALLOC(nw->layers, nw->num_layers + 1);
    nw->layers[nw->num_layers] = layer;
    nw->num_layers++;
}

void network_free(network_st *nw)
{
    FREE(nw->layers);
}