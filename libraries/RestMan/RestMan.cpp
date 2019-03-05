
#include "RestMan.h"
uint8_t received = 1;
void PostIssueMessage(char* message){
while(received){
	received = 0;
	ResetWatchDogTimer();
	DEBUGprintln("Beginning the POST");
	EthernetClient client_rest;
	
	//client_rest.connect(IPAddress(10,10,20,128), PUERTO_REST,500);//Timeout set to 500 microseconds

	char* rx_buffer = (char*)malloc(TAM_BUFFER);
	char* tx_buffer = (char*)malloc(TAM_BUFFER);
	
	ResetWatchDogTimer();

	DisableWatchDog();
	while(!client_rest.connected()){
		ResetWatchDogTimer();
		
		//DEBUGprintln("Trying to connect with the server");
		client_rest.connect(IPAddress(10,10,20,128), PUERTO_REST,1000);//Timeout set to 500 microseconds
	}
	EnableWatchDog();
	
	if (client_rest.connected()){
		ResetWatchDogTimer();
		DEBUGprintln("Connected cliente");
		ResetWatchDogTimer();
		sprintf(tx_buffer,"POST /abs/alert/ HTTP/1.1\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nissue=%s\r\n",strlen(message)+6,message);
		ResetWatchDogTimer();

		client_rest.print(tx_buffer);

		ResetWatchDogTimer();
		int time1 = millis();
		int time2 = time1;
		while(!client_rest.available()){
			//DEBUGprintln("Unavailable answer.");
			ResetWatchDogTimer();
			time2 = millis();
			if ((time2 -time1) >= 50){
				received = 1;
				break;			
			}
		}
		if (received==0){
			DEBUGprintln("Reading ...");
			int len = ReadReplyMessage(client_rest,rx_buffer,TAM_BUFFER-1);//We should let at least one free space for terminating char
			ResetWatchDogTimer();
			DEBUGprint("It has been read ");
			DEBUGprint(len);
			DEBUGprintln(" caracteres.");
			DEBUGprint("Received reply:\n");
			ResetWatchDogTimer();
			DEBUGprintln(rx_buffer);
			ResetWatchDogTimer();

			client_rest.stop();//Here it does the flush, so it's not neccessary to execute a flush function
			ResetWatchDogTimer();
		}
	}

	free(rx_buffer);
	free(tx_buffer);
}
received = 1;
}



int ReadReplyMessage(EthernetClient client, char* buffer, int n){
	int i=0;
	int b=0;
	if (client.available()<=0)
		return -1;

	for (i = 0; i < n; i++) {
		ResetWatchDogTimer();
		b = client.read();
		if (b == -1)
			break;
		buffer[i] = b;
	}
	buffer[i]='\0';
	return i;
}
