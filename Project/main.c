#include <stdio.h>
#include <string.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "uart.h"
#include <math.h>
#include "GLCD.h"

unsigned int val_Tx = 0, val_Rx = 0;            /* Globals used for display */
extern volatile uint32_t UART0_Count;
extern volatile uint8_t UART0_Buffer[BUFSIZE];
extern volatile uint32_t UART2_Count;
extern volatile uint8_t UART2_Buffer[BUFSIZE];
int stepsByHour[24];
int maxNumSteps;

char * sendData(char * command, const int timeout);
void delay( uint32_t del);
void initializeESP(void);
void insertDummyDataIntoArray(void);
void insertDataIntoArray(char * stepData);
void drawBargraph(void);

int main(){
	char string[BUFSIZE] = {0};
	SystemInit();
	UARTInit(0, 9600);//putty serial terminal
	UARTInit(2, 115200);//esp8266
  LCD_Initialization(); // Initialize graphical LCD 
	LCD_Clear(White);
	
  // (2) Timer 0 configuration;
  LPC_SC->PCONP |= 1 << 1; // Power up Timer 0 
  LPC_SC->PCLKSEL0 |= 1 << 2; // Clock for timer = CCLK, i.e., CPU Clock
  LPC_TIM0->MR0 = 1 << 24; // 24: give a value suitable for the LED blinking 
                           // frequency based on the clock frequency 
  LPC_TIM0->MCR |= 1 << 0; // Interrupt on Match 0 compare 
  LPC_TIM0->MCR |= 1 << 1; // Reset timer on Match 0    
  LPC_TIM0->TCR |= 1 << 1; // Manually Reset Timer 0 (forced) 
  LPC_TIM0->TCR &= ~(1 << 1); // Stop resetting the timer 
	
	initializeESP();
	UARTSend(0, (uint8_t *)"Server on ESP8266 module ready!\r\n", 34);
	
	insertDummyDataIntoArray();
	drawBargraph();

  NVIC_EnableIRQ(TIMER0_IRQn);
  LPC_TIM0->TCR |= 1 << 0; // Start timer
	
	//wait for the web application to send data to the module
	while(1){	
		if(UART2_Count != 0){
			delay(2000000);
			LPC_UART2->IER = IER_THRE | IER_RLS; // Disable RBR 
			strncpy(string, (char *)UART2_Buffer, UART2_Count);
			insertDataIntoArray(string);
			UARTSend( 0, (uint8_t *)UART2_Buffer, UART2_Count );
			UART2_Count = 0;
			LPC_UART2->IER = IER_THRE | IER_RLS | IER_RBR; // Re-enable RBR 
		}
	}	
}

void initializeESP(){
	sendData("AT+CWMODE=2\r\n",200000);
	sendData("AT+CIFSR\r\n",200000);
	sendData("AT+CIPMUX=1\r\n",200000);
	sendData("AT+CIPSERVER=1,80\r\n",200000);
}

char * sendData(char * command, const int timeout){
	char * response;
	
	UARTSend(2, (unsigned char *)command, strlen(command));
	if(timeout == 0){
		delay(2000000);
		UART2_Count = 0;
		return 0;
	}
	while(UART2_Count == 0){};
		
	delay(2000000);
		
	LPC_UART2->IER = IER_THRE | IER_RLS; // Disable RBR 
	UARTSend( 0, (uint8_t *)UART2_Buffer, UART2_Count );
	UART2_Count = 0;
	LPC_UART2->IER = IER_THRE | IER_RLS | IER_RBR; // Re-enable RBR 

	UARTSend(0, (uint8_t *)"\r\n", 2);	
	return response;
}

volatile uint32_t temp;
void delay( uint32_t del)
{
  uint32_t i;
  for ( i=0; i<del; i++)
    temp = i;
}

void insertDataIntoArray(char * stepData){
	char *dummy;
	int time, steps;
	dummy = strstr(stepData, "time: ");
	time = ((dummy[7] - '0') * 10) + ((dummy[8] - '0') * 10);
	dummy = strstr(stepData, "value: ");
	steps = dummy[7] - '0';//todo: update this so that it can read in multiple digit numbers
	
	stepsByHour[time] = steps;
	if(steps > maxNumSteps)
		maxNumSteps = steps;
}

void insertDummyDataIntoArray(void){
	//insert dummy data into the array to test the display
	stepsByHour[0] = 200; stepsByHour[1] = 10; stepsByHour[2] = 99; stepsByHour[3] = 580; 
	stepsByHour[4] = 200; stepsByHour[5] = 1000; stepsByHour[6] = 99; stepsByHour[7] = 580;
	stepsByHour[8] = 200; stepsByHour[9] = 1000; stepsByHour[10] = 99; stepsByHour[11] = 580;
	stepsByHour[12] = 200; stepsByHour[13] = 1000; stepsByHour[14] = 99; stepsByHour[15] = 580;
	stepsByHour[16] = 200; stepsByHour[17] = 1000; stepsByHour[18] = 99; stepsByHour[19] = 580;
	stepsByHour[20] = 200; stepsByHour[21] = 1000; stepsByHour[22] = 99; stepsByHour[23] = 580;
	
	maxNumSteps = 1000;
}

void drawBargraph(){
	int i, j, y;
	int numSteps;
	uint16_t color;
	int width = 13;
	
	//draw the bar graph every time to make sure that data hasn't been updated since last time.
	for(i = 0; i < 24; i ++){
		numSteps = stepsByHour[i];
		if(numSteps >= 1000)
			color = Green;
		else if(numSteps >= 500)
			color = Yellow;
		else
			color = Red;
		numSteps = numSteps * 240/(maxNumSteps);
		for(j = 0; j < width; j ++){
			y = (i * width) + j;
			LCD_DrawLine(0, y, numSteps, y, color);
		}
	}
}

// Here, we describe what should be done when the interrupt on Timer 0 is handled;
// We do that by writing this function, whose address is “recorded” in the vector table
// from file startup_LPC17xx.s under the name TIMER0_IRQHandler;
void TIMER0_IRQHandler(void)
{
  if ( (LPC_TIM0->IR & 0x01) == 0x01 ) // if MR0 interrupt (this is a sanity check);
  {
    LPC_TIM0->IR |= 1 << 0; // Clear MR0 interrupt flag
		drawBargraph();
  }
}
