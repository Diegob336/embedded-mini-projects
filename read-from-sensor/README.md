# read-from-sensor (MPU6050 Sensor Prototype)

This project tests basic functionality of the MPU6050 sensor by reading accelerometer, gyroscope, and temperature data over I2C. It’s designed as a quick prototype to verify hardware and communication before using the sensor as a motion controller for a bigger project.

---

## What It Does

- Initializes MPU6050 sensor via I2C communication.
- Reads raw accelerometer, gyroscope, and temperature data periodically.
- Prints sensor readings to the console for verification.
- Lays groundwork for using motion data to control the Google Dinosaur game.
- Serves as a functional test bench for sensor integration.

---

## Core Concepts

- **I2C communication:** Interface with MPU6050 using ESP-IDF’s I2C driver.
- **Sensor data acquisition:** Periodic reading of accelerometer and gyro values.
- **Data verification:** Console logging to confirm accurate sensor output.
- **Prototyping:** Building blocks for future motion-controlled game input.

---

