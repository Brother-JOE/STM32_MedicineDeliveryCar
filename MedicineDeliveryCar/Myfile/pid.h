
#ifndef PID_H_
#define PID_H_

/**
 * @brief 
 *  Kp, Ki, Kd, err, last_err, integral, target_val, output_val
 */
typedef struct{

    float Kp;
    float Ki;
    float Kd;
    float err;
    float last_err;
    float integral;
    float target_val;
    float output_val;

} PID;


void pid_init(PID *pid);
void pid_param_set(PID *pid,float kp,float ki,float kd);
void pid_target_set(PID *pid,float target);
float pid_realize(PID *pid,float actual_val);

#endif /* PID_H_ */
