/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */

#include "link.h"
#include "memory.h"
#include "vmath.h"
#include <stdio.h>
#include <string.h>

void link_print(link_st* link)
{
    printf("\t\t\tLink: { .name=%s, .weight=%f, .err_der=%f, .acc_err_der=%f, .num_acc_err_ders=%zu }\n",
        link->name,
        link->weight,
        link->error_derivate,
        link->acc_error_derivate,
        link->num_acc_derivates);
}

void link_init(link_st* link, char* name, neuron_st* source, neuron_st* dest)
{
    link->name = strdup(name);
    link->source = source;
    link->dest = dest;
    link->weight = VMATH_RANDF();
    link->error_derivate = 0;
    link->acc_error_derivate = 0;
    link->num_acc_derivates = 0;

    neuron_push_link(source, LINK_TYPE_OUTPUT, link);
    neuron_push_link(dest, LINK_TYPE_INPUT, link);
}

extern void link_free(link_st* link)
{
    FREE(link->name);
}