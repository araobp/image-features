/*
 * ov7670_arducam.h
 *
 *  Created on: 2019/05/18
 */

#ifndef OV7670_VIDEOCAM_H_
#define OV7670_VIDEOCAM_H_

#include "ov7670.h"
#include "stm32f4xx_hal.h"
#include <string.h>

// VGA Default
#define ThirtyFPS

typedef enum {
  THIRTY_FPS, NIGHT_MODE
} fps_mode;

char AWBParam[] = "SAWB";
char AECParam[] = "HistAEC";
char YUVMatrixParam[] = "YUVMatrixOn";
char DenoiseParam[] = "DenoiseNo";
char EdgeParam[] = "EdgeNo";
char ABLCParam[] = "AblcON";

// Camera input/output pin connection to Arduino
#define WRST  25      // Output Write Pointer Reset
#define RRST  23      // Output Read Pointer Reset
#define WEN   24      // Output Write Enable
#define VSYNC 22      // Input Vertical Sync marking frame capture
#define RCLK  26      // Output FIFO buffer output clock
// set OE to low gnd

// FIFO Ram input pins
#define DO7   28
#define DO6   29
#define DO5   30
#define DO4   31
#define DO3   32
#define DO2   33
#define DO1   34
#define DO0   35

// Register addresses and values
#define CLKRC                 0x11
#define CLKRC_VALUE_VGA       0x01  // Raw Bayer
#define CLKRC_VALUE_QVGA      0x01
#define CLKRC_VALUE_QQVGA     0x01
#define CLKRC_VALUE_NIGHTMODE_FIXED   0x03 // Fixed Frame
#define CLKRC_VALUE_NIGHTMODE_AUTO    0x80 // Auto Frame Rate Adjust

#define COM7                                   0x12
#define COM7_VALUE_VGA                         0x01   // Raw Bayer
#define COM7_VALUE_VGA_COLOR_BAR               0x03   // Raw Bayer
#define COM7_VALUE_VGA_PROCESSED_BAYER         0x05   // Processed Bayer
#define COM7_VALUE_QVGA                        0x00
#define COM7_VALUE_QVGA_COLOR_BAR              0x02
#define COM7_VALUE_QVGA_PREDEFINED_COLOR_BAR   0x12
#define COM7_VALUE_QQVGA                       0x00
#define COM7_VALUE_QQVGA_COLOR_BAR             0x02
#define COM7_VALUE_QCIF                        0x08     // Predefined QCIF format
#define COM7_VALUE_COLOR_BAR_QCIF              0x0A     // Predefined QCIF Format with ColorBar
#define COM7_VALUE_RESET                       0x80

#define COM3                            0x0C
#define COM3_VALUE_VGA                  0x00 // Raw Bayer
#define COM3_VALUE_QVGA                 0x04
#define COM3_VALUE_QQVGA                0x04  // From Docs
#define COM3_VALUE_QQVGA_SCALE_ENABLED  0x0C  // Enable Scale and DCW
#define COM3_VALUE_QCIF                 0x0C  // Enable Scaling and enable DCW

#define COM14                            0x3E
#define COM14_VALUE_VGA                  0x00 // Raw Bayer
#define COM14_VALUE_QVGA                 0x19
#define COM14_VALUE_QQVGA                0x1A
#define COM14_VALUE_MANUAL_SCALING       0x08   // Manual Scaling Enabled
#define COM14_VALUE_NO_MANUAL_SCALING    0x00   // Manual Scaling DisEnabled

#define SCALING_XSC                                  0x70
#define SCALING_XSC_VALUE_VGA                        0x3A  // Raw Bayer
#define SCALING_XSC_VALUE_QVGA                       0x3A
#define SCALING_XSC_VALUE_QQVGA                      0x3A
#define SCALING_XSC_VALUE_QQVGA_SHIFT1               0x3A
#define SCALING_XSC_VALUE_COLOR_BAR                  0xBA
#define SCALING_XSC_VALUE_QCIF_COLOR_BAR_NO_SCALE    0x80 // Predefined QCIF with Color Bar and NO Scaling

#define SCALING_YSC                                   0x71
#define SCALING_YSC_VALUE_VGA                         0x35 // Raw Bayer
#define SCALING_YSC_VALUE_QVGA                        0x35
#define SCALING_YSC_VALUE_QQVGA                       0x35
#define SCALING_YSC_VALUE_COLOR_BAR                   0x35  // 8 bar color bar
#define SCALING_YSC_VALUE_COLOR_BAR_GREY              0xB5  // fade to grey color bar
#define SCALING_YSC_VALUE_COLOR_BAR_SHIFT1            0xB5  // fade to grey color bar
#define SCALING_YSC_VALUE_QCIF_COLOR_BAR_NO_SCALE     0x00  // Predefined QCIF with Color Bar and NO Scaling

#define SCALING_DCWCTR               0x72
#define SCALING_DCWCTR_VALUE_VGA     0x11  // Raw Bayer
#define SCALING_DCWCTR_VALUE_QVGA    0x11
#define SCALING_DCWCTR_VALUE_QQVGA   0x22

#define SCALING_PCLK_DIV              0x73
#define SCALING_PCLK_DIV_VALUE_VGA    0xF0 // Raw Bayer
#define SCALING_PCLK_DIV_VALUE_QVGA   0xF1
#define SCALING_PCLK_DIV_VALUE_QQVGA  0xF2

#define SCALING_PCLK_DELAY              0xA2
#define SCALING_PCLK_DELAY_VALUE_VGA    0x02 // Raw Bayer
#define SCALING_PCLK_DELAY_VALUE_QVGA   0x02
#define SCALING_PCLK_DELAY_VALUE_QQVGA  0x02

// Controls YUV order Used with COM13
// Need YUYV format for Android Decoding- Default value is 0xD
#define TSLB                                         0x3A
#define TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_ENABLED   0x01 // No custom scaling
#define TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_DISABLED  0x00 // For adjusting HSTART, etc. YUYV format
#define TSLB_VALUE_UYVY_AUTO_OUTPUT_WINDOW_DISABLED  0x08
#define TSLB_VALUE_TESTVALUE                         0x04 // From YCbCr Reference

// Default value is 0x88
// ok if you want YUYV order, no need to change
#define COM13                      0x3D
#define COM13_VALUE_DEFAULT        0x88
#define COM13_VALUE_NOGAMMA_YUYV   0x00
#define COM13_VALUE_GAMMA_YUYV     0x80
#define COM13_VALUE_GAMMA_YVYU     0x82
#define COM13_VALUE_YUYV_UVSATAUTOADJ_ON 0x40

// Works with COM4
#define COM17                                 0x42
#define COM17_VALUE_AEC_NORMAL_NO_COLOR_BAR   0x00
#define COM17_VALUE_AEC_NORMAL_COLOR_BAR      0x08 // Activate Color Bar for DSP

#define COM4   0x0D

// RGB Settings and Data format
#define COM15    0x40

// Night Mode
#define COM11                             0x3B
#define COM11_VALUE_NIGHTMODE_ON          0x80     // Night Mode
#define COM11_VALUE_NIGHTMODE_OFF         0x00
#define COM11_VALUE_NIGHTMODE_ON_EIGHTH   0xE0     // Night Mode 1/8 frame rate minimum
#define COM11_VALUE_NIGHTMODE_FIXED       0x0A
#define COM11_VALUE_NIGHTMODE_AUTO        0xEA     // Night Mode Auto Frame Rate Adjust

// Color Matrix Control YUV
#define MTX1      0x4f
#define MTX1_VALUE  0x80

#define MTX2      0x50
#define MTX2_VALUE  0x80

#define MTX3      0x51
#define MTX3_VALUE  0x00

#define MTX4      0x52
#define MTX4_VALUE  0x22

#define MTX5      0x53
#define MTX5_VALUE  0x5e

#define MTX6      0x54
#define MTX6_VALUE  0x80

#define CONTRAS     0x56
#define CONTRAS_VALUE 0x40

#define MTXS      0x58
#define MTXS_VALUE  0x9e

// COM8
#define COM8                    0x13
#define COM8_VALUE_AWB_OFF      0xE5
#define COM8_VALUE_AWB_ON       0xE7

// Automatic White Balance
#define AWBC1     0x43
#define AWBC1_VALUE 0x14

#define AWBC2     0x44
#define AWBC2_VALUE 0xf0

#define AWBC3     0x45
#define AWBC3_VALUE   0x34

#define AWBC4     0x46
#define AWBC4_VALUE 0x58

#define AWBC5         0x47
#define AWBC5_VALUE 0x28

#define AWBC6     0x48
#define AWBC6_VALUE 0x3a

#define AWBC7           0x59
#define AWBC7_VALUE     0x88

#define AWBC8          0x5A
#define AWBC8_VALUE    0x88

#define AWBC9          0x5B
#define AWBC9_VALUE    0x44

#define AWBC10         0x5C
#define AWBC10_VALUE   0x67

#define AWBC11         0x5D
#define AWBC11_VALUE   0x49

#define AWBC12         0x5E
#define AWBC12_VALUE   0x0E

#define AWBCTR3        0x6C
#define AWBCTR3_VALUE  0x0A

#define AWBCTR2        0x6D
#define AWBCTR2_VALUE  0x55

#define AWBCTR1        0x6E
#define AWBCTR1_VALUE  0x11

#define AWBCTR0                0x6F
#define AWBCTR0_VALUE_NORMAL   0x9F
#define AWBCTR0_VALUE_ADVANCED 0x9E

// Gain
#define COM9                        0x14
#define COM9_VALUE_MAX_GAIN_128X    0x6A
#define COM9_VALUE_4XGAIN           0x10    // 0001 0000

#define BLUE          0x01    // AWB Blue Channel Gain
#define BLUE_VALUE    0x40

#define RED            0x02    // AWB Red Channel Gain
#define RED_VALUE      0x40

#define GGAIN            0x6A   // AWB Green Channel Gain
#define GGAIN_VALUE      0x40

#define COM16     0x41
#define COM16_VALUE 0x08 // AWB Gain on

#define GFIX      0x69
#define GFIX_VALUE  0x00

// Edge Enhancement Adjustment
#define EDGE_  0x3f
#define EDGE_VALUE  0x00

#define REG75     0x75
#define REG75_VALUE 0x03d

#define REG76     0x76
#define REG76_VALUE 0xe1

// DeNoise
#define DNSTH     0x4c
#define DNSTH_VALUE 0x00

#define REG77     0x77
#define REG77_VALUE 0x00

// Denoise and Edge Enhancement
#define COM16_VALUE_DENOISE_OFF_EDGE_ENHANCEMENT_OFF_AWBGAIN_ON     0x08 // Denoise off, AWB Gain on
#define COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_OFF__AWBGAIN_ON    0x18
#define COM16_VALUE_DENOISE_OFF__EDGE_ENHANCEMENT_ON__AWBGAIN_ON    0x28
#define COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_ON__AWBGAIN_ON     0x38 // Denoise on,  Edge Enhancement on, AWB Gain on

// 30FPS Frame Rate , PCLK = 24Mhz
#define CLKRC_VALUE_30FPS  0x80

#define DBLV               0x6b
#define DBLV_VALUE_30FPS   0x0A

#define EXHCH              0x2A
#define EXHCH_VALUE_30FPS  0x00

#define EXHCL              0x2B
#define EXHCL_VALUE_30FPS  0x00

#define DM_LNL               0x92
#define DM_LNL_VALUE_30FPS   0x00

#define DM_LNH               0x93
#define DM_LNH_VALUE_30FPS   0x00

#define COM11_VALUE_30FPS    0x0A

// Saturation Control
#define SATCTR      0xc9
#define SATCTR_VALUE  0x60

// AEC/AGC - Automatic Exposure/Gain Control
#define GAIN    0x00
#define GAIN_VALUE  0x00

#define AEW     0x24
#define AEW_VALUE 0x95

#define AEB     0x25
#define AEB_VALUE 0x33

#define VPT     0x26
#define VPT_VALUE 0xe3

// AEC/AGC Control- Histogram
#define HAECC1      0x9f
#define HAECC1_VALUE  0x78

#define HAECC2      0xa0
#define HAECC2_VALUE  0x68

#define HAECC3      0xa6
#define HAECC3_VALUE  0xd8

#define HAECC4      0xa7
#define HAECC4_VALUE  0xd8

#define HAECC5      0xa8
#define HAECC5_VALUE  0xf0

#define HAECC6      0xa9
#define HAECC6_VALUE  0x90

#define HAECC7                          0xaa  // AEC Algorithm selection
#define HAECC7_VALUE_HISTOGRAM_AEC_ON 0x94
#define HAECC7_VALUE_AVERAGE_AEC_ON     0x00

// Array Control
#define CHLF      0x33
#define CHLF_VALUE  0x0b

#define ARBLM     0x34
#define ARBLM_VALUE 0x11

// ADC Control
#define ADCCTR1     0x21
#define ADCCTR1_VALUE 0x02

#define ADCCTR2     0x22
#define ADCCTR2_VALUE 0x91

#define ADC_     0x37
#define ADC_VALUE_       0x1d

#define ACOM      0x38
#define ACOM_VALUE  0x71

#define OFON      0x39
#define OFON_VALUE  0x2a

// Black Level Calibration
#define ABLC1     0xb1
#define ABLC1_VALUE 0x0c

#define THL_ST    0xb3
#define THL_ST_VALUE  0x82

// Window Output
#define HSTART               0x17
#define HSTART_VALUE_DEFAULT 0x11
#define HSTART_VALUE_VGA     0x13
#define HSTART_VALUE_QVGA    0x13
#define HSTART_VALUE_QQVGA   0x13   // Works

#define HSTOP                0x18
#define HSTOP_VALUE_DEFAULT  0x61
#define HSTOP_VALUE_VGA      0x01
#define HSTOP_VALUE_QVGA     0x01
#define HSTOP_VALUE_QQVGA    0x01   // Works

#define HREF                  0x32
#define HREF_VALUE_DEFAULT    0x80
#define HREF_VALUE_VGA        0xB6
#define HREF_VALUE_QVGA       0x24
#define HREF_VALUE_QQVGA      0xA4

#define VSTRT                0x19
#define VSTRT_VALUE_DEFAULT  0x03
#define VSTRT_VALUE_VGA      0x02
#define VSTRT_VALUE_QVGA     0x02
#define VSTRT_VALUE_QQVGA    0x02

#define VSTOP                0x1A
#define VSTOP_VALUE_DEFAULT  0x7B
#define VSTOP_VALUE_VGA      0x7A
#define VSTOP_VALUE_QVGA     0x7A
#define VSTOP_VALUE_QQVGA    0x7A

#define VREF                 0x03
#define VREF_VALUE_DEFAULT   0x03
#define VREF_VALUE_VGA       0x0A
#define VREF_VALUE_QVGA      0x0A
#define VREF_VALUE_QQVGA     0x0A

// I2C
#define OV7670_I2C_ADDRESS                 0x21
#define I2C_ERROR_WRITING_START_ADDRESS      11
#define I2C_ERROR_WRITING_DATA               22

#define DATA_TOO_LONG                  1      // data too long to fit in transmit buffer
#define NACK_ON_TRANSMIT_OF_ADDRESS    2      // received NACK on transmit of address
#define NACK_ON_TRANSMIT_OF_DATA       3      // received NACK on transmit of data
#define OTHER_ERROR                    4      // other error

#define I2C_READ_START_ADDRESS_ERROR        33
#define I2C_READ_DATA_SIZE_MISMATCH_ERROR   44

void ReadRegisters() {
  uint8_t data = 0;

  sccb_read(CLKRC, &data);
  printf("CLKRC = %x\n", data);

  sccb_read(COM7, &data);
  printf("COM7 = %x\n", data);

  sccb_read(COM3, &data);
  printf("COM3 = %x\n", data);

  sccb_read(COM14,&data);
  printf("COM14 = %x\n", data);

  sccb_read(SCALING_XSC, &data);
  printf("SCALING_XSC = %x\n", data);

  sccb_read(SCALING_YSC, &data);
  printf("SCALING_YSC = %x\n", data);

  sccb_read(SCALING_DCWCTR, &data);
  printf("SCALING_DCWCTR = %x\n", data);

  sccb_read(SCALING_PCLK_DIV, &data);
  printf("SCALING_PCLK_DIV = %x\n", data);

  sccb_read(SCALING_PCLK_DELAY, &data);
  printf("SCALING_PCLK_DELAY = %x\n", data);

  // default value D
  sccb_read(TSLB, &data);
  printf("TSLB (YUV Order- Higher Bit, Bit[3]) = %x\n", data);

  // default value 88
  sccb_read(COM13, &data);
  printf("COM13 (YUV Order - Lower Bit, Bit[1]) = %x\n", data);

  sccb_read(COM17, &data);
  printf("COM17 (DSP Color Bar Selection) = %x\n", data);

  sccb_read(COM4, &data);
  printf("COM4 (works with COM 17) = %x\n", data);

  sccb_read(COM15, &data);
  printf("COM15 (COLOR FORMAT SELECTION) = %x\n", data);

  sccb_read(COM11, &data);
  printf("COM11 (Night Mode) = %x\n", data);

  sccb_read(COM8, &data);
  printf("COM8 (Color Control, AWB) = %x\n", data);

  sccb_read(HAECC7, &data);
  printf("HAECC7 (AEC Algorithm Selection) = %x\n", data);

  sccb_read(GFIX, &data);
  printf("GFIX = %x\n", data);

  // Window Output
  sccb_read(HSTART, &data);
  printf("HSTART = %x\n", data);

  sccb_read(HSTOP, &data);
  printf("HSTOP = %x\n", data);

  sccb_read(HREF, &data);
  printf("HREF = %x\n", data);

  sccb_read(VSTRT, &data);
  printf("VSTRT = %x\n", data);

  sccb_read(VSTOP, &data);
  printf("VSTOP = %x\n", data);

  sccb_read(VREF, &data);
  printf("VREF = %x\n", data);
}

void ResetCameraRegisters() {
  // Reset Camera Registers
  // Reading needed to prevent error
  uint8_t result = sccb_write(COM7, COM7_VALUE_RESET);
  printf("OV7670WriteReg: %d\n", result);

  // Delay at least 500ms
  HAL_Delay(500);
}

void SetupCameraAdvancedAutoWhiteBalanceConfig() {
  sccb_write(AWBC1, AWBC1_VALUE);
  sccb_write(AWBC2, AWBC2_VALUE);
  sccb_write(AWBC3, AWBC3_VALUE);
  sccb_write(AWBC4, AWBC4_VALUE);
  sccb_write(AWBC5, AWBC5_VALUE);
  sccb_write(AWBC6, AWBC6_VALUE);
  sccb_write(AWBC7, AWBC7_VALUE);
  sccb_write(AWBC8, AWBC8_VALUE);
  sccb_write(AWBC9, AWBC9_VALUE);
  sccb_write(AWBC10, AWBC10_VALUE);
  sccb_write(AWBC11, AWBC11_VALUE);
  sccb_write(AWBC12, AWBC12_VALUE);
  sccb_write(AWBCTR3, AWBCTR3_VALUE);
  sccb_write(AWBCTR2, AWBCTR2_VALUE);
  sccb_write(AWBCTR1, AWBCTR1_VALUE);
}

void SetupCameraUndocumentedRegisters() {
  sccb_write(0xB0, 0x84);
}

void SetupCameraFor30FPS() {
  sccb_write(CLKRC, CLKRC_VALUE_30FPS);
  sccb_write(DBLV, DBLV_VALUE_30FPS);
  sccb_write(EXHCH, EXHCH_VALUE_30FPS);
  sccb_write(EXHCL, EXHCL_VALUE_30FPS);
  sccb_write(DM_LNL, DM_LNL_VALUE_30FPS);
  sccb_write(DM_LNH, DM_LNH_VALUE_30FPS);
  sccb_write(COM11, COM11_VALUE_30FPS);
}

void SetupCameraABLC() {
  if (strcmp(ABLCParam, "AblcOFF") == 0) {
    return;
  }
  sccb_write(ABLC1, ABLC1_VALUE);
  sccb_write(THL_ST, THL_ST_VALUE);
}

void SetupCameraAverageBasedAECAGC() {
  sccb_write(AEW, AEW_VALUE);
  sccb_write(AEB, AEB_VALUE);
  sccb_write(VPT, VPT_VALUE);
  sccb_write(HAECC7, HAECC7_VALUE_AVERAGE_AEC_ON);
}

void SetCameraHistogramBasedAECAGC() {
  sccb_write(AEW, AEW_VALUE);
  sccb_write(AEB, AEB_VALUE);
  sccb_write(HAECC1, HAECC1_VALUE);
  sccb_write(HAECC2, HAECC2_VALUE);
  sccb_write(HAECC3, HAECC3_VALUE);
  sccb_write(HAECC4, HAECC4_VALUE);
  sccb_write(HAECC5, HAECC5_VALUE);
  sccb_write(HAECC6, HAECC6_VALUE);
  sccb_write(HAECC7, HAECC7_VALUE_HISTOGRAM_AEC_ON);
}

void SetupCameraNightMode() {
  sccb_write(CLKRC, CLKRC_VALUE_NIGHTMODE_AUTO);
  sccb_write(COM11, COM11_VALUE_NIGHTMODE_AUTO);
}

void SetupCameraSimpleAutomaticWhiteBalance() {
  sccb_write(COM8, COM8_VALUE_AWB_ON);
  sccb_write(AWBCTR0, AWBCTR0_VALUE_NORMAL);
}

void SetupCameraAdvancedAutomaticWhiteBalance() {
  sccb_write(0x13, 0xE7);
  sccb_write(0x6f, 0x9E);
}

void SetupCameraGain() {
  sccb_write(COM9, COM9_VALUE_4XGAIN);
  sccb_write(BLUE, BLUE_VALUE);
  sccb_write(RED, RED_VALUE);
  sccb_write(GGAIN, GGAIN_VALUE);
  sccb_write(COM16, COM16_VALUE);
}

void SetCameraSaturationControl() {
  sccb_write(SATCTR, SATCTR_VALUE);
}

void SetCameraColorMatrixYUV() {
  sccb_write(MTX1, MTX1_VALUE);
  sccb_write(MTX2, MTX2_VALUE);
  sccb_write(MTX3, MTX3_VALUE);
  sccb_write(MTX4, MTX4_VALUE);
  sccb_write(MTX5, MTX5_VALUE);
  sccb_write(MTX6, MTX6_VALUE);
  sccb_write(CONTRAS, CONTRAS_VALUE);
  sccb_write(MTXS, MTXS_VALUE);
}

void SetCameraFPSMode(fps_mode mode) {
  if (mode == THIRTY_FPS) {
    SetupCameraFor30FPS();
  } else if (mode == NIGHT_MODE) {
    SetupCameraNightMode();
  }
}

void SetCameraAEC() {
  // Process AEC
  if (strcmp(AECParam, "AveAEC") == 0) {
    // Set Camera's Average AEC/AGC Parameters
    SetupCameraAverageBasedAECAGC();
  } else if (strcmp(AECParam, "HistAEC") == 0) {
    // Set Camera AEC algorithim to Histogram
    SetCameraHistogramBasedAECAGC();
  }
}

void SetupCameraAWB() {
  // Set AWB Mode
  if (strcmp(AWBParam, "SAWB") == 0) {
    // Set Simple Automatic White Balance
    SetupCameraSimpleAutomaticWhiteBalance(); // OK

    // Set Gain Config
    SetupCameraGain();
  } else if (strcmp(AWBParam, "AAWB") == 0) {
    // Set Advanced Automatic White Balance
    SetupCameraAdvancedAutomaticWhiteBalance(); // ok

    // Set Camera Automatic White Balance Configuration
    SetupCameraAdvancedAutoWhiteBalanceConfig(); // ok

    // Set Gain Config
    SetupCameraGain();
  }
}

void SetupCameraDenoise() {
  sccb_write(DNSTH, DNSTH_VALUE);
  sccb_write(REG77, REG77_VALUE);
}

void SetupCameraEdgeEnhancement() {
  sccb_write(EDGE_, EDGE_VALUE);
  sccb_write(REG75, REG75_VALUE);
  sccb_write(REG76, REG76_VALUE);
}

void SetupCameraDenoiseEdgeEnhancement() {
  if (strcmp(DenoiseParam, "DenoiseYes") == 0 && strcmp(EdgeParam, "EdgeYes") == 0) {
    SetupCameraDenoise();
    SetupCameraEdgeEnhancement();
    sccb_write(COM16,
        COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_ON__AWBGAIN_ON);
  } else if (strcmp(DenoiseParam, "DenoiseYes") == 0 && strcmp(EdgeParam, "EdgeNo") == 0) {
    SetupCameraDenoise();
    sccb_write(COM16,
        COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_OFF__AWBGAIN_ON);
  } else if (strcmp(DenoiseParam, "DenoiseNo") == 0 && strcmp(EdgeParam, "EdgeYes") == 0) {
    SetupCameraEdgeEnhancement();
    sccb_write(COM16,
        COM16_VALUE_DENOISE_OFF__EDGE_ENHANCEMENT_ON__AWBGAIN_ON);
  }
}

void SetupCameraArrayControl() {
  sccb_write(CHLF, CHLF_VALUE);
  sccb_write(ARBLM, ARBLM_VALUE);
}

void SetupCameraADCControl() {
  sccb_write(ADCCTR1, ADCCTR1_VALUE);
  sccb_write(ADCCTR2, ADCCTR2_VALUE);
  sccb_write(ADC_, ADC_VALUE_);
  sccb_write(ACOM, ACOM_VALUE);
  sccb_write(OFON, OFON_VALUE);
}

void SetupOV7670ForQQVGAYUV(fps_mode mode) {
  sccb_write(CLKRC, CLKRC_VALUE_QQVGA);
  sccb_write(COM7, COM7_VALUE_QQVGA);
  sccb_write(COM3, COM3_VALUE_QQVGA);
  sccb_write(COM14, COM14_VALUE_QQVGA);
  sccb_write(SCALING_XSC, SCALING_XSC_VALUE_QQVGA);
  sccb_write(SCALING_YSC, SCALING_YSC_VALUE_QQVGA);
  sccb_write(SCALING_DCWCTR, SCALING_DCWCTR_VALUE_QQVGA);
  sccb_write(SCALING_PCLK_DIV, SCALING_PCLK_DIV_VALUE_QQVGA);
  sccb_write(SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_VALUE_QQVGA);
  sccb_write(TSLB, TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_DISABLED); // Works
  sccb_write(COM13, 0xC8);  // Gamma Enabled, UV Auto Adj On
  sccb_write(COM17, COM17_VALUE_AEC_NORMAL_NO_COLOR_BAR);

  // Set Additional Parameters
  // Set Camera Frames per second
  SetCameraFPSMode(mode);

  // Set Camera Automatic Exposure Control
  SetCameraAEC();

  // Set Camera Automatic White Balance
  SetupCameraAWB();

  // Setup Undocumented Registers - Needed Minimum
  SetupCameraUndocumentedRegisters();

  // Set Color Matrix for YUV
  if (strcmp(YUVMatrixParam, "YUVMatrixOn") == 0) {
    SetCameraColorMatrixYUV();
  }

  // Set Camera Saturation
  SetCameraSaturationControl();

  // Denoise and Edge Enhancement
  SetupCameraDenoiseEdgeEnhancement();

  // Set New Gamma Values
  //SetCameraGamma();

  // Set Array Control
  SetupCameraArrayControl();

  // Set ADC Control
  SetupCameraADCControl();

  // Set Automatic Black Level Calibration
  SetupCameraABLC();

  // Change Window Output parameters after custom scaling
  sccb_write(HSTART, HSTART_VALUE_QQVGA);
  sccb_write(HSTOP, HSTOP_VALUE_QQVGA);
  sccb_write(HREF, HREF_VALUE_QQVGA);
  sccb_write(VSTRT, VSTRT_VALUE_QQVGA);
  sccb_write(VSTOP, VSTOP_VALUE_QQVGA);
  sccb_write(VREF, VREF_VALUE_QQVGA);
}

#endif /* OV7670_VIDEOCAM_H_ */
