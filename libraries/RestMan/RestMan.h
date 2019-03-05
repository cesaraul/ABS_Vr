

#ifndef RestMan_h
#define RestMan_h

#include <Energia.h>
#include <EthernetMan.h>
#include <MensajesMan.h>
#include <WatchdogMan.h>
#include <stdio.h>
#include <string.h>
#ifndef TAM_BUFFER
#define TAM_BUFFER 2048
#endif

#define PUERTO_REST 8888



void PostIssueMessage(char* message);

int ReadReplyMessage(EthernetClient client, char* buffer, int n);

#endif


