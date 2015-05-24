#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS

#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
//#include <arpa/inet.h>    //close
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include "mystructs.h"

void processing_message(INFO_ABOUT_CLIENT* client, CLIENTS* clients, char message[1024]);
void send_message(MY_MESSAGE* message, INFO_ABOUT_CLIENT* client, CLIENTS* clients);
MY_MESSAGE create_message(INFO_ABOUT_CLIENT* client, char* message);
int find_by_login(char*, CLIENTS*, INFO_ABOUT_CLIENT*);



#endif
