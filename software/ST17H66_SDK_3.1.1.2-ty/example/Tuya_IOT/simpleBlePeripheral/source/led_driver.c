
#include "gpio.h"
#include "led_driver.h"

#define LED_IO  P18

#define LED_NET_CFG_BLINK_MS    200
#define LED_NET_CONNECTING_BLINK_MS 500
#define LED_NET_CON_SUC_BLINK_MS 1000
#define LED_WIFI_CLOUD_CON_SUC_ON_MS 1000
#define LED_WIFI_CLOUD_CON_SUC_OFF_MS 5000


static led_status_t led_status = 0;

static uint16_t led_cnt_ms = 0;

typedef struct
{
    uint16_t on_ms;     //亮时间  0xffff:常亮 0:常灭
    uint16_t off_ms;    //灭时间
}led_blink_t;


static const led_blink_t blink_ctr[led_max] = 
{
    {0xffff},
    {LED_NET_CFG_BLINK_MS, LED_NET_CFG_BLINK_MS},
    {LED_NET_CONNECTING_BLINK_MS, LED_NET_CONNECTING_BLINK_MS},
    {LED_NET_CON_SUC_BLINK_MS, LED_NET_CON_SUC_BLINK_MS},
    {LED_WIFI_CLOUD_CON_SUC_ON_MS, LED_WIFI_CLOUD_CON_SUC_OFF_MS},
    {0, 0},
    {0xffff, 0},
    {0, 0},
};

void led_dirver_init(void)
{
    hal_gpio_pin_init(LED_IO, GPIO_OUTPUT);
    hal_gpio_pull_set(LED_IO, GPIO_PULL_UP_S);
    hal_gpio_fast_write(LED_IO, 1);

    //睡眠时保持IO状态
    hal_gpioretention_register(LED_IO);

    led_status = led_init;
}

void led_pro(uint16_t tick_ms)
{
    led_cnt_ms += tick_ms;

    if(blink_ctr[led_status].on_ms == 0)
    {
        hal_gpio_fast_write(LED_IO, 1);
    }
    else if(blink_ctr[led_status].on_ms == 0xffff)
    {
        hal_gpio_fast_write(LED_IO, 0);
    }
    else if(blink_ctr[led_status].on_ms > 0)
    {
        if((led_cnt_ms > (blink_ctr[led_status].on_ms + blink_ctr[led_status].off_ms)))
        {
            led_cnt_ms = 0;
        }
        else if(led_cnt_ms > blink_ctr[led_status].on_ms)
        {
            hal_gpio_fast_write(LED_IO, 1);
        }
        else if(led_cnt_ms <= blink_ctr[led_status].on_ms)
        {
            hal_gpio_fast_write(LED_IO, 0);
        }
    }
    
}

void led_set_status(led_status_t status)
{
    if(led_status != status)
    {
        led_cnt_ms = 0;
        led_status = status;
    }

}
