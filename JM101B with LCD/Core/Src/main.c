/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD1602.h"
uint8_t shoot=0;
uint8_t shoot1=0;

void debounce(void)
{
	// Counter for number of equal states
	static uint8_t count = 0;
	// Keeps track of current (debounced) state
	static uint8_t button_state = 0;
	// Check if button is high or low for the moment
	uint8_t current_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	if (current_state != button_state) 
	{
		// Button state is about to be changed, increase counter
		count++;
		if (count >= 5) {
			// The button have not bounced for some checks, change state
			button_state = current_state;
			if (current_state != 0) {
				shoot=1;
			}
			count = 0;
		}
	} else {
	// Reset counter
		count = 0;
	}
}

void debounce2(void)
{
	// Counter for number of equal states
	static uint8_t count2 = 0;
	// Keeps track of current (debounced) state
	static uint8_t button_state2 = 0;
	// Check if button is high or low for the moment
	uint8_t current_state2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
	if (current_state2 != button_state2) 
	{
		// Button state is about to be changed, increase counter
		count2++;
		if (count2 >= 5) {
			// The button have not bounced for some checks, change state
			button_state2 = current_state2;
			if (current_state2 != 0) {
				shoot1=1;
			}
			count2 = 0;
		}
	} else {
	// Reset counter
		count2 = 0;
	}
}

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t finger_id=1; 
uint8_t FPM10A_RECEICE_BUFFER[12]={0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0xA,0x0,0x00};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */


uint8_t FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF}; 
uint8_t FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    
uint8_t FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; 
uint8_t FPM10A_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; 
uint8_t FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08};
uint8_t FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; 
uint8_t FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09};
uint8_t FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};

void FPM10A_Cmd_Send_Pack_Head(void)
{
	int i;	
	for(i=0;i<6;i++) 
  {
	 HAL_UART_Transmit(&huart1, &FPM10A_Pack_Head[i],1,10000 );   
	}		
}

void FPM10A_Cmd_Get_Img(void)
{
    unsigned char i;
    FPM10A_Cmd_Send_Pack_Head(); //????????
    for(i=0;i<6;i++) 
		{
				HAL_UART_Transmit(&huart1, &FPM10A_Get_Img[i],1,10000);
		
		}
}

void FPM10A_Receive_Data(unsigned char ucLength)
{
  unsigned char i;


		HAL_UART_Receive(&huart1,FPM10A_RECEICE_BUFFER, 12,1000);

	 

}

void FPM10A_Cmd_Reg_Model(void)
{
       unsigned char i;	   
	    
			 FPM10A_Cmd_Send_Pack_Head(); 

       for(i=0;i<6;i++)
       {
    	      HAL_UART_Transmit(&huart1,&FPM10A_Reg_Model[i], 1,1000);   
   		 }
}


void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head();
	for(i=0;i<7;i++)   
	{
		HAL_UART_Transmit(&huart1,&FPM10A_Img_To_Buffer1[i], 1,1000);
	}
}

void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
    unsigned char i;
		for(i=0;i<6;i++)    
		{
			HAL_UART_Transmit(&huart1,&FPM10A_Pack_Head[i], 1,1000);   
		}
		for(i=0;i<7;i++)  
		{
			HAL_UART_Transmit(&huart1,&FPM10A_Img_To_Buffer2[i], 1,1000);
		}
}



void FPM10A_Cmd_Save_Finger( unsigned int storeID )
{
		unsigned long temp = 0;
		unsigned char i;
		FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
		FPM10A_Save_Finger[6] = (storeID&0x00FF);
	
		for(i=0;i<7;i++)
			temp = temp + FPM10A_Save_Finger[i]; 
		FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8;
		FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
		FPM10A_Cmd_Send_Pack_Head(); 	
	
		for(i=0;i<9;i++)  
			HAL_UART_Transmit(&huart1, &FPM10A_Save_Finger[i], 1,1000);      
}
void FPM10A_Cmd_Search_Finger(void)
{
	unsigned char i;	   
	FPM10A_Cmd_Send_Pack_Head(); 
	for(i=0;i<11;i++)
	{
		HAL_UART_Transmit(&huart1, &FPM10A_Search[i], 1,1000);   
	}
}

uint8_t fraud=0;
void FPM10A_Find_Fingerprint()
{
	HAL_Delay(2000);
	unsigned int find_fingerid = 0;
	//notify the user to place his finger 
	lcd_send_string("Place your finger ");
	HAL_Delay(2000);
	lcd_clear();
	FPM10A_Cmd_Get_Img(); 
  // if Recieved successfully 
	if(FPM10A_RECEICE_BUFFER[9]==0)
	{
		HAL_Delay(100);
		FINGERPRINT_Cmd_Img_To_Buffer1();
		HAL_Delay(100);
		FPM10A_Receive_Data(12);
		FPM10A_Cmd_Search_Finger();
		HAL_Delay(100);
		FPM10A_Receive_Data(16);
		// if Recieved successfully 
		if(FPM10A_RECEICE_BUFFER[9] == 0) //???  
		{
			HAL_Delay(100);
			lcd_send_string("Access Granted ");
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
			HAL_Delay(1500);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
			fraud=0;
			HAL_Delay(2000);
			lcd_clear();
		}
		else
		{ HAL_Delay(100);
			lcd_send_string("Access Denied ");
			fraud = fraud + 1;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
			HAL_Delay(1500);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
			HAL_Delay(2000);
			lcd_clear();
		}
	}
	else
		{ HAL_Delay(100);
			lcd_send_string("Access Denied ");
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
			HAL_Delay(1500);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
			fraud = fraud + 1;
			HAL_Delay(2000);
			lcd_clear();
		}
}




/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int row=0;
int col=0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	

	HAL_TIM_Base_Start(&htim1);
  lcd_init ();
  lcd_put_cur(0, 0);
  lcd_send_string("HELLO ");
	HAL_Delay(3000);
  lcd_clear();
	lcd_send_string("TO ADD -> 1");
	lcd_send_string("");
	lcd_put_cur(1, 0);
	lcd_send_string("TO VERIFY -> 2");
	lcd_send_string("");
  HAL_Delay(3000);
  lcd_clear();
	

	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		debounce();
		if(shoot)
		{
				lcd_clear();
				HAL_Delay(100);
        // Notify the user to press 
				lcd_send_string("ADD FINGER");
		    HAL_Delay(3000);
	     	lcd_clear();
			  // send the command packet to the sensor 
		    FPM10A_Cmd_Get_Img(); 
				HAL_Delay(100);
			  // Rescieve the respond packet from the sensor through UART2
				FPM10A_Receive_Data(12);
				HAL_Delay(100);
			  // check if the data recieved successfully 
				if(FPM10A_RECEICE_BUFFER[9]==0)
				 {
					HAL_Delay(100);
					 // send the sensor to save the finger print to the buffer 
					FINGERPRINT_Cmd_Img_To_Buffer1();
					HAL_Delay(100);
				        FPM10A_Receive_Data(12);
					lcd_send_string("Successful entry");
					lcd_put_cur(1, 0);
					HAL_Delay(3000);
					lcd_clear();
					// notify the user again to put his finger 
					lcd_send_string(" ADD FINGER ");
			  	         HAL_Delay(3000);
					 FPM10A_Cmd_Get_Img(); 
					 HAL_Delay(100);
					 FPM10A_Receive_Data(12);
					 HAL_Delay(100);
					 lcd_clear();
					 // if the packet saved suucessfully 
					if(FPM10A_RECEICE_BUFFER[9]==0)
					{
						HAL_Delay(2000);
						lcd_send_string("Successful entry");
						lcd_put_cur(1, 0);
						// assign a unique ID to each user 
						lcd_send_string(" ID is " );
						char currID = finger_id + 48;
						lcd_send_data(currID);
						FINGERPRINT_Cmd_Img_To_Buffer2();
						HAL_Delay(100);
				  	FPM10A_Receive_Data(12);
						FPM10A_Cmd_Reg_Model();//??????
						HAL_Delay(100);
	         	FPM10A_Receive_Data(12); 
					  
            HAL_Delay(100);						
	          
						HAL_Delay(3000);
						finger_id=finger_id+1;
						HAL_Delay(3000);
            lcd_clear();

				  	}
					else if( FPM10A_RECEICE_BUFFER[9]==2)
						
					{
					 lcd_send_string("Fail to detect");
						HAL_Delay(3000);
            lcd_clear();
					
					}
					else 
						lcd_send_string("Nothing happened");
					  HAL_Delay(3000);
            lcd_clear();
				   }
		     else if( FPM10A_RECEICE_BUFFER[9]==2)
						
					{
					  lcd_send_string("Fail to detect2");
						HAL_Delay(3000);
            lcd_clear();
					
					}
					else 
					{
						lcd_send_string("Nothing happened");
					}
			shoot=0;
			HAL_Delay(1000);
			lcd_clear();
		}
		debounce2();
		if(shoot1)
		{
			
			FPM10A_Find_Fingerprint();

			shoot1=0;
			HAL_Delay(1000);
			lcd_clear();
			
		}
		if(fraud==3)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,1);
			fraud=0; 
			HAL_Delay(2000);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,0);
		}
  }
	
	
	
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_2;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4
                           PA5 PA6 PA7 PA8
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
