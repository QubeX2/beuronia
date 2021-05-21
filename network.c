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