/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : TouchGFXHAL.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.26.1. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include <TouchGFXHAL.hpp>

/* USER CODE BEGIN TouchGFXHAL.cpp */

#include <TouchGFXGeneratedHAL.hpp>
#include "stm32h5xx_hal.h"
#include "main.h"

/**
  * @brief  ST7789H2 Registers
  */
#define ST7789H2_SLEEP_OUT                    0x11U
#define ST7789H2_DISPLAY_INVERSION_ON         0x21U
#define ST7789H2_DISPLAY_ON                   0x29U
#define ST7789H2_CASET                        0x2AU
#define ST7789H2_RASET                        0x2BU
#define ST7789H2_WRITE_RAM                    0x2CU
#define ST7789H2_TE_LINE_ON                   0x35U
#define ST7789H2_COLOR_MODE                   0x3AU
#define ST7789H2_PV_GAMMA_CTRL                0xE0U /* Positive voltage */
#define ST7789H2_NV_GAMMA_CTRL                0xE1U /* Negative voltage */

#define ST7789H2_FORMAT_RBG565                0x05U /* Pixel format chosen is RGB565 : 16 bpp */

__weak void LCD_IO_Init(void);
__weak void LCD_IO_WriteData(uint16_t RegValue);
__weak void LCD_IO_WriteMultipleData(uint16_t* pData, uint32_t Size);
__weak void LCD_IO_WriteReg(uint8_t Reg);
__weak uint16_t LCD_IO_ReadData(void);
__weak void LCD_IO_Delay(uint32_t Delay);

void initLCD(void);

using namespace touchgfx;

void TouchGFXHAL::initialize()
{
    // Calling parent implementation of initialize().
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.
    // Please note, HAL::initialize() must be called to initialize the framework.

	initLCD();

    TouchGFXGeneratedHAL::initialize();
}

/**
 * Gets the frame buffer address used by the TFT controller.
 *
 * @return The address of the frame buffer currently being displayed on the TFT.
 */
uint16_t* TouchGFXHAL::getTFTFrameBuffer() const
{
    // Calling parent implementation of getTFTFrameBuffer().
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.

    return TouchGFXGeneratedHAL::getTFTFrameBuffer();
}

/**
 * Sets the frame buffer address used by the TFT controller.
 *
 * @param [in] address New frame buffer address.
 */
void TouchGFXHAL::setTFTFrameBuffer(uint16_t* address)
{
    // Calling parent implementation of setTFTFrameBuffer(uint16_t* address).
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.

    TouchGFXGeneratedHAL::setTFTFrameBuffer(address);
}

/**
 * This function is called whenever the framework has performed a partial draw.
 *
 * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
 *
 * @see flushFrameBuffer().
 */
void TouchGFXHAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
    // Calling parent implementation of flushFrameBuffer(const touchgfx::Rect& rect).
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.
    // Please note, HAL::flushFrameBuffer(const touchgfx::Rect& rect) must
    // be called to notify the touchgfx framework that flush has been performed.
    // To calculate the start address of rect,
    // use advanceFrameBufferToRect(uint8_t* fbPtr, const touchgfx::Rect& rect)
    // defined in TouchGFXGeneratedHAL.cpp

    TouchGFXGeneratedHAL::flushFrameBuffer(rect);
}

bool TouchGFXHAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
    return TouchGFXGeneratedHAL::blockCopy(dest, src, numBytes);
}

/**
 * Configures the interrupts relevant for TouchGFX. This primarily entails setting
 * the interrupt priorities for the DMA and LCD interrupts.
 */
void TouchGFXHAL::configureInterrupts()
{
    // Calling parent implementation of configureInterrupts().
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.

    TouchGFXGeneratedHAL::configureInterrupts();
}

/**
 * Used for enabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::enableInterrupts()
{
    // Calling parent implementation of enableInterrupts().
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.

    TouchGFXGeneratedHAL::enableInterrupts();
}

/**
 * Used for disabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::disableInterrupts()
{
    // Calling parent implementation of disableInterrupts().
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.

    TouchGFXGeneratedHAL::disableInterrupts();
}

/**
 * Configure the LCD controller to fire interrupts at VSYNC. Called automatically
 * once TouchGFX initialization has completed.
 */
void TouchGFXHAL::enableLCDControllerInterrupt()
{
    // Calling parent implementation of enableLCDControllerInterrupt().
    //
    // To overwrite the generated implementation, omit the call to the parent function
    // and implement the needed functionality here.

    TouchGFXGeneratedHAL::enableLCDControllerInterrupt();
}

bool TouchGFXHAL::beginFrame()
{
    return TouchGFXGeneratedHAL::beginFrame();
}

void TouchGFXHAL::endFrame()
{
    TouchGFXGeneratedHAL::endFrame();
}

void initLCD(void)
{
    /* Reset ST7789H2 */
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(1); /* wait at least 10us according ST7789H2 datasheet */
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(120); /* wait maximum 120ms according ST7789H2 datasheet */

    /* Init display */
    LCD_IO_WriteReg(ST7789H2_COLOR_MODE);
    LCD_IO_WriteData(ST7789H2_FORMAT_RBG565);
    LCD_IO_WriteReg(ST7789H2_DISPLAY_INVERSION_ON);

    /* Voltage Gamma Control */
    const uint8_t arrayLength = 14;
    const uint16_t PVparameters[arrayLength] = {0x00D0, 0x0008, 0x0011, 0x0008, 0x000C, 0x0015, 0x0039, 0x0033, 0x0050, 0x0036, 0x0013, 0x0014, 0x0029, 0x002D};
    const uint16_t NVparameters[arrayLength] = {0x00D0, 0x0008, 0x0010, 0x0008, 0x0006, 0x0006, 0x0039, 0x0044, 0x0051, 0x000B, 0x0016, 0x0014, 0x002F, 0x0031};
    LCD_IO_WriteReg(ST7789H2_PV_GAMMA_CTRL);
    for (uint32_t i = 0; i < arrayLength; i++)
    {
        LCD_IO_WriteData(PVparameters[i]);
    }
    LCD_IO_WriteReg(ST7789H2_NV_GAMMA_CTRL);
    for (uint32_t i = 0; i < arrayLength; i++)
    {
        LCD_IO_WriteData(NVparameters[i]);
    }

    /* Fill with black pixels */
    LCD_IO_WriteReg(ST7789H2_WRITE_RAM);
    for (uint32_t i = 0; i < 240 * 240; i++)
    {
        LCD_IO_WriteData(0x0000);
    }

    /* Display on */
    LCD_IO_WriteReg(ST7789H2_DISPLAY_ON);
    LCD_IO_WriteReg(ST7789H2_SLEEP_OUT);

    /* Tearing effect line on */
    LCD_IO_WriteReg(ST7789H2_TE_LINE_ON);
    LCD_IO_WriteData(0x00); // TE Mode 1
}

void setWindow(uint32_t Xpos, uint32_t Ypos, uint32_t Xwidth, uint32_t Ywidth)
{
    // set pixel x pos:
    LCD_IO_WriteReg(ST7789H2_CASET);

    LCD_IO_WriteData(Xpos >> 8);                   /* XS[15:8] */
    LCD_IO_WriteData((uint8_t)Xpos);               /* XS[7:0] */
    LCD_IO_WriteData((Xpos + Xwidth - 1) >> 8);        /* XE[15:8] */
    LCD_IO_WriteData((uint8_t)(Xpos + Xwidth - 1));    /* XE[7:0] */


    // set pixel y pos:
    LCD_IO_WriteReg(ST7789H2_RASET);

    LCD_IO_WriteData(Ypos >> 8);                 /* YS[15:8] */
    LCD_IO_WriteData((uint8_t)Ypos);             /* YS[7:0] */
    LCD_IO_WriteData((Ypos + Ywidth - 1) >> 8);        /* YE[15:8] */
    LCD_IO_WriteData((uint8_t)(Ypos + Ywidth - 1));    /* YE[7:0] */
}


/* USER CODE END TouchGFXHAL.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
