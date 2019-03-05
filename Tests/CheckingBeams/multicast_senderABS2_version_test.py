import socket
import struct
import sys
import time
import random
import subprocess
import os
def GenerateRandomConfig(n):
	result = ""
	for i in range(n):
		beam =random.randrange(64)
		result = result + chr(beam+33)	

	return result
	
def FillWithZeros(number,n):
	cad = str(number)
	l = len(cad)
	while (l<n):
		cad = "0"+cad
		l += 1
	return cad

	
multicast_group = ('224.3.29.71', 10000)

# Create the datagram socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(1.0)
local_ip="10.10.10.95"
sock.bind((local_ip,10000))

sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_IF, socket.inet_aton(local_ip))
print "Test de multicast sender"
subprocess.call(["rm","output_beams0.txt"])
while(True):
	cmd = raw_input()#"Ingrese el comando a utilizar MNTR, CHGB,SNDF, RBOT, GETC, GETB:  "


	if (cmd=="CHGB"):	
		beam = raw_input()#"Ingrese el numero de beam: "
		message = cmd+beam
		sent = sock.sendto(message, multicast_group)
	elif (cmd=="MNTR"):
		message = cmd
		sent = sock.sendto(message, multicast_group)

	elif (cmd == "SNDF"):
		nbeams = raw_input()
		tamanio = raw_input()
		bloque_id = raw_input()
		beamchars = raw_input()# Nbeams times the nmodules is the number of characters that beamchars has
		message = cmd+nbeams+tamanio+bloque_id+beamchars			#The max number of char that can be send is 1472
		sent = sock.sendto(message, multicast_group)

	elif (cmd == "RBOT"):
		message = cmd;
		sent = sock.sendto(message, multicast_group)

	elif (cmd == "GETC"):
		message = cmd;
		sent = sock.sendto(message, multicast_group)

	elif (cmd == "GETB"):
		message = cmd;
		sent = sock.sendto(message, multicast_group)		


	else:
		print cmd, "Comando erroneo"
	# Send data to the multicast group
	#print >>sys.stderr, 'sending "%s"' % message



	#Waits for all the modules to respond
	connecteds=[0]*64
	
	try:
		if (cmd!="RBOT"):
			nmodules = 0
			while(nmodules!=64):			
				data,address = sock.recvfrom(1024)
			
				#print data,address
				if (data[0]=="1"):
					connecteds[int(address[0][10:])-1]=1
					nmodules += 1
					if(cmd == "GETB" or cmd == "GETC"):
						with open("output_beams0"+".txt","a+") as f:
							f.write(data[1:]+"\n")
					


	except:
		pass
	for i in range(64):
		if (connecteds[i]==1):
			#print "Modulo n"+str(i+1) + "is connected"	
			pass
		else:
			#print "Modulo n"+str(i+1) + "is disconnected"
			pass
		
	# Look for responses from all recipients
	#time.sleep(2)
