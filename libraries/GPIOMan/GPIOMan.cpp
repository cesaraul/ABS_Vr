#include "GPIOMan.h"

void configPins (void){

  //Outputs
  pinMode(CTRL_UP0,OUTPUT);
  pinMode(CTRL_UP1,OUTPUT);
  pinMode(CTRL_UP2,OUTPUT);
  pinMode(CTRL_DW0,OUTPUT);
  pinMode(CTRL_DW1,OUTPUT);
  pinMode(CTRL_DW2,OUTPUT);
  
  //Inputs
  pinMode(MON_UP0,INPUT_PULLDOWN);
  pinMode(MON_UP1,INPUT_PULLDOWN);
  pinMode(MON_UP2,INPUT_PULLDOWN);
  pinMode(MON_DW0,INPUT_PULLDOWN);
  pinMode(MON_DW1,INPUT_PULLDOWN);
  pinMode(MON_DW2,INPUT_PULLDOWN);
}



void ConfigGPIOInterrupts(){
	DisableGPIOInterrupts();
	ROM_SysCtlDelay(10);//Setting a delay of 30 clock cycles to let the GPIO PINs be on an stable state

	//Note: Only the ports P and Q can have individual interrupts per pin
	//Configuring the GPIO interrupts on the used pins of the port E, PE4 and PE5
	ROM_GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5,GPIO_BOTH_EDGES);

	//Configuring the GPIO interrupts on the used pins of the port C, PC4, PC5 and PC6
	ROM_GPIOIntTypeSet(GPIO_PORTC_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6,GPIO_BOTH_EDGES);

	//Configuring the GPIO interrupts on the used pins of the port D, PD3
	ROM_GPIOIntTypeSet(GPIO_PORTD_BASE,GPIO_INT_PIN_3,GPIO_BOTH_EDGES);

	//Register the GPIOIntHandler function as a ISR for port E, C and D
	IntRegister(INT_GPIOE_TM4C129 , GPIOIntHandler);
	IntRegister(INT_GPIOC_TM4C129 , GPIOIntHandler);
	IntRegister(INT_GPIOD_TM4C129 , GPIOIntHandler);	

}

void EnableGPIOInterrupts(){
	ROM_IntEnable(INT_GPIOE_TM4C129);
	ROM_IntEnable(INT_GPIOC_TM4C129);
	ROM_IntEnable(INT_GPIOD_TM4C129);
	//Enabling the GPIO interrupts on the used pins of the port E, PE4 and PE5
	ROM_GPIOIntEnable(GPIO_PORTE_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5);

	//Enabling the GPIO interrupts on the used pins of the port C, PC4, PC5 and PC6
	ROM_GPIOIntEnable(GPIO_PORTC_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6);

	//Enabling the GPIO interrupts on the used pins of the port D, PD3
	ROM_GPIOIntEnable(GPIO_PORTD_BASE,GPIO_INT_PIN_3);
}

void DisableGPIOInterrupts(){

	//Disabling the GPIO interrupts on the used pins of the port E, PE4 and PE5
	ROM_GPIOIntDisable(GPIO_PORTE_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5);

	//Disabling the GPIO interrupts on the used pins of the port C, PC4, PC5 and PC6
	ROM_GPIOIntDisable(GPIO_PORTC_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6);

	//Disabling the GPIO interrupts on the used pins of the port D, PD3
	ROM_GPIOIntDisable(GPIO_PORTD_BASE,GPIO_INT_PIN_3);

	ROM_IntDisable(INT_GPIOE_TM4C129);
	ROM_IntDisable(INT_GPIOC_TM4C129);
	ROM_IntDisable(INT_GPIOD_TM4C129);
}

volatile uint32_t statusE = 0;
volatile uint32_t statusC = 0;
volatile uint32_t statusD = 0;
//Handler of the interrupts for the GPIO
void GPIOIntHandler(void){
	if (statusE==0)
		statusE = ROM_GPIOIntStatus(GPIO_PORTE_BASE,true);
	if (statusC==0)
		statusC = ROM_GPIOIntStatus(GPIO_PORTC_BASE,true);
	if (statusD==0)
		statusD = ROM_GPIOIntStatus(GPIO_PORTD_BASE,true);

	ROM_GPIOIntClear(GPIO_PORTE_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5);
	ROM_GPIOIntClear(GPIO_PORTC_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6);
	ROM_GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_3);
}
void SendGPIOInformation(){	
	uint8_t current_beam = Read_EEPROM(GetActualBeam());
	//Serial.println(current_beam);
	char buffer[2048]="";
	
	//It's better to use else if because more than one interrupt can be activated at the same time, and the handler is going to
	//be called for each interrupt flag.
	if (statusE!=0){
		//Clearing the GPIO interrupts on the used pins of the port E, PE4 and PE5
		
		strcpy(buffer,"Damaged GPIO_E. PINS:");
		if (digitalRead(MON_UP1)==((current_beam&0x10)>>4)){
			strcat(buffer," PE_5");
		}		
		if (digitalRead(MON_DW0)==(current_beam&0x01)){
			strcat(buffer," PE_4");
		}	
		strcat(buffer,".");

		PostIssueMessage(buffer);
		DEBUGprintln(buffer);
	}
	if(statusC!=0){
		//Clearing the GPIO interrupts on the used pins of the port C, PC4, PC5 and PC6	
		strcpy(buffer,"Damaged GPIO_C. PINS:");
		if (digitalRead(MON_UP2)==((current_beam&0x20)>>5)){
			strcat(buffer," PC_6");
		}		
		if (digitalRead(MON_DW1)==((current_beam&0x02)>>1)){
			strcat(buffer," PC_4");
		}	
		if (digitalRead(MON_DW2)==((current_beam&0x04)>>2)){
			strcat(buffer," PC_5");
		}
		strcat(buffer,".");

		PostIssueMessage(buffer);
		DEBUGprintln(buffer);
	}
	if(statusD!=0){
		//Clearing the GPIO interrupts on the used pins of the port D, PD3
		strcpy(buffer,"Damaged GPIO_D. PINS: PD_3.");

		PostIssueMessage(buffer);
		DEBUGprintln(buffer);
	}
	else{
	//Nothing to do here
		//DEBUGprintln("No hay GPIO daniado");
	}
	statusE = 0;
	statusC = 0;
	statusD = 0;
}

void SendGPIOInformation2(){
	
	uint8_t show = Read_EEPROM(6143);
	if(show==1){

	
		uint8_t current_beam = Read_EEPROM(GetActualBeam());
		//Serial.println(current_beam);
		char buffer[2048]="";
	
		//It's better to use else if because more than one interrupt can be activated at the same time, and the handler is going to
		//be called for each interrupt flag.

		//Clearing the GPIO interrupts on the used pins of the port E, PE4 and PE5
	
		strcpy(buffer,"Damaged GPIO_E. PINS:");
		if (digitalRead(MON_UP1)==((current_beam&0x10)>>4)){
			strcat(buffer," PE_5");
		}		
		if (digitalRead(MON_DW0)==(current_beam&0x01)){
			strcat(buffer," PE_4");
		}	
		strcat(buffer,".");

		PostIssueMessage(buffer);
		DEBUGprintln(buffer);

		//Clearing the GPIO interrupts on the used pins of the port C, PC4, PC5 and PC6	
		strcpy(buffer,"Damaged GPIO_C. PINS:");
		if (digitalRead(MON_UP2)==((current_beam&0x20)>>5)){
			strcat(buffer," PC_6");
		}		
		if (digitalRead(MON_DW1)==((current_beam&0x02)>>1)){
			strcat(buffer," PC_4");
		}	
		if (digitalRead(MON_DW2)==((current_beam&0x04)>>2)){
			strcat(buffer," PC_5");
		}
		strcat(buffer,".");

		PostIssueMessage(buffer);
		DEBUGprintln(buffer);

		//Clearing the GPIO interrupts on the used pins of the port D, PD3
		strcpy(buffer,"Damaged GPIO_D. PINS:");

		if (digitalRead(MON_UP0)==((current_beam&0x08)>>3)){
			strcat(buffer," PD_3");
		}
		strcat(buffer,".");
		PostIssueMessage(buffer);
		DEBUGprintln(buffer);

		Write_EEPROM(6143,0);
	}
	
}	

