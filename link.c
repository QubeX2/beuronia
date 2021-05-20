/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */

#include "link.h"
#include "vmath.h"

void link_init(link_st *link, neuron_st *source, neuron_st *dest)
{
    link->source = source;
    link->dest = dest;
    link->weight = VMATH_RANDF();
}