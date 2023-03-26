/*
 * lcd.c
 *
 *  Created on: Jan 2, 2023
 *      Author: asong
 */

#include "ssd1315.h"
#include "../../Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_i2c.h"
#include "../../Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_ll_utils.h"

//#define SSD1315_DEVICE_ADDRESS 0x78
#define SSD1315_DEVICE_ADDRESS 0x3C


// Master transmitter
int8_t I2C_MasterTx(uint8_t devAddr, uint8_t* buffer, uint16_t len, uint16_t ms)
{
  // timeout counter
  uint16_t cnt = 0;

  // 1. start condition
  LL_I2C_GenerateStartCondition(I2C1);

  // 2. check start bit flag
  while(!LL_I2C_IsActiveFlag_SB(I2C1))
  {
    LL_mDelay(1);
    if(cnt++ > ms) return -1;
  }

  // 3. write device address (WRITE)
  LL_I2C_TransmitData8(I2C1, (devAddr << 1) | 0x00);

  // 4. wait address sent
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
    LL_mDelay(1);
    if(cnt++ > ms) return -1;
  }

  // 5. clear ADDR flag
  LL_I2C_ClearFlag_ADDR(I2C1);

  // 6. check TXE flag & write data
  for(int i=0; i < len; i++)
  {
    while(!LL_I2C_IsActiveFlag_TXE(I2C1))
    {
      LL_mDelay(1);
      if(cnt++ > ms) return -1;
    }

    LL_I2C_TransmitData8(I2C1, buffer[i]);
  }

  // 7. wait BTF flag (TXE flag set & empty DR condition)
  while(!LL_I2C_IsActiveFlag_BTF(I2C1))
  {
    LL_mDelay(1);
    if(cnt++ > ms) return -1;
  }

  // 8. stop condition
  LL_I2C_GenerateStopCondition(I2C1);

  return 0;
}


void    LCD_IO_Init(void) {
	return ;
}

void    LCD_IO_DeInit(void) {

}

void    LCD_IO_WriteCommand(uint8_t Cmd) {
	uint8_t sand_data[2] = { 0x80, Cmd};

	I2C_MasterTx(SSD1315_DEVICE_ADDRESS, sand_data, 2, 1000);
}

void    LCD_IO_WriteData(uint8_t Value) {
	uint8_t sand_data[2] = { 0xC0, Value};
	I2C_MasterTx(SSD1315_DEVICE_ADDRESS, sand_data, 2, 1000);
}

void    LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size) {
	uint8_t sand_data[Size+1];
	sand_data[0] = 0x40;
	memcpy(sand_data +1, pData, Size);
	I2C_MasterTx(SSD1315_DEVICE_ADDRESS, sand_data, Size+1, 1000);
}



