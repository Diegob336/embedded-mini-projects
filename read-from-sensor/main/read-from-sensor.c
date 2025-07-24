#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_log.h"

#define MPU_ADDR         0x68
#define WHO_AM_I_REG     0x75
#define PWR_MGMT_1_REG   0x6B


void app_main(void) {
	
	i2c_master_bus_handle_t bus_handle; 	

	i2c_master_bus_config_t bus_config = {
		.i2c_port = I2C_NUM_0,
		.sda_io_num = 21,
		.scl_io_num = 22,
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.glitch_ignore_cnt = 7,
		.flags.enable_internal_pullup = true,
	};

	ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

	i2c_master_dev_handle_t dev_handle;

	i2c_device_config_t dev_config = {
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = MPU_ADDR,
		.scl_speed_hz = 400000,
	};


	ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle));

	uint8_t wake_up[2]  = { PWR_MGMT_1_REG, 0x00};

	ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, wake_up, sizeof(wake_up), -1));

	uint8_t Who_am_i_reg = WHO_AM_I_REG;
	uint8_t who_am_i = 0;

ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &Who_am_i_reg, 1, &who_am_i, 1, -1));
	ESP_LOGI("MPU6050", "who_am_i = 0x%X", who_am_i);
}
