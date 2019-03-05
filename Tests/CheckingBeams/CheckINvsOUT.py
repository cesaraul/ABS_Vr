import subprocess
import sys
if __name__=="__main__":
	f_input = open("input_beams.txt","r")
	f_output = open("output_beams"+sys.argv[1]+".txt","r")

	cond = True
	for i in range(4):
		pack_str = f_input.readline()[20*int(sys.argv[1]):20*int(sys.argv[1])+20]
		for j in range(20):
			beam = f_output.readline()[0]
			
			cond = cond and (pack_str[j]==beam)
			if (not cond):
				print ("Paquete %d con beam recibido %s y mandado %s",i,beam,pack_str[j])
		

	pack_str = f_input.readline()[5*int(sys.argv[1]):5*int(sys.argv[1])+5]
	for j in range(5):
		beam = f_output.readline()[0]
		cond = cond and (pack_str[j]==beam)
		if (not cond):
			print ("Paquete 5 con beam %s",beam)


	if (cond):
		subprocess.call(["touch","successful.txt"])
	else:

		subprocess.call(["touch","unsuccessful.txt"])

