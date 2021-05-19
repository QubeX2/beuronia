#include "vector.h"

double vector_dot(neuron_st n, int size) {
    double r = 0;

    for(int i = 0; i < size; i++) {
        r += n.inputs[i] * n.weights[i] + n.bias;
    }

    return r;
}

double vector_sigmoid(double x) {
    return 1 / (1 + exp(-x));
}