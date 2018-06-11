#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_drv_timer.h"

#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "adxl.h"

/* TWI instance ID. */
#if TWI0_ENABLED
#define TWI_INSTANCE_ID     0
#elif TWI1_ENABLED
#define TWI_INSTANCE_ID     1
#endif


// TWI instance.
const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

// Timer instance.
const nrf_drv_timer_t TIMER_ADXL = NRF_DRV_TIMER_INSTANCE(0);

// Handler for timer events.
void timer_adxl_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    int16_t x_val;
		int16_t y_val;
		int16_t z_val;
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
						ADXL345_I2C_acc_data_read(&x_val,&y_val,&z_val);
						NRF_LOG_INFO("ACC DATA:  X: %d  Y: %d  Z: %d ", x_val ,y_val, z_val);
            break;
        default:
            //Do nothing.
            break;
    }
}


// Function for Timer Init.
void timer_init (void)
{
    uint32_t time_ms = 100; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;


    //Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_ADXL, &timer_cfg, timer_adxl_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_ADXL, time_ms);

    nrf_drv_timer_extended_compare(&TIMER_ADXL, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER_ADXL);
	
}

// TWI initialization.
void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = 
    {
       .scl                = ARDUINO_SCL_PIN,
       .sda                = ARDUINO_SDA_PIN,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config,NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}

int main(void)
{	
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("TWI scanner started.");

    twi_init();
    timer_init();
	
		ADXL345_I2C_init(&m_twi);
		ADXL345_I2C_data_rate_set(0x0A);
    ADXL345_I2C_offsets_set(0,0,0);

    while (true)
    {
				__WFI();
    }
}
