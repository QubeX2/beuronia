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

static void _neuron_id(char *id)
{
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
}

void neuron_print(neuron_st *n)
{
    // convert id to string
    char id[24 + 1];
    char *p = &id[0];
    for(int i = 0; i < 12; i++) {
        p += sprintf(p, "%02x", (uint8_t)n->id[i]);
    }
    id[24] = '\0';

    printf("Neuron: { .id=0x%s, .output=%f, .total_input=%f, .bias=%f, .num_inputs=%zu, .num_outputs=%zu }\n",
        id,
        n->output,
        n->total_input,
        n->bias,
        n->num_inputs,
        n->num_outputs
    );
}

void neuron_init(neuron_st *n, double (*func)(double))
{
    memset(n->id, '\0', 12);
    _neuron_id(n->id);
    n->bias = VMATH_RANDF();
    n->func = func;
    n->inputs = NULL;
    n->outputs = NULL;
    n->total_input = 0;
    n->output = 0;
    n->num_inputs = 0;
    n->num_outputs = 0;
}

void neuron_update_output(neuron_st *n)
{
    n->total_input = n->bias;
    for(size_t i = 0; i < n->num_inputs; i++) {
        n->total_input += n->inputs[i]->weight * n->inputs[i]->source->output;
    }
    n->output = n->func(n->total_input);
}

void neuron_push_link(neuron_st *n, link_type_e type, link_st *link)
{
    switch(type) {
        case LINK_TYPE_INPUT:
            REALLOC(n->inputs, n->num_inputs + 1);
            n->inputs[n->num_inputs] = link;
            n->num_inputs++;
            break;

        case LINK_TYPE_OUTPUT:
            REALLOC(n->outputs, n->num_outputs + 1);
            n->outputs[n->num_outputs] = link;
            n->num_outputs++;
            break;
    }
}

void neuron_free(neuron_st *n)
{
    FREE(n->inputs);
    FREE(n->outputs);
}