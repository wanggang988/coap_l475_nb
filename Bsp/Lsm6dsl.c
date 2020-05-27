/* Includes ------------------------------------------------------------------*/
//#include "stm32l4xx_hal.h"
#include "Lsm6dsl.h"



/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
int16_t pDataXYZ[3] = {0};
float pGyroDataXYZ[3] = {0};

static ACCELERO_DrvTypeDef *AccelerometerDrv;
/* Private function prototypes -----------------------------------------------*/
uint32_t Serial_Scanf(uint32_t value)
{
  uint16_t tmp = 0;
  
  tmp = getchar();
  
  if (tmp > value)
  {
    printf("\n\r  !!! Please enter valid number between 0 and %u \n", value);
    return 0xFF;
  }
  return tmp;
}
/**
  * @brief  Get XYZ acceleration values.
  * @param  pDataXYZ Pointer on 3 angular accelerations table with  
  *                  pDataXYZ[0] = X axis, pDataXYZ[1] = Y axis, pDataXYZ[2] = Z axis
  * @retval None
  */
void BSP_ACCELERO_AccGetXYZ(int16_t *pDataXYZ)
{
  if(AccelerometerDrv != NULL)
  {
    if(AccelerometerDrv->GetXYZ != NULL)
    {   
      AccelerometerDrv->GetXYZ(pDataXYZ);
    }
  }
}
/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10909CEC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

}
void SENSOR_IO_Init(void)
{
  MX_I2C1_Init();
}
static void I2Cx_Error(I2C_HandleTypeDef *i2c_handler, uint8_t Addr)
{
  /* De-initialize the I2C communication bus */
  HAL_I2C_DeInit(i2c_handler);
  
  /* Re-Initialize the I2C communication bus */
  MX_I2C1_Init();
}
/**
  * @brief  Reads multiple data.
  * @param  i2c_handler : I2C handler
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  MemAddress: memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_ReadMultiple(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occured */
    I2Cx_Error(i2c_handler, Addr);
  }
  return status;
}
/**
  * @brief  Writes a value in a register of the device through BUS in using DMA mode.
  * @param  i2c_handler : I2C handler
  * @param  Addr: Device address on BUS Bus.
  * @param  Reg: The target register address to write
  * @param  MemAddress: memory address
  * @param  Buffer: The target register value to be written
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_WriteMultiple(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Write(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the I2C Bus */
    I2Cx_Error(i2c_handler, Addr);
  }
  return status;
}
/**
  * @brief  Reads multiple data with I2C communication
  *         channel from TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval HAL status
  */
uint16_t SENSOR_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
 return I2Cx_ReadMultiple(&hi2c1, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}

/**
  * @brief  Writes multiple data with I2C communication
  *         channel from MCU to TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval None
  */
void SENSOR_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
  I2Cx_WriteMultiple(&hi2c1, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}
/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  Value: Data to be written
  * @retval None
  */
void SENSOR_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  I2Cx_WriteMultiple(&hi2c1, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT,(uint8_t*)&Value, 1);
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @retval Data to be read
  */
uint8_t SENSOR_IO_Read(uint8_t Addr, uint8_t Reg)
{
  uint8_t read_value = 0;

  I2Cx_ReadMultiple(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&read_value, 1);

  return read_value;
}
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  Test of LSM6DSL accelerometer sensor.
  */
void Accelero_Test(void)
{
  uint32_t ret = 0;
  printf("\n***************************************************************\n");
  printf("\n******************* Accelerometer Test ************************\n");
  printf("\n***************************************************************\n\n");
  BSP_ACCELERO_Init();
//  printf("\n*** Type n or N to get a first accelero data ***\n\n");
//  printf("\n*** Type q or Q to quit accelerometer Test ***\n\n");
//  while(1)
//  {
//    ret = Serial_Scanf(255);
//    if((ret == 'n') || (ret == 'N'))
//    {
      printf("\n*** This is a new data ***\n\n");
      BSP_ACCELERO_AccGetXYZ(pDataXYZ);
      printf("ACCELERO_X = %d \n", pDataXYZ[0]);
      printf("ACCELERO_Y = %d \n", pDataXYZ[1]);
      printf("ACCELERO_Z = %d \n", pDataXYZ[2]);
//      printf("\n*** This is a new data ***\n\n");
//      printf("\n*** Type n or N to get a new data ***\n\n"); 
//      printf("\n*** Type q or Q to quit Accelerometer Test ***\n\n");      
//    }
//    else if((ret == 'q') || (ret == 'Q'))
//    {
//      BSP_ACCELERO_DeInit();
//      printf("\n*** End of Accelerometer Test ***\n\n");
//      return;
//    }
//    else
//    {
//      printf("\n*** Type n or N to get a new data ***\n\n"); 
//      printf("\n*** Type q or Q to quit Accelerometer Test ***\n\n"); 
//    }    
//  }    
}

/** @defgroup STM32L475E_IOT01_ACCELERO_Private_Functions ACCELERO Private Functions
  * @{
  */ 
/**
  * @brief  Initialize the ACCELERO.
  * @retval ACCELERO_OK or ACCELERO_ERROR
  */
ACCELERO_StatusTypeDef BSP_ACCELERO_Init(void)
{  
  ACCELERO_StatusTypeDef ret = ACCELERO_OK;
  uint16_t ctrl = 0x0000;
  ACCELERO_InitTypeDef LSM6DSL_InitStructure;

  if(Lsm6dslAccDrv.ReadID() != LSM6DSL_ACC_GYRO_WHO_AM_I)
  {
    ret = ACCELERO_ERROR;
  }
  else
  {
    /* Initialize the ACCELERO accelerometer driver structure */
    AccelerometerDrv = &Lsm6dslAccDrv;
  
    /* MEMS configuration ------------------------------------------------------*/
    /* Fill the ACCELERO accelerometer structure */
    LSM6DSL_InitStructure.AccOutput_DataRate = LSM6DSL_ODR_52Hz;
    LSM6DSL_InitStructure.Axes_Enable = 0;
    LSM6DSL_InitStructure.AccFull_Scale = LSM6DSL_ACC_FULLSCALE_2G;
    LSM6DSL_InitStructure.BlockData_Update = LSM6DSL_BDU_BLOCK_UPDATE;
    LSM6DSL_InitStructure.High_Resolution = 0;
    LSM6DSL_InitStructure.Communication_Mode = 0;
        
    /* Configure MEMS: data rate, full scale  */
    ctrl =  (LSM6DSL_InitStructure.AccOutput_DataRate | LSM6DSL_InitStructure.AccFull_Scale);
    
    /* Configure MEMS: BDU and Auto-increment for multi read/write */
    ctrl |= ((LSM6DSL_InitStructure.BlockData_Update | LSM6DSL_ACC_GYRO_IF_INC_ENABLED) << 8);

    /* Configure the ACCELERO accelerometer main parameters */
    AccelerometerDrv->Init(ctrl);
  }  

  return ret;
}

/**
  * @brief  DeInitialize the ACCELERO.
  * @retval None.
  */
void BSP_ACCELERO_DeInit(void)
{
  /* DeInitialize the accelerometer IO interfaces */
  if(AccelerometerDrv != NULL)
  {
    if(AccelerometerDrv->DeInit != NULL)
    {
      AccelerometerDrv->DeInit();
    }
  }
}

/** @defgroup LSM6DSL_Private_Variables LSM6DSL Private Variables
  * @{
  */ 
ACCELERO_DrvTypeDef Lsm6dslAccDrv =
{
  LSM6DSL_AccInit,
  LSM6DSL_AccDeInit,
  LSM6DSL_AccReadID,
  0,
  LSM6DSL_AccLowPower,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  LSM6DSL_AccReadXYZ
};

/**
  * @brief  Set LSM6DSL Accelerometer Initialization.
  * @param  InitStruct: Init parameters
  */
void LSM6DSL_AccInit(uint16_t InitStruct)
{  
  uint8_t ctrl = 0x00;
  uint8_t tmp;

  /* Read CTRL1_XL */
  tmp = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL);

  /* Write value to ACC MEMS CTRL1_XL register: FS and Data Rate */
  ctrl = (uint8_t) InitStruct;
  tmp &= ~(0xFC);
  tmp |= ctrl;
  SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL, tmp);

  /* Read CTRL3_C */
  tmp = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL3_C);

  /* Write value to ACC MEMS CTRL3_C register: BDU and Auto-increment */
  ctrl = ((uint8_t) (InitStruct >> 8));
  tmp &= ~(0x44);
  tmp |= ctrl; 
  SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL3_C, tmp);
}

/**
  * @brief  LSM6DSL Accelerometer De-initialization.
  */
void LSM6DSL_AccDeInit(void)
{
  uint8_t ctrl = 0x00;
  
  /* Read control register 1 value */
  ctrl = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL);

  /* Clear ODR bits */
  ctrl &= ~(LSM6DSL_ODR_BITPOSITION);

  /* Set Power down */
  ctrl |= LSM6DSL_ODR_POWER_DOWN;
  
  /* write back control register */
  SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL, ctrl);
}

/**
  * @brief  Read LSM6DSL ID.
  * @retval ID 
  */
uint8_t LSM6DSL_AccReadID(void)
{  
  /* IO interface initialization */
  SENSOR_IO_Init();
  /* Read value at Who am I register address */
  return (SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_WHO_AM_I_REG));
}

/**
  * @brief  Set/Unset Accelerometer in low power mode.
  * @param  status 0 means disable Low Power Mode, otherwise Low Power Mode is enabled
  */
void LSM6DSL_AccLowPower(uint16_t status)
{
  uint8_t ctrl = 0x00;
  
  /* Read CTRL6_C value */
  ctrl = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL6_C);

  /* Clear Low Power Mode bit */
  ctrl &= ~(0x10);

  /* Set Low Power Mode */
  if(status)
  {
    ctrl |= LSM6DSL_ACC_GYRO_LP_XL_ENABLED;
  }else
  {
    ctrl |= LSM6DSL_ACC_GYRO_LP_XL_DISABLED;
  }
  
  /* write back control register */
  SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL6_C, ctrl);
}

/**
  * @brief  Read X, Y & Z Acceleration values 
  * @param  pData: Data out pointer
  */
void LSM6DSL_AccReadXYZ(int16_t* pData)
{
  int16_t pnRawData[3];
  uint8_t ctrlx= 0;
  uint8_t buffer[6];
  uint8_t i = 0;
  float sensitivity = 0;
  
  /* Read the acceleration control register content */
  ctrlx = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL);
  
  /* Read output register X, Y & Z acceleration */
  SENSOR_IO_ReadMultiple(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_OUTX_L_XL, buffer, 6);
  
  for(i=0; i<3; i++)
  {
    pnRawData[i]=((((uint16_t)buffer[2*i+1]) << 8) + (uint16_t)buffer[2*i]);
  }
  
  /* Normal mode */
  /* Switch the sensitivity value set in the CRTL1_XL */
  switch(ctrlx & 0x0C)
  {
  case LSM6DSL_ACC_FULLSCALE_2G:
    sensitivity = LSM6DSL_ACC_SENSITIVITY_2G;
    break;
  case LSM6DSL_ACC_FULLSCALE_4G:
    sensitivity = LSM6DSL_ACC_SENSITIVITY_4G;
    break;
  case LSM6DSL_ACC_FULLSCALE_8G:
    sensitivity = LSM6DSL_ACC_SENSITIVITY_8G;
    break;
  case LSM6DSL_ACC_FULLSCALE_16G:
    sensitivity = LSM6DSL_ACC_SENSITIVITY_16G;
    break;    
  }
  
  /* Obtain the mg value for the three axis */
  for(i=0; i<3; i++)
  {
    pData[i]=( int16_t )(pnRawData[i] * sensitivity);
  }
}