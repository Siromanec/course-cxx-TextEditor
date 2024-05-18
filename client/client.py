import socket

from context.interpreter import Interpreter
from context.protocol import COMM_PROTOCOL

UDP_IP = "localhost"  # Replace with the target IP address
UDP_PORT = 8080  # Replace with the target port
# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# Send the message (convert it to bytes first)
sock.sendto(COMM_PROTOCOL, (UDP_IP, UDP_PORT))
res = sock.recvfrom(512)
print(res)

interpreter = Interpreter()
"""
some commands require immediate response containing data
some need only an acknowledgment

some data may come from the server
(aka "host closed the lobby") - but this message is kind of an error 
or other errors

errors can be:

internal (fault of the client)
external (fault of someone else)
server (fault of the server)

"""
try:
    while True:
        context = interpreter.get_current_context()
        command = input(context + "> ").split(" ")
        parsed = interpreter.parse(command)
        if parsed is not None:
            sock.sendto(parsed, (UDP_IP, UDP_PORT))
            # when to recieve the response?
            res = sock.recvfrom(512)

            print(res)
except SystemExit:
    sock.close()
