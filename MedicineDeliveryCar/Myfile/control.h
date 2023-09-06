#ifndef CONTROL_H
#define CONTROL_H


#include "main.h"
#include "encoder.h"


#define POINT_FIT_TIMES 6
#define ERR_FIT_VALUE 4    //mm
#define DEVIATION_TARGET 4
#define WHEEL_DISTANCE 165

typedef enum{
    left_30,
    left_90,
    right_30,
    right_60,
    right_90,
    back_180
} SpinDir_t;

extern uint8_t get_num_cmd[4];
extern uint8_t get_dir_cmd[4];
extern uint8_t get_four_dir_cmd[4];

extern uint8_t make_slave_start_cmd[4];
extern uint8_t make_slave_led_off[4];
extern uint8_t make_slave_getdrugstart_cmd[4];
extern uint8_t make_slave_restart2far_cmd[4];

extern uint8_t openmv_run;
extern uint8_t line_flag;
extern uint8_t stop_flag;
extern uint8_t dis_ctrl_flag;
extern uint8_t dis_ctrl_fin_flag;
extern uint8_t spin_flag;
extern uint8_t spin_fin_flag;
extern uint8_t crossing_flag;
extern uint8_t finded_flag;

extern float dis_outval_l;
extern float dis_outval_r;

extern float pwm_out_left;
extern float pwm_out_right;
extern float actual_speed_left;
extern float actual_speed_right;

extern uint8_t task_errfin_cnt;
extern uint8_t errfin_cnt_l_dis;
extern uint8_t errfin_cnt_r_dis;

extern uint8_t cur_action;
extern uint8_t TASK;
extern uint8_t target_num;
extern int8_t LoR;
extern int8_t last_LoR;
extern char fin_area;
extern uint8_t delivery_flag;


void speed_control(void);
float left_distance_control(void);
float right_distance_control(void);
void Spin_Turn(SpinDir_t goal_dir);
void Car_go(float dis_mm);






#endif






