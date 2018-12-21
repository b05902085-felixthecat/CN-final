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
	int sockfd, ret;
	struct sockaddr_in serverAddr;
	int newSocket;
	struct sockaddr_in newAddr;
	char buffer[1024];
	pid_t childpid;
	socklen_t addr_size;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		fprintf(stderr, "ERR:SOCKET\n");
		exit(1);
	}
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		fprintf(stderr, "ERR:BIND\n");
		exit(1);
	}
	if (listen(sockfd, 10) != 0)
	{
		fprintf(stderr, "ERR:LISTEN\n");
		exit(1);
	}
	while (1)
	{
		newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
		if (newSocket < 0)
		{
			fprintf(stderr, "ERR:NEW_SOCKET\n");
			exit(1);
		}
		printf("[System]Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		if ((childpid = fork()) == 0)
		{
			close(sockfd);
			while (1)
			{
				recv(newSocket, buffer, 1024, 0);
				if (strcmp(buffer, ":exit") == 0)
				{
					printf("[System]Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
				else
				{
					printf("%s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}
	}
	close(newSocket);
	return 0;
}
