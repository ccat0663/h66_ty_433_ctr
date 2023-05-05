
#include "gpio.h"
#include "key_driver.h"

#define KEY_IO  P20

#define KEY_LONG_TIME_MS   3000

static key_event_cb cb_s = NULL;

static uint16_t key_cnt_ms = 0;

static void key_int_cb(gpio_pin_e pin,gpio_polarity_e type)
{
    if(pin != KEY_IO)
    {
        return;
    }

    if(cb_s && (type == POL_FALLING))
    {
        key_event_t e;
        e.event = KEY_EVENT_INT;

        cb_s(&e);
    }
}

void key_driver_init(key_event_cb cb)
{
    cb_s = cb;

    hal_gpio_pin_init(KEY_IO, GPIO_INPUT);
    hal_gpio_pull_set(KEY_IO, GPIO_PULL_UP_S);

    hal_gpioin_enable(KEY_IO);

    hal_gpioin_register(KEY_IO, NULL, key_int_cb);
}

uint8_t key_pro(uint16_t tick_ms)
{
    if(hal_gpio_read(KEY_IO) == 0)
    {
        if((key_cnt_ms & (1 << 15)) == 0)
        {
            key_cnt_ms += tick_ms;

            if(key_cnt_ms >= KEY_LONG_TIME_MS)
            {
                if(cb_s)
                {
                    key_event_t e;
                    e.event = KEY_EVENT_LONG;

                    cb_s(&e);

                    key_cnt_ms |= (1 << 15);
                }
            }
        }
    }
    else
    {
        key_cnt_ms = 0;
    }

    return (key_cnt_ms && 1);
}

