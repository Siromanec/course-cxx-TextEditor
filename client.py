import socket

UDP_IP = "localhost"  # Replace with the target IP address
UDP_PORT = 8080  # Replace with the target port
COMM_PROTOCOL=bytearray((0x70,0x42,0xE1,0x1C))
HEADER = bytearray(16)
MESSAGE = COMM_PROTOCOL + "Hello, World!".encode()  # Your message

LOBBY_CONTEXT = bytearray((0x01,))
LOBBY_CREATE = bytearray((0x01,))
LOBBY_JOIN = bytearray((0x02,))
LOBBY_GET = bytearray((0x03,))
LOBBY_LEAVE = bytearray((0x04,))
LOBBY_PLAYERS = bytearray((0x05,))

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send the message (convert it to bytes first)
sock.sendto(COMM_PROTOCOL, (UDP_IP, UDP_PORT))
res = sock.recvfrom(512)
print(res)
while True:
    command = input("> ").split(" ")
    if command[0] == "exit":
        sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_LEAVE, (UDP_IP, UDP_PORT))
        break
    match command[0]:
        case "lobby":
            match command[1]:
                case "create":
                    if len(command) != 2:
                        print("usage: lobby create")
                        continue
                    sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_CREATE, (UDP_IP, UDP_PORT))
                    res = sock.recvfrom(512)
                    print(res)
                case "get":
                    if len(command) != 2:
                        print("usage: lobby get")
                        continue
                    sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_GET, (UDP_IP, UDP_PORT))
                    res = sock.recvfrom(512)
                    print(res)
                case "join":
                    if len(command) != 3:
                        print("usage: lobby join <lobby_id>")
                        continue
                    lobby_id = int(command[2])

                    sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_JOIN + lobby_id.to_bytes(8, "little"), (UDP_IP, UDP_PORT))
                    res = sock.recvfrom(512)
                    print(res)
                case "leave":
                    if len(command) != 2:
                        print("usage: lobby leave")
                        continue
                    sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_LEAVE, (UDP_IP, UDP_PORT))
                case "players":
                    if len(command) != 3:
                        print("usage: lobby players <lobby_id>")
                        continue
                    lobby_id = int(command[2])

                    sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_PLAYERS + lobby_id.to_bytes(8, "little"), (UDP_IP, UDP_PORT))
                    res = sock.recvfrom(512)
                    print(res)
                case _:
                    print("Invalid command")
    # sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_CREATE, (UDP_IP, UDP_PORT))
    # res = sock.recvfrom(512)
    # print(res)
    # sock.sendto(COMM_PROTOCOL + HEADER + LOBBY_CONTEXT + LOBBY_GET, (UDP_IP, UDP_PORT))
    # res = sock.recvfrom(512)
    # print(res)
# packets_recieved = 1
# print(COMM_PROTOCOL == res[0][:4])
# print(COMM_PROTOCOL)
# print(res[0][:4])
# while res:
#     print(res)
#     try:
#         res = sock.recvfrom(128)
#     except KeyboardInterrupt:
#         break
#     packets_recieved += 1

# Close the socket
sock.close()
