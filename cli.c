#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *out_serv (void *arg)
{
	char server_reply[1024];
	int sock= *(int*)arg;
	while(1)	{
		if( recv(sock, server_reply, sizeof(server_reply), 0) < 0)	{
			puts("recv failed");
			break;
		}
		system("mpg123 pop.mp3 command 2> /dev/null");
		printf("%s\n", server_reply);
	}
	return 0;
}

void *sender(void *arg)
{
	char message[1024];
	memset(message, '\0', sizeof(message));
	int sock = *(int*)arg;
	while(1)	{
		//scanf("%s\n", message);
		fgets(message, sizeof(message), stdin);
		message[strlen(message)-1] = 0; // To avoid sending \n

		//Send some data
		if( send(sock, message, strlen(message)+1, 0) < 0)	{
			puts("Send failed");
			return 0;
		}
	}
	return 0;
}


int main(int argc, char *argv[])
{
	char s_addr[20];
	puts("Enter address:");
	fgets(s_addr, 20, stdin);
	s_addr[strlen(s_addr)-1] = 0;
	if(!strcmp(s_addr, "localhost")) {
		strcpy(s_addr, "127.0.0.1");
	}

	int sock;
	struct sockaddr_in server;

	pthread_t out_from_server,sendy;

	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(s_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)	{
		perror("connect failed. Error");
		return 1;
	}

    	puts("Connected\n");


	if(pthread_create(&out_from_server, &threadAttr, out_serv, &sock))	{
		printf("beda");
	}
	if(pthread_create(&sendy, &threadAttr, sender, &sock))	{
		printf("beda");
	}

    	//keep communicating with server
	while(1)	{
		//Receive a reply from the server
	}

	close(sock);
	return 0;
}
