/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stdbool.h>
#include "beuronia.h"
#include "vmath.h"
#include "neuron.h"

int main(void)
{
    // length, width, color 0, 1
    double t_dataset[8][3] = {
        { 2, 1, 0 },
        { 3, 1, 0 },
        { 2, .5, 0 },
        { 1, 1, 0 },

        { 3, 1.5, 1, },
        { 3.5, .5, 1, },
        { 4, 1.5, 1 },
        { 5.5, 1, 1},
        
    };

    // solve this
    //double u_point[3] = { 4.5, 1, -1};

    //double learning_rate = 0.2;

    neuron_st n;
    neuron_init(&n, 2);
    double learning_rate = 0.2;

    for(int i = 0; i < 100000; i++) {
        int w = vmath_randomi(0, 7);
        double *p = t_dataset[w];
        double target = p[2];
        double z = vmath_dot(p, n.weights, 2) + n.bias;
        double pred = vmath_sigmoid(z);
        //double cost = vmath_cost(pred, target);
        double slope = vmath_slope(pred, target);
        double dz = vmath_sigmoid(z) * (1 - vmath_sigmoid(z));
        double cost_w1 = slope * dz * p[0];
        double cost_w2 = slope * dz * p[1];
        double cost_b = slope * dz * 1;

        n.weights[0] -= learning_rate * cost_w1;
        n.weights[1] -= learning_rate * cost_w2;
        n.bias -= learning_rate * cost_b;

        /*
        printf("%d: %f, %f, %f\n", w, p[0], p[1], p[2]);
        printf("prediction: %f, target: %f\n", pred, target);
        printf("cost: %f\n", cost);
        printf("slope: %f\n", slope);
        printf("dz: %f\n", dz); */
        double pren = pred > 0.5 ? 1 : 0;
        bool right = (int)pren == (int)target ? true : false;
        printf("iteration: %d. prediction: %d, target: %d, object %d: prediction is %s\n", i, (int)pren, (int)target, w, (right ? "true" : "false"));
    }


    /*
    neuron_set_inputs(&n, 1.3, 2.4, 0.1);
    printf("output: %f\n", neuron_output(&n, vmath_relu));
    */

    return 0;
}