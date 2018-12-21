#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 4444

int main()
{
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		fprintf(stderr, "ERR:SOCKET");
		exit(1);
	}
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		fprintf(stderr, "ERR:CONNECT");
		exit(1);
	}
	while (1)
	{
		scanf("%s", buffer);
		send(clientSocket, buffer, strlen(buffer), 0);
		if (strcmp(buffer, ":exit") == 0)
		{
			close(clientSocket);
			printf("[system]Disconnected from server.\n");
			exit(1);
		}
		if (recv(clientSocket, buffer, 1024, 0) < 0)
			printf("[system]Error when receiving data.\n");
		else
			printf("%s\n", buffer);
	}
	return 0;
}
