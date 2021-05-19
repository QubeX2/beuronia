/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include "beuronia.h"
#include "vmath.h"
#include "neuron.h"

int main(void)
{
    neuron_st n;
    neuron_init(&n, 3);
    neuron_set_inputs(&n, 1.3, 2.4, 0.1);

    printf("output: %f\n", neuron_output(&n));

    printf("slope: %f\n", vmath_slope(5.8, 4));

    return 0;
}