server: server.c client
	gcc server.c -o server
client: client.c
	gcc client.c -o client
