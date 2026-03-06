#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);

    printf("Echo ready\r\n");

    while (true) {
        int ch = getchar();
        printf("received char: %c [ %d ]\r\n", ch, ch);
    }
}