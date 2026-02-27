#include "led.h"
#include "hardware/gpio.h"

#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 25
#endif

static const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void led_init(void) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
}

void led_enable(void) {
    gpio_put(LED_PIN, 1);
}

void led_disable(void) {
    gpio_put(LED_PIN, 0);
}