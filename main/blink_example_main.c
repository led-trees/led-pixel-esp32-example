#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

static uint8_t s_led_state = 1;

#define BLINK_GPIO_1 15
#define BLINK_GPIO_2 18
#define BLINK_GPIO_3 6

#define LED_STRIP_LEDS_COUNT 2048
#define REZOLUTION_HZ 10 * 1000 * 1000 // 10MHz

static led_strip_handle_t led_strip_1;
static led_strip_handle_t led_strip_2;
static led_strip_handle_t led_strip_3;

static void blink_led(led_strip_handle_t *led_strip, u_int64_t type)
{

        for (int i = 0; i < 100;i++)
        {
            
            u_char color = rand() % 3; 

            if(type == 0)
            switch(color)
            {
                case 0:
                    led_strip_set_pixel(*led_strip, i, 255, 0, 0);
                    break;
                case 1:
                    led_strip_set_pixel(*led_strip, i, 0, 255, 0);
                    break;
                case 2:
                    led_strip_set_pixel(*led_strip, i, 0, 0, 255);
                    break;
            }

            if(type == 1)
            switch(color)
            {
                case 0:
                    led_strip_set_pixel(*led_strip, i, 255, 0, 255);
                    break;
                case 1:
                    led_strip_set_pixel(*led_strip, i, 255, 255, 0);
                    break;
                case 2:
                    led_strip_set_pixel(*led_strip, i, 0, 255, 255);
                    break;
            }

            if(type == 2)
            switch(color)
            {
                case 0:
                    led_strip_set_pixel(*led_strip, i, 128, 128, 128);
                    break;
                case 1:
                    led_strip_set_pixel(*led_strip, i, 255, 255, 255);
                    break;
                case 2:
                    led_strip_set_pixel(*led_strip, i, 0, 0, 0);
                    break;
            }
        }
        led_strip_refresh(*led_strip);
}

static void configure_led(led_strip_handle_t *led_strip, uint32_t max_leds, u_char gpio_num, u_int32_t resolution_hz)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    led_strip_config_t strip_config = { 
        .strip_gpio_num = gpio_num,
        .max_leds = max_leds, 
        
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = resolution_hz,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, led_strip));
    led_strip_clear(*led_strip);
}

void app_main(void)
{
    led_strip_handle_t *led_strip_1_ptr = &led_strip_1;
    led_strip_handle_t *led_strip_2_ptr = &led_strip_2;
    led_strip_handle_t *led_strip_3_ptr = &led_strip_3;

    configure_led(led_strip_1_ptr, LED_STRIP_LEDS_COUNT, BLINK_GPIO_1, REZOLUTION_HZ);
    configure_led(led_strip_2_ptr, LED_STRIP_LEDS_COUNT, BLINK_GPIO_2, REZOLUTION_HZ);
    configure_led(led_strip_3_ptr, LED_STRIP_LEDS_COUNT, BLINK_GPIO_3, REZOLUTION_HZ);

    while (1) {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led(led_strip_1_ptr, 0);
        blink_led(led_strip_2_ptr, 1);
        blink_led(led_strip_3_ptr, 2);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
