/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */

#include "link.h"
#include "vmath.h"
#include "memory.h"

void link_init(link_st *link, neuron_st *source, neuron_st *dest)
{
    link->source = source;
    link->dest = dest;
    link->is_dead = false;
    link->weight = VMATH_RANDF();

    neuron_push_link(source, LINK_TYPE_OUTPUT, link);
    neuron_push_link(dest, LINK_TYPE_INPUT, link);
}

extern void link_free(link_st *link)
{
    FREE(link->source);
    FREE(link->dest);
}