#include "led-task.h"
#include "led.h"

#include "pico/stdlib.h"


static uint32_t LED_BLINK_PERIOD_US = 500000; // период мигания
static uint64_t led_ts = 0;
static led_state_t led_state = LED_STATE_OFF;

void led_task_init(void)
{
    led_init();
    led_state = LED_STATE_OFF;
    led_ts = 0;
    led_disable();
}

void led_task_state_set(led_state_t state)
{
    led_state = state;
    led_ts = time_us_64(); // чтобы blink стартовал сразу
}

void led_task_set_blink_period_ms(uint32_t period_ms)
{
    // period_ms в миллисекундах, а переменная в микросекундах
    LED_BLINK_PERIOD_US = period_ms * 1000u;
}

void led_task_process(void)
{
    switch (led_state)
    {
        case LED_STATE_OFF:
            led_disable();
            break;

        case LED_STATE_ON:
            led_enable();
            break;

        case LED_STATE_BLINK:
            if (time_us_64() >= led_ts)
            {
                led_ts = time_us_64() + (LED_BLINK_PERIOD_US / 2);

                // переключаем состояние
                static bool on = false;
                on = !on;
                if (on) led_enable();
                else    led_disable();
            }
            break;

        default:
            break;
    }
}