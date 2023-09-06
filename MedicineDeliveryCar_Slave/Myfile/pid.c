#include "pid.h"


void pid_init(PID *pid)
{
    pid->Kp=0.0;
    pid->Ki=0.0;
    pid->Kd=0.0;
    pid->err=0.0;
    pid->last_err=0.0;
    pid->integral=0.0;
    pid->target_val=0.0;
    pid->output_val=0.0;
}
void pid_param_set(PID *pid,float kp,float ki,float kd)
{
    pid->Kp=kp;
    pid->Ki=ki;
    pid->Kd=kd;
}
void pid_target_set(PID *pid,float target)
{
    pid->target_val=target;
}

float pid_realize(PID *pid,float actual_val)
{
    pid->err=pid->target_val-actual_val;

    pid->integral+=pid->err;

    pid->output_val=pid->Kp*pid->err+
                   pid->Ki*pid->integral+
                   pid->Kd*(pid->err-pid->last_err);
    pid->last_err=pid->err;

    return pid->output_val;
}
