/*
 *  netmon.h
 *
 */

#ifndef NETMON_H
#define NETMON_H

#include <stdint.h>

#include "addl_config.h"
#include "console.h"

#ifdef __cplusplus
extern "C" {
#endif

void netmon_update();

extern uint8_t last_radio_event;
extern uint8_t last_event2;
extern uint8_t last_event3;
extern uint8_t last_event4;
extern uint8_t last_event5;

extern uint8_t nm_screen;
extern uint8_t nm_started;
extern uint8_t nm_started5;
extern uint8_t nm_started6;
extern uint8_t rx_voice;

extern uint8_t lh_cc;
extern uint8_t lh_ts;
extern uint8_t lh_tg;

inline int is_netmon_enabled() { return global_addl_config.netmon != 0; }

inline int is_netmon_visible() {
    return nm_screen != 0;
    // return !is_menu_visible();
}

inline int is_statusline_visible() { return global_addl_config.datef == 5; }

#ifdef __cplusplus
}
#endif

#endif /* NETMON_H */
