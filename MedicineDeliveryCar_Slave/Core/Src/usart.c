/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
//todo
#include "control.h"
#include "gpio.h"

uint8_t U1RxBuffer[RXBUFFERSIZE] = {0};
uint8_t U1RXdata_len = 0;
uint8_t u1_aRxBuffer;			
uint8_t Uart1_Rx_Cnt = 0;		
uint8_t uart1_rxstate = 0;
uint8_t uart1_rx_finish = 0;
uint8_t uart1_tx_finish = 0;


uint8_t U3RxBuffer[RXBUFFERSIZE] = {0};
uint8_t U3RXdata_len = 0;
uint8_t u3_aRxBuffer;			
uint8_t Uart3_Rx_Cnt = 0;		
uint8_t uart3_rxstate = 0;
uint8_t uart3_rx_finish = 0;
uint8_t uart3_tx_finish = 0;


uint8_t U2RxBuffer[RXBUFFERSIZE] = {0};
uint8_t U2RXdata_len = 0;
uint8_t u2_aRxBuffer;			
uint8_t Uart2_Rx_Cnt = 0;		
uint8_t uart2_rxstate = 0;
uint8_t uart2_rx_finish = 0;
uint8_t uart2_tx_finish = 0;

//todo
/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* UART4 init function */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}
/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = Screen_TX_Pin|Screen_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = K210_TX_Pin|K210_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = Bluetooth_TX_Pin|Bluetooth_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = JY901S_TX_Pin|JY901S_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOC, Screen_TX_Pin|Screen_RX_Pin);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, K210_TX_Pin|K210_RX_Pin);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, Bluetooth_TX_Pin|Bluetooth_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, JY901S_TX_Pin|JY901S_RX_Pin);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//todo

  void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
	if(huart->Instance == USART1)//*USART1
	{

		if((u1_aRxBuffer == 0xD1)&&(uart1_rxstate == 0))
		{
			U1RxBuffer[Uart1_Rx_Cnt++] = 0xD1;
			uart1_rxstate = 1;
		}
		else if(uart1_rxstate == 1)
		{
			U1RxBuffer[Uart1_Rx_Cnt++] = u1_aRxBuffer;
			if((u1_aRxBuffer == 0xD2)||(Uart1_Rx_Cnt >= RXBUFFERSIZE))
			{
				uart1_rxstate = 2;
			}
			
		}
		

    if((u1_aRxBuffer == 0xa1)&&(uart1_rxstate == 0))
		{
			U1RxBuffer[Uart1_Rx_Cnt++] = 0xa1;
			uart1_rxstate = 3;
		}
		else if(uart1_rxstate == 3)
		{
			U1RxBuffer[Uart1_Rx_Cnt++] = u1_aRxBuffer;
			if((u1_aRxBuffer == 0xa2)||(Uart1_Rx_Cnt >= RXBUFFERSIZE))
			{
				uart1_rxstate = 4;
			}
		}


		if(uart1_rxstate == 2 || uart1_rxstate == 4)
		{
			if((U1RxBuffer[0] == 0xD1)&&(U1RxBuffer[Uart1_Rx_Cnt-1]== 0xD2))
			{
        /*Reception Complete*/
        /*巡线数据*/
        if(openmv_run == 0) openmv_run = 1;

        deviation = (int16_t)(U1RxBuffer[1] - 90);
        if(U1RxBuffer[2] == 255) crossing_flag = 1;
        else crossing_flag = 0;
        if(U1RxBuffer[3] == 255) stop_flag = 1;
        else stop_flag =0;


				uart1_rxstate = 0;
				Uart1_Rx_Cnt = 0;
			}
      else if((U1RxBuffer[0] == 0xa1)&&(U1RxBuffer[Uart1_Rx_Cnt-1]== 0xa2))
			{
        /*Reception Complete*/
        /*方向判断*/
        if(U1RxBuffer[1] == 0){
          finded_flag = U1RxBuffer[2];
          target_num = U1RxBuffer[3];
        }  

        if(U1RxBuffer[1] == 1){
          finded_flag = U1RxBuffer[2];
          LoR = (int8_t)U1RxBuffer[3];
        }  

				uart1_rxstate = 0;
				Uart1_Rx_Cnt = 0;
			}
			else
			{
				uart1_rxstate = 0;
				Uart1_Rx_Cnt = 0;
				memset(U1RxBuffer,0x00,sizeof(U1RxBuffer));
			}
			
		}
		
			
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&u1_aRxBuffer, 1);   
		
	}//*USART1

	if(huart->Instance == USART3)//*USART3
	{

		if((u3_aRxBuffer == 0xD1)&&(uart3_rxstate == 0))
		{
			U3RxBuffer[Uart3_Rx_Cnt++] = 0xD1;
			uart3_rxstate = 1;
		}
		else if(uart3_rxstate == 1)
		{
			U3RxBuffer[Uart3_Rx_Cnt++] = u3_aRxBuffer;
			if((u3_aRxBuffer == 0xD2)||(Uart3_Rx_Cnt >= RXBUFFERSIZE))
			{
				uart3_rxstate = 2;
			}
			
		}
		
		
		if(uart3_rxstate == 2)
		{
			if((U3RxBuffer[0] == 0xD1)&&(U3RxBuffer[Uart3_Rx_Cnt-1]== 0xD2))
			{
        /*Reception Complete*/





				uart3_rxstate = 0;
				Uart3_Rx_Cnt = 0;
			}
			else
			{
				uart3_rxstate = 0;
				Uart3_Rx_Cnt = 0;
				memset(U3RxBuffer,0x00,sizeof(U3RxBuffer));
			}
			
		}
		
			
		
		HAL_UART_Receive_IT(&huart3, (uint8_t *)&u3_aRxBuffer, 1);   
		
	}//*USART3

  if(huart->Instance == USART2)//*USART2
    {

      if((u2_aRxBuffer == 0xD1)&&(uart2_rxstate == 0))
      {
        U2RxBuffer[Uart2_Rx_Cnt++] = 0xD1;
        uart2_rxstate = 1;
      }
      else if(uart2_rxstate == 1)
      {
        U2RxBuffer[Uart2_Rx_Cnt++] = u2_aRxBuffer;
        if((u2_aRxBuffer == 0xD2)||(Uart2_Rx_Cnt >= RXBUFFERSIZE))
        {
          uart2_rxstate = 2;
        }
        
      }
      
      
      if(uart2_rxstate == 2)
      {
        if((U2RxBuffer[0] == 0xD1)&&(U2RxBuffer[Uart2_Rx_Cnt-1]== 0xD2))
        {
          /*Reception Complete*/

          if(U2RxBuffer[1] == 0)
          {
            if(U2RxBuffer[2] == 1) slave_mid_start_flag = 1;
            if(U2RxBuffer[2] == 2) LED_YELLOW_OFF();
            if(U2RxBuffer[2] == 3) slave_get_start_flag = 1;
            if(U2RxBuffer[2] == 4) slave_restart2far_flag = 1;
          }



          uart2_rxstate = 0;
          Uart2_Rx_Cnt = 0;
        }
        else
        {
          uart2_rxstate = 0;
          Uart2_Rx_Cnt = 0;
          memset(U2RxBuffer,0x00,sizeof(U2RxBuffer));
        }
        
      }
      
        
      
      HAL_UART_Receive_IT(&huart2, (uint8_t *)&u2_aRxBuffer, 1);   
      
    }//*USART2




	
}




void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
    //LED_YELLOW_ON();
	}
}




//todo
/* USER CODE END 1 */
