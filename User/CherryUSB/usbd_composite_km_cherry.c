#include "usbd_core.h"
#include "usb_hid.h"
#include "usbd_composite_km_cherry.h"

#define USBD_VID 0x1A86
#define USBD_PID 0xFE00

#define KB_EP_ADDR 0x81
#define MS_EP_ADDR 0x82
#define CC_EP_ADDR 0x83

const uint8_t keyboard_report_desc[] = {
    0x05, 0x01,
    0x09, 0x06,
    0xA1, 0x01,
    0x05, 0x07,
    0x19, 0xE0,
    0x29, 0xE7,
    0x15, 0x00,
    0x25, 0x01,
    0x75, 0x01,
    0x95, 0x08,
    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x08,
    0x81, 0x01,
    0x95, 0x03,
    0x75, 0x01,
    0x05, 0x08,
    0x19, 0x01,
    0x29, 0x03,
    0x91, 0x02,
    0x95, 0x05,
    0x75, 0x01,
    0x91, 0x01,
    0x95, 0x06,
    0x75, 0x08,
    0x26, 0xFF, 0x00,
    0x05, 0x07,
    0x19, 0x00,
    0x29, 0x91,
    0x81, 0x00,
    0xC0
};

const uint8_t mouse_report_desc[] = {
    0x05, 0x01,
    0x09, 0x02,
    0xA1, 0x01,
    0x09, 0x01,
    0xA1, 0x00,

    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x05,
    0x15, 0x00,
    0x25, 0x01,
    0x95, 0x05,
    0x75, 0x01,
    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x03,
    0x81, 0x03,

    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x16, 0x01, 0x80,
    0x26, 0xFF, 0x7F,
    0x75, 0x10,
    0x95, 0x02,
    0x81, 0x06,

    0x09, 0x38,
    0x15, 0x81,
    0x25, 0x7F,
    0x75, 0x08,
    0x95, 0x01,
    0x81, 0x06,

    0x95, 0x01,
    0x75, 0x08,
    0x81, 0x03,

    0xC0,
    0xC0
};

const uint8_t consumer_report_desc[] = {
    0x05, 0x0C,        // USAGE_PAGE (Consumer)
    0x09, 0x01,        // USAGE (Consumer Control)
    0xA1, 0x01,        // COLLECTION (Application)
    
    // --- Byte 0 ��ӳ�� ---
    0x15, 0x00,        // LOGICAL_MINIMUM (0)
    0x25, 0x01,        // LOGICAL_MAXIMUM (1)
    0x75, 0x01,        // REPORT_SIZE (1)
    
    // Bit 0-3: ��� (4λ)
    0x95, 0x04,        
    0x81, 0x03,        
    
    // Bit 4: ���� (Mute)
    0x09, 0xE2,        // USAGE (Mute)
    0x95, 0x01,        
    0x81, 0x02,        
    
    // Bit 5: ��� (1λ)
    0x95, 0x01,        
    0x81, 0x03,        
    
    // Bit 6: ����+ (Volume Increment)
    0x09, 0xE9,        // USAGE (Volume Increment)
    0x95, 0x01,        
    0x81, 0x02,        
    
    // Bit 7: ����- (Volume Decrement)
    0x09, 0xEA,        // USAGE (Volume Decrement)
    0x95, 0x01,        
    0x81, 0x02,        

    // --- Byte 1 ��ӳ�� ---
    // Bit 0-3: ��� (4λ)
    0x95, 0x04,        
    0x81, 0x03,        
    
    // Bit 4 (�ܵ�12λ): ����+
    0x09, 0x6F,        // USAGE (Brightness Increment)
    0x95, 0x01,        
    0x81, 0x02,        
    
    // Bit 5 (�ܵ�13λ): ����-
    0x09, 0x70,        // USAGE (Brightness Decrement)
    0x95, 0x01,        
    0x81, 0x02,        
    
    // Bit 6-7: ��� (2λ)
    0x95, 0x02,        
    0x81, 0x03,        

    0xC0               // END_COLLECTION
};

#define KEYBOARD_REPORT_DESC_SIZE sizeof(keyboard_report_desc)
#define MOUSE_REPORT_DESC_SIZE    sizeof(mouse_report_desc)
#define CONSUMER_REPORT_DESC_SIZE sizeof(consumer_report_desc)

static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0100, 0x01)
};

#define CONFIG_TOTAL_LEN (USB_SIZEOF_CONFIG_DESC + HID_KEYBOARD_DESCRIPTOR_LEN + HID_MOUSE_DESCRIPTOR_LEN + HID_KEYBOARD_DESCRIPTOR_LEN)

static const uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT(CONFIG_TOTAL_LEN, 0x03, 0x01, USB_CONFIG_BUS_POWERED | USB_CONFIG_REMOTE_WAKEUP, 100),
    HID_KEYBOARD_DESCRIPTOR_INIT(0x00, 0x01, KEYBOARD_REPORT_DESC_SIZE, KB_EP_ADDR, USBD_KB_EP_SIZE, 0x01),
    HID_MOUSE_DESCRIPTOR_INIT(0x01, 0x02, MOUSE_REPORT_DESC_SIZE, MS_EP_ADDR, USBD_MS_EP_SIZE, 0x01),
    0x09,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    0x02,
    0x00,
    0x01,
    0x03,
    0x00,
    0x00,
    0x00,
    0x09,
    HID_DESCRIPTOR_TYPE_HID,
    0x11, 0x01,
    0x00,
    0x01,
    0x22,
    WBVAL(CONSUMER_REPORT_DESC_SIZE),
    0x07,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    CC_EP_ADDR,
    0x03,
    WBVAL(USBD_CC_EP_SIZE),
    0x0A
};

static const char *string_descriptors[] = {
    (const char[]){ 0x09, 0x04 },
    "wch.cn",
    "CH32V20x Composite KM",
    "BLEKMv6"
};

volatile uint8_t kb_led_status = 0;

static volatile bool kb_ep_busy = false;
static volatile bool ms_ep_busy = false;
static volatile bool cc_ep_busy = false;

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            kb_ep_busy = false;
            ms_ep_busy = false;
            cc_ep_busy = false;
            break;
        case USBD_EVENT_CONFIGURED:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_RESUME:
            break;
        default:
            break;
    }
}

static void kb_ep_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    kb_ep_busy = false;
}

static void ms_ep_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    ms_ep_busy = false;
}

static void cc_ep_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    cc_ep_busy = false;
}

static struct usbd_endpoint kb_ep = {
    .ep_addr = KB_EP_ADDR,
    .ep_cb = kb_ep_callback
};

static struct usbd_endpoint ms_ep = {
    .ep_addr = MS_EP_ADDR,
    .ep_cb = ms_ep_callback
};

static struct usbd_endpoint cc_ep = {
    .ep_addr = CC_EP_ADDR,
    .ep_cb = cc_ep_callback
};

static const uint8_t *device_descriptor_callback(uint8_t speed)
{
    return device_descriptor;
}

static const uint8_t *config_descriptor_callback(uint8_t speed)
{
    return config_descriptor;
}

static const char *string_descriptor_callback(uint8_t speed, uint8_t index)
{
    if (index >= sizeof(string_descriptors) / sizeof(string_descriptors[0])) {
        return NULL;
    }
    return string_descriptors[index];
}

static struct usb_descriptor descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .device_quality_descriptor_callback = NULL,
    .other_speed_descriptor_callback = NULL,
    .string_descriptor_callback = string_descriptor_callback,
    .msosv1_descriptor = NULL,
    .msosv2_descriptor = NULL,
    .webusb_url_descriptor = NULL,
    .bos_descriptor = NULL
};

static int kb_class_interface_handler(uint8_t busid, struct usb_setup_packet *setup, uint8_t **data, uint32_t *len)
{
    switch (setup->bRequest) {
        case HID_REQUEST_SET_REPORT:
            if (setup->wLength > 0) {
                kb_led_status = (*data)[0];
            }
            *len = 0;
            break;
        case HID_REQUEST_SET_IDLE:
            *len = 0;
            break;
        default:
            return -1;
    }
    return 0;
}

static int ms_class_interface_handler(uint8_t busid, struct usb_setup_packet *setup, uint8_t **data, uint32_t *len)
{
    switch (setup->bRequest) {
        case HID_REQUEST_SET_IDLE:
            *len = 0;
            break;
        default:
            return -1;
    }
    return 0;
}

static int cc_class_interface_handler(uint8_t busid, struct usb_setup_packet *setup, uint8_t **data, uint32_t *len)
{
    switch (setup->bRequest) {
        case HID_REQUEST_SET_IDLE:
            *len = 0;
            break;
        default:
            return -1;
    }
    return 0;
}

static void kb_notify_handler(uint8_t busid, uint8_t event, void *arg)
{
}

static void ms_notify_handler(uint8_t busid, uint8_t event, void *arg)
{
}

static void cc_notify_handler(uint8_t busid, uint8_t event, void *arg)
{
}

static struct usbd_interface kb_intf = {
    .class_interface_handler = kb_class_interface_handler,
    .class_endpoint_handler = NULL,
    .vendor_handler = NULL,
    .notify_handler = kb_notify_handler,
    .hid_report_descriptor = keyboard_report_desc,
    .hid_report_descriptor_len = KEYBOARD_REPORT_DESC_SIZE
};

static struct usbd_interface ms_intf = {
    .class_interface_handler = ms_class_interface_handler,
    .class_endpoint_handler = NULL,
    .vendor_handler = NULL,
    .notify_handler = ms_notify_handler,
    .hid_report_descriptor = mouse_report_desc,
    .hid_report_descriptor_len = MOUSE_REPORT_DESC_SIZE
};

static struct usbd_interface cc_intf = {
    .class_interface_handler = cc_class_interface_handler,
    .class_endpoint_handler = NULL,
    .vendor_handler = NULL,
    .notify_handler = cc_notify_handler,
    .hid_report_descriptor = consumer_report_desc,
    .hid_report_descriptor_len = CONSUMER_REPORT_DESC_SIZE
};

void composite_km_init(uint8_t busid, uintptr_t reg_base)
{
    usbd_desc_register(busid, &descriptor);
    usbd_add_interface(busid, &kb_intf);
    usbd_add_interface(busid, &ms_intf);
    usbd_add_interface(busid, &cc_intf);
    usbd_add_endpoint(busid, &kb_ep);
    usbd_add_endpoint(busid, &ms_ep);
    usbd_add_endpoint(busid, &cc_ep);
    usbd_initialize(busid, reg_base, usbd_event_handler);
}

int keyboard_send_report(uint8_t *report, uint8_t len)
{
    if (kb_ep_busy) {
        return -1;
    }
    kb_ep_busy = true;
    usbd_ep_start_write(0, KB_EP_ADDR, report, len);
    return 0;
}

int mouse_send_report(uint8_t *report, uint8_t len)
{
    if (ms_ep_busy) {
        return -1;
    }
    ms_ep_busy = true;
    usbd_ep_start_write(0, MS_EP_ADDR, report, len);
    return 0;
}

int consumer_send_report(uint8_t *report, uint8_t len)
{
    if (cc_ep_busy) {
        return -1;
    }
    cc_ep_busy = true;
    usbd_ep_start_write(0, CC_EP_ADDR, report, len);
    return 0;
}

bool is_usb_configured(void)
{
    return usb_device_is_configured(0);
}
