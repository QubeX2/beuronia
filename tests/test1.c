/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stdbool.h>
#include "../beuronia.h"
#include "../vmath.h"
#include "../node.h"

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
    double u_point[3] = { 4.5, 1, -1};

    node_st n;
    node_init(&n, 2, vmath_sigmoid);
    double learning_rate = 0.1;

    for(int i = 0; i < 100000; i++) {
        int w = vmath_randomi(0, 7);
        n.target = t_dataset[w][2];
        node_set_inputs(&n, t_dataset[w][0], t_dataset[w][1]);
        
        double prediction = node_predict(&n, learning_rate);

        int p = prediction > 0.5 ? 1 : 0;
        int t = (int)n.target;

        printf("iteration: %d. object %d: prediction is %s\n", i, w, (p == t ? "true" : "false"));
    }

    // predict unknown
    node_set_inputs(&n, u_point[0], u_point[1]);
    n.target = 1;
    double pp = node_predict(&n, 0);
    printf("unkown prediction: %f\n", pp);

    return 0;
}

