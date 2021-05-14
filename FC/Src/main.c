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

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart7;
UART_HandleTypeDef huart8;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART8_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_UART7_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void delay_us(uint32_t);
void HAL_Delay(uint32_t);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void delay_us(uint32_t n)
//{
//	for(uint32_t i = 0; i < n; ++i)
//	{
//		for(uint32_t j = 0; j < 216 / 4; ++j)
//		{}
//	}
//}
//
//void HAL_Delay(uint32_t n)
//{
//	for(uint32_t i = 0; i < n; ++i)
//	{
//		for(uint32_t j = 0; j < 216 / 4 * 1000; ++j)
//		{}
//	}
//}
#define SELF_TEST_X_GYRO  0x00
#define SELF_TEST_Y_GYRO  0x01
#define SELF_TEST_Z_GYRO  0x02

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define XG_OFFS_USRH      0x13
#define XG_OFFS_USRL      0x14
#define YG_OFFS_USRH      0x15
#define YG_OFFS_USRL      0x16
#define ZG_OFFS_USRH      0x17
#define ZG_OFFS_USRL      0x18

#define SMPLRT_DIV        0x19

#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_CONFIG_2    0x1D

#define LOW_POWER_MODE		0x1E

#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存�??

#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存�??
#define MPU_INT_EN_REG			0X38	//中断使能寄存�??
#define MPU_INT_STA_REG			0X3A	//中断状�?�寄存器

#define ACCEL_XOUT_H      0x3B
#define ACCEL_XOUT_L      0x3C
#define ACCEL_YOUT_H      0x3D
#define ACCEL_YOUT_L      0x3E
#define ACCEL_ZOUT_H      0x3F
#define ACCEL_ZOUT_L      0x40
#define TEMP_OUT_H        0x41
#define TEMP_OUT_L        0x42
#define GYRO_XOUT_H       0x43
#define GYRO_XOUT_L       0x44
#define GYRO_YOUT_H       0x45
#define GYRO_YOUT_L       0x46
#define GYRO_ZOUT_H       0x47
#define GYRO_ZOUT_L       0x48

#define PWR_MGMT_1        0x6B
#define PWR_MGMT_2        0x6C
#define WHO_AM_I          0x75
#define ICM_20689_ENABLE HAL_GPIO_WritePin(SPI1_CS1_ICM20689_GPIO_Port, SPI1_CS1_ICM20689_Pin, GPIO_PIN_RESET)
#define ICM_20689_DISABLE HAL_GPIO_WritePin(SPI1_CS1_ICM20689_GPIO_Port, SPI1_CS1_ICM20689_Pin, GPIO_PIN_SET)

static uint8_t ICM20689_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	ICM_20689_ENABLE;
	status = HAL_SPI_Transmit(&hspi1, &reg, 1, 0xFFFF);
	status = HAL_SPI_Transmit(&hspi1, &value, 1, 0xFFFF);
	ICM_20689_DISABLE;
	return(status);
}

static uint8_t ICM20689_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	ICM_20689_ENABLE;
	reg = reg|0x80;
	HAL_SPI_Transmit(&hspi1, &reg, 1, 0xFFFF);
 	HAL_SPI_Receive(&hspi1, &reg_val, 1, 0xFFFF);
	ICM_20689_DISABLE;
	return(reg_val);
}

void icm20689_reset()
{
	ICM20689_Write_Reg(PWR_MGMT_1,0X80);	//复位
	HAL_Delay(100);
	ICM20689_Write_Reg(PWR_MGMT_1, 0x00);	//解除休眠状�??
}

uint8_t Init_ICM20689(void)
{
	ICM20689_Write_Reg(PWR_MGMT_1,0X80);	//复位
		HAL_Delay(100);
	if(ICM20689_Read_Reg(WHO_AM_I)==0x98)
	{
	}
	else
	{
		return 1;
	}

	ICM20689_Write_Reg(PWR_MGMT_1, 0x00);	//解除休眠状�??

	ICM20689_Write_Reg(GYRO_CONFIG,3<<3);//设置�??螺仪满量程范�??
	ICM20689_Write_Reg(ACCEL_CONFIG,0<<3);//加�?�度计工作范�?? 2G
	ICM20689_Write_Reg(ACCEL_CONFIG_2, 0x08);//加�?�计高�?�滤波频�?? 典型�?? �??0x08  �??1.13kHz�??

	ICM20689_Write_Reg(SMPLRT_DIV,19);
	ICM20689_Write_Reg(CONFIG,4);					//低�?�滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K

	ICM20689_Write_Reg(MPU_INT_EN_REG,0X00);	//关闭�??有中�??
	ICM20689_Write_Reg(MPU_FIFO_EN_REG,0X00);	//关闭FIFO

	ICM20689_Write_Reg(PWR_MGMT_1,0X01);	//设置CLKSEL,PLL X轴为参�??
	ICM20689_Write_Reg(PWR_MGMT_2,0X00);	//加�?�度与陀螺仪都工�??


	return 0;
}
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
  MX_UART8_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_UART7_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  uint8_t a_1[9] = {'H','E','L','L','O','_','1','\r','\n'};
//	  uint8_t a_2[9] = {'H','E','L','L','O','_','2','\r','\n'};
//	  uint8_t a_3[9] = {'H','E','L','L','O','_','3','\r','\n'};
//	  uint8_t a_4[9] = {'H','E','L','L','O','_','4','\r','\n'};
//	  uint8_t a_5[9] = {'H','E','L','L','O','_','5','\r','\n'};
//	  uint8_t a_6[9] = {'H','E','L','L','O','_','6','\r','\n'};
//	  uint8_t a_7[9] = {'H','E','L','L','O','_','7','\r','\n'};
//	  uint8_t a_8[9] = {'H','E','L','L','O','_','8','\r','\n'};
//	  HAL_UART_Transmit(&huart1, a_1, 9, 0xffff);
//	  HAL_UART_Transmit(&huart2, a_2, 9, 0xffff);
//	  HAL_UART_Transmit(&huart3, a_3, 9, 0xffff);
//	  HAL_UART_Transmit(&huart4, a_4, 9, 0xffff);
//	  HAL_UART_Transmit(&huart5, a_5, 9, 0xffff);
//	  HAL_UART_Transmit(&huart6, a_6, 9, 0xffff);
//	  HAL_UART_Transmit(&huart7, a_7, 9, 0xffff);
//	  HAL_UART_Transmit(&huart8, a_8, 9, 0xffff);
//	  HAL_GPIO_WritePin(nLED_RED_GPIO_Port, nLED_RED_Pin, GPIO_PIN_RESET);
//	  HAL_Delay(500);
//	  HAL_GPIO_WritePin(nLED_GREEN_GPIO_Port, nLED_GREEN_Pin, GPIO_PIN_RESET);
//	  HAL_Delay(500);
//	  HAL_GPIO_WritePin(nLED_BLUE_GPIO_Port, nLED_BLUE_Pin, GPIO_PIN_RESET);
//	  HAL_Delay(500);
//	  HAL_GPIO_WritePin(nLED_BLUE_GPIO_Port, nLED_BLUE_Pin, GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(nLED_GREEN_GPIO_Port, nLED_GREEN_Pin, GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(nLED_RED_GPIO_Port, nLED_RED_Pin, GPIO_PIN_SET);
//	  HAL_Delay(500);
	  uint8_t reg;
	  uint8_t ack;
//	  HAL_GPIO_WritePin(SPI1_CS1_ICM20689_GPIO_Port, SPI1_CS1_ICM20689_Pin, GPIO_PIN_RESET);
//	  HAL_GPIO_WritePin(SPI1_CS3_BMI055_GYRO_GPIO_Port, SPI1_CS3_BMI055_GYRO_Pin, GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(SPI1_CS4_BMI055_ACC_GPIO_Port, SPI1_CS4_BMI055_ACC_Pin, GPIO_PIN_SET);

//	  reg[0] = 0x6B;
//	  reg[1] = 0x80;
//	  HAL_SPI_Transmit(&hspi1, reg, 2, 0xffff);
//	  HAL_Delay(1);
//
//	  reg[0] = 0x68;
//	  reg[1] = 0x03;
//	  HAL_SPI_Transmit(&hspi1, reg, 2, 0xffff);
//	  HAL_Delay(1);
	  int result = Init_ICM20689();
	  ack = ICM20689_Read_Reg(WHO_AM_I);
	  HAL_Delay(100);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART6
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_UART5
                              |RCC_PERIPHCLK_UART7|RCC_PERIPHCLK_UART8;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart8ClockSelection = RCC_UART8CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
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
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief UART7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART7_Init(void)
{

  /* USER CODE BEGIN UART7_Init 0 */

  /* USER CODE END UART7_Init 0 */

  /* USER CODE BEGIN UART7_Init 1 */

  /* USER CODE END UART7_Init 1 */
  huart7.Instance = UART7;
  huart7.Init.BaudRate = 115200;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART7_Init 2 */

  /* USER CODE END UART7_Init 2 */

}

/**
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

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
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
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
  huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
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
  huart3.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, nLED_BLUE_Pin|nLED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS1_ICM20689_GPIO_Port, SPI1_CS1_ICM20689_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(nLED_RED_GPIO_Port, nLED_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SPI1_DRDY1_ICM20689_Pin SPI1_DRDY2_BMI055_GYRO_Pin SPI1_DRDY3_BMI055_ACC_Pin */
  GPIO_InitStruct.Pin = SPI1_DRDY1_ICM20689_Pin|SPI1_DRDY2_BMI055_GYRO_Pin|SPI1_DRDY3_BMI055_ACC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : nLED_BLUE_Pin nLED_GREEN_Pin */
  GPIO_InitStruct.Pin = nLED_BLUE_Pin|nLED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS1_ICM20689_Pin */
  GPIO_InitStruct.Pin = SPI1_CS1_ICM20689_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS1_ICM20689_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : nLED_RED_Pin */
  GPIO_InitStruct.Pin = nLED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(nLED_RED_GPIO_Port, &GPIO_InitStruct);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
