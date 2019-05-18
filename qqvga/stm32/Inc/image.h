/*
 * image.h
 *
 *  Created on: 2019/02/19
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include "ov7670.h"
#include "arm_math.h"

/**
 * Enable averaging of 4x4 pixels
 */
#define AVERAGE_4x4

void qcif_to_128x128(uint16_t src_image[QQVGA_HEIGHT][QQVA_WIDTH], uint16_t dst_image[128][128]);

void qcif_to_32x32(uint16_t src_image[QQVGA_HEIGHT][QQVA_WIDTH], uint16_t dst_image[32][32]);

#ifdef OUTPUT_128x128
void diff(uint16_t prev_image[128][128], uint16_t image[128][128]);
#elif defined OUTPUT_32x32
void diff(uint16_t prev_image[32][32], uint16_t image[32][32]);
void to_grayscale(uint16_t image[32][32], uint8_t gray[32][32]);
void to_32x32x3(uint16_t src[32][32], float32_t dst[32][32][3]);
#endif

#endif /* IMAGE_H_ */
