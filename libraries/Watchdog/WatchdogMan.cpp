#include "WatchdogMan.h"

//*****************************************************************************
//
// The interrupt handler for the watchdog.  
//*****************************************************************************
//The handler NMISR is in /home/acastro/.energia15/packages/energia/hardware/tivac/1.0.3/cores/tivac/startup_gcc

/*
Function used to config the WatchDog as a Timer and NMI. Here it could be put if is needed.
*/
void ConfigWatchDog(void){
    //
    // Enable the peripherals WatchDog
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);


    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

	//Set the watchdog interrupt to be NMI
	ROM_WatchdogIntTypeSet(WATCHDOG0_BASE,WATCHDOG_INT_TYPE_NMI);

	if (ROM_WatchdogLockState(WATCHDOG0_BASE))
		ROM_WatchdogUnlock(WATCHDOG0_BASE);
	

    //
    // Set the period of the watchdog timer to 1ms.
    //
    ROM_WatchdogReloadSet(WATCHDOG0_BASE, F_CPU/1000);

	ROM_WatchdogStallEnable(WATCHDOG0_BASE);
    //
    // Enable the watchdog interrupt.
    //
    ROM_IntEnable(INT_WATCHDOG);
	ROM_WatchdogIntEnable(WATCHDOG0_BASE);	

    //
    // Enable reset generation from the watchdog timer.
    //
    ROM_WatchdogResetEnable(WATCHDOG0_BASE);

    //
    // Enable the watchdog timer.
    //
    ROM_WatchdogEnable(WATCHDOG0_BASE);


	//This locks the watchdog timer registers, so it cannot be accessed for write operations
	ROM_WatchdogLock(WATCHDOG0_BASE);
	//IntRegister(INT_WATCHDOG_TM4C129,WatchDog_Handler);
	//watchdogint_exc = 0;
}

/*
This function is used to reset the watchdog timer to avoid letting it reset the uC, it should be called in the
main code before the timer counter gets the 0 value
*/
void ResetWatchDogTimer(void){
	if (ROM_WatchdogLockState(WATCHDOG0_BASE))
		ROM_WatchdogUnlock(WATCHDOG0_BASE);
    ROM_WatchdogReloadSet(WATCHDOG0_BASE, F_CPU/1000);
	ROM_WatchdogLock(WATCHDOG0_BASE);
}


/*
This function should be called at the begining of the program to check if the uC has been reseted by the watchdog and in affirmative
case send a POST to the server
*/
bool CheckResetWatchDog(void){

	uint32_t reset_cause = ROM_SysCtlResetCauseGet();
	ROM_SysCtlResetCauseClear(SYSCTL_CAUSE_WDOG0);
	return (reset_cause&SYSCTL_CAUSE_WDOG0) == SYSCTL_CAUSE_WDOG0;
}


void EnableWatchDog(){
	ConfigWatchDog();
}
void DisableWatchDog(){
	ResetWatchDogTimer();
	ROM_SysCtlPeripheralReset(SYSCTL_PERIPH_WDOG0);
}

