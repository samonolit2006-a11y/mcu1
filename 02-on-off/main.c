#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "main.h"
#include "led-task.h"
#include "protocol-task.h"

// ====== прототипы обработчиков команд ======
static void cmd_help(const char *args);
static void cmd_version(const char *args);
static void cmd_on(const char *args);
static void cmd_off(const char *args);
static void cmd_blink(const char *args);
static void led_blink_set_period_ms_callback(const char *args);
static void cmd_mem(const char *args);
static void cmd_wmem(const char *args);

// ====== таблица команд (device_api) ======
static api_t device_api[] = {
    { "version",    cmd_version, "get device name and firmware version" },
    { "on",         cmd_on,      "switch on led" },
    { "off",        cmd_off,     "switch off led" },
    { "blink",      cmd_blink,   "blink led" },
    { "help",       cmd_help,    "print commands description" },
    { "set_period", led_blink_set_period_ms_callback, "set blink period in ms (e.g. set_period 1000)" },
    { "mem",        cmd_mem,     "read memory: mem <addr_hex>" },
    { "wmem",       cmd_wmem,    "write memory: wmem <addr_hex> <value_hex>" },
    { NULL, NULL, NULL }   
};


// ====== обработчики команд ======

static void cmd_help(const char *args)
{
    (void)args;

    for (int i = 0; device_api[i].command_name != NULL; i++) {
        printf("Команда '%s': '%s'\r\n",
               device_api[i].command_name,
               device_api[i].command_help ? device_api[i].command_help : "");
    }
}

static void cmd_version(const char *args)
{
    (void)args;
    printf("%s FW %s\r\n", DEVICE_NAME, FW_VERSION);
}

static void cmd_on(const char *args)
{
    (void)args;
    led_task_state_set(LED_STATE_ON);
    printf("LED ON\r\n");
}

static void cmd_off(const char *args)
{
    (void)args;
    led_task_state_set(LED_STATE_OFF);
    printf("LED OFF\r\n");
}

static void cmd_blink(const char *args)
{
    (void)args;
    led_task_state_set(LED_STATE_BLINK);
    printf("LED BLINK\r\n");
}

static void led_blink_set_period_ms_callback(const char *args)
{
    uint32_t period_ms = 0;

    // args может быть NULL или пустая строка
    if (args == NULL) args = "";

    // парсим число
    sscanf(args, "%lu", (unsigned long*)&period_ms);

    if (period_ms == 0) {
        printf("Error: period_ms must be > 0\r\n");
        return;
    }

    led_task_set_blink_period_ms(period_ms);
    printf("Blink period set to %lu ms\r\n", (unsigned long)period_ms);
}

static void cmd_mem(const char *args)
{
    if (!args || strlen(args) == 0) {
        printf("Usage: mem <addr_hex>\r\n");
        return;
    }

    uint32_t addr = 0;

    // читаем hex
    sscanf(args, "%x", &addr);

    volatile uint32_t *ptr = (uint32_t *)addr;

    uint32_t value = *ptr;

    printf("0x%08X = 0x%08X\r\n", addr, value);
}

static void cmd_wmem(const char *args)
{
    if (!args) {
        printf("Usage: wmem <addr_hex> <value_hex>\r\n");
        return;
    }

    uint32_t addr = 0;
    uint32_t value = 0;

    int parsed = sscanf(args, "%x %x", &addr, &value);

    if (parsed != 2) {
        printf("Usage: wmem <addr_hex> <value_hex>\r\n");
        return;
    }

    volatile uint32_t *ptr = (uint32_t *)addr;

    *ptr = value;

    printf("Wrote 0x%08X to 0x%08X\r\n", value, addr);
}

// ====== main ======
int main(void)
{
    stdio_init_all();
    sleep_ms(2000);

    printf("Device ready\r\n");

    led_task_init();

    protocol_task_init(device_api);

    char line[128];
    size_t len = 0;

    while (true)
    {
        // обязательно вызываем обработчик машины состояний
        led_task_process();

        // неблокирующее чтение UART
        int ch = getchar_timeout_us(0);

        if (ch == PICO_ERROR_TIMEOUT)
            continue;

        // Enter
        if (ch == '\r' || ch == '\n')
        {
            if (len > 0)
            {
                line[len] = '\0';
                protocol_task_handle(line);
                len = 0;
            }
            continue;
        }

        // backspace
        if (ch == 8 || ch == 127)
        {
            if (len > 0)
                len--;
            continue;
        }

        // обычный символ
        if (len + 1 < sizeof(line))
        {
            line[len++] = (char)ch;
        }
        else
        {
            len = 0;
            printf("Line too long\r\n");
        }
    }
}