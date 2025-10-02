/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       usb_task.c/h
  * @brief      usb outputs the error message.usb输出错误信息
  * @note       
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */
#include "Robo_USB.h"
#include "gpio.h"
#include "Robo_Common.h"

#include "stdint.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

//Prototypes of functions
static void send(ReceivePacket *packet);
static void receive(void);
static void init_packet(ReceivePacket *packet);

static uint8_t calc_checksum(uint16_t cmd) {
    return (uint8_t)((cmd & 0xFF) + ((cmd >> 8) & 0xFF));
}//使用AI
//End of prototypes

//把类型和变量名弄成一致的
ReceivePacket received_packet;
SendPacket send_packet;

void USB_Init()
{
    MX_USB_DEVICE_Init();
	init_packet(&send_packet);
}

void USB_Task(void *conifg)
{
	receive();
	send(&send_packet);
}

//初始化发送包
static void init_packet(SendPacket *packet)
{
    packet->cmd = 0x0000; 
    packet->checksum = calc_checksum(packet->cmd);
}

//发送函数
static void send(SendPacket *packet)
{
    packet->checksum = calc_checksum(packet->cmd);
    CDC_Transmit_FS((uint8_t *)packet, sizeof(SendPacket));
}


/* 
 * @brief 接收函数
 */使用AI
static void receive(void)
{
	extern uint8_t UserRxBufferFS[3];
    memcpy(&received_packet, UserRxBufferFS, sizeof(ReceivePacket));
    if (received_packet.checksum == calc_checksum(received_packet.cmd)) {
        // 校验通过，可以处理 received_packet.cmd
    } else {
        // 校验失败，丢弃
    }
}

