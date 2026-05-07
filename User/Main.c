#include "debug.h"
#include "ch32v20x_flash.h"
#include "usbd_core.h"
#include "usbd_composite_km_cherry.h"
#include "CONFIG.h"
#include "HAL.h"
#include "central.h"

uint8_t KB_Data_Pack[DEF_ENDP_SIZE_KB] = { 0x00 };
uint8_t MS_Data_Pack[DEF_ENDP_SIZE_MS] = { 0x00 };

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];   

const uint8_t MacAddr[6] = {0x99, 0xC2, 0xE4, 0x03, 0x05, 0x02};      

void Print_My_MAC(void) {
    uint8_t myMac[6] = {0};

    FLASH_GetMACAddress(myMac);

    printf("Chip MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
           myMac[5], myMac[4], myMac[3], myMac[2], myMac[1], myMac[0]);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("BLE-USB Proxy Demo\r\n");

    composite_km_init(0, 0x40005C00);
    printf("CherryUSB Init OK!\r\n");

    WCHBLE_Init();
    printf("BLE Init OK!\r\n");

    HAL_Init();
    printf("HAL Init OK!\r\n");

    GAPRole_CentralInit();
    printf("GAPRole Central Init OK!\r\n");

    Central_Init();
    printf("Central Init OK!\r\n");
    Print_My_MAC();
    while (1) {
        TMOS_SystemProcess();
        if (is_usb_configured()) {
            /* TODO: 后续透传逻辑 */
        }
    }
}

/* ==================== 蓝牙协议栈专属硬件中断 ==================== */

void RTC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void RTC_IRQHandler(void)
{
    uint32_t time = RTC_GetCounter();
    TMOS_TimerIRQHandler(&time);
    RTC_SetCounter(time);
}

void BB_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void BB_IRQHandler(void)
{
    BB_IRQLibHandler();
}