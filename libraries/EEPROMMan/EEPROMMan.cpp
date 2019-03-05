#include "EEPROMMan.h"

//Function to initialize the EEPROM and configure it, should be called in the setup
void Conf_EEPROM (void){

	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);  //#include <driverlib/sysctl.h> also necessary
	if(ROM_EEPROMInit()==EEPROM_INIT_ERROR){
		//Generate EEPROM CORRUPTED POST to let the admin know that the EEPROM should be written again
		PostIssueMessage("CORRUPT_EEPROM");
	}
	
	ROM_EEPROMBlockProtectSet(0,EEPROM_PROT_RW_LRO_URW);//Setting the blocking mode to the whole EEPROM

	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockPasswordSet(0,password,3); //Setting the password for the whole EEPROM
	ROM_EEPROMBlockLock(0);
}


//Function to write the EEPROM with the values of the beams sent in the current package
void Writting_beams(char* beams, uint8_t id){

	uint16_t nbeams=0;
	uint16_t aux=0;


	nbeams = strlen(beams);//It's one character per beam
	DEBUGprint("Escribiendo la eeprom");

	//It starts to write in address two because addresses  one and zero are the last changed beam 
	for(aux=0;aux<nbeams;aux++){
		char beam;
		beam = *(beams+aux)-33;//This is shifted by two to get the first character pointed by beams saved in the address 2 of EEPROM
		Write_EEPROM(EEPROM_OFFSET+id*20+aux,beam);//Writes in the position EEPROM_OFFSET(beam_initial, nbeastot, flags_beams)plus 														//the offset corresponding to the id of the package
		
	}
  
}

//Function to read a beam and set the corresponding control pins to the respective values, it's used to change between beams
void Read_Beam(uint16_t position){
	uint8_t beam;
	char buf[7];
	DEBUGprint("Selected beam: ");
	ResetWatchDogTimer();
	beam = Read_EEPROM(position);
	ResetWatchDogTimer();
	DEBUGprintln(beam);
	ResetWatchDogTimer();
	DEBUGprint("Nro of the selected beam ");
	DEBUGprintln(position-EEPROM_OFFSET);
  	ResetWatchDogTimer();
	if(beam < 64) ToBinary(beam,buf);
	else strcpy(buf,"NOBEAM");
	ResetWatchDogTimer();
	DEBUGprint("Extracted Buffer: ");
	ResetWatchDogTimer();
	DEBUGprintln(buf);
	ResetWatchDogTimer();
	DEBUGprint("UP2: ");
	ResetWatchDogTimer();
	DEBUGprintln(buf[0]);
	DEBUGprint("UP1: ");
	ResetWatchDogTimer();
	DEBUGprintln(buf[1]);
	ResetWatchDogTimer();
	DEBUGprint("UP0: ");
	ResetWatchDogTimer();
	DEBUGprintln(buf[2]);
	ResetWatchDogTimer();
	DEBUGprint("DW2: ");
	ResetWatchDogTimer();
	DEBUGprintln(buf[3]);
	ResetWatchDogTimer();
	DEBUGprint("DW1: ");
	ResetWatchDogTimer();
	DEBUGprintln(buf[4]);
	ResetWatchDogTimer();
	DEBUGprint("DW0: ");
	ResetWatchDogTimer();
	DEBUGprintln(buf[5]);

	if(strcmp(buf,"NOBEAM")!=0){
	  digitalWrite(PC_7,LOW);
	  delayMicroseconds(10);//Clearing the FFD_CLK for 10 microseconds. 
	  digitalWrite(CTRL_UP2,(buf[0]==1? HIGH:LOW));
	  digitalWrite(CTRL_UP1,(buf[1]==1? HIGH:LOW));
	  digitalWrite(CTRL_UP0,(buf[2]==1? HIGH:LOW));
	  digitalWrite(CTRL_DW2,(buf[3]==1? HIGH:LOW));
	  digitalWrite(CTRL_DW1,(buf[4]==1? HIGH:LOW));  
	  digitalWrite(CTRL_DW0,(buf[5]==1? HIGH:LOW));
	  
	  digitalWrite(PC_7,HIGH);//Setting the FFD_CLK for 10 microsend, here the rising edge should be detected by the FF-D chip
	  delayMicroseconds(10);//Clearing the FFD_CLK for 10 microseconds. 
	}


}

//The two functions below implement reading and writing from a password blocked EEPROM


uint8_t Read_EEPROM(uint16_t position){
	//DisableWatchDog();
	//Unlocks the EEPROM because blocking the whole EEPROM doesn't let reading its data
	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockUnlock (0, password,3);

	uint32_t byteAddr = position - (position % BYTES_PER_WORD);
	uint32_t wordVal = 0;
	ROM_EEPROMRead(&wordVal, byteAddr, 4);
	wordVal = wordVal >> (8*(position % BYTES_PER_WORD));
	
	//Blocking the EEPROM again
	ROM_EEPROMBlockLock(0);
	//EnableWatchDog();
	return (uint8_t) wordVal;
}

//Because writing to the eeprom takes too much time, the watchdog timer will be disabled to avoid undesired reset
void Write_EEPROM(uint32_t position, uint8_t value){

	DisableWatchDog();
	
	//Unlocks the EEPROM before a read and write operation
	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockUnlock(0, password,3);

	uint32_t byteAddr = position - (position % BYTES_PER_WORD);
	uint32_t wordVal = 0;
	
	ROM_EEPROMRead(&wordVal, byteAddr, 4);
	wordVal &= ~(0xFF << (8*(position % BYTES_PER_WORD)));
	wordVal += value << (8*(position % BYTES_PER_WORD));
	ROM_EEPROMProgram(&wordVal, byteAddr,4);

	//Blocking the EEPROM again to avoid from being corrupted by some extern problem
	ROM_EEPROMBlockLock(0);

	EnableWatchDog();
}

//Function to decompose a value into its respective bits, in this case only the first 6 bits are necessary
void ToBinary (uint8_t value, char buff[7]){
  buff[6] = '\0';
  buff[5] = (value>>0)%2;
  buff[4] = (value>>1)%2;
  buff[3] = (value>>2)%2;
  buff[2] = (value>>3)%2;
  buff[1] = (value>>4)%2;
  buff[0] = (value>>5)%2;
  
}

//Function used to save the total number of beams of the experiment in the bytes 2 and 3 of the EEPROM
void SaveNbeamsTotal(uint32_t nbeamstotal){
	DisableWatchDog();
	//Unlocking the EEPROM
	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockUnlock (0, password,3);

	//Save the nbeamstotal in the bytes 2 and 3 of the EEPROM
	uint32_t wordVal = 0;
	ROM_EEPROMRead(&wordVal, 0, 4);
	wordVal &= 0x0000FFFF;
	wordVal += nbeamstotal<<16;
	ROM_EEPROMProgram(&wordVal, 0, 4);
	
	//Blocking the EEPROM
	ROM_EEPROMBlockLock(0);
	EnableWatchDog();
}
//Function used to get the total number of beams of the experiment from the bytes 2 and 3 of the EEPROM
uint32_t GetNbeamsTotal(){
	//Unlocking the EEPROM
	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockUnlock (0, password,3);

	//Get the nbeamstotal in the bytes 2 and 3 of the EEPROM
	uint32_t wordVal = 0;
	ROM_EEPROMRead(&wordVal, 0, 4);
	wordVal = wordVal >> 16;
	
	//Blocking the EEPROM
	ROM_EEPROMBlockLock(0);

	return wordVal;
}

//Function used to get the actual beam of the module
uint32_t GetActualBeam(){
	//Unlocks the EEPROM because blocking the whole EEPROM doesn't let reading its data
	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockUnlock (0, password,3);

	uint32_t wordVal = 0;
	ROM_EEPROMRead(&wordVal,0, 4);
	wordVal = wordVal % (1<<16);//Now wordVal has the 16-bit saved value
	//Blocking the EEPROM
	ROM_EEPROMBlockLock(0);

	return wordVal;
}

//Function used to save the actual beam of the experiment, should be called always after a change of beam
void SaveActualBeam(uint32_t pos){
	DisableWatchDog();
	//Unlocking the EEPROM
	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockUnlock (0, password,3);

	//Save the last beam in the EEPROM in its first two bytes
	uint32_t wordVal = 0;
	ROM_EEPROMRead(&wordVal, 0, 4);
	wordVal &= 0xFFFF0000;
	wordVal += pos;//The last position will be two bytes in the EEPROM
	ROM_EEPROMProgram(&wordVal, 0, 4);
	
	//Blocking the EEPROM
	ROM_EEPROMBlockLock(0);
	EnableWatchDog();
}
