
#include "gpio.h"
#include "uart.h"

#include "ty_mcu_sdk_app.h"
#include "wifi.h"
#include "OSAL.h"
#include "log.h"
#include "pwrmgr.h"

#include "led_driver.h"
#include "chx_driver.h"
#include "key_driver.h"

//ty power IO
#define TY_POWER_IO P15

#define TY_APP_RELOAD_TIME 10

static uint8_t ty_task_id = 0;

static void ty_uart_init(uart_Hdl_t evt_handler)
{
    uart_Cfg_t cfg_user =
    {
        .tx_pin = P2,
        .rx_pin = P3,

        .rts_pin = GPIO_DUMMY,
        .cts_pin = GPIO_DUMMY,
        .baudrate = 9600,
        .use_fifo = TRUE,
        .hw_fwctrl = FALSE,
        .use_tx_buf = FALSE,
        .parity = FALSE,
        .evt_handler = evt_handler,
    };
    // hal_gpio_pin_init(P14, OEN);
    // hal_gpio_pin_init(P15, IE);
    // m_uartCtx.enable = FALSE;
    hal_uart_init(cfg_user, UART1); // uart init

    //power on
    hal_gpio_pin_init(TY_POWER_IO, GPIO_OUTPUT);
    hal_gpio_pull_set(TY_POWER_IO, GPIO_PULL_UP_S);
    hal_gpio_fast_write(TY_POWER_IO, 1);
    //睡眠时保持IO状态
    hal_gpioretention_register(TY_POWER_IO);
}

void ty_uart_send_byte(uint8_t data)
{
    hal_uart_send_byte(UART1, data);
}

static void ty_uart_event_handle(uart_Evt_t* pev)
{
    if(pev == NULL)
    {
        return;
    }

    if(pev->len > 0)
    {
        uart_receive_buff_input(pev->data, pev->len);
    }
}

static void ty_key_event_cb(key_event_t *e)
{
    if(e->event == KEY_EVENT_LONG)
    {
        osal_set_event(ty_task_id, TY_KEY_LONG_EVENT);
    }
}

void ty_mcu_sdk_app_init(uint8_t task_id)
{
    ty_task_id =  task_id;

    //锁住系统，不休眠
    hal_pwrmgr_register(MOD_USR8, NULL, NULL);
    hal_pwrmgr_lock(MOD_USR8);

    key_driver_init(ty_key_event_cb);

    wifi_protocol_init();

    ty_uart_init(ty_uart_event_handle);

    osal_start_timerEx(ty_task_id, TY_UART_REC_DATA, TY_APP_RELOAD_TIME);

}

uint16_t ty_mcu_sdk_app_event(uint8_t task_id, uint16_t events)
{
    if(task_id != ty_task_id)
    {
        return 0;
    }

    if( events & TY_UART_REC_DATA)
    {
        //LOG("wifi state = %d\n", mcu_get_wifi_work_state());
        switch(mcu_get_wifi_work_state())
        {
            case SMART_CONFIG_STATE:
            case AP_STATE:
            case SMART_AND_AP_STATE:

                led_set_status(led_net_cfg);

                break;
            case WIFI_NOT_CONNECTED:

                led_set_status(led_net_connect);

                break;
            case WIFI_CONNECTED:

                led_set_status(led_net_con_suc);

                break;
            case WIFI_CONN_CLOUD:

                led_set_status(led_cloud_con_suc);

                break;

            case WIFI_LOW_POWER:

                led_set_status(led_wifi_low_power);

                break;

            default:
                break;  
        }

        wifi_uart_service();

        led_pro(TY_APP_RELOAD_TIME);

        chx_pro(TY_APP_RELOAD_TIME);

        key_pro(TY_APP_RELOAD_TIME);

        osal_start_timerEx(ty_task_id, TY_UART_REC_DATA, TY_APP_RELOAD_TIME);

        return (events ^ TY_UART_REC_DATA);
    }

    if( events & TY_KEY_LONG_EVENT)
    {
        LOG("TY_KEY_LONG_EVENT, state = %d\r\n", mcu_get_wifi_work_state());
        //判断当前状态，未配网->进入配网，已配网->退网
        switch(mcu_get_wifi_work_state())
        {
            case SMART_CONFIG_STATE:
            case AP_STATE:
            case SMART_AND_AP_STATE:

                //配网中，不处理

                break;
            case WIFI_NOT_CONNECTED:
            case WIFI_CONNECTED:
            case WIFI_CONN_CLOUD:

                //退网
                mcu_reset_wifi();

                break;

            case WIFI_LOW_POWER:

                //进入配网
                mcu_set_wifi_mode(AP_CONFIG);

                break;

            default:
                break;  
        }

        return (events ^ TY_KEY_LONG_EVENT);
    }
		
		return 0;
}

void ty_rec_open_cmd(void)
{
    LOG("open cmd\n");

    chx_key_on(chx_key1);
}

void ty_rec_close_cmd(void)
{
    LOG("close cmd\n");

    chx_key_on(chx_key3);
}

void ty_rec_pause_cmd(void)
{
    LOG("pause cmd\n");

    chx_key_on(chx_key2);
}

