
#ifndef ProcesaMan_h
#define ProcesaMan_h

#include <Energia.h>
#include <EEPROMMan.h>
#include <GPIOMan.h>
#include <RestMan.h>
#include <MensajesMan.h>

#ifndef TAM_BUFFER
#define TAM_BUFFER 2048
#endif



//Functions from Server_TCP_3 ABSControlModule

void SplitFrame(char *frame);
void Process_Petition(char *rx_buffer, char *tx_buffer, uint32_t packet_size);
void Change_Beam(char *puntero_char);
void Initial_Beam();

void SetBeamFlags(uint8_t id, uint8_t beams_package);
void InitializeFlags();
bool CheckFlag(uint32_t nbeam);
bool CheckCurrentBeam(uint32_t current_beam);
void FromDec2BinStr(char* bin_str,uint8_t beam);

#endif
