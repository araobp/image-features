/*
 * ov7670.c
 *
 *  Created on: 2019/02/12
 */

#include "ov7670.h"

//#define DEBUG

I2C_HandleTypeDef *phi2c;
DCMI_HandleTypeDef *phdcmi;

uint8_t pid;
uint8_t ver;

int sccb_write(uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2] = { 0 };
  HAL_StatusTypeDef status;

  buf[0] = reg_addr;
  buf[1] = data;
  status = HAL_I2C_Master_Transmit(phi2c, SCCB_WRITE_SLAVE_ADDR, buf, 2, 100);
  if (status == HAL_OK) {
    return 0;
  } else {
    return 1;
  }
}

int sccb_read(uint8_t reg_addr, uint8_t *pdata) {
  uint8_t buf[1] = { 0 };
  HAL_StatusTypeDef status;

  buf[0] = reg_addr;
  status = HAL_I2C_Master_Transmit(phi2c, SCCB_WRITE_SLAVE_ADDR, buf, 1, 100);
  if (status == HAL_OK) {
    status = HAL_I2C_Master_Receive(phi2c, SCCB_READ_SLAVE_ADDR, pdata, 1, 100);
    if (status == HAL_OK) {
      printf("reg_addr: %x, data: %x\n", reg_addr, *pdata);
      return 0;
    } else {
      return 2;
    }
  } else {
    return 1;
  }
}

void stop_capturing(void) {
  HAL_DCMI_Stop(phdcmi);
  HAL_Delay(30);
}

/**
 * Take a snapshot
 */
void ov7670_take_snapshot(uint32_t buf_addr, int len) {
  //HAL_StatusTypeDef status;
  //status = HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_SNAPSHOT, buf_addr, len);
  HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_SNAPSHOT, buf_addr, len);
  //printf("%lu\n", (uint32_t)status);
}

void ov7670_take_continuous(uint32_t buf_addr, int len) {
  HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_CONTINUOUS, buf_addr, len);
}

/**
 * OV7670 initialization
 */
void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi) {
  phi2c = p_hi2c;
  phdcmi = p_hdcmi;

#ifdef DEBUG
  printf("OV7670 init\n");
#endif

  // Hardware reset
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);  // GPIO PE1: LOW
  HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);  // GPIO PE1: HIGH
  HAL_Delay(100);

  // Software reset: reset all registers to default values
  sccb_write(COM7_ADDR, RESET_TO_DEFAULT);
  HAL_Delay(30);

  // Read product ID and version
  sccb_read(PID_ADDR, &pid);  // pid is 0x76
  sccb_read(VER_ADDR, &ver);  // ver is 0x73
#ifdef DEBUG
  printf("PID: 0x%x, VER: 0x%x\n", pid, ver);
#endif

}
