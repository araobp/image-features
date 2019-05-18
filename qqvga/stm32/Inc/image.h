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

void qqvga_to_120x120(uint16_t *pSrc, uint16_t *pDst);

void qqvga_to_30x30(uint16_t src_image[QQVGA_HEIGHT][QQVGA_WIDTH], uint16_t dst_image[32][32]);

#endif /* IMAGE_H_ */
