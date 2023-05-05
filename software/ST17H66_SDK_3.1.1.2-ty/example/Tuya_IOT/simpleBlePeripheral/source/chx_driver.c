
#include "OSAL.h"
#include "gpio.h"

#include "chx_driver.h"

#define CHX_POWER_IO    P14
#define CHX_EN_IO       P34
#define CHX_IN0_IO      P7
#define CHX_IN1_IO      P11

#define ON_KEY_TIME_MS 200

enum{

    STEP_NORMAL = 0,
    STEP_POWER_ON,
    STEP_KEY_ON,
    STEP_EN_ON,
    STEP_WAIT_ON

};

typedef struct
{
    chx_on_key_t on_key;        //当前打开的按键
    uint16_t on_key_cnt;        //打开按键计时
    uint16_t on_key_interval;   //打开按键的间隔时间

    uint8_t step;               
}chx_driver_t;

static chx_driver_t chx_dirver = {0};

void chx_driver_init(void)
{
    //power
    hal_gpio_pin_init(CHX_POWER_IO, GPIO_OUTPUT);
    hal_gpio_pull_set(CHX_POWER_IO, GPIO_PULL_UP_S);
    hal_gpio_fast_write(CHX_POWER_IO, 0);
    //睡眠时保持IO状态
    hal_gpioretention_register(CHX_POWER_IO);

    //ch en
    hal_gpio_pin_init(CHX_EN_IO, GPIO_OUTPUT);
    hal_gpio_pull_set(CHX_EN_IO, GPIO_PULL_UP_S);
    hal_gpio_fast_write(CHX_EN_IO, 1);
    //睡眠时保持IO状态
    hal_gpioretention_register(CHX_EN_IO);

    //IN0
    hal_gpio_pin_init(CHX_IN0_IO, GPIO_OUTPUT);
    hal_gpio_pull_set(CHX_IN0_IO, GPIO_PULL_UP_S);
    hal_gpio_fast_write(CHX_IN0_IO, 1);
    //睡眠时保持IO状态
    hal_gpioretention_register(CHX_IN0_IO);

    //IN1
    hal_gpio_pin_init(CHX_IN1_IO, GPIO_OUTPUT);
    hal_gpio_pull_set(CHX_IN1_IO, GPIO_PULL_UP_S);
    hal_gpio_fast_write(CHX_IN1_IO, 1);
    //睡眠时保持IO状态
    hal_gpioretention_register(CHX_IN1_IO);

    chx_dirver.on_key = chx_key_max;
    chx_dirver.on_key_interval = 0;
    chx_dirver.step = STEP_NORMAL;
}

static void chx_hal_key_on(chx_on_key_t key)
{
    switch(key)
    {
        case chx_key1:

            hal_gpio_fast_write(CHX_IN0_IO, 0);
            hal_gpio_fast_write(CHX_IN1_IO, 0);

            break;

        case chx_key2:

            hal_gpio_fast_write(CHX_IN0_IO, 1);
            hal_gpio_fast_write(CHX_IN1_IO, 0);

            break;

        case chx_key3:

            hal_gpio_fast_write(CHX_IN0_IO, 0);
            hal_gpio_fast_write(CHX_IN1_IO, 1);

            break;

        case chx_key4:

            hal_gpio_fast_write(CHX_IN0_IO, 1);
            hal_gpio_fast_write(CHX_IN1_IO, 1);

            break;

        default:
            break;
    }
}

uint8_t chx_pro(uint16_t tick_ms)
{
    switch(chx_dirver.step)
    {
        case STEP_NORMAL:
            break;

        case STEP_POWER_ON:

            hal_gpio_fast_write(CHX_POWER_IO, 1);

            chx_dirver.step = STEP_KEY_ON;

            break;

        case STEP_KEY_ON:

            chx_dirver.step = STEP_EN_ON;

            chx_hal_key_on(chx_dirver.on_key);

            break;

        case STEP_EN_ON:

            chx_dirver.on_key_cnt = 0;

            chx_dirver.step = STEP_WAIT_ON;

            hal_gpio_fast_write(CHX_EN_IO, 0);


            break;

        case STEP_WAIT_ON:

            chx_dirver.on_key_cnt += tick_ms;

            if(chx_dirver.on_key_cnt >= ON_KEY_TIME_MS)
            {
                hal_gpio_fast_write(CHX_EN_IO, 1);

                hal_gpio_fast_write(CHX_IN0_IO, 1);
                hal_gpio_fast_write(CHX_IN1_IO, 1);

                hal_gpio_fast_write(CHX_POWER_IO, 0);

                chx_dirver.on_key = chx_key_max;
                chx_dirver.on_key_interval = 0;
                chx_dirver.step = STEP_NORMAL;
            }

            break;

        default:
            break;
    }

    return chx_dirver.step;
}

void chx_key_on(chx_on_key_t key)
{
    if(chx_dirver.on_key != chx_key_max)
    {
        return;
    }

    chx_dirver.on_key = key;
    chx_dirver.on_key_interval = 0;
    chx_dirver.step = STEP_POWER_ON;
}
