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
#ifdef OUTPUT_128x128
void diff(uint16_t prev_image[128][128], uint16_t image[128][128]) {
#elif defined OUTPUT_32x32
void diff(uint16_t prev_image[32][32], uint16_t image[32][32]) {
#endif
  int32_t red, green, blue;
  uint16_t prev_pixel, pixel;
#ifdef OUTPUT_128x128
  for (int j=0; j<128; j++) {
    for (int i=0; i<128; i++) {
#elif defined OUTPUT_32x32
  for (int j=0; j<32; j++) {
    for (int i=0; i<32; i++) {
#endif
#if defined OUTPUT_128x128 || defined OUTPUT_32x32
      prev_pixel = prev_image[j][i];
      pixel = image[j][i];
      red = TO_RED(prev_pixel) - TO_RED(pixel);
      green = TO_GREEN(prev_pixel) - TO_GREEN(pixel);
      blue = TO_BLUE(prev_pixel) - TO_BLUE(pixel);
      if (red < 0) red = 0;
      if (green < 0) green = 0;
      if (blue < 0) blue = 0;
      if (red > 255) red = 255;
      if (green > 255) green = 255;
      if (blue > 255) blue = 255;
      prev_image[j][i] = image[j][i];
      image[j][i] = TO_RGB565(red, green, blue);
    }
  }
}
#endif

#ifdef OUTPUT_32x32
/**
 * Convert RGB to gray scale.
 * Note: taking Y component of YUV format might be better.
 */
void to_grayscale(uint16_t image[32][32], uint8_t gray[32][32]) {
  uint16_t pixel;
  for (int j=0; j<32; j++) {
    for (int i=0; i<32; i++) {
      pixel = image[j][i];
      // convert RGB into Y
      gray[j][i] = 0.299 * (TO_RED(pixel) << 3) + 0.587 * (TO_GREEN(pixel) << 2) + 0.144 * (TO_BLUE(pixel) << 3);
    }
  }
}

/**
 * Convert 32x32 RGB565 into 32x32x3 float array
 */
void to_32x32x3(uint16_t src[32][32], float32_t dst[32][32][3]) {
  uint16_t pixel;
  for (int j=0; j<32; j++) {
    for (int i=0; i<32; i++) {
      pixel = src[j][i];
      dst[j][i][0] = (float32_t)(TO_RED(pixel));
      dst[j][i][1] = (float32_t)(TO_GREEN(pixel));
      dst[j][i][2] = (float32_t)(TO_BLUE(pixel));
    }
  }
}
#endif
