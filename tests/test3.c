/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 * 
 * RECOGNITION OF LETTERS
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "network.h"
#include "layer.h"
#include "neuron.h"
#include "vmath.h"

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
    uint32_t magic;
    uint32_t num_labels;
} label_dat_st;

int main(void)
{
    FILE *fpimg = fopen("./data/train-images-idx3-ubyte", "rb");
    FILE *fplbl = fopen("./data/train-labels-idx1-ubyte", "rb");

    image_dat_st idat;
    fread(&idat, sizeof(idat), 1, fpimg);

    uint32_t magic = vmath_bytes2uint32(idat.magic);
    uint32_t num_images = vmath_bytes2uint32(idat.num_images);
    uint32_t rows = vmath_bytes2uint32(idat.rows);
    uint32_t columns = vmath_bytes2uint32(idat.columns);

    printf("%d, %d, %d, %d", magic, num_images, rows, columns);

    fclose(fpimg);
    fclose(fplbl);
}
