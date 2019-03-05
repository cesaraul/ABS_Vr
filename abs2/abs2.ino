#include <WatchdogMan.h>


#include <EthernetMan.h>
#include <GPIOMan.h>
#include <ProcesaMan.h>
#include <EEPROMMan.h>
#include <MultiMan.h>
#include <RestMan.h>
#include <MensajesMan.h>




void setup(){
  configSerial();
  ConfigWatchDog();
  //DisableWatchDog();
  ResetWatchDogTimer();
  Conf_EEPROM();
  ResetWatchDogTimer();
  configEthernet();
  printEthernetData();
  
  ResetWatchDogTimer();
  configPins();
  ConfigGPIOInterrupts();
  ResetWatchDogTimer();
  
  Initial_Beam();//It has to be called after activating the GPIOInterrupts
  configMultiUDP();
  ResetWatchDogTimer();
  PrintInfo();
  if (CheckResetWatchDog()){
          ResetWatchDogTimer();
          DEBUGprintln("It was reseted by the watchdog");
          PostIssueMessage("RESET");
          ResetWatchDogTimer();
          //escribe_EEPROM(6143,0);
          //ResetWatchDogTimer();
  }

  ConfigSSIInterface();
  ResetLCD();
  EnableSlave();

  InitLCD();
  
  ShowImage(IGP_logo);
  ResetWatchDogTimer();
  
  int i=0;
  for (i=0;i<4000;i++){
    delayMicroseconds(500);
    ResetWatchDogTimer();
  }
  
  ClearLCD();
  ResetWatchDogTimer();
  for (i=0;i<2000;i++){
  delayMicroseconds(500);
  ResetWatchDogTimer();
  }
}

void loop(){
      ShowModuleInformationLCD();
      server_MultiUDP();

}

