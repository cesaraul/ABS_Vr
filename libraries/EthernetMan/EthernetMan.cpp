#include "EthernetMan.h"


void configEthernet(void){
	Ethernet.begin(0,ip, dns, gw, mask);
	Ethernet.beginMulticast(&ip,&ipMulti);
 }

 
void printEthernetData(void) {
  // print your IP address:
	ResetWatchDogTimer();
  	DEBUGprintln("");
  	DEBUGprintln("IP Address Information:");  
	ResetWatchDogTimer();
  	IPAddress ip = Ethernet.localIP();
	
  	DEBUGprint("IP Address:\t");
  	DEBUGprintln(ip);
	ResetWatchDogTimer();
  // print your MAC address:
  	IPAddress subnet = Ethernet.subnetMask();
  	DEBUGprint("NetMask:\t");
  	DEBUGprintln(subnet);
	ResetWatchDogTimer();
  // print your gateway address:
  	IPAddress gateway = Ethernet.gatewayIP();
  	DEBUGprint("Gateway:\t");
  	DEBUGprintln(gateway);
	ResetWatchDogTimer();
  // print your gateway address:
  	IPAddress dns = Ethernet.dnsServerIP();
  	DEBUGprint("DNS:\t\t");
  	DEBUGprintln(dns);
	ResetWatchDogTimer();
}

byte mac[] = {0x00, 0x1A, 0xB6, 0x02, 0xE8, 0xBE};
/*
IPAddress ip = IPAddress(192,168,1,1);
IPAddress dns = IPAddress(192,168,1,128);
IPAddress gw = IPAddress(192,168,1,128);
IPAddress mask = IPAddress(255,255,255,0);
*/
IPAddress ip = IPAddress(10,10,10,95);

IPAddress dns = IPAddress(10,10,10,1);

IPAddress gw = IPAddress(10,10,10,1);

IPAddress mask = IPAddress(255,255,255,0);

IPAddress ipMulti(224,3,29,71);




