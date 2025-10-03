/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       usb_task.c/h
  * @brief      usb outputs the error message.usbÊä³ö´íÎóÐÅÏ¢
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     -     			CodeAlan        1. solve trajectory
  *  V1.1.0		Jan-2-2024		Xindi Lv		2. complete air resistance model
  *	 V2.0.0		Jan-4-2024		HansonZhang		3. done
  *	 V2.0.1		Jan-5-2024		HansonZhang		4. bug fixed: PC communication failed when auto aiming enabled
  *	 V2.0.2		Jan-19-2024		HansonZhang		5. bug fixed: added restriction to auto aiming control speed
  *	 V3.0.0		Mar-19-2024		HansonZhang		6. deploy in event os
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */



#include <math.h>
#include <stdio.h>

#include "Robo_AA.h"
#include "gpio.h"
#include "AHRS.h"
#include "Dji_Motor.h"
#include "EzTuner.h"
#include "main.h"
#include "Robo_Common.h"
#include "Robo_USB.h"
#include "pid.h"

auto_aiming_control_t auto_aiming_control; // 全局变量声明

// 弹道解算函数声明
static void calculate_trajectory(float target_x, float target_y, float target_z, 
                                float imu_yaw, float imu_pitch,
                                float *result_yaw, float *result_pitch);

void AA_Init()
{
    // 初始化PID控制器
    auto_aiming_control.yaw_pid = PID_Create(1.0f, 0.1f, 0.05f, 1000.0f, 360.0f);
    auto_aiming_control.pitch_pid = PID_Create(1.2f, 0.15f, 0.06f, 1000.0f, 360.0f);
    
    // 初始化期望角度
    auto_aiming_control.expect_delta_yaw = 0.0f;
    auto_aiming_control.expect_delta_pitch = 0.0f;
    
    // 初始化USB通信数据包
    init_packet(&auto_aiming_control.send);
    printf("Auto Aiming System Initialized\r\n");

}

// 从USB接收数据
    receive();
    
    // 获取IMU数据（假设有相应的接口函数）
    float imu_yaw = Get_IMU_Yaw();
    float imu_pitch = Get_IMU_Pitch();
    
    // 检查是否有有效的目标数据
    if (auto_aiming_control.receive.has_target) 
    {
        // 获取目标位置（从USB接收的数据）
        float target_x = auto_aiming_control.receive.target_x;
        float target_y = auto_aiming_control.receive.target_y;
        float target_z = auto_aiming_control.receive.target_z;
        
        // 弹道解算
        float calculated_yaw, calculated_pitch;
        calculate_trajectory(target_x, target_y, target_z, 
                            imu_yaw, imu_pitch,
                            &calculated_yaw, &calculated_pitch);
        
        // 使用PID控制器平滑输出
        auto_aiming_control.expect_delta_yaw = 
            PID_Calculate(auto_aiming_control.yaw_pid, 
                         calculated_yaw, imu_yaw);
        
        auto_aiming_control.expect_delta_pitch = 
            PID_Calculate(auto_aiming_control.pitch_pid, 
                         calculated_pitch, imu_pitch);
        
        // 更新发送给PC的数据
        auto_aiming_control.send.calculated_yaw = calculated_yaw;
        auto_aiming_control.send.calculated_pitch = calculated_pitch;
        auto_aiming_control.send.has_solution = 1;
    }
    else 
    {
        // 没有目标时重置期望角度
        auto_aiming_control.expect_delta_yaw = 0.0f;
        auto_aiming_control.expect_delta_pitch = 0.0f;
        auto_aiming_control.send.has_solution = 0;
    }
    
    // 通过USB发送数据到PC
    send(&auto_aiming_control.send);
}

/**
  * @brief          弹道解算函数（包含空气阻力模型）
  * @param[in]      target_x, target_y, target_z: 目标位置坐标
  * @param[in]      imu_yaw, imu_pitch: IMU当前角度
  * @param[out]     result_yaw, result_pitch: 解算出的云台角度
  * @retval         none
  */
static void calculate_trajectory(float target_x, float target_y, float target_z,
                                float imu_yaw, float imu_pitch,
                                float *result_yaw, float *result_pitch)
{
    // 计算水平距离
    float horizontal_dist = sqrtf(target_x * target_x + target_y * target_y);
    
    // 基础弹道解算（不考虑空气阻力）
    float base_pitch = atan2f(target_z, horizontal_dist) * 180.0f / M_PI;
    float base_yaw = atan2f(target_y, target_x) * 180.0f / M_PI;
    
    // 空气阻力补偿（简化模型）
    float distance = sqrtf(horizontal_dist * horizontal_dist + target_z * target_z);
    float air_resistance_compensation = distance * 0.02f; // 经验系数
    
    // 应用补偿
    *result_pitch = base_pitch + air_resistance_compensation;
    *result_yaw = base_yaw;
    
    // 限制输出角度范围
    *result_yaw = fmodf(*result_yaw, 360.0f);
    if (*result_yaw > 180.0f) *result_yaw -= 360.0f;
    if (*result_yaw < -180.0f) *result_yaw += 360.0f;
    
    *result_pitch = fmaxf(fminf(*result_pitch, 80.0f), -80.0f); // 限制俯仰角范围
}

/**
  * @brief          获取期望的云台yaw角度
  * @param[in]      none
  * @retval         期望的yaw角度增量
  */
float AA_Get_Expect_Yaw(void)
{
    return auto_aiming_control.expect_delta_yaw;
}
/**
  * @brief          获取期望的云台pitch角度
  * @param[in]      none
  * @retval         期望的pitch角度增量
  */
float AA_Get_Expect_Pitch(void)
{
    return auto_aiming_control.expect_delta_pitch;
}
