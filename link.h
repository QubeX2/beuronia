/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */

#ifndef __LINK_H__
#define __LINK_H__

#include "types.h"
#include "neuron.h"

struct link_st {
    neuron_st *source;
    neuron_st *dest;
    double weight;
    double cost_derivate;
    double acc_cost_derivate;
};

extern void link_init(link_st *link, neuron_st *source, neuron_st *dest);

#endif /* __LINK_H__ */