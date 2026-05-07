#include "usb_osal.h"
#include "usb_config.h"
#include <stdlib.h>
#include "ch32v20x.h"
#include "debug.h"

usb_osal_thread_t usb_osal_thread_create(const char *name, uint32_t stack_size, uint32_t prio, usb_thread_entry_t entry, void *args)
{
    return NULL;
}

void usb_osal_thread_delete(usb_osal_thread_t thread)
{
}

void usb_osal_thread_schedule_other(void)
{
}

usb_osal_sem_t usb_osal_sem_create(uint32_t initial_count)
{
    return NULL;
}

usb_osal_sem_t usb_osal_sem_create_counting(uint32_t max_count)
{
    return NULL;
}

void usb_osal_sem_delete(usb_osal_sem_t sem)
{
}

int usb_osal_sem_take(usb_osal_sem_t sem, uint32_t timeout)
{
    return 0;
}

int usb_osal_sem_give(usb_osal_sem_t sem)
{
    return 0;
}

void usb_osal_sem_reset(usb_osal_sem_t sem)
{
}

usb_osal_mutex_t usb_osal_mutex_create(void)
{
    return NULL;
}

void usb_osal_mutex_delete(usb_osal_mutex_t mutex)
{
}

int usb_osal_mutex_take(usb_osal_mutex_t mutex)
{
    return 0;
}

int usb_osal_mutex_give(usb_osal_mutex_t mutex)
{
    return 0;
}

usb_osal_mq_t usb_osal_mq_create(uint32_t max_msgs)
{
    return NULL;
}

void usb_osal_mq_delete(usb_osal_mq_t mq)
{
}

int usb_osal_mq_send(usb_osal_mq_t mq, uintptr_t addr)
{
    return 0;
}

int usb_osal_mq_recv(usb_osal_mq_t mq, uintptr_t *addr, uint32_t timeout)
{
    return 0;
}

struct usb_osal_timer *usb_osal_timer_create(const char *name, uint32_t timeout_ms, usb_timer_handler_t handler, void *argument, bool is_period)
{
    return NULL;
}

void usb_osal_timer_delete(struct usb_osal_timer *timer)
{
}

void usb_osal_timer_start(struct usb_osal_timer *timer)
{
}

void usb_osal_timer_stop(struct usb_osal_timer *timer)
{
}

size_t usb_osal_enter_critical_section(void)
{
    size_t irq_state = __get_MSTATUS();
    __disable_irq();
    return irq_state;
}

void usb_osal_leave_critical_section(size_t flag)
{
    __set_MSTATUS(flag);
}

void usb_osal_msleep(uint32_t delay)
{
    Delay_Ms(delay);
}

void *usb_osal_malloc(size_t size)
{
    return malloc(size);
}

void usb_osal_free(void *ptr)
{
    free(ptr);
}
