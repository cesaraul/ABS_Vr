//Etherner Manager header



#ifndef EthernerMan_h
#define EthernerMan_h
#define CAMBIO_POR_ENERGIA 1
#include <Energia.h>
#include <Ethernet.h>
#include <MensajesMan.h>


extern byte mac[];

extern IPAddress ip;
extern IPAddress dns;
extern IPAddress gw;
extern IPAddress mask;

extern IPAddress ipMulti;


void printEthernetData(void);

void configEthernet(void);





#endif
