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
#include "usbd_cdc_if.h"
#include "Robo_Common.h"
#include "CRC8_CRC16.h"  

#include "stdint.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

//把类型和变量名弄成一致的
static ReceivePacket received_packet;
static SendPacket send_packet;

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

//初始化发送包。使用readme中的CRC算法抽象
static void init_packet(SendPacket *packet)
{
    packet->cmd = 0x0000; 
    packet->crc16 = get_CRC16_check_sum((uint8_t *)&packet->cmd, sizeof(packet->cmd), 0xffff);
}

//发送函数
static void send(SendPacket *packet)
{
    packet->checksum = get_CRC16_check_sum((uint8_t *)&packet->cmd, sizeof(packet->cmd), 0xffff) & 0xFF;
    CDC_Transmit_FS((uint8_t *)packet, sizeof(ReceivePacket));
}


/* 
 * @brief 接收函数.使用AI
 */
static void receive(void)
{
	uint8_t rx_buf[sizeof(ReceivePacket)];
    // 这里假设USB接收缓冲区有数据
    memcpy(&receive_packet, rx_buf, sizeof(ReceivePacket));
    uint8_t crc = get_CRC16_check_sum((uint8_t *)&receive_packet.cmd, sizeof(receive_packet.cmd), 0xffff) & 0xFF;
    if (crc == receive_packet.checksum) {
        // 校验通过，可处理 receive_packet.cmd
        
    }
}

