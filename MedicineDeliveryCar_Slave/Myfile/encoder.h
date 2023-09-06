#ifndef ENCODER_H
#define ENCODER_H


#include "tim.h"

extern int32_t left_timer_overflow;
extern uint32_t left_timer_count;
// extern int32_t left_cur_total_count;
extern uint32_t left_last_total_count;

extern int32_t right_timer_overflow;
extern uint32_t right_timer_count;
// extern int32_t right_cur_total_count;
extern uint32_t right_last_total_count;

extern int32_t last_T_cnt;
extern uint32_t left_last_dis_cnt;
extern uint32_t right_last_dis_cnt;

uint32_t Left_getpos(void); 
uint32_t Right_getpos(void);






#endif




