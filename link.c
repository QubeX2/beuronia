/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */

#include <stdio.h>
#include <string.h>
#include "link.h"
#include "vmath.h"
#include "memory.h"

void link_print(link_st *link)
{
    printf("\t\t\tLink: { .name=%s, .is_dead=%s, .weight=%f }\n", 
        link->name, 
        link->is_dead ? "true" : "false", 
        link->weight
    );
}

void link_init(link_st *link, char *name, neuron_st *source, neuron_st *dest)
{
    link->name = strdup(name);
    link->source = source;
    link->dest = dest;
    link->is_dead = false;
    link->weight = VMATH_RANDF();

    neuron_push_link(source, LINK_TYPE_OUTPUT, link);
    neuron_push_link(dest, LINK_TYPE_INPUT, link);
}

extern void link_free(link_st *link)
{
    FREE(link->name);
    FREE(link->source);
    FREE(link->dest);
}