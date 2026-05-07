#ifndef __USBD_COMPOSITE_KM_CHERRY_H
#define __USBD_COMPOSITE_KM_CHERRY_H

#include "debug.h"
#include "string.h"
#include "ch32v20x.h"

#define DEF_KEY_CHAR_W 0x1A
#define DEF_KEY_CHAR_A 0x04
#define DEF_KEY_CHAR_S 0x16
#define DEF_KEY_CHAR_D 0x07

#define USBD_KB_EP_SIZE 8
#define USBD_MS_EP_SIZE 7
#define USBD_CC_EP_SIZE 2

#define DEF_ENDP_SIZE_KB USBD_KB_EP_SIZE
#define DEF_ENDP_SIZE_MS USBD_MS_EP_SIZE

extern const uint8_t keyboard_report_desc[];
extern const uint8_t mouse_report_desc[];
extern const uint8_t consumer_report_desc[];
extern volatile uint8_t kb_led_status;

void composite_km_init(uint8_t busid, uintptr_t reg_base);
int keyboard_send_report(uint8_t *report, uint8_t len);
int mouse_send_report(uint8_t *report, uint8_t len);
int consumer_send_report(uint8_t *report, uint8_t len);
bool is_usb_configured(void);

#endif
