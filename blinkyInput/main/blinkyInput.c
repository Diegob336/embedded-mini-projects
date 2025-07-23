#include <stdio.h>
#include <stdlib.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"

#define LED          2

volatile int led_delay_ms = 500;

QueueHandle_t uart_queue;

void task1(void *paramater){
	while(1){
		vTaskDelay(pdMS_TO_TICKS(led_delay_ms));
		gpio_set_level(LED, 1);
		vTaskDelay(pdMS_TO_TICKS(led_delay_ms));
		gpio_set_level(LED, 0);
	}
}


void task2(void *paramater){

	uint8_t data[60];
	uart_event_t event;
	char input_buffer[60];
	uint8_t input_buffer_index = 0;

	while(1){
		if(xQueueReceive(uart_queue, &event, portMAX_DELAY)){
			switch(event.type) {
				case UART_DATA: {
					
									uint32_t length = uart_read_bytes(UART_NUM_0, data, event.size, portMAX_DELAY);
									if (length > 0) {
										ESP_LOGI("UART", "Received %d bytes", length);
										for (int i = 0; i < length; i ++){
											if(data[i] == '\n' || data[i] == '\r'){
												input_buffer[input_buffer_index] = '\0';
												int new_delay = atoi((char *)input_buffer);
												led_delay_ms = new_delay;

												input_buffer_index = 0;

												ESP_LOGI("UART", "updated LED delay to %d ms\n", new_delay);					
											} 
											else if (input_buffer_index < sizeof(input_buffer) - 1) {
												input_buffer[input_buffer_index] = data[i];
												input_buffer_index++;
											}
										}

									} else {
										ESP_LOGI("UART", "No bytes received");
									}
									break;
								}
				default: 
								break;
			}
		}	
	}
}


void app_main(void)
{
	TaskHandle_t task1Handle = NULL;

	const uart_port_t uart_num = UART_NUM_0;

	ESP_ERROR_CHECK(uart_driver_install(uart_num, 1024, 1024, 10, &uart_queue,0));
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.rx_flow_ctrl_thresh = 122,
	};

	ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

	gpio_set_direction(LED, GPIO_MODE_OUTPUT);

	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	xTaskCreatePinnedToCore(&task1, "blinkLed", 1024, NULL, 1, &task1Handle, 1);

	xTaskCreatePinnedToCore(&task2, "Uart_input", 3072, NULL, 2, NULL, 1);
}
