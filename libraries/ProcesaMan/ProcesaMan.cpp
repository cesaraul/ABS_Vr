#include "ProcesaMan.h"


char *cmd = NULL;
char *beam;
char *beamchars;

uint8_t ID_pack_multi = 0;
uint8_t nbeams_pack_multi = 0;
uint32_t nbeams_total = 0;
/*
This function process the petition and sends a response
 */


void Process_Petition(char *rx_buffer, char *tx_buffer, uint32_t packet_size){
	ResetWatchDogTimer();
	SplitFrame(rx_buffer);
	ResetWatchDogTimer();
    if (cmd == NULL){
    	DEBUGprintln("processingPetition: Incorrect message format");
        
    }
    else{
        if(strcmp(cmd,"SNDF") == 0){

			if ((nbeams_pack_multi*64+11)==packet_size){//11 is the nbytes corresponding to cmd, nbeams_tot,nbeams_pck and ID
				ResetWatchDogTimer();
		    	Writting_beams(beamchars,ID_pack_multi);
		    	ResetWatchDogTimer();
				if (ID_pack_multi==0){//If its the package n°0 then all the flags should be set to 0 before continue working
					ResetWatchDogTimer();
					InitializeFlags();
					ResetWatchDogTimer();
					SaveNbeamsTotal(nbeams_total);
					ResetWatchDogTimer();
				}
				ResetWatchDogTimer();
				SetBeamFlags(ID_pack_multi,nbeams_pack_multi);
				ResetWatchDogTimer();
		    	Change_Beam("0");
		    	ResetWatchDogTimer();
				free(beamchars);
			}
			else{
				DEBUGprintln("Corrupt beams package");
				PostIssueMessage("Corrupt_Package");
			}
			sprintf(tx_buffer,"1"); 
        }

        else if(strcmp(cmd,"CHGB") == 0){
			ResetWatchDogTimer();
			if (CheckFlag(atoi(beam))&&(GetNbeamsTotal()>atoi(beam))){
				ResetWatchDogTimer();
		    	Change_Beam(beam);
		    	ResetWatchDogTimer();
				free(beam);
			}
			else{
				//Generate POST of invalid access, access to a beam that hasn't been sent
				DEBUGprintln("Access to invalid beam");
				PostIssueMessage("Invalid_Beam");
			}
			sprintf(tx_buffer,"1"); 
        }

        else if(strcmp(cmd,"MNTR") == 0){
			DEBUGprint("The actual beam is: ");

			DEBUGprint(digitalRead(CTRL_UP2));        	
			DEBUGprint(digitalRead(CTRL_UP1));

			DEBUGprint(digitalRead(CTRL_UP0));


			DEBUGprint(digitalRead(CTRL_DW2));

			DEBUGprint(digitalRead(CTRL_DW1));

			DEBUGprint(digitalRead(CTRL_DW0));

			sprintf(tx_buffer,"1");    
        }
		
		else if(strcmp(cmd,"RBOT") == 0){
			ROM_SysCtlReset();
		}

		else if(strcmp(cmd,"GETB") == 0){
			uint32_t aux = GetActualBeam()-EEPROM_OFFSET;
			DEBUGprintln(aux);
			sprintf(tx_buffer,"1%d",aux);
		}

		else if(strcmp(cmd,"GETC") == 0){
			ResetWatchDogTimer();
			uint32_t aux = GetActualBeam();
			ResetWatchDogTimer();
			char beam_tx = 0;
			beam_tx = Read_EEPROM(aux)+33;//The char send corresponds to the encoded character that means the actual beam
			ResetWatchDogTimer();
			sprintf(tx_buffer,"1%c",beam_tx);//character plus 33
		}

        else{
        	DEBUGprintln("Processing_petition: Unknown command");
			PostIssueMessage("Wrong_CMD");
        }
		ResetWatchDogTimer();
    }
	free(cmd);
	

}

/*
This function splits a multicast package into the command and its respective parameters
*/
void SplitFrame(char *frame){
	char* aux = frame; //Aux pointer
	
	cmd = (char*)malloc(5);
	strncpy(cmd,aux,4);
	*(cmd + 4) = '\0';
	aux = aux +4;
	
	if (strlen(frame)>4){
		if (strcmp(cmd,"CHGB")==0){
			int len_tmp = strlen(aux);
			
			beam = (char*)malloc(len_tmp);
			strncpy(beam,aux,len_tmp);
			*(beam+len_tmp) = '\0';
		}
		else if (strcmp(cmd,"SNDF")==0){

			//Getting the total number of beams
			char* nbeams_total_str = (char*) malloc(4);//The number of beams can be from 001 to 999 
			strncpy(nbeams_total_str,aux,3);
			*(nbeams_total_str+3)='\0';
			aux = aux + 3;
			nbeams_total = atoi(nbeams_total_str);

			//Getting the size of the actual package
			char* nbeams_pck_str = (char*) malloc(3);//The size of a package is always 20 but the last package that is <=20
			strncpy(nbeams_pck_str,aux,2);
			*(nbeams_pck_str+2)='\0';
			aux = aux + 2;
			nbeams_pack_multi = (uint8_t)atoi(nbeams_pck_str);

			//Getting the ID of the current package
			char* ID_pack_str = (char*) malloc(3);//Because the number of beams can be from 001 to 999 and the package has a maximum 								//size of 20 beams then the ID is going to be from 0 to 49 that why we used 2 characters in the package
			strncpy(ID_pack_str,aux,2);		
			*(ID_pack_str+2)='\0';
			aux = aux + 2;
			ID_pack_multi = (uint8_t)atoi(ID_pack_str);
			
			//Getting the corresponding character to the written beams
			
			aux = aux+ nbeams_pack_multi*MODULE_NUM;//module_num is going to be a global constant in GpioMan.h
			beamchars = (char*) malloc(nbeams_pack_multi+1);
			strncpy(beamchars,aux,nbeams_pack_multi);//beamchars have now the corresponding characters for the corresponding module
			*(beamchars+nbeams_pack_multi)='\0';

			free(ID_pack_str);
			free(nbeams_pck_str);
			free(nbeams_total_str);
		}		
		else
			DEBUGprintln("Corrupt Package");
	}
}



/*
 This function receive a number in a char format, use it to read the respective EEPROM position and later change the control pins
 */
void Change_Beam(char *puntero_char){

	DEBUGprint("Searching beam in position: ");
	DEBUGprint(puntero_char);


	uint16_t orden = (uint16_t) strtol(puntero_char,NULL,10); 


	DEBUGprint(" - In uint32_t: ");
	DEBUGprintln(orden);

	DisableGPIOInterrupts(); //This is to avoid raising an interrupt when the pins toggle due to a change beam operation
	Read_Beam(orden+EEPROM_OFFSET);//Because of the initial beam, nbeamstotal and flags
  
	SaveActualBeam(orden+EEPROM_OFFSET);//Save the position of the last changed beam

	EnableGPIOInterrupts();
	if (!CheckCurrentBeam(orden+EEPROM_OFFSET)){
		//Generate POST of written beam are different to the saved beam
		DEBUGprintln("The written beam doesn't correspond to the received beam");
		//PostIssueMessage("MNTR_DIFFERENT_CONTROL");
		Write_EEPROM(6143,1);
	}

}

/*
This function saves the last beam in the EEPROM
*/
void Initial_Beam(){
	//Unlocks the EEPROM because blocking the whole EEPROM doesn't let reading its data
	uint32_t password[3] = {1,2,3};
	ROM_EEPROMBlockUnlock (0, password,3);

	ResetWatchDogTimer();
	uint32_t current_beam = GetActualBeam();

	DEBUGprintln("Loading the last pointed beam: ");
	Read_Beam(current_beam);
	EnableGPIOInterrupts();
	ResetWatchDogTimer();
	//Checking if the current beam is written on GPIOs correctly
	if (!CheckCurrentBeam(current_beam)){
		ResetWatchDogTimer();
		//Generate POST of written beam are different to the saved beam
		DEBUGprintln("The written beam doesn't correspond to the received beam");
		ResetWatchDogTimer();
		//PostIssueMessage("MNTR_DIFFERENT_CONTROL");
		Write_EEPROM(6143,1);
	}

}

/*
This function set the flags of valid data of the beams in the 4-128 position of the EEPROM using one bit per beam
*/
void SetBeamFlags(uint8_t id, uint8_t beams_package){
	uint32_t pos = 0;
	uint8_t aux = 0;
	uint32_t offset = 0;
	
	if((id % 2) == 1){
		offset = 4 + (id>>1)*5 + 2+2000;

		if(beams_package<=4){
			aux = Read_EEPROM(offset);
			aux |= ((1<<beams_package)-1)<<4;
			Write_EEPROM(offset,aux);
			
		}

		else if(beams_package<=12){
			aux = Read_EEPROM(offset);
			aux |= 0xF0;
			Write_EEPROM(offset,aux);

			aux = Read_EEPROM(offset+1);
			aux |= (1<<(beams_package-4))-1;
			Write_EEPROM(offset+1,aux);
		}

		else if(beams_package<=20){
			aux = Read_EEPROM(offset);
			aux |= 0xF0;
			Write_EEPROM(offset,aux);

			Write_EEPROM(offset+1,0xFF);

			aux = Read_EEPROM(offset+2);
			aux |= (1<<(beams_package-12))-1;
			Write_EEPROM(offset+2,aux);
		}

		else{
			DEBUGprintln("ERROR: The package sent has more than 20 beams.");
		}
	}

	else{
		offset = 4+(id>>1)*5;

		if(beams_package<=8){
			aux = Read_EEPROM(offset);
			aux |= (1<<beams_package)-1;
			Write_EEPROM(offset,aux);
		}

		else if(beams_package<=16){
			Write_EEPROM(offset,0xFF);

			aux = Read_EEPROM(offset+1);
			aux |= (1<<(beams_package-8))-1;
			Write_EEPROM(offset+1,aux);
		}

		else if(beams_package<=20){
			Write_EEPROM(offset,0xFF);
			Write_EEPROM(offset+1,0xFF);

			aux = Read_EEPROM(offset+2);
			aux |= (1<<(beams_package-16))-1;

			Write_EEPROM(offset+2,aux);

		}

		else{
			DEBUGprintln("ERROR: The package sent has more than 20 beams.");
			PostIssueMessage("SENT_MORE_THAN_20_BEAMS");
		}
	}

}
/*
This function initialize the data valid beam flags to zero, this should be called every time a zero ID is received, because that means
that new beams data is going to be sent
*/
void InitializeFlags(){
	int i = 0;

	for (i = 0 ; i < 125;i++){
		ResetWatchDogTimer();
		Write_EEPROM(4+i,0);
	}
	ResetWatchDogTimer();
}
/*
Function to check the data valid flag in the EEPROM of a beam
*/
bool CheckFlag(uint32_t nbeam){
	uint8_t offset = 0;
	uint8_t aux = 0;
	
	offset = nbeam%8;
	aux = Read_EEPROM(4 + (nbeam>>3));//beam>>3 gets the number of bytes before the current beam

	return ((aux>>offset)%2)==1;
}


//This function checks if the beams written on the GPIOs correspond to the beam saved on the EEPROM
bool CheckCurrentBeam(uint32_t pos_current_beam){
 	uint8_t beam_saved = Read_EEPROM(pos_current_beam);
	uint8_t beam_written = 	~(digitalRead(MON_UP2)<<5)|~(digitalRead(MON_UP1)<<4)|~(digitalRead(MON_UP0)<<3)|
							~(digitalRead(MON_DW2)<<2)|~(digitalRead(MON_DW1)<<1)|~(digitalRead(MON_DW0));
	beam_written &=0x3F;
	DEBUGprint("Saved beam: ");	
	DEBUGprintln(beam_saved);
	DEBUGprint("Written beam: ");
	DEBUGprintln(beam_written);
	return beam_written == beam_saved;
}

void FromDec2BinStr(char* bin_str,uint8_t beam){
	bin_str[0] = '\0';

    int z;
    for (z = 32; z > 0; z >>= 1)
    {
        strcat(bin_str, ((beam & z) == z) ? "1" : "0");
    }
}
