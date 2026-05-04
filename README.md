# Create and debug TouchGFX application with in Secure and Non-Secure project setup

Prerequisites:
- STM32CubeMX(1)
- STM32CubeIDE
- STM32H573I-DK

In this repository you fill find complete working project tested on STM32H573I-DK board.

## Configure the Option Bytes

At first, it is needed to prepare STM32H5 device and configure its ***Option Bytes*** to be able to run secure and non-secure application.

1) Connect the board to the PC.
2) Open STM32CubeProgrammer GUI.
3) Connect to the STM32H573.
4) Activate TZ (TrustZone®) ***TZEN*** == B4 and ***Flash Water Mark*** for Flash ***Bank 1*** (Secure) and ***Bank2*** (Non-Secure)*

(*) keeping the _STRT value greater than _END value means sector disabled. See the picture bellow and Bank2 configuration.

![](imgs/programmer.png)

## Create a new project using STM32CubeMX

![](imgs/ICacheMPUYes.png)

![](imgs/TZYes.png)

Configure PI1 pin as GPIO output push/pull and add User Label "GREEN_LED":

![](imgs/PinMX.png)

Give the project some name and don't forgot to **uncheck** "Generate Under Root" (this is required by TouchGFX project which will be added later. TouchGFX generator needs to touch .cprojet file and if generated under root, TouchGFX will not find that project file):

![](imgs/UnderRoot.png)

## Open the project in STM32CubeIDE

Add these lines in Non-Secure main.c which make the green LED blink:

```cpp
int main(void)
{
...
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	  HAL_Delay(250);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
```

### Setup Debug

![](imgs/DebugSetup.png)

The application boots in a secure state when ***TrustZone®*** is enabled. The debugger sets the ***Program Counter*** using information from the last image in the ***Load image*** and ***Symbols*** table. Make sure the **secure** image is **at the bottom** of the load list.

![](imgs/SecureBottom.png)

When you run debug next time, be sure to select Secure project when launching debug.

![](imgs/Launch.png)

If you run the application you should see the LD6 blinking. This is to verify we have working Secure and Non-Secure application setup and we are able to debug it.

## Add TouchGFX SW packgage X-CUBE-TouchGFX

Go to ***Software Packs*** drop down menu and click ***Select Components*** option.

![](imgs/OpenSWPacks.png)

In the ***Software Packs Component Selector*** be sure to select ***Cortex-M33 non secure*** context because the TouchGFX can run only in non secure context.

![](imgs/SWPacksTGFX.png)

Then we need to activate TouchGFX middleware.

![](imgs/EnableTGFX.png)

Solve the Dependencies error. The TouchGFX needs to have available CRC peripheral to proper function. You don't need to configure the CRC peripheral, just activate it using check box.

![](imgs/ActivateCRC.png)

Activate the FMC

The display on STM32H573I-DK board is connected with 16-bit data bus. To interface this display (equipped with Sitronix ST7789H2 controller) we can use FMC peripheral with LCD mode.

Configure the FMC according to the pictures bellow:

![](imgs/MX_FMC_1.png)

Adjust pin assignment according the picture bellow (some of the dedicated pins would need to be moved to alternate position). All the pins, as FMC peripheral itself, belongs to the non-secure world:

![](imgs/MX_FMC_pins.png)

Adjust TouchGFX configuration in CubeMX

Once we have activated the FMC controller, we can adjust configuration in the CubeMX for TouchGFX middleware to use FMC.

![](imgs/CubeMX_TGFX_FMC.png)

If you generate by CubeMX the project now (just enabling TouchGFX and CRC) and then try to build the project, you will receive an errors, because the project is not complete.

![](imgs/Errors.png)

We must generate project in the TouchGFX Designer to have complete project.

1) Open the ***.touchgfx.part*** partial project file located in /NonSecure/TouchGFX/:

![](imgs/part.png)

2) This will open the project in the ***TouchGFX Designer***. There you can select ***blank UI*** for initial project and click on ***Import*** button.

![](imgs/TouchGFXDesigner-BlankUI.png)

3) Add some basic shapes on the screen (canvas) to see later at least something on the display. You can put a white rectangle and spread it across all the screen to create a white background and then place somewhere a circle of any color.

![](imgs/TouchGFXDesigner_canvas.png)

4) Now just click Generate button (or press F4) to generate TouchGFX project.

![](imgs/TouchGFXDesignergenerate.png)

5) You should see green label at the bottom status bar informing about successfull generation.

![](imgs/TouchGFXDesigner-success.png)

At this point the TouchGFX application will not be showing anything on the display and would require some more adjustments.

## GPIOs

We need to add some more GPIOs to handle display correctly. Configure the GPIO pins with the given **User Labels**.

1) The pin enabling power to display **PC6** (with label "**LCD_DISP**"). Default GPIO output level **Low** means the display will have a power just after initialization of this pin.

2) Input pin catching tearing effect (TE) signal from the display **PD3** (put there label "**LCD_TE**"). We need to enable **GPIO_EXTI3** on this pin to catch interrupts from the display TE pin.
![](imgs/EXTI3.png)

3) The pin controlling the display reset line **PH13** (with label "**LCD_RESET**").

4) The pin controlling the backlight **PI3** (with label "**LCD_BL_CTRL**"). Nothing special here, we just configure the pin with the **GPIO output level** to be **High** which means that after the pin initialization the backlight will be on.

See the summary bellow:
![](imgs/GPIOs.png)

## Adjust avaliable heap and stack size

TouchGFX application would require more RAM memory than default values for heap and stack size. Enlarge stack and heap size for non-secure application in CubeMX Project Manager.

![](imgs/MX_heapStack.png)

Be aware that if you modify the linker file manually in the linker file then it will be re-generated (reverted) to default values when you re-generate the project in the CubeMX.

## MPCWM settings in GTZC (Global TZ Controller)

By default all externall memories address ranges are allocated for secure world. The FMC Bank1 address space will not be working until we enable the access for non-secure application. To enable it we need to configure MPCWM (Memory Protection Controller - Watermark Based) in CubeMX under GTZC_S section.

![](imgs/MPCWM.png)

## GPDMA settings

You need to change the GPDMA channel security attribute to "Priviledged"

![](imgs/GPDMA_priviledged.png)

---
By default (default CubeMX settings) every SRAM region is secure and there is allocated unneccessary amount of SRAM for secure application and only part of available SRAM for non-secure application. The allocated SRAM for secure application can be reduced allowing more SRAM to be allocated to non-secure application (by adjusting linker file and Block-Based Memory Protection Contrloller tab in MX in GTZC_S).