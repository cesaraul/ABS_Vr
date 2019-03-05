
#ifndef MultiMan_h
#define MultiMan_h

#include <Energia.h>
#include <EthernetUdp.h>
#include <ProcesaMan.h>
#include <MensajesMan.h>
#include <GPIOMan.h>
#include <WatchdogMan.h>
#ifndef TAM_BUFFER
#define TAM_BUFFER 2048
#endif

extern EthernetUDP Udp_2;
extern char  packetBuffer[TAM_BUFFER] ; 
extern char  ReplyBuffer[TAM_BUFFER] ; 

extern const uint16_t portMulti;      // local port to listen on

void server_MultiUDP(void);
void configMultiUDP(void);

#endif
