#ifndef ADXL_H
#define ADXL_H

#include "stdint.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_drv_twi.h"

#define ADXL345_ADDR        0x53U

// ADXL Registers Addresses
#define DEVICE_ID               0x00
#define OFSX                    0x1E
#define BW_RATE                 0x2C
#define POWER_CTL               0x2D
#define DATAX0                  0x32

extern const nrf_drv_twi_t m_twi;

void ADXL345_I2C_register_read(uint8_t slave_addr, uint8_t reg_addr,  uint8_t* pdata, uint32_t bytes);
void ADXL345_I2C_register_write(uint8_t slave_addr,uint8_t * pdata, uint32_t bytes);
void ADXL345_I2C_acc_data_read(int16_t* x_value, int16_t* y_value, int16_t* z_value);
void ADXL345_I2C_init(const nrf_drv_twi_t* p_twi_instance);
void ADXL345_I2C_data_rate_set(uint8_t rate);
void ADXL345_I2C_offsets_set(uint8_t x_offset, uint8_t y_offset,uint8_t z_offset);

#endif
