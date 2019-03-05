

#ifndef EEPROMMan_h
#define EEPROMMan_h

#include <Energia.h>
#include <GPIOMan.h>
#include <MensajesMan.h>
#include <RestMan.h>
#include <WatchdogMan.h>
#include <driverlib/sysctl.h>
#include <driverlib/eeprom.h>
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"


#define BYTES_PER_WORD 	4
#define WORDS_PER_BLOCK 16
#define NUM_BLOCKS		96
#define EEPROM_OFFSET 129


void Conf_EEPROM(void);
void Writting_beams(char* beams,uint8_t id);
void Read_Beam(uint16_t position);
void ToBinary(uint8_t value, char buff[7]);
uint8_t Read_EEPROM(uint16_t position);
void Write_EEPROM(uint32_t position, uint8_t value);

uint32_t GetActualBeam();
void SaveActualBeam(uint32_t pos);

uint32_t GetNbeamsTotal();
void SaveNbeamsTotal(uint32_t nbeamstotal);
#endif
