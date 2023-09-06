#include "control.h"


uint8_t get_num_cmd[4] = {0xa1, 0x01, 0x00, 0xa2};
uint8_t get_dir_cmd[4] = {0xa1, 0x00, 0x01, 0xa2};
uint8_t get_four_dir_cmd[4] = {0xa1, 0x00, 0x02, 0xa2};

uint8_t make_slave_start_cmd[4] = {0xd1, 0x00, 0x01, 0xd2};
uint8_t make_slave_led_off[4] = {0xd1, 0x00, 0x02, 0xd2};
uint8_t make_slave_getdrugstart_cmd[4] = {0xd1, 0x00, 0x03, 0xd2};
uint8_t make_slave_restart2far_cmd[4] = {0xd1, 0x00, 0x04, 0xd2};


uint8_t openmv_run = 0;
uint8_t line_flag = 0;
uint8_t stop_flag = 0;          //*注意会被多次赋值为1，应在下次使用前赋值为0
uint8_t dis_ctrl_flag = 0;
uint8_t dis_ctrl_fin_flag = 0;
uint8_t spin_flag = 0;
uint8_t spin_fin_flag = 0;
uint8_t crossing_flag = 0;      //*注意会被多次赋值为1，应在下次使用前赋值为0
uint8_t finded_flag = 0;

float dis_outval_l = 0.0;
float dis_outval_r = 0.0;

float pwm_out_left;
float pwm_out_right;
float actual_speed_left;
float actual_speed_right;

uint8_t task_errfin_cnt = 0;
uint8_t errfin_cnt_l_dis = 0;
uint8_t errfin_cnt_r_dis = 0;
uint8_t cur_action = 0;
uint8_t TASK = 0;
uint8_t delivery_flag = 0;

uint8_t target_num = 0;
int8_t LoR = 0;
int8_t last_LoR = 0;
char fin_area = 'A';

/**
 * @brief 
 * 
 */
void speed_control(void)
{
    actual_speed_left=60.0*ONE_P_TIM2H*(int)(Left_getpos()-left_last_total_count)/4/390;
    left_last_total_count = Left_getpos();
    pwm_out_left=pid_realize(&l_pid, actual_speed_left);

    if(pwm_out_left>=0)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
        if(pwm_out_left>MAX_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, MAX_OUT);
        else if(pwm_out_left<MIN_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, MIN_OUT);
        else
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, (int)pwm_out_left);                

    }
    else if(pwm_out_left<0)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
        if(fabs(pwm_out_left)>MAX_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, MAX_OUT);
        else if(fabs(pwm_out_left)<MIN_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, MIN_OUT);
        else
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, (int)fabs(pwm_out_left));                

    }


    actual_speed_right=60.0*ONE_P_TIM2H*(int)(Right_getpos()-right_last_total_count)/4/390;
    right_last_total_count = Right_getpos();
    pwm_out_right=pid_realize(&r_pid, actual_speed_right);

    if(pwm_out_right>=0)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);
        if(pwm_out_right>MAX_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, MAX_OUT);
        else if(pwm_out_right<MIN_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, MIN_OUT);
        else
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, (int)pwm_out_right);                

    }
    else if(pwm_out_right<0)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);
        if(fabs(pwm_out_right)>MAX_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, MAX_OUT);
        else if(fabs(pwm_out_right)<MIN_OUT)
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, MIN_OUT);
        else
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, (int)fabs(pwm_out_right));                

    }


}


/**
 * @brief Call 'pid_target_set()' and assign a value to 'left_last_dis_cnt' before use
 * (Pay attention to the input which has been limited)
 * @return float - speed_goal
 */
float left_distance_control(void)
{
    float output;
    float cur_dis = (int)(Left_getpos()-left_last_dis_cnt)*213.63/4/390;
    if(fabs(cur_dis) > 350){
        if(cur_dis > 0) cur_dis = 350.0;
        else cur_dis = -350.0;
    } 
    output = pid_realize(&pid_distance_l, cur_dis);
    if(pid_distance_l.err < 0.2f && pid_distance_l.err > -0.2f)  output = 0;
    if(fabs(output) > 90){
        if(output > 0) output = 90.0;
        else output = -90.0;
    }
    return output;
}


/**
 * @brief Call 'pid_target_set()' and assign a value to 'right_last_dis_cnt' before use
 * (Pay attention to the input which has been limited)
 * @return float - speed_goal
 */
float right_distance_control(void)
{
    float output;
    float cur_dis = (int)(Right_getpos()-right_last_dis_cnt)*213.63/4/390;
    if(fabs(cur_dis) > 350){
        if(cur_dis > 0) cur_dis = 350.0;
        else cur_dis = -350.0;
    } 
    output = pid_realize(&pid_distance_r, cur_dis);
    if(pid_distance_r.err < 0.2f && pid_distance_r.err > -0.2f)  output = 0;
    if(fabs(output) > 90){
        if(output > 0) output = 90.0;
        else output = -90.0;
    }
    return output;
}


/**
 * @brief 
 * 
 * @param goal_dir 
 */
void Spin_Turn(SpinDir_t goal_dir)
{
    //*
    line_flag = 0;
    dis_ctrl_flag = 0;
    spin_fin_flag = 0;

    if(goal_dir == left_90)
    {
        spin_flag = 1;
        left_last_dis_cnt = Left_getpos();
        right_last_dis_cnt = Right_getpos();
        pid_target_set(&pid_distance_l, -0.25*3.1416*WHEEL_DISTANCE);
        pid_target_set(&pid_distance_r, 0.25*3.1416*WHEEL_DISTANCE);
    }
    else if(goal_dir == right_90)
    {
        spin_flag = 1;
        left_last_dis_cnt = Left_getpos();
        right_last_dis_cnt = Right_getpos();
        pid_target_set(&pid_distance_l, 0.25*3.1416*WHEEL_DISTANCE);
        pid_target_set(&pid_distance_r, -0.25*3.1416*WHEEL_DISTANCE);
    }
    else if(goal_dir == back_180)
    {
        spin_flag = 1;
        left_last_dis_cnt = Left_getpos();
        right_last_dis_cnt = Right_getpos();
        pid_target_set(&pid_distance_l, 0.5*3.1416*WHEEL_DISTANCE);        //90deg
        pid_target_set(&pid_distance_r, -0.5*3.1416*WHEEL_DISTANCE);
    }
    else if(goal_dir == left_30)
    {
        spin_flag = 1;
        left_last_dis_cnt = Left_getpos();
        right_last_dis_cnt = Right_getpos();
        pid_target_set(&pid_distance_l, -3.1416*180/24);    //15deg
        pid_target_set(&pid_distance_r, 3.1416*180/24);     
    }
    else if(goal_dir == right_30)
    {
        spin_flag = 1;
        left_last_dis_cnt = Left_getpos();
        right_last_dis_cnt = Right_getpos();
        pid_target_set(&pid_distance_l, 3.1416*180/24);     //15deg
        pid_target_set(&pid_distance_r, -3.1416*180/24);
    }
    else if(goal_dir == right_60)
    {
        spin_flag = 1;
        left_last_dis_cnt = Left_getpos();
        right_last_dis_cnt = Right_getpos();
        pid_target_set(&pid_distance_l, 3.1416*180/12);     //30deg
        pid_target_set(&pid_distance_r, -3.1416*180/12);
    }
}


/**
 * @brief 
 * 
 * @param dis_mm 
 */
void Car_go(float dis_mm)
{
    line_flag = 0;
    spin_flag = 0;
    dis_ctrl_fin_flag = 0;

    dis_ctrl_flag = 1;
    left_last_dis_cnt = Left_getpos();
    right_last_dis_cnt = Right_getpos();
    pid_target_set(&pid_distance_l, dis_mm);
    pid_target_set(&pid_distance_r, dis_mm);

}





