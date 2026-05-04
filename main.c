#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// I2C address for the LCD (default is often 0x27)
const int addr = 0x27;

// LCD Commands
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_FUNCTIONSET = 0x20;

// Flags for display on/off control
const int LCD_DISPLAYON = 0x04;
const int LCD_BACKLIGHT = 0x08;
const int LCD_ENABLE_BIT = 0x04;

// Modes
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
}

void lcd_toggle_enable(uint8_t val) {
    sleep_us(600);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(600);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(600);
}

void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear() {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | 0x02, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | 0x08, LCD_COMMAND);
    lcd_clear();
}

void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_send_byte(*s++, LCD_CHARACTER);
    }
}

float read_temperature() {
    adc_select_input(4);
    uint16_t raw = adc_read();
    float voltage = raw * 3.3f / (1 << 12);
    float temp_c = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temp_c;
}

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) sleep_ms(100);
    sleep_ms(500);
    printf("LCD initializing...\n");

    // Initialize I2C at 100 kHz
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // Initialize ADC and temperature sensor
    adc_init();
    adc_set_temp_sensor_enabled(true);

    lcd_init();
    printf("LCD initialized\n");

    int count = 0;
    int show_fahrenheit = 0;  // toggle flag
    char buf[16];

    while (1) {
        float temp_c = read_temperature();
        float temp_f = (temp_c * 9.0f / 5.0f) + 32.0f;

        printf("Loop: %d | C: %.2f | F: %.2f\n", count, temp_c, temp_f);

        // Only clear second line instead of whole screen (smoother)
        lcd_set_cursor(1, 0);
        lcd_string("                ");

        lcd_set_cursor(0, 0);
        lcd_string("CPU Temp:");

        lcd_set_cursor(1, 0);

        if (show_fahrenheit) {
            snprintf(buf, sizeof(buf), "%.2f F", temp_f);
        } else {
            snprintf(buf, sizeof(buf), "%.2f C", temp_c);
        }

        lcd_string(buf);

        show_fahrenheit = !show_fahrenheit;  // alternate
        count++;

        sleep_ms(1000);
    }

    return 0;
}