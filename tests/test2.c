#include <stdio.h>
#include "network.h"
#include "layer.h"
#include "neuron.h"
#include "vmath.h"

int main(void)
{
    network_st netw;
    network_init(&netw, "network");

    layer_st lin, lout;
    layer_init(&lin, "layer in");
    layer_init(&lout, "layer out");

    network_push_layer(&netw, &lin);
    network_push_layer(&netw, &lout);

    neuron_st ni, no;
    neuron_init(&ni, "nn in", vmath_sigmoid);
    neuron_init(&no, "nn out", vmath_sigmoid);    

    layer_push_neuron(&lin, &ni);
    layer_push_neuron(&lout, &no);
    
    link_st l1;
    link_init(&l1, "link 1.1", &ni, &no);

    ni.output = 2;
    neuron_update_output(&no);

    network_print(&netw);
}