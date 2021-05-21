/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */

#ifndef __LINK_H__
#define __LINK_H__

#include <stdbool.h>
#include "types.h"
#include "neuron.h"

struct link_st {
    char *name;
    neuron_st *source;
    neuron_st *dest;
    bool is_dead;
    double weight;
    double cost_derivate;
    double acc_cost_derivate;
};

extern void link_print(link_st *link);
extern void link_init(link_st *link, char *name, neuron_st *source, neuron_st *dest);
extern void link_free(link_st *link);

#endif /* __LINK_H__ */