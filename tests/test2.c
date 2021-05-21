#include <stdio.h>
#include "network.h"
#include "neuron.h"
#include "vmath.h"

int main(void)
{

    neuron_st ni, no;
    neuron_init(&ni, vmath_sigmoid);
    neuron_init(&no, vmath_sigmoid);    
    
    link_st l1;
    link_init(&l1, &ni, &no);

    ni.output = 2;
    neuron_update_output(&no);

    neuron_print(&ni);
    neuron_print(&no);
}