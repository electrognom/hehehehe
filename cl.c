#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "mystructs.h"
#include "myfunctions.h"


MY_MESSAGE message_from_server;


int valread;




//void *out_serv (void *arg) {
//        char buff[1024];
//        int socket = *(int*)arg;
//        while(1){
//
//
//            valread = recv( socket ,message_from_server, sizeof(message_from_server), 0);
//
////            memset(buff,'\0',sizeof(buff));
////            strncpy(buff,*message_from_server.login,sizeof(*message_from_server.login));
////            strncpy(buff,": ",3);
////            strncpy(buff,*message_from_server.message,sizeof(*message_from_server.message));
////            printf("%s\n",buff);
//
//        }
//}

void *chat_with_server(void *arg){
    char message[1024]="";

    int socket = *(int*)arg;
    while(1){

        gets(message);

        send(socket, message, sizeof(message), 0);
        printf("\n%s","otpravleno");



}


asdasfdasfdasd
asdasdasdas
int main()
{
    int sock;
    struct sockaddr_in addr;
     pthread_t out_from_server;
     pthread_t dialog;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
//    if(pthread_create(&out_from_server, NULL, out_serv, &sock)){
//       printf("beda");
//       }

       if(pthread_create(&dialog, NULL, chat_with_server, &sock)){
       printf("beda");
       }

       while(1){}


    //recv(sock, buf, sizeof(message), 0);

   // printf(buf);
    close(sock);

    return 0;
}
