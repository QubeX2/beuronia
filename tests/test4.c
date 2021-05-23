/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 *
 * RECOGNITION OF LETTERS
 */
#include "layer.h"
#include "memory.h"
#include "network.h"
#include "neuron.h"
#include "vmath.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** LABELS
 * [offset] [type]          [value]          [description]
 *  0000     32 bit integer  0x00000801(2049) magic number (MSB first)
 *  0004     32 bit integer  60000            number of items
 *  0008     unsigned byte   ??               label
 *  0009     unsigned byte   ??               label
 *  ........
 *  xxxx     unsigned byte   ??               label
 */

/** IMAGES
 * [offset] [type]          [value]          [description]
 *  0000     32 bit integer  0x00000803(2051) magic number
 *  0004     32 bit integer  60000            number of images
 *  0008     32 bit integer  28               number of rows
 *  0012     32 bit integer  28               number of columns
 *  0016     unsigned byte   ??               pixel
 *  0017     unsigned byte   ??               pixel
 *  ........
 *  xxxx     unsigned byte   ??               pixel
 */

typedef struct image_dat_st {
    char magic[4];
    char num_images[4];
    char rows[4];
    char columns[4];
} image_dat_st;

typedef struct label_dat_st {
    char magic[4];
    char num_labels[4];
} label_dat_st;

#define INPUTS 728
#define HIDDEN1 64
#define HIDDEN2 32
#define OUTPUTS 10

uint8_t** images;
uint8_t* labels;

void load_images()
{
    FILE* fpimg = fopen("./data/train-images-idx3-ubyte", "rb");
    if (fpimg == NULL) {
        system_die("couldn't open file");
    }
    image_dat_st idat;
    fread(&idat, sizeof(idat), 1, fpimg);
    uint32_t num_images = vmath_bytes2uint32(idat.num_images);
    uint32_t rows = vmath_bytes2uint32(idat.rows);
    uint32_t columns = vmath_bytes2uint32(idat.columns);
    uint32_t size = rows * columns;

    MALLOC(images, num_images);

    char bytes[size];
    for (size_t t = 0; t < num_images; t++) {
        fread(bytes, size, 1, fpimg);
        MALLOC(images[t], size);
        memcpy(images[t], bytes, size);
    }
    fclose(fpimg);
}

void load_labels()
{
    FILE* fplbl = fopen("./data/train-labels-idx1-ubyte", "rb");
    if (fplbl == NULL) {
        system_die("couldn't open file");
    }
    label_dat_st ldat;
    fread(&ldat, sizeof(ldat), 1, fplbl);
    uint32_t num_labels = vmath_bytes2uint32(ldat.num_labels);

    MALLOC(labels, num_labels);

    char byte;
    for(size_t t = 0; t < num_labels; t++) {
        fread(&byte, 1, 1, fplbl);
        labels[t] = (uint8_t)byte;
    }

    fclose(fplbl);
}

void draw_label(size_t index)
{
    printf("Label: %d\n", labels[index]);
}

void draw_image(uint8_t* bytes, size_t size, size_t width)
{
    char dither[8] = { ' ', '.', '-', '=', '+', '*', '@', '#' };
    for (size_t i = 0; i < size; i++) {
        int idx = (uint8_t)bytes[i] >> 5;
        printf("%c", dither[idx]);
        if ((i % width) == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int main(void)
{
    load_images();
    load_labels();

    network_st netw;
    network_init(&netw, "network");
    size_t neuron_counts[] = { 728, 32, 16, 10 };
    network_setup(&netw, 4, neuron_counts, vmath_sigmoid);

    /**
     * train iterations
     */

    //network_print(&netw);

    size_t iterations = 10;
    double learning_rate = 0.05;
    double targets[OUTPUTS][OUTPUTS] = {
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    };

    double inputs[INPUTS];
    for(size_t iter = 0; iter < iterations; iter++) {
        int index = vmath_randomi(0, 59999);
        
        // set inputs
        uint8_t *bytes = images[index];
        for(size_t b = 0; b < INPUTS; b++) {
            inputs[b] = (double)bytes[b] / 255;
        }
        network_forward(&netw, inputs, INPUTS);

        // set targets
        uint8_t num = labels[index];
        network_backward(&netw, targets[num], 10);
        network_update_weights(&netw, learning_rate);
        if((network_print_training(&netw, targets[num], 10, iter)) == true) {
            draw_image(images[index], 728, 28);    
            draw_label(index);
        }
    }
    // save
    network_save(&netw, "../saves/my.netw");
    // cleanup
    network_destroy(&netw);
}
