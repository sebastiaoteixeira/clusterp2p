# send a socket to a server
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('localhost', 12345))
s.sendall(b"""Hello, world! this is a very long message to test the performance of the server 
and the client. I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
I hope this message is long enough to test the performance of the server and the client.
          """)

data = s.recv(2048)
print('Received', repr(data))
s.close()