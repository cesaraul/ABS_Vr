#include "MultiMan.h"

const uint16_t portMulti = 10000;      // local port to listen on

void configMultiUDP(void){
	Udp_2.begin(portMulti);
}

void server_MultiUDP(void){

	//Multicast
	uint32_t packetSize_2 = Udp_2.parsePacket();
	ResetWatchDogTimer();
	SendGPIOInformation();
	SendGPIOInformation2();
	if(packetSize_2) {
    	DEBUGprint("Received packet multicast of size ");
    	DEBUGprintln(packetSize_2);
    	DEBUGprint("From ");
   
		ResetWatchDogTimer();
		IPAddress remote_2 = Udp_2.remoteIP();
		
		for (int i_2 =0; i_2 < 4; i_2++) {
		    DEBUGprint2(remote_2[i_2], DEC);
		  if (i_2 < 3) {
		      DEBUGprint(".");
		  }
		}

		ResetWatchDogTimer();
		DEBUGprint(", port ");
		uint16_t puerto_2 = Udp_2.remotePort();
		DEBUGprintln(puerto_2);
	 

		// read the packet into packetBufffer
		Udp_2.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);

		ResetWatchDogTimer();
		DEBUGprintln("Contents:");

		packetBuffer[packetSize_2]='\0';
		DEBUGprintln(packetBuffer);

		ResetWatchDogTimer();
		Process_Petition(packetBuffer,ReplyBuffer,packetSize_2);
		ResetWatchDogTimer();
		// send a reply, to the IP address and port that sent us the packet we received
		Udp_2.beginPacket(Udp_2.remoteIP(), Udp_2.remotePort());
		Udp_2.write(ReplyBuffer);
		Udp_2.endPacket();
		ResetWatchDogTimer();
	}

}

EthernetUDP Udp_2;
char  packetBuffer[TAM_BUFFER] ; 
char  ReplyBuffer[TAM_BUFFER] ; 
