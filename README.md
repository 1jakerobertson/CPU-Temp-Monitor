# CPU Temperature Monitor

A Raspberry Pi Pico project that reads the internal CPU temperature and displays it on a 16x2 I2C LCD in real time, built using the Pico C SDK.

## Features
- Reads internal CPU temperature via ADC
- Displays live temperature on a 16x2 I2C LCD
- Serial monitor output for debugging

## Hardware
- Raspberry Pi Pico
- 16x2 I2C LCD (IIC LCD1602)
- Breadboard and jumper wires

## Wiring

| LCD Pin | Pico Pin |
|---------|----------|
| GND     | GND (Pin 38) |
| VCC     | VBUS 5V (Pin 40) |
| SDA     | GP4 (Pin 6) |
| SCL     | GP5 (Pin 7) |

> Note: I2C address is 0x27

## Dependencies
- Pico SDK 2.2.0
- CMake 3.13+
- Ninja build system
- arm-none-eabi-gcc toolchain

## Build

```bash
# Configure
cmake -S . -B build

# Build
cmake --build build
```

## Flash

Put Pico in BOOTSEL mode then:
```bash
copy build\project_files.uf2 E:\
```

Or if already running:
```bash
picotool reboot -f -u
copy build\project_files.uf2 E:\
```

## Serial Monitor
- Baud rate: 115200
- Open serial monitor in VS Code
- Shows temperature readings and loop iterations

## License
MIT