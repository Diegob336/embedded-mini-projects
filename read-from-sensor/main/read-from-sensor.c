#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_log.h"

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

	ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle)));

	i2c_device_handle_t dev_handle;

	i2c_device_config_t dev_config = {
		dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = I2C_ADDRESS,
		.scl_speed_hz = 1000000;
	}


	ESP_ERROR_CHECK(i2c_new_master_bus_add_device(bus_config, &dev_config, &dev_handle)));
}
