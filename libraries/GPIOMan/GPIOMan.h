

#define CAMBIO_POR_ENERGIA 1

#ifndef GPIOMan_h
#define GPIOMan_h

#include <Energia.h>
#include <MensajesMan.h>
#include <EEPROMMan.h>
#include <RestMan.h>
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include <string.h>

//Configuration of the control and monitoring pins
//Monitoring - Outputs

#define MODULE_NUM 10
#define CTRL_UP0 PQ_0 
#define CTRL_UP1 PD_5
#define CTRL_UP2 PD_4

#define CTRL_DW0 PN_4
#define CTRL_DW1 PN_5
#define CTRL_DW2 PP_4

//Monitoring - Inputs
#define MON_UP0 PD_3
#define MON_UP1 PE_5
#define MON_UP2 PC_6

#define MON_DW0 PE_4
#define MON_DW1 PC_4
#define MON_DW2 PC_5

void configPins (void);
void ConfigGPIOInterrupts();
void EnableGPIOInterrupts();
void DisableGPIOInterrupts();
void GPIOIntHandler();
void SendGPIOInformation();
void SendGPIOInformation2();
#endif
