/*
 * image.c
 *
 *  Created on: 2019/02/19
 */

#include "image.h"
#include "ov7670.h"

// RGB565 (uint16_t)
#define RED_FIELD 0b1111100000000000
#define GREEN_FIELD 0b0000011111100000
#define BLUE_FIELD 0b0000000000011111
// Conversion macros
#define TO_RED(pixel) (((pixel) & RED_FIELD) >> 11)
#define TO_GREEN(pixel) (((pixel) & GREEN_FIELD) >> 5)
#define TO_BLUE(pixel) (((pixel) & BLUE_FIELD))
#define TO_RGB565(red, green, blue) ((red << 11) | (green << 5) | blue)

/**
 * Truncate QQVGA to 120x120
 */
void qqvga_to_120x120(uint16_t *pSrc, uint16_t *pDst) {
  // Raster scanning
  int src_idx = 0;
  int dst_idx = 0;
  for (int j=0; j<120; j++) {
    for (int i=0; i<120; i++) {
      src_idx = j * QQVGA_WIDTH + i + 20;
      dst_idx = j * 120 + i;
      pDst[dst_idx] = pSrc[src_idx];
    }
  }
}

/**
 * QCIF to 32x32
 *
 * This function applies 128x128 pixel window to cut out a square from QCIF,
 * then takes averages of each 4x4 pixels for RGB.
 */
void qqvga_to_30x30(uint16_t src_image[QQVGA_HEIGHT][QQVGA_WIDTH], uint16_t dst_image[32][32]) {
#ifdef AVERAGE_4x4
  uint16_t red, green, blue, pixel;
#endif
  for (int j=0; j<32; j++) {
    for (int i=0; i<32; i++) {
#ifdef AVERAGE_4x4
      red = 0;
      green = 0;
      blue = 0;
      // Sum of 16 pixels
      for (int l=0; l<4; l++) {
        for (int k=0; k<4; k++) {
          pixel = src_image[j*4+l+8][i*4+k+22];
          red += TO_RED(pixel);
          green += TO_GREEN(pixel);
          blue += TO_BLUE(pixel);
        }
      }
      // Average of 16 pixels
      red = red/16;
      green = green/16;
      blue = blue/16;
      dst_image[j][i] = TO_RGB565(red, green, blue);
#else
    // Significant noises w/o averaging
    dst_image[j][i] = src_image[j*4+8][i*4+22];
#endif
    }
  }
}

/**
 * Diff between frames.
 */
void diff(uint8_t *pCurrent, uint8_t *pPrev) {
  int idx;
  uint8_t pixel;
  for (int j=0; j<120; j++) {
    for (int i=0; i<240; i++) {

      idx = j*240+i;
      pixel = pCurrent[idx];
      pCurrent[idx] = pPrev[idx] - pCurrent[idx];
      if (pCurrent[idx] < 0) {
        pCurrent[idx] = 0;
      } else if (pCurrent[idx] > 255) {
        pCurrent[idx] = 255;
      }
      pPrev[idx] = pixel;
    }
  }
}
