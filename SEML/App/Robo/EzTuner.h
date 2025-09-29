#ifndef _EZTUNER_H_
#define _EZTUNER_H_
#endif

//Robo_Gimbal.c
//Motor Electric param
#define Pitch_ID 1
#define Pitch_CAN can2

#define Yaw_ID 3
#define Yaw_CAN can1

//PID param
#define Pitch_Speed_KP 800
#define Pitch_Speed_KI 1000
#define Pitch_Speed_KD 0.7f

#define Pitch_Position_KP 120
#define Pitch_Position_KI 0
#define Pitch_Position_KD 4
#define Pitch_Position_MAXOUT 360

#define Pitch_AA_KP 1
#define Pitch_AA_KI 0
#define Pitch_AA_KD 0
#define Pitch_AA_MAXOUT 1


#define Yaw_Speed_KP 1000
#define Yaw_Speed_KI 1000
#define Yaw_Speed_KD 0

#define Yaw_Position_KP 180
#define Yaw_Position_KI 0
#define Yaw_Position_KD 3
#define Yaw_Position_MAXOUT 1080

#define Yaw_AA_KP 1
#define Yaw_AA_KI 0
#define Yaw_AA_KD 0
#define Yaw_AA_MAXOUT 1



//Robo_Shoot.c
//Motor Electric param
#define FRIC_L_ID 1
#define FRIC_L_CAN can2

#define FRIC_R_ID 2
#define FRIC_R_CAN can2

#define DIAL_ID 3
#define DIAL_CAN can2

//PID param
#define FRIC_KP 30
#define FRIC_KI 600
#define FRIC_KD 0

#define DIAL_Speed_KP 10
#define DIAL_Speed_KI 0
#define DIAL_Speed_KD 0

#define DIAL_Position_KP 1
#define DIAL_Position_KI 0
#define DIAL_Position_KD 0
#define DIAL_Position_MAXOUT 360

//Fire power
#define Bullet_Speed 700
#define Fire_Rate 1

// #endif
