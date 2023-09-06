#include "encoder.h"


uint32_t left_timer_count=0;
uint32_t left_last_total_count=ENCODER_INIT_CNT;
int32_t left_timer_overflow=0;
//int32_t left_cur_total_count=0;


uint32_t right_timer_count=0;
uint32_t right_last_total_count=ENCODER_INIT_CNT;
int32_t right_timer_overflow=0;
//int32_t right_cur_total_count=0;


int32_t last_T_cnt = 0;

uint32_t left_last_dis_cnt = ENCODER_INIT_CNT;
uint32_t right_last_dis_cnt = ENCODER_INIT_CNT;


uint32_t Left_getpos(void)
{
    // int32_t cur_total;
    left_timer_count = __HAL_TIM_GET_COUNTER(&htim2);
    // cur_total = left_timer_count + left_timer_overflow*0xffff;
    // left_cur_total_count = cur_total;
    return __HAL_TIM_GET_COUNTER(&htim2);
}


uint32_t Right_getpos(void)
{
    // int32_t cur_total;
    right_timer_count = __HAL_TIM_GET_COUNTER(&htim5);
    // cur_total = right_timer_count + right_timer_overflow*0xffff;
    // right_cur_total_count = cur_total;
    return __HAL_TIM_GET_COUNTER(&htim5);
}





