# LED Blink Rate Control via UART (ESP-IDF + FreeRTOS)

This embedded project demonstrates how to dynamically control an LED's blink rate on the ESP32 using user input over UART. It uses ESP-IDF and FreeRTOS to create two tasks: one to blink an LED, and one to handle serial input from the user. Input is parsed as an integer (in milliseconds), which determines the LED's on/off delay.

---

##  What It Does

- An LED (GPIO 2) blinks with a delay specified by the user.
- A user sends a numeric value (e.g., `1000`) over UART.
- The LED delay is updated in real time using `atoi()` to parse the input string.
- FreeRTOS tasks handle blinking and UART input concurrently.
- A shared global variable is used to control the delay.

---

##  Core Concepts

| Feature               | Implementation                            |
|-----------------------|--------------------------------------------|
| UART Event Handling   | `uart_driver_install()` + `xQueueReceive()` |
| Input Buffering       | Character array with index tracking       |
| Input Parsing         | `atoi()` for string-to-integer conversion |
| LED Output            | `gpio_set_level()`                        |
| RTOS Features         | Task creation with `xTaskCreatePinnedToCore()` |
| Debugging             | `ESP_LOGI()` for UART logs and debugging  |

---

##  How to Build & Run

### 1. Set up ESP-IDF (if not already done)

### 2. Build

- idf.py set-target esp32
- idf.py build

### 3. Flash to board

- idf.py -p /dev/ttyUSB0 flash

### 4. Monitor

- idf.py -p /dev/ttyUSB0 monitor

