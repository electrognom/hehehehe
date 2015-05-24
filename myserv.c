#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include "mystructs.h"
#include "myfunctions.h"

int main(int argc , char *argv[])
{
    int opt = 1;
    int master_socket, addrlen, new_socket, activity, i, j, valread, sd, max_sd;
    struct sockaddr_in address;
    INFO_ABOUT_CLIENT sd_client;
    char buffer[1024], buff[1024];
    char* analyzer;
    CLIENTS clients;
    /*log_list.client_list_size = MAX_REG_LIST;
    log_list.current_client_maximum = 0;*/
    clients.client_list_size = MAX_CLIENTS;
    clients.current_client_maximum=0;
    fd_set readfds;

    for(i=0; i < clients.client_list_size; ++i)
    {
        clients.client_list[i].adress = 0;
        clients.client_list[i].color = 0;
        clients.client_list[i].logged = 0;
        strcpy(clients.client_list[i].login, "Anonymus");
    }

    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i=0; i < clients.client_list_size; i++)
        {
            sd = clients.client_list[i].adress;
            if(sd > 0)
            {
                FD_SET( sd , &readfds);
            }
            if(sd > max_sd)
            {
                max_sd = sd;
            }
        }
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            printf("Select error\n");
        }
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address,
                                     (socklen_t*)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("New connection, socket fd is %d, ip is: %s, port: %d \n",
                   new_socket,
                   inet_ntoa(address.sin_addr),
                   ntohs(address.sin_port));
//            		for (i = 0; i < clients.client_list_size; i++)	{
//                		if(clients.client_list[i].adress == 0)	{
//                    			clients.client_list[i].adress = new_socket;
//                    			printf("Adding to list of sockets as %d\n", i);
//                    			break;
//                		}
//            		}
            clients.client_list[clients.current_client_maximum].adress = new_socket;
            printf("Adding to list of sockets as %d\n", clients.current_client_maximum);
            clients.current_client_maximum++;
        }
        for(i=0; i < clients.client_list_size; ++i)
        {
            sd = clients.client_list[i].adress;
            if(FD_ISSET(sd, &readfds))
            {
                if((valread = recv(sd, buffer, sizeof(buffer), 0)) == 0)
                {
                    getpeername(sd,
                                (struct sockaddr*)&address,
                                (socklen_t*)&addrlen);
                    printf("Host disconnected, ip %s, port %d\n",
                           inet_ntoa(address.sin_addr),
                           ntohs(address.sin_port));
                    close( sd );
                    clients.client_list[i].adress = 0;
                }
                else
                {
                    puts(buffer);
                    processing_message(
                        &clients.client_list[i],
                        &clients,
                        buffer);
                }
            }
        }
    }
    return 0;
}




