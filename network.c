/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include "network.h"
#include "layer.h"
#include "link.h"
#include "memory.h"
#include "neuron.h"
#include "system.h"
#include "vmath.h"
#include <stdio.h>
#include <string.h>

/**
 * FILE FORMAT
 * 4 bytes magic 0x01831337
 * 1 ulong strlen nw->name
 * * bytes nw->name
 * 1 ulong nw->num_layers
 * [
 *      1 ulong layer->num_neurons
 *      1 ulong neuron->outputs
 *      * double weights
 * ]
 */
void network_load(network_st *nw, char* filename)
{
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL) {
        system_die("couldn't open load file");
    }
    uint8_t magic[4];
    fread(&magic, sizeof(magic), 1, fp);
    size_t lname;
    fread(&lname, sizeof(size_t),1, fp);
    char name[lname + 1];
    fread(&name, lname, 1, fp);
    network_init(nw, name);
    size_t n_layers;

    fread(&n_layers, sizeof(size_t), 1, fp);
    size_t neurons_count[n_layers];

    for(size_t i = 0; i < n_layers; i++) {
        fread(&neurons_count[i], sizeof(size_t), 1, fp);
    }

    network_setup(nw, n_layers, neurons_count, vmath_sigmoid);

    for(size_t a = 0; a < nw->num_layers; a++) {
        layer_st *layer = nw->layers[a];
        for(size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st *nn = layer->neurons[n];
            for(size_t i = 0; i < nn->num_outputs; i++) {
                link_st *link = nn->outputs[i];
                fread(&link->weight, sizeof(double), 1, fp);
            }
        }
    } 

    fclose(fp);
}

void network_save(network_st *nw, char* filename)
{
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL) {
        system_die("couldn't open save file");
    }
    uint8_t magic[] = { 0x01, 0x83, 0x13, 0x37 };
    fwrite(magic, sizeof(magic), 1, fp);
    size_t lname = strlen(nw->name);
    // strlen(nw->name);
    fwrite(&lname, sizeof(size_t), 1, fp);
    // nw->name
    fwrite(nw->name, lname, 1, fp);
    // num_layers
    fwrite(&nw->num_layers, sizeof(size_t), 1, fp);

    for(size_t a = 0; a < nw->num_layers; a++) {
        layer_st *layer = nw->layers[a];
        // num_neurons
        fwrite(&layer->num_neurons, sizeof(size_t), 1, fp);
    }

    for(size_t a = 0; a < nw->num_layers; a++) {
        layer_st *layer = nw->layers[a];
        for(size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st *nn = layer->neurons[n];
            for(size_t i = 0; i < nn->num_outputs; i++) {
                link_st *link = nn->outputs[i];
                // link weights
                fwrite(&link->weight, sizeof(double), 1, fp);   
            }
        }
    }

    fclose(fp);
}

void network_destroy(network_st* nw)
{
    for (size_t a = 0; a < nw->num_layers; a++) {
        layer_st* layer = nw->layers[a];
        for (size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st* nn = layer->neurons[n];
            for (size_t t = 0; t < nn->num_outputs; t++) {
                link_st* link = nn->outputs[t];
                link_free(link);
                FREE(link);
            }
            neuron_free(nn);
            FREE(nn);
        }
        layer_free(layer);
        FREE(layer);
    }
    nw->num_layers = 0;
    network_free(nw);
}

void network_setup(network_st* nw, size_t layer_count, size_t* neuron_count_list, double (*func)(double))
{
    char name[80];
    // setup layers and neurons
    for (size_t a = 0; a < layer_count; a++) {
        size_t n_count = *(neuron_count_list++);
        layer_st* layer;
        MALLOC(layer, sizeof(layer_st));
        memset(&name, 0, strlen(name));
        sprintf(name, "layer %zu", a);
        layer_init(layer, name);
        network_push_layer(nw, layer);
        for (size_t n = 0; n < n_count; n++) {
            neuron_st* nn;
            memset(&name, 0, strlen(name));
            sprintf(name, "neuron %zu:%zu", a, n);
            MALLOC(nn, sizeof(neuron_st));
            neuron_init(nn, name, func);
            layer_push_neuron(layer, nn);
        }
    }
    // link all layers
    for (size_t a = nw->num_layers - 1; a > 0; a--) {
        layer_st* layert = nw->layers[a];
        layer_st* layerp = nw->layers[a - 1];
        for (size_t n = 0; n < layerp->num_neurons; n++) {
            neuron_st* n1 = layerp->neurons[n];
            for (size_t t = 0; t < layert->num_neurons; t++) {
                link_st* link;
                memset(&name, 0, strlen(name));
                MALLOC(link, sizeof(link_st));
                sprintf(name, "link %zu:%zu:%zu", a, n, t);
                neuron_st* n2 = layert->neurons[t];
                link_init(link, name, n1, n2);
            }
        }
    }
}

void network_get_outputs(network_st* nw, double *outputs)
{
    layer_st* olayer = nw->layers[nw->num_layers - 1];
    for (size_t t = 0; t < olayer->num_neurons; t++) {
        neuron_st* nn = olayer->neurons[t];
        outputs[t] = nn->output;
    }

}

bool network_print_training(network_st* nw, double* targets, size_t size, size_t iter)
{
    layer_st* olayer = nw->layers[nw->num_layers - 1];

    if (size != olayer->num_neurons) {
        system_die("the number of targets doesn't match the number of predictions");
    }

    bool guess = false;
    size_t count = 0;
    for (size_t t = 0; t < olayer->num_neurons; t++) {
        neuron_st* nn = olayer->neurons[t];
        guess = (int)(nn->output > 0.5 ? 1 : 0) == (int)(targets[t]);
        if (guess) {
            count++;
        }
    }

    if (count == size) {
        for (size_t t = 0; t < olayer->num_neurons; t++) {
            neuron_st* nn = olayer->neurons[t];
            bool g = (int)(nn->output > 0.5 ? 1 : 0) == (int)(targets[t]);
            printf("%zu: %0.2f %s %0.2f\n", t, nn->output, g ? "==" : "<>", targets[t]);
        }

        printf("ITER: %zu, GUESS: %s\n", iter, count == size ? "RIGHT" : "WRONG");
        return count == size;
    }
    return false;
}

void network_print(network_st* nw)
{
    printf("Network: { .name=%s, .num_layers=%zu }\n", nw->name, nw->num_layers);
    for (size_t y = 0; y < nw->num_layers; y++) {

        // layers
        layer_st* layer = nw->layers[y];
        layer_print(layer);

        for (size_t n = 0; n < layer->num_neurons; n++) {
            // neurons
            neuron_st* nn = layer->neurons[n];
            neuron_print(nn);

            if (nn->num_inputs > 0) {
                printf("\t\t\tINPUTS:\n");
                for (size_t i = 0; i < nn->num_inputs; i++) {
                    link_st* link = nn->inputs[i];
                    link_print(link);
                }
                printf("\n");
            }

            if (nn->num_outputs > 0) {
                printf("\t\t\tOUTPUTS:\n");
                for (size_t i = 0; i < nn->num_outputs; i++) {
                    link_st* link = nn->outputs[i];
                    link_print(link);
                }
                printf("\n");
            }
        }
    }
}

void network_forward(network_st* nw, double* inputs, size_t size)
{
    if (nw->num_layers == 0) {
        system_die("no layers");
    }

    layer_st* ilayer = nw->layers[0];
    if (size != ilayer->num_neurons) {
        system_die("the number of inputs doesn't match number of neurons");
    }

    // assign inputs to neuron outputs
    for (size_t i = 0; i < ilayer->num_neurons; i++) {
        ilayer->neurons[i]->output = inputs[i];
    }

    // forward propagation
    for (size_t a = 1; a < nw->num_layers; a++) {
        layer_st* layer = nw->layers[a];
        for (size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st* nn = layer->neurons[n];
            neuron_update_output(nn);
        }
    }
}

void network_backward(network_st* nw, double* targets, size_t size)
{
    if (nw->num_layers == 0) {
        system_die("no layers");
    }

    // output layer
    layer_st* olayer = nw->layers[nw->num_layers - 1];

    if (size != olayer->num_neurons) {
        system_die("the number of targets doesn't match the number of outputs");
    }

    // derivate of squared error cost in output layer
    for (size_t o = 0; o < olayer->num_neurons; o++) {
        neuron_st* nn = olayer->neurons[o];
        nn->output_derivate = vmath_d_error(nn->output, targets[o]);
    }

    for (size_t t = nw->num_layers - 1; t >= 1; t--) {
        layer_st* layer = nw->layers[t];

        // derivate of its total input regard to each of its input weights
        for (size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st* nn = layer->neurons[n];
            nn->input_derivate = nn->output_derivate * vmath_d_afunc(nn->total_input, nn->func);
            nn->acc_input_derivate += nn->input_derivate;
            nn->num_acc_derivates++;
        }

        // derivate of each weight coming into the node
        for (size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st* nn = layer->neurons[n];
            for (size_t i = 0; i < nn->num_inputs; i++) {
                link_st* link = nn->inputs[i];
                link->error_derivate = nn->input_derivate * link->source->output;
                link->acc_error_derivate += link->error_derivate;
                link->num_acc_derivates++;
            }
        }
        if (t == 1) {
            continue;
        }

        // previous layer
        layer_st* player = nw->layers[t - 1];
        for (size_t n = 0; n < player->num_neurons; n++) {
            // error derivate of outputs
            neuron_st* nn = player->neurons[n];
            nn->output_derivate = 0;
            for (size_t i = 0; i < nn->num_outputs; i++) {
                link_st* link = nn->outputs[i];
                nn->output_derivate += link->weight * link->dest->input_derivate;
            }
        }
    }
}

void network_update_weights(network_st* nw, double learning_rate)
{
    for (size_t a = 1; a < nw->num_layers; a++) {
        layer_st* layer = nw->layers[a];
        for (size_t n = 0; n < layer->num_neurons; n++) {
            neuron_st* nn = layer->neurons[n];

            // update node's bias
            if (nn->num_acc_derivates > 0) {
                nn->bias -= learning_rate * (nn->acc_input_derivate / nn->num_acc_derivates);
                nn->acc_input_derivate = 0;
                nn->num_acc_derivates = 0;
            }

            // update weights
            for (size_t i = 0; i < nn->num_inputs; i++) {
                link_st* link = nn->inputs[i];
                if (link->num_acc_derivates > 0) {
                    link->weight -= learning_rate * (link->acc_error_derivate / link->num_acc_derivates);
                    link->acc_error_derivate = 0;
                    link->num_acc_derivates = 0;
                }
            }
        }
    }
}

void network_init(network_st* nw, char* name)
{
    nw->name = strdup(name);
    nw->layers = NULL;
    nw->num_layers = 0;
}

void network_push_layer(network_st* nw, layer_st* layer)
{
    REALLOC(nw->layers, nw->num_layers + 1);
    nw->layers[nw->num_layers] = layer;
    nw->num_layers++;
}

void network_free(network_st* nw)
{
    FREE(nw->name);
    FREE(nw->layers);
}