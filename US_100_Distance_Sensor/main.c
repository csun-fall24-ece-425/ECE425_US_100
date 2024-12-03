/*
 * @file main.c
 *
 * @brief Main source code for the US_100_UART program.
 *
 * This file contains the main entry point and function definitions for the US_100_UART program.
 * 
 * The following modules are used:
 *  - UART0: Used to print the measured distance from the US-100 on the serial terminal
 *  - UART1: Used to establish communication between the Tiva TM4C123G LaunchPad
 *           and a US-100 Ultrasonic Distance Sensor
 *
 * It interfaces with the following:
 *  - US-100 Ultrasonic Distance Sensor (UART)
 *
 * The US-100 Ultrasonic Distance Sensor uses the following pinout:
 *  - US-100 Pin 1 (VCC)  <-->  Tiva LaunchPad 3.3V
 *  - US-100 Pin 2 (TX)   <-->  Tiva LaunchPad Pin PB1 (U1TX)
 *  - US-100 Pin 3 (RX)   <-->  Tiva LaunchPad Pin PB0 (U1RX)
 *  - US-100 Pin 4 (GND)  <-->  Tiva LaunchPad GND
 *  - US-100 Pin 5 (GND)  <-->  Tiva LaunchPad GND
 *
 * @note For more information regarding the UART module, refer to the
 * Universal Asynchronous Receivers / Transmitters (UARTs) section
 * of the TM4C123GH6PM Microcontroller Datasheet.
 * Link: https://www.ti.com/lit/gpn/TM4C123GH6PM
 *
 * @author Aaron Nanas
 */
 
#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "UART0.h"
#include "UART1.h"

#define BUFFER_SIZE   64
#define READ_DISTANCE 0x55

int main(void)
{		
	// Initialize the SysTick timer used to provide blocking delay functions
	SysTick_Delay_Init();
	
	// Initialize an array to store the measured distance values from the US-100 Ultrasonic Distance Sensor
	char US_100_UART_Buffer[BUFFER_SIZE] = {0};

	// Initialize the UART0 module which will be used to print characters on the serial terminal
	UART0_Init();
	
	// Initialize the UART1 module which will be used to communicate with the US-100 Ultrasonic Distance Sensor
	UART1_Init();
	
	while(1)
	{		
		UART1_Output_Character(READ_DISTANCE);
		
		US_100_UART_Buffer[0] = UART1_Input_Character();
		US_100_UART_Buffer[1] = UART1_Input_Character();
		
		uint16_t distance_value = (US_100_UART_Buffer[1] | (US_100_UART_Buffer[0] << 8));
		
		UART0_Output_String("Distance (mm): ");
		UART0_Output_Unsigned_Decimal(distance_value);
		UART0_Output_Newline();
		
		SysTick_Delay1ms(100);
	}
}
