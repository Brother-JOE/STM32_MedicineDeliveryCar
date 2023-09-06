/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//todo
#include "encoder.h"
#include "control.h"
#include "usart.h"
#include "gpio.h"

//todo
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId SpeedCtrlTaskHandle;
osThreadId CarGoalCtrlTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Task_SpeedCtrl(void const * argument);
void Task_CarGoalCtrl(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of SpeedCtrlTask */
  osThreadDef(SpeedCtrlTask, Task_SpeedCtrl, osPriorityNormal, 0, 128);
  SpeedCtrlTaskHandle = osThreadCreate(osThread(SpeedCtrlTask), NULL);

  /* definition and creation of CarGoalCtrlTask */
  osThreadDef(CarGoalCtrlTask, Task_CarGoalCtrl, osPriorityBelowNormal, 0, 256);
  CarGoalCtrlTaskHandle = osThreadCreate(osThread(CarGoalCtrlTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Task_SpeedCtrl */
/**
* @brief Function implementing the SpeedCtrlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_SpeedCtrl */
void Task_SpeedCtrl(void const * argument)
{
  /* USER CODE BEGIN Task_SpeedCtrl */


  /* Infinite loop */
  for(;;)
  {

    static uint32_t line_timer = 0;
    line_timer++;
    if(line_timer == 2){
      line_timer = 0;

      if(line_flag == 1)
      {
        speed_line_offset = pid_realize(&pid_line, deviation);
        pid_target_set(&l_pid, speed_target + speed_line_offset);
        pid_target_set(&r_pid, speed_target - speed_line_offset);

      }

      if(spin_flag == 1)
      {
        dis_outval_l = left_distance_control();
        dis_outval_r = right_distance_control();
        pid_target_set(&l_pid, dis_outval_l);   //set speed
        pid_target_set(&r_pid, dis_outval_r);   //set speed

        if(errfin_cnt_l_dis < POINT_FIT_TIMES || errfin_cnt_r_dis < POINT_FIT_TIMES){
            if(fabs(pid_distance_l.err) < ERR_FIT_VALUE) errfin_cnt_l_dis++;
            else errfin_cnt_l_dis /=2;
            if(fabs(pid_distance_r.err) < ERR_FIT_VALUE) errfin_cnt_r_dis++;
            else errfin_cnt_r_dis /=2;
        }
        else{
          spin_fin_flag = 1;
          errfin_cnt_l_dis = 0;
          errfin_cnt_r_dis = 0;
        }

      }

      if(dis_ctrl_flag == 1)
      {
        dis_outval_l = left_distance_control();
        dis_outval_r = right_distance_control();
        pid_target_set(&l_pid, dis_outval_l);   //set speed
        pid_target_set(&r_pid, dis_outval_l);   //set speed   //????

        if(errfin_cnt_l_dis < POINT_FIT_TIMES || errfin_cnt_r_dis < POINT_FIT_TIMES){
            if(fabs(pid_distance_l.err) < ERR_FIT_VALUE) errfin_cnt_l_dis++;
            else errfin_cnt_l_dis /=2;
            // if(fabs(pid_distance_r.err) < ERR_FIT_VALUE) errfin_cnt_r_dis++;
            // else errfin_cnt_r_dis /=2;
        }
        else{
          dis_ctrl_fin_flag = 1;
          errfin_cnt_l_dis = 0;
          errfin_cnt_r_dis = 0;
        }

      }

    }

    speed_control();


    osDelay(10);    //10ms
  }




  /* USER CODE END Task_SpeedCtrl */
}

/* USER CODE BEGIN Header_Task_CarGoalCtrl */
/**
* @brief Function implementing the CarGoalCtrlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_CarGoalCtrl */
void Task_CarGoalCtrl(void const * argument)
{
  /* USER CODE BEGIN Task_CarGoalCtrl */


  /* Infinite loop */
  for(;;)
  {

    
    if(delivery_flag == 0)
    {
      if(TASK == 0)
      {
        if(cur_action == 0)
        {
          if(openmv_run == 1)
          {
            HAL_UART_Transmit_IT(&huart1, get_num_cmd, sizeof(get_num_cmd));
            cur_action++;
          }
        }
        else if(cur_action == 1)
        {
          if( (finded_flag == 1) && ((Drug_ReadState() == GPIO_PIN_RESET) || slave_get_start_flag == 1) )  //****
          {
            //LED_YELLOW_OFF();         //?test
            slave_get_start_flag = 0;

            finded_flag = 0;
            speed_target = 112.0;   //0.4m/s
            line_flag = 1;
            cur_action++;        
          }
        }
        else if(cur_action == 2)
        {
          if(crossing_flag == 1)
          {
            if(target_num >= 3)   //*num 3~8 area 'C'~'H'
            {
              left_last_dis_cnt = Left_getpos(); 
              crossing_flag = 0; 
              TASK++;             //
              cur_action = 0;
            }
            else  // area 'A' or 'B'
            {
              crossing_flag = 0;    //其实会被多次赋值为1，应在下次使用前赋值为0
              line_flag = 0;
              Car_go(220);  //220mm
              cur_action++;
            }
          }        
        }
        else if(cur_action == 3)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            if(target_num == 1){  //area 'A'
              Spin_Turn(left_90);
              fin_area = 'A';
            }
            else{                 //area 'B'
              Spin_Turn(right_90);
              fin_area = 'B';
            }
            cur_action++;
          }

        }
        else if(cur_action == 4)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }             
        }
        else if(cur_action == 5)
        {
          if(stop_flag == 1)
          {
            stop_flag = 0;      
            line_flag = 0;
            Car_go(150);  //150mm
            cur_action++;
          }
        }
        else if(cur_action == 6)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            LED_RED_ON();   //LED_red on
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);
            
            delivery_flag = 1;
            TASK = 0; 
            cur_action = 0;
          }        
        }

      }//TASK == 0  area 'A'&'B'

      else if(TASK == 1)  //num 3~8 area 'C'~'H' 
      {
        if(cur_action == 0)
        {
          if(crossing_flag == 1 && (int)(Left_getpos() - left_last_dis_cnt)*213.63/4/390 > 280) //
          {
              crossing_flag = 0;    
              line_flag = 0;
              Car_go(20);  //20mm
              HAL_UART_Transmit_IT(&huart1, get_dir_cmd, sizeof(get_dir_cmd));
              cur_action++;
          }
        }
        else if(cur_action == 1)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);          
            cur_action++;
          }
        }
        else if(cur_action == 2)
        {
          if(finded_flag == 1)
          {
            finded_flag = 0;
            if(LoR == 0){   //*直行  area 'E'~'H' 但先去暂停点

              Car_go(260);  //260mm
              cur_action = 150; //*
            }
            else{
              Car_go(260);  //260mm
              cur_action++;
            }
          }
        }
        else if(cur_action == 150)  //去远端前先去暂停点
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            Spin_Turn(right_90);  
            cur_action++;
          }          
        }
        else if(cur_action == 151)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }            
        }
        else if(cur_action == 152)
        {
          if(stop_flag == 1)  //准备停止在暂停点再到远端病房
          {
            stop_flag = 0;      
            line_flag = 0;
            Car_go(180);  //180mm
            cur_action++;   
          }          
        }
        else if(cur_action == 153)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);
            cur_action++;
          }                    
        }
        else if(cur_action == 154)
        {
          if(slave_restart2far_flag == 1)
          {
            slave_restart2far_flag = 0;
            Spin_Turn(back_180);
            cur_action++;            
          }
        }
        else if(cur_action == 155)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }          
        }
        else if(cur_action == 156)
        {
          if(crossing_flag == 1) //
          {
              crossing_flag = 0;    
              line_flag = 0;
              Car_go(270);  //270mm
              cur_action++;
          }          
        }
        else if(cur_action == 157)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            Spin_Turn(right_90);
            cur_action++;
          }                  
        }
        else if(cur_action == 158)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;

            left_last_dis_cnt = Left_getpos(); 
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            TASK++;   //*
            cur_action = 0;
          }          
        }
        else if(cur_action == 3)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            if(LoR == -1){  //area 'C' 但先去'D'
              Spin_Turn(right_90);  
              fin_area = 'C';
            }
            else{                 //area 'D' 但先去'C'
              Spin_Turn(left_90);
              fin_area = 'D';
            }
            cur_action++;
          }
        }
        else if(cur_action == 4)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }  
        }
        else if(cur_action == 5)
        {
          if(stop_flag == 1)  //准备停止在暂停点
          {
            stop_flag = 0;      
            line_flag = 0;
            Car_go(180);  //180mm
            cur_action = 100;   //*
          }
        }
        else if(cur_action == 100)  //去目标中端病房前先在暂停点暂停
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            LED_YELLOW_ON();   //LED_yellow on
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);
            cur_action++;
          }          
        }
        else if(cur_action == 101)
        {
          if(slave_mid_start_flag == 1)
          {
            slave_mid_start_flag = 0;
            Spin_Turn(back_180);
            cur_action++;
          }
        }
        else if(cur_action == 102)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }           
        }
        else if(cur_action == 103)
        {
          if(stop_flag == 1)  //准备停止在最终区域
          {
            stop_flag = 0;      
            line_flag = 0;
            Car_go(180);  //180mm
            cur_action = 6;   //*
          }
        }
        else if(cur_action == 6)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            LED_RED_ON();   //LED_red on
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);
            
            delivery_flag = 1;
            TASK = 0; 
            cur_action = 0;
          }
        }

      }//TASK == 1  --area 'C'&'D'

      else if(TASK == 2)  //area 'E'~'H'  
      {
        if(cur_action == 0)
        {
          if(crossing_flag == 1 && (int)(Left_getpos() - left_last_dis_cnt)*213.63/4/390 > 200) //
          {
            crossing_flag = 0;    
            line_flag = 0;
            Car_go(100);  //100mm
            cur_action++;
          }
        }
        else if(cur_action == 1)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            Spin_Turn(left_30);        
            cur_action++;
          }
        }
        else if(cur_action == 2)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);            
            //HAL_UART_Transmit_IT(&huart1, get_four_dir_cmd, sizeof(get_four_dir_cmd));
            cur_action++;
            //!test
            finded_flag = 1;
            LoR = -1;
          }            

        }
        else if(cur_action == 3)
        {
          if(finded_flag == 1)
          {
            finded_flag = 0;
            Spin_Turn(right_30);
            cur_action++;
          }
        }
        // else if(cur_action == 4)
        // {
        //   if(spin_fin_flag == 1)
        //   {
        //     spin_fin_flag = 0;
        //     spin_flag = 0;
        //     pid_target_set(&l_pid, 0);
        //     pid_target_set(&r_pid, 0);            
        //     HAL_UART_Transmit_IT(&huart1, get_dir_cmd, sizeof(get_dir_cmd));
        //     cur_action++;
        //   }            
        // }
        // else if(cur_action == 5)
        // {
        //   if(finded_flag == 1)
        //   {
        //     finded_flag = 0;
        //     Spin_Turn(left_30);
        //     cur_action++;
        //   }          
        // }
        else if(cur_action == 4)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            Car_go(220);  //220mm
            cur_action++;
          }             
        }
        else if(cur_action == 5)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            if(LoR == -1){  //area 'E'&'G'
              Spin_Turn(left_90);
              last_LoR = -1;
            }
            else{                 //area 'F'&'H'
              Spin_Turn(right_90);
              last_LoR = 1;
            }
            cur_action++;
          }          
        }
        else if(cur_action == 6)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }  
        }
        else if(cur_action == 7)
        {
          if(crossing_flag == 1) 
          {
            crossing_flag = 0;    
            line_flag = 0;
            Car_go(10);  //10mm
            HAL_UART_Transmit_IT(&huart1, get_dir_cmd, sizeof(get_dir_cmd));
            cur_action++;
          }
        }
        else if(cur_action == 8)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);          
            cur_action++;
          }
        }
        else if(cur_action == 9)
        {
          if(finded_flag == 1)
          {
            finded_flag = 0;
            Car_go(260);  //260mm
            cur_action++;
          }
        }
        else if(cur_action == 10)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            if(LoR == -1){  //area 'E'&'H'
              Spin_Turn(left_90);
              if(last_LoR == -1) fin_area = 'E';
              else fin_area = 'H';
              cur_action++;

            }
            else if(LoR == 1){                 //area 'F'&'G'
              Spin_Turn(right_90);
              if(last_LoR == -1) fin_area = 'G';
              else fin_area = 'F';
              cur_action++;
            }
            else{         //前面判断错误
              Spin_Turn(back_180);
              cur_action = 200;
            }
          }      
        }
        else if(cur_action == 200)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            left_last_dis_cnt = Left_getpos();
            cur_action++;
          }    
        }
        else if(cur_action == 201)
        {
          if(crossing_flag == 1 && (int)(Left_getpos() - left_last_dis_cnt)*213.63/4/390 > 1000) //
          {
            crossing_flag = 0;    
            line_flag = 0;
            Car_go(20);  //20mm
            HAL_UART_Transmit_IT(&huart1, get_dir_cmd, sizeof(get_dir_cmd));
            cur_action = 8;     //*
          }

        }
        else if(cur_action == 11)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }    
        }
        else if(cur_action == 12)
        {
          if(stop_flag == 1)
          {
            stop_flag = 0;      
            line_flag = 0;
            Car_go(180);  //180mm
            cur_action++;
          }          
        }
        else if(cur_action == 13)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            LED_RED_ON();   //LED_red on
            pid_target_set(&l_pid, 0);
            pid_target_set(&r_pid, 0);
            
            delivery_flag = 1;
            TASK = 0; 
            cur_action = 0;
          }          
        }

      }//TASK == 2  --area 'E'~'H'

    }//delivery_flag == 0

    else if(delivery_flag == 1) //Go Back
    {
      if(TASK == 0)
      {
        if(cur_action == 0)
        {
          if((Drug_ReadState() == GPIO_PIN_SET && fin_area <= 'D')||(Drug_ReadState() == GPIO_PIN_RESET && fin_area >= 'E'))  //药品被拿走或放置
          {
            LED_RED_OFF();
            Spin_Turn(back_180);
            cur_action++;
          }
        }
        else if(cur_action == 1)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            cur_action++;
          }
        }
        else if(cur_action == 2)
        {
          if(crossing_flag == 1)
          {
            crossing_flag = 0;    //
            line_flag = 0;
            Car_go(220);  //220mm
            cur_action++;
          }          
        }
        else if(cur_action == 3)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            if(fin_area == 'A' || fin_area == 'C' || fin_area == 'E' || fin_area == 'H')  //需右转的区域
            {  
              Spin_Turn(right_90);
            }
            else{                 //需左转的区域
              Spin_Turn(left_90);
            }
            cur_action++;
          }
        }
        else if(cur_action == 4)
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;
            if(fin_area >= 'A' && fin_area <= 'D'){   //直接巡线到终点
              TASK++;
              cur_action = 0;
            }
            else cur_action++;
          }
        }
        else if(cur_action == 5)
        {
          if(crossing_flag == 1)
          {
            crossing_flag = 0;    //
            line_flag = 0;
            Car_go(270);  //270mm
            cur_action++;
          } 
        }
        else if(cur_action == 6)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;      
            if(fin_area == 'E' || fin_area == 'G')   //需右转的区域
            {  
              Spin_Turn(right_90);
            }
            else
            {
              Spin_Turn(left_90);
            }
            cur_action++;
          }
        }
        else if(cur_action == 7)  
        {
          if(spin_fin_flag == 1)
          {
            spin_fin_flag = 0;
            spin_flag = 0;
            pid_target_set(&l_pid, speed_target);
            pid_target_set(&r_pid, speed_target);
            line_flag = 1;        //巡线到终点
            TASK++;
            cur_action = 0;
          }

        }

      }//TASK == 0
      else if(TASK == 1)  //巡线到终点
      {
        if(cur_action == 0)
        {
          if(stop_flag == 1)
          {
            stop_flag = 0;      
            line_flag = 0;
            Car_go(180);  //180mm
            cur_action++;
          }
        }
        else if(cur_action == 1)
        {
          if(dis_ctrl_fin_flag == 1)
          {
            dis_ctrl_fin_flag = 0;
            dis_ctrl_flag = 0;
            LED_GREEN_ON();   //LED_green on
            pid_target_set(&l_pid, 0);          //停止
            pid_target_set(&r_pid, 0);
          }
        }
      }


    }//delivery_flag == 1



    osDelay(1);     //1ms
  }
  /* USER CODE END Task_CarGoalCtrl */



}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */



