#include "MensajesMan.h"

void configSerial (void) {
	Serial.begin(115200);
}


void PrintInfo(void){
	ResetWatchDogTimer();

	DEBUGprintln("UDPSendReceiveString Multicast setup");
	ResetWatchDogTimer();
	DEBUGprint("listening on ");
	ResetWatchDogTimer();
	#ifdef DEBUG_MODE 
	ipMulti.printTo(Serial);
	#endif
	ResetWatchDogTimer();
	DEBUGprint(":");
	ResetWatchDogTimer();
	DEBUGprintln(portMulti);
  	ResetWatchDogTimer();
	#ifdef CAMBIO_POR_ENERGIA
		DEBUGprintln("Test by ENERGIA");
	#else
		DEBUGprintln("Test by MACRO");
	#endif
	ResetWatchDogTimer();
}


void SerialPrint(char message[]){
	DisableWatchDog();
	Serial.print(message);
	EnableWatchDog();
}
void SerialPrint(int n){
	DisableWatchDog();
	Serial.print(n);
	EnableWatchDog();
}
void SerialPrint(char n){
	DisableWatchDog();
	Serial.print(n);
	EnableWatchDog();
}

void SerialPrint(const char message[]){
	DisableWatchDog();
	Serial.print(message);
	EnableWatchDog();
}
void SerialPrintln(int n){
	DisableWatchDog();
	Serial.println(n);
	EnableWatchDog();
}
void SerialPrintln(char n){
	DisableWatchDog();
	Serial.println(n);
	EnableWatchDog();
}

void SerialPrintln(char message[]){
	DisableWatchDog();
	Serial.println(message);
	EnableWatchDog();
}
void SerialPrintln(const char message[]){
	DisableWatchDog();
	Serial.println(message);
	EnableWatchDog();
}

void SerialPrintln(const Printable& message){
	DisableWatchDog();
	Serial.println(message);
	EnableWatchDog();
}

void SerialPrint(const Printable& message){
	DisableWatchDog();
	Serial.print(message);
	EnableWatchDog();
}


void ShowModuleInformationLCD(){
	ClearLCD();

	IPAddress ip = Ethernet.localIP();
	IPAddress gw = Ethernet.gatewayIP();
	IPAddress nmask = Ethernet.subnetMask();

	char cad[2048];

	sprintf(cad,"IP: %d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
	PrintStr(0,0,cad); //Printing the message to the LCD

	sprintf(cad,"Gw:%d.%d.%d.%d",gw[0],gw[1],gw[2],gw[3]);
	PrintStr(0,1,cad); //Printing the message to the LCD

	sprintf(cad,"Nm:%d.%d.%d.%d",nmask[0],nmask[1],nmask[2],nmask[3]);
	PrintStr(0,2,cad); //Printing the message to the LCD

	sprintf(cad,"Beam %d",GetActualBeam()-EEPROM_OFFSET);
	PrintStr(0,4,cad); //Printing the message to the LCD


	char aux[7];
	FromDec2BinStr(aux,Read_EEPROM(GetActualBeam()));//Function from ProcesaMan library
	sprintf(cad,"Beam:  %s",aux);
	PrintStr(0,5,cad); //Printing the message to the LCD



  	int i=0;
  	for (i=0;i<1000;i++){
    	delayMicroseconds(500);
    	ResetWatchDogTimer();
  	}

	PrintSquare(76,5);

	for (i=0;i<1000;i++){
    	delayMicroseconds(500);
    	ResetWatchDogTimer();
  	}
}
