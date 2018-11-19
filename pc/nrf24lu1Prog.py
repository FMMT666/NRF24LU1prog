#fangbo, 2018.11.19,
#a python tool for https://github.com/FMMT666/NRF24LU1prog

import serial, time, os, binascii

comPort = 'com35'
baudRate = 115200
binFilename = 'cradio-0.53.BIN'

if __name__ == '__main__':
	t = serial.Serial(comPort, baudRate, timeout = 5);

	time.sleep(1)
	
	t.write("?\r\n")	
	strr = t.read_until("nop",1000)
	t.reset_input_buffer()
	print "display hello :\r\n", strr

	fsize = os.path.getsize("./" + binFilename)
	print "bin size = " , fsize
	fbin = open("./" + binFilename, "rb")
	fbuffer = fbin.read(fsize)    
	fbin.close()

	if (fsize%512) == 0:
		packet_count = fsize/512
	else:
		packet_count = fsize/512+1

	print 'page cntr = ', packet_count

	t.write('W\r\n')	
	strr = t.read_until("nop",200)
	t.reset_input_buffer()
	print "\r\nenable write: ", strr
	
	t.write('E\r\n')	
	strr = t.read_until("nop",200)
	t.reset_input_buffer()
	print "\r\nerase chip: ", strr

	t.write('W\r\n')	
	strr = t.read_until("nop",200)
	t.reset_input_buffer()
	print "\r\nenable write: ", strr

	t.write('0\r\n')	
	strr = t.read_until("nop",200)
	t.reset_input_buffer()
	print "\r\nset page 0: ", strr

	for i in range(packet_count):
		print '\r\n\r\nstart program page = ', i, ' ...\r\n'

		for j in range(32):

			pageBuffer = fbuffer[(i*512 + j*16):(i*512 + j*16 + 16)]
			dataBuffer = 'b'+ (hex(j)[2:]).zfill(2) + pageBuffer.encode('hex')
			
			print dataBuffer	
			t.write(dataBuffer + '\r\n')
			#time.sleep(0.1)
			strr = t.read_until("nop",200)
			t.reset_input_buffer()
			print "\r\ngot message : ", strr

		t.write('P\r\n')	
		strr = t.read_until("nop",200)
		t.reset_input_buffer()
		print "\r\nprogram current page: ", strr
	
		t.write('+\r\n')	
		strr = t.read_until("nop",200)
		t.reset_input_buffer()
		print "\r\nset next page: ", strr

		t.write('W\r\n')	
		strr = t.read_until("nop",200)
		t.reset_input_buffer()
		print "\r\nenable write: ", strr

	print "end.\r\n"
