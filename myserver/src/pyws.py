#!/usr/bin/env python
#coding: utf-8
 
import socket
import threading
import sys
import os
import base64
import hashlib
import struct
# import rospy 
# ====== config ======
# init_node('pyws')

HOST = '127.0.0.1'
PORT = 8000
MAGIC_STRING = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'
HANDSHAKE_STRING = "HTTP/1.1 101 Switching Protocols\r\n" \
                   "Upgrade:websocket\r\n" \
                   "Connection: Upgrade\r\n" \
                   "Sec-WebSocket-Accept: {1}\r\n" \
                   "WebSocket-Location: ws://{2}/chat\r\n" \
                   "WebSocket-Protocol:chat\r\n\r\n"
 
class Th(threading.Thread):
	def __init__(self, connection,):
		threading.Thread.__init__(self)
		self.con = connection
 
	def run(self):
		while 1:
			msg = self.recv_data(100)
			if msg == False:
				continue
			if (msg == 'start'):
				print "start shopping"
				os.system("roslaunch wpb_home_apps shopping.launch")
			if (msg == 'topic'):
				#os.system("rostopic list")
				import subprocess     #Popen 
				cmd = "rostopic list"
				proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
				for line in iter(proc.stdout.readline, 'b'):
					#print (line)
					self.send_data(line)
					if not subprocess.Popen.poll(proc) is None:
						if line == "": 
							break
				proc.stdout.close()

			else:
				print "othermsg"

		self.con.close()
 
	def recv_data(self, num):
		try:
			all_data = self.con.recv(num)
			if not len(all_data):
				return False
		except:
			return False
		else:
			code_len = ord(all_data[1]) & 127
			if code_len == 126:
				masks = all_data[4:8]
				data = all_data[8:]
			elif code_len == 127:
				masks = all_data[10:14]
				data = all_data[14:]
			else:
				masks = all_data[2:6]
				data = all_data[6:]
			raw_str = ""
			i = 0
			for d in data:
				raw_str += chr(ord(d) ^ ord(masks[i % 4]))
				i += 1
			return raw_str

	# send data
	def send_data(self, data):
		if data:
			data = str(data)
		else:
			return False
		token = "\x81"
		length = len(data)
		if length < 126:
			token += struct.pack("B", length)
		elif length <= 0xFFFF:
			token += struct.pack("!BH", 126, length)
		else:
			token += struct.pack("!BQ", 127, length)
		#struct为Python中处理二进制数的模块，二进制流为C，或网络流的形式。
		data = '%s%s' % (token, data)
		self.con.send(data)
		return True

 	
	# handshake
def handshake(con):
	headers = {}
	shake = con.recv(1024)

	if not len(shake):
		return False
 
	header, data = shake.split('\r\n\r\n', 1)
	for line in header.split('\r\n')[1:]:
		key, val = line.split(': ', 1)
		headers[key] = val

	if 'Sec-WebSocket-Key' not in headers:
		print 'This socket is not websocket, client close.'
		con.close()
		return False

	sec_key = headers['Sec-WebSocket-Key']
	res_key = base64.b64encode(hashlib.sha1(sec_key + MAGIC_STRING).digest())
	
	str_handshake = HANDSHAKE_STRING.replace('{1}', res_key).replace('{2}', HOST + ':' + str(PORT))
	print str_handshake
	con.send(str_handshake)
	return True

def new_service():
	"""start a service socket and listen
	when coms a connection, start a new thread to handle it"""
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	try:
		sock.bind((HOST, PORT))
		sock.listen(1000)
		print "bind 8000,ready to use"
	except:
		print "Server is already running,quit"
		sys.exit()

	while True:
		connection, address = sock.accept()
		#返回元组（socket,add），accept调用时会进入waite状态
		print "Got connection from ", address
		if handshake(connection):
			print "handshake success"
			try:
				t = Th(connection)
				t.start()
				print 'new thread for client ...'
			except Exception as e:
				print 'start new thread error: %s' %(e.message)
				connection.close()


if __name__ == '__main__':
	new_service()
