
#include "mbed.h"
#include "BufferedSerial.h"


// Include the actual image
#include "flash_image.h"

// Cross include definitions
//#define DEBUG 1
//#define LED_STATUS 1
//DigitalOut led_blue(PF_2);
//DigitalOut led_green(PF_3);

#include "slip.h"
#include "esp_command.h"


int main() {
	DigitalOut led_blue(PF_2);
	DigitalOut led_green(PF_3);
    ESPCommand<BufferedSerial> esp(U5Tx, U5Rx, PF_4); // Change PTC6 to user button on your platform
    led_blue = 1;
    led_green = 1;

    for (int i = 0; i < FLASH_COUNT; i++) {
        if (!esp.flash_write(
                FLASH_PAGES[i].address,
                FLASH_PAGES[i].data,
                FLASH_PAGES[i].size))
            error("Error writing flash at 0x%05x!\r\n", FLASH_PAGES[i].address);
    }

    // Signal we're done
    printf("Finished\r\n");

    while (true) {
        led_green = !led_green; // toggle led
        wait(0.2f);
    }
}
