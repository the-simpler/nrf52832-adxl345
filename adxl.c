#include "adxl.h"

// ADXL345 read.
void ADXL345_I2C_register_read(uint8_t slave_addr, uint8_t reg_addr,  uint8_t * pdata, uint32_t bytes)
{   
    ret_code_t ret_code = nrf_drv_twi_tx(&m_twi,slave_addr, &reg_addr,1,false);
    APP_ERROR_CHECK(ret_code);
    
    ret_code = nrf_drv_twi_rx(&m_twi,slave_addr, pdata, bytes);
    APP_ERROR_CHECK(ret_code);
}

// ADXL345 write.
void ADXL345_I2C_register_write(uint8_t slave_addr,uint8_t * pdata, uint32_t bytes) 
{
    ret_code_t ret_code = nrf_drv_twi_tx(&m_twi, slave_addr, pdata, bytes , false);   
    APP_ERROR_CHECK(ret_code);
}

// ADXL345 read accelerometer
void ADXL345_I2C_acc_data_read(int16_t* x_value, int16_t* y_value, int16_t* z_value)
{
    uint8_t rx_buffer[6];
    ADXL345_I2C_register_read(ADXL345_ADDR, DATAX0, rx_buffer, sizeof(rx_buffer));    
    
    *x_value = (rx_buffer[1]<<8)|rx_buffer[0];
    *y_value = (rx_buffer[3]<<8)|rx_buffer[2];
    *z_value = (rx_buffer[5]<<8)|rx_buffer[4];
}

// ADXL345 init
void ADXL345_I2C_init(const nrf_drv_twi_t* p_twi_instance)
{
    // Put the ADXL345 in Measurement Mode by writing 0x08 to the POWER_CTL Register
    uint8_t tx_data[2] = {POWER_CTL, 0x08}; 
    ADXL345_I2C_register_write(ADXL345_ADDR, tx_data, sizeof(tx_data));
}

// ADXL345 Data Rate Set.
void ADXL345_I2C_data_rate_set(uint8_t rate)
{   
    uint8_t tx_data[2] = {BW_RATE,rate};
    ADXL345_I2C_register_write(ADXL345_ADDR,tx_data, sizeof(tx_data));
    
    uint8_t rx_data = 0x00;
    ADXL345_I2C_register_read(ADXL345_ADDR, BW_RATE, &rx_data,1);
    
    if(rx_data == rate) {
        NRF_LOG_INFO("Setting Data rate Sucessful!");
    }
    else {
        NRF_LOG_INFO("Setting Data rate Failed!");
    }
}

// ADXL345 Offsets Set.
void ADXL345_I2C_offsets_set(uint8_t x_offset, uint8_t y_offset,uint8_t z_offset)
{   
    uint8_t tx_data[4] = {OFSX, x_offset,y_offset,z_offset};
    ADXL345_I2C_register_write(ADXL345_ADDR, tx_data, sizeof(tx_data));
    
    uint8_t rx_data[3] ={0x00,0x00,0x00};
    ADXL345_I2C_register_read(ADXL345_ADDR, OFSX, rx_data, sizeof(rx_data)); 
    
    if( rx_data[0] == x_offset &&
        rx_data[1] == y_offset &&
        rx_data[2] == z_offset) {
        NRF_LOG_INFO(0,"Setting Offsets Sucessful! \n");
    } else {
        NRF_LOG_INFO(0,"Setting Offsets Failed! \n");
    }
}
