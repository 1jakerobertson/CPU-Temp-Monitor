# CPU Temperature Monitor

## Project Description

This project implements a CPU temperature monitor using the Raspberry Pi Pico microcontroller and an I2C LCD display. The device reads the internal temperature sensor of the Pico and displays it on the LCD screen in real-time.

## Wiring Diagram

Connect the I2C LCD display to the Raspberry Pi Pico as follows:

- LCD VCC → Pico 3.3V
- LCD GND → Pico GND
- LCD SDA → Pico GPIO 4 (I2C0 SDA)
- LCD SCL → Pico GPIO 5 (I2C0 SCL)

## How to Build and Flash

1. Ensure you have the Pico SDK installed and configured.
2. Open the project in VS Code with the Pico extension.
3. Run the "Compile Project" task to build the firmware.
4. Flash the firmware using the "Flash" task or "Run Project" task.

## Serial Monitor Setup

To monitor the serial output:

1. Connect the Pico to your computer via USB.
2. Use a serial monitor tool like `minicom`, `screen`, or the Arduino IDE serial monitor.
3. Set the baud rate to 115200.
4. The device will output temperature readings over serial.