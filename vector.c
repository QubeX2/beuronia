#include "vector.h"

double vector_dot(neuron_st n, int a, int b) {
    return n.inputs[a] * n.weights[a] + n.inputs[b] * n.weights[b];
}

double vector_sigmoid(double x) {
    return 1 / (1 + exp(-x));
}