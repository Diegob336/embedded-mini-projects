#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>

#define MPU_ADDR         	0x68
#define WHO_AM_I_REG     	0x75
#define PWR_MGMT_1_REG   	0x6B
#define SMPLRT_DIV_REG   	0X19
#define CONFIG_REG       	0x1A
#define GYRO_CONFIG_REG  	0X1B
#define ACCEL_CONFIG_REG 	0X1C

#define LSB_PER_G               16384.0
#define LSB_PER_DEG             131.0

i2c_master_dev_handle_t dev_handle;

void sendData(i2c_master_dev_handle_t *dev_handle, uint8_t addr, uint8_t val){
	
	uint8_t data[] = { addr, val };
	ESP_ERROR_CHECK(i2c_master_transmit(*dev_handle, data, sizeof(data), -1));
}



void handleData( void *parameter) {
	
	while(1){
	
		uint8_t sensor_data[14];  // accelX, accelY, accelZ, temp, gyroX, gyroY, gyroZ

		uint8_t reg_addr = 0x3B;
		ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &reg_addr, 1, sensor_data, 14, -1));

		// Parse the raw values
		int16_t raw_accel_x = (sensor_data[0] << 8) | sensor_data[1];
		int16_t raw_accel_y = (sensor_data[2] << 8) | sensor_data[3];
		int16_t raw_accel_z = (sensor_data[4] << 8) | sensor_data[5];

		int16_t temp_raw = (sensor_data[6] << 8) | sensor_data[7];

		int16_t raw_gyro_x = (sensor_data[8]  << 8) | sensor_data[9];
		int16_t raw_gyro_y = (sensor_data[10] << 8) | sensor_data[11];
		int16_t raw_gyro_z = (sensor_data[12] << 8) | sensor_data[13];

		float g_force_x = (float)raw_accel_x / LSB_PER_G;
		float g_force_y = (float)raw_accel_y / LSB_PER_G;
		float g_force_z = (float)raw_accel_z / LSB_PER_G;

		float ang_velocity_x = (float)raw_gyro_x / LSB_PER_DEG;
		float ang_velocity_y = (float)raw_gyro_y / LSB_PER_DEG;
		float ang_velocity_z = (float)raw_gyro_z / LSB_PER_DEG;

		//ESP_LOGI("MPU6050", "Accel: X=%f, Y=%f, Z=%f", g_force_x, g_force_y, g_force_z);
		//ESP_LOGI("MPU6050", "Gyro:  X=%f, Y=%f, Z=%f", ang_velocity_x, ang_velocity_y, ang_velocity_z);


		float roll  = atan2f(g_force_y, g_force_z) * 180.0f / 3.14159;
		float pitch = atan2f(-g_force_x, sqrtf(g_force_y * g_force_y + g_force_z * g_force_z)) * 180.0f / 3.14159;
 		
		ESP_LOGI("MPU6050", "Roll:%f  Pitch: %f", roll, pitch);		
		vTaskDelay(pdMS_TO_TICKS(100));
	}


}



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


	// Set sample rate divider, digital low pass filter, and full scale ranges for gyro and accelerometer

	sendData(&dev_handle ,SMPLRT_DIV_REG, 0x07); // sample rate divider
	
	sendData(&dev_handle ,CONFIG_REG, 0x03); // digital low pass filter

	sendData(&dev_handle, GYRO_CONFIG_REG, 0x00); // Gyro full scale range

	sendData(&dev_handle, ACCEL_CONFIG_REG, 0x00); // Accel full scale range


	xTaskCreatePinnedToCore(&handleData, "handling data", 3072, NULL, 1, NULL, 1);

}

