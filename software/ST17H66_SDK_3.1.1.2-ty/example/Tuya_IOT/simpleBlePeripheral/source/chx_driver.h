#ifndef _CHX_DRIVER_H 
#define _CHX_DRIVER_H 
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef enum{

    chx_key1 = 0,
    chx_key2,
    chx_key3,
    chx_key4,

    chx_key_max

}chx_on_key_t;

void chx_driver_init(void);

uint8_t chx_pro(uint16_t tick_ms);

void chx_key_on(chx_on_key_t key);

#ifdef __cplusplus
}
#endif
#endif	// _CHX_DRIVER_H

