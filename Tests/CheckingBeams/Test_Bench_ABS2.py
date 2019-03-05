import random
import time
import subprocess
import os
def GenerateRandomConfig(n):
	result = ""
	for i in range(n):
		beam =random.randrange(64)
		result = result + chr(beam+33)	

	return result

beamchars = []
f = open("input_beams.txt","w")
for i in range(4):
	pack_str = GenerateRandomConfig(20*64)
	beamchars.append(pack_str)
	f.write(pack_str+"\n")

pack_str = GenerateRandomConfig(5*64)
beamchars.append(pack_str) 
f.write(pack_str+"\n")
f.close()
cond = True

for i in range(4):
	print("SNDF")
	print("085")#nbeams
	print("20")#tamanio
	print("0"+str(i))#ID
	print(beamchars[i])


	for j in range(20):	
		print ("CHGB")
		print (20*i+j)		
		print ("GETC")


print("SNDF")
print("085")#nbeams
print("05")#tamanio
print("04")#ID
print(beamchars[4])

for j in range(5):	
	print ("CHGB")
	print (80+j)		
	print ("GETC")




