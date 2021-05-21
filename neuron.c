/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include "neuron.h"
#include "vmath.h"
#include "memory.h"


void neuron_print(neuron_st *nn)
{
    printf("\t\tNeuron: {\n\t\t\t.name=%s, .output=%f, .total_input=%f, .bias=%f, .num_inputs=%zu, .num_outputs=%zu\n",
        nn->name,
        nn->output,
        nn->total_input,
        nn->bias,
        nn->num_inputs,
        nn->num_outputs
    );
    printf("\t\t\t.out_der=%f, .acc_out_der=%f, .num_acc_out_ders=%zu",
        nn->output_derivate,
        nn->acc_input_derivate,
        nn->num_acc_derivates
    );
    printf("\n\t\t}\n");
}

void neuron_init(neuron_st *nn, char *name, double (*func)(double))
{
    nn->name = strdup(name);
    nn->bias = VMATH_RANDF();
    nn->func = func;
    nn->inputs = NULL;
    nn->outputs = NULL;
    nn->total_input = 0;
    nn->output = 0;
    nn->num_inputs = 0;
    nn->num_outputs = 0;
    nn->input_derivate = 0;
    nn->output_derivate = 0;
    nn->acc_input_derivate = 0;
    nn->num_acc_derivates = 0;
}

void neuron_update_output(neuron_st *nn)
{
    nn->total_input = nn->bias;
    for(size_t i = 0; i < nn->num_inputs; i++) {
        nn->total_input += nn->inputs[i]->weight * nn->inputs[i]->source->output;
    }
    nn->output = nn->func(nn->total_input);
}

void neuron_push_link(neuron_st *nn, link_type_e type, link_st *link)
{
    switch(type) {
        case LINK_TYPE_INPUT:
            REALLOC(nn->inputs, nn->num_inputs + 1);
            nn->inputs[nn->num_inputs] = link;
            nn->num_inputs++;
            break;

        case LINK_TYPE_OUTPUT:
            REALLOC(nn->outputs, nn->num_outputs + 1);
            nn->outputs[nn->num_outputs] = link;
            nn->num_outputs++;
            break;
    }
}

void neuron_free(neuron_st *nn)
{
    FREE(nn->inputs);
    FREE(nn->outputs);
    FREE(nn->name);
}