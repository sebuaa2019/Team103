#!/usr/bin/env python
#coding: utf-8
from socket import *
from time import ctime 
import threading 
import time 
import subprocess     #Popen 
import os
import base64
import hashlib
import struct
import rospy

rospy.init_node('shownstate')
def get_host_ip(): 
    s = socket(AF_INET, SOCK_DGRAM)
    s.connect(('8.8.8.8', 80))
    ip = s.getsockname()[0]
    s.close()
    return ip

HOST = get_host_ip()
print HOST
#HOST='10.135.192.202' 
PORT=8000
BUFSIZ=1024
ADDR = (HOST,PORT) 
  
tcpSerSock=socket(AF_INET,SOCK_STREAM) 
tcpSerSock.bind(ADDR) 
tcpSerSock.listen(5) 
 

MAGIC_STRING = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'
HANDSHAKE_STRING = "HTTP/1.1 101 Switching Protocols\r\n" \
                   "Upgrade:websocket\r\n" \
                   "Connection: Upgrade\r\n" \
                   "Sec-WebSocket-Accept: {1}\r\n" \
                   "WebSocket-Location: ws://{2}/chat\r\n" \
                   "WebSocket-Protocol:chat\r\n\r\n"
statusBuf = ""
statusChanged = False
threadLock = threading.Lock()
toResume = False
resumeLock = threading.Lock()
class Th(threading.Thread):
	def __init__(self, connection, addr):
		threading.Thread.__init__(self)
		self.con = connection
		self.addr = addr

	def run(self):
		global statusBuf,threadLock, statusChanged, toResume, resumeLock
		if addr[0] == HOST:
			while True:
				thedata = self.con.recv(BUFSIZ)
				if not thedata:
					resumeLock.acquire()
					if toResume:
						self.con.send("resume")
						toResume = False
					resumeLock.release()
					continue;
				threadLock.acquire()
				statusChanged = True
				statusBuf = thedata
				#print statusBuf
				threadLock.release()
		else :
			self.con.setblocking(0)
			while True:
				
				msg = self.recv_data(BUFSIZ)
				if not msg:
					threadLock.acquire()
					if statusChanged:
						self.send_data(statusBuf)
						statusChanged = False
					threadLock.release()
					continue
				if (msg == 'start'):
					print "start shopping"
					cmd = "roslaunch wpb_home_apps shopping.launch"
					proc = subprocess.Popen(cmd, stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
				if (msg == 'topic'):
					#os.system("rostopic list")
					cmd = "rostopic list"
					proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
					for line in iter(proc.stdout.readline, 'b'):
						#print (line)
						self.send_data(line)
						if not subprocess.Popen.poll(proc) is None:
							if line == "":
								break
					proc.stdout.close()
				if (msg == 'stop'):
					threadLock.acquire()
					if (statusBuf == "1"):#STATE_FOLLOW
						cmd = "rosservice call wpb_home_follow/stop 0"
						proc = subprocess.Popen(cmd, stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
					else if (statusBuf == "3"):#STATE_GOTO
						cmd = "rostopic pub -1 /move_base/cancel actionlib_msgs/GoalID -- {}"
						proc = subprocess.Popen(cmd, stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
					else :
						print("error stop time")
					threadLock.release()
				if (msg == 'resume'):
					threadLock.acquire()
					if (statusBuf == "1"):#STATE_FOLLOW
						cmd = "rosservice call wpb_home_follow/resume 0.7"
						proc = subprocess.Popen(cmd, stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
					if (statusBuf == "3"):#GOTO
						resumeLock.acquire()
						toResume = True
						resumeLock.release()
					threadLock.acquire()
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


def handshake(con):
	headers = {}
	shake = con.recv(BUFSIZ)

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

 
if __name__ == '__main__': 
	#t.start() 
	print 'waiting for connecting...'
	while True: 
 		clientSock,addr = tcpSerSock.accept() 
		print 'connected from:', addr
		#clientSock.setblocking(0)
		if addr[0] != HOST:
			if handshake(clientSock):
				print "handshake success"
				try:
					t = Th(clientSock, addr)
					t.start()
					print 'new thread for client ...'
				except Exception as e:
					print 'start new thread error: %s' %(e.message)
					clientSock.close()
		else :
			t = Th(clientSock, addr)
			t.start()
