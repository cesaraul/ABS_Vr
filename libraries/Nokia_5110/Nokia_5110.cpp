#include "Nokia_5110.h"



void ConfigSSIInterface(){
	ResetWatchDogTimer();
    // enable portA and SSI0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    // Configure SSI pins
    ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);    // clk pa2
    ROM_GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);  // tx, pa4
    ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4);

    // Configure DC, CE and RST pins
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);  //dc, pa6
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);//CE
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);//RST

    // Disable SSI0
    ROM_SSIDisable(SSI0_BASE);

    // Configure SSI0
    ROM_SSIConfigSetExpClk(SSI0_BASE, F_CPU, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);

    // Enable SSI0
    ROM_SSIEnable(SSI0_BASE);

    // wait on any prior transaction
    while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}
}

//Set the cursor in the position given, 0<=x<=83, 0<=y<=5
void SetCursor(uint8_t x, uint8_t y){
	ResetWatchDogTimer();
	//To avoid undefined behavior in the LCD
	if (x>83)
		x = 83;
	if (y>5)
		y = 5;

    ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
    ROM_SysCtlDelay(10);
    ROM_SSIDataPut(SSI0_BASE, 0x80|x);
    ROM_SSIDataPut(SSI0_BASE, 0x40|y);

	while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}
}

void ClearLCD(){
	ResetWatchDogTimer();
	SetCursor(0,0);
	ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
	ROM_SysCtlDelay(10);

	int i = 0;
    for(i=0;i<84*6;i++){
		ResetWatchDogTimer();
    	ROM_SSIDataPut(SSI0_BASE,0);
    }
	while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}
}
void ShowImage(const uint8_t image[]){
	ResetWatchDogTimer();
	int i = 0;

    ClearLCD();
	SetCursor(0,0);

	ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
	ROM_SysCtlDelay(10);
    for(i=0;i<84*6;i++){
		ResetWatchDogTimer();
        ROM_SSIDataPut(SSI0_BASE,image[i]);
    }
	while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}	
}
void EnableSlave(){
	ResetWatchDogTimer();
    //Enabling communication to the LCD (C/E)
    ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
    // pin setup time
    ROM_SysCtlDelay(10);
}
void ResetLCD(){
	ResetWatchDogTimer();
	//Reseting the LCD
    ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5);
    ROM_SysCtlDelay(100);
    ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0);
    ROM_SysCtlDelay(100);
    ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5);
}

void InitLCD(){
	ResetWatchDogTimer();
    // set DC pin for commands (low)
    ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
	ROM_SysCtlDelay(10);
    // LCD INIT
    ROM_SSIDataPut(SSI0_BASE, 0x21);// chip active
    ROM_SSIDataPut(SSI0_BASE, 0x14);// LCD bias mode 1:48
    ROM_SSIDataPut(SSI0_BASE, 0xBF);// 0xB8 (for 3.3V blue SparkFun)
    
    ROM_SSIDataPut(SSI0_BASE, 0x20);// send 0x20 before modifying the display control mode
    ROM_SSIDataPut(SSI0_BASE, 0x0C);// set display control to normal mode
    
	// wait on any prior transaction
    while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}

	ClearLCD();//Clear the LCD
}

void PrintChar(uint8_t x, uint8_t y, char character){
	ResetWatchDogTimer();
	int i=0;
	SetCursor(x,y);

	ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
	ROM_SysCtlDelay(10);

    for(i=0;i<5;i++){
		ResetWatchDogTimer();
        ROM_SSIDataPut(SSI0_BASE,ascii[character-' '][i]);
    }

	while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}	
}

//Prints out a string to the Nokia LCD, fitting the chars in to the LCD, should take in account that the length of string should be
//less or equal than 16*6=96
//There shouldn't be non-printable characters in string
void PrintStr(int x, int y, char string []){
	ResetWatchDogTimer();
	int l = strlen(string);
	int i = 0;

	x = x - x%5;//Puts the cursor in a allowed position to avoid missaligning of the printed char

	SetCursor(x,y);
	ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
	ROM_SysCtlDelay(10);

	char character;
	int j = 0;
    for(i=0;i<l;i++){
		ResetWatchDogTimer();
		if ((x+5*i)>=80){
			x -= 80;
			j ++;
		}

		if ((y+j)>=6){
			j -= 6;
		}		

		character = string[i];
        PrintChar(x+5*i,y+j,character);
    }

	while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}	
}

//Print a square 8x8 pixels
void PrintSquare(uint8_t x, uint8_t y){
	ResetWatchDogTimer();
	uint8_t circle[8] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	int i=0;

	SetCursor(x,y);
	ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
	ROM_SysCtlDelay(10);


    for(i=0;i<8;i++){
		ResetWatchDogTimer();
        ROM_SSIDataPut(SSI0_BASE,circle[i]);
    }

	while(ROM_SSIBusy(SSI0_BASE)){ResetWatchDogTimer();}
}
