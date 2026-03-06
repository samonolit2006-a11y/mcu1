#include "adc-task.h"

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/time.h"

#define ADC_GPIO 26
#define ADC_CHANNEL 0
#define TEMP_ADC_CHANNEL 4
#define ADC_TASK_MEAS_PERIOD_US 100000

static adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
static uint64_t last_meas_time_us = 0;

void adc_task_init(void)
{
    adc_init();
    adc_gpio_init(ADC_GPIO);
    adc_set_temp_sensor_enabled(true);
}

float adc_task_get_voltage(void)
{
    adc_select_input(ADC_CHANNEL);

    uint16_t voltage_counts = adc_read();
    float voltage_V = voltage_counts * 3.3f / 4096.0f;

    return voltage_V;
}

float adc_task_get_temp(void)
{
    adc_select_input(TEMP_ADC_CHANNEL);

    uint16_t temp_counts = adc_read();
    float temp_V = temp_counts * 3.3f / 4096.0f;
    float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;

    return temp_C;
}

void adc_task_set_state(adc_task_state_t state)
{
    adc_state = state;

    if (state == ADC_TASK_STATE_RUN)
    {
        last_meas_time_us = time_us_64();
    }
}

void adc_task_handle(void)
{
    if (adc_state != ADC_TASK_STATE_RUN)
    {
        return;
    }

    uint64_t now_us = time_us_64();

    if ((now_us - last_meas_time_us) >= ADC_TASK_MEAS_PERIOD_US)
    {
        last_meas_time_us = now_us;

        float voltage_V = adc_task_get_voltage();
        float temp_C = adc_task_get_temp();

        printf("%f %f\n", voltage_V, temp_C);
    }
}