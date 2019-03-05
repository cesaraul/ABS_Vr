
#ifndef WatchdogMan_h
#define WatchdogMan_h

#include <Energia.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/watchdog.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"




//void WatchDog_Handler(void);
void ConfigWatchDog(void);
void ResetWatchDogTimer(void);
bool CheckResetWatchDog(void);
void DisableWatchDog();
void EnableWatchDog();
#endif


