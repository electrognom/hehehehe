#include "myfunctions.h"

#define DEF_COLOR "\e[0;30m"


void send_message(MY_MESSAGE* message, INFO_ABOUT_CLIENT* client, CLIENTS* clients)	{
	char buffer[1024];
	memset(buffer,'\0',sizeof(buffer));

	if(client->logged) {
		switch(client->color) {
			case 0:	{
				strcat(buffer, DEF_COLOR);
			}	break;
			case 1:	{
				strcat(buffer, "\e[0;31m");
			}	break;
			case 3:	{
				strcat(buffer, "\e[0;32m");
			}	break;
			case 2:	{
				strcat(buffer, "\e[0;34m");
			}	break;
			default:{
				strcat(buffer, DEF_COLOR);
			}	break;
		}
		strcat(buffer, message->login);
		strcat(buffer,": ");
	} else {
		strcat(buffer, DEF_COLOR);
        strcat(buffer, "Anonymus: ");
	}

	//memset(buffer,'\0',strlen(buffer));
	strcat(buffer, message->message);
	strcat(buffer, DEF_COLOR);
	printf("In send_message() : %s\n", buffer);

	int i;
	for(i=0; i < clients->client_list_size; ++i)	{
    	if(client->adress != clients->client_list[i].adress)
		send(clients->client_list[i].adress, buffer, sizeof(buffer), 0);
	}
}



MY_MESSAGE create_message(INFO_ABOUT_CLIENT* client, char message[1024])	{
	MY_MESSAGE mess;

	//memset(mess.message,'\0',strlen(mess.message));
	//memset(mess.message,'\0',strlen(mess.message));
	mess.color = client->color;
	if(client->logged) {
		strncat(mess.login,client->login,strlen(client->login));
	} else {
        	strncat(mess.login,"Anonymus: ",15);
	}

	strcpy(mess.message,message);
	printf("Create message %s\n", mess.message);
	//puts(mess.message);
	return mess;
}



void processing_message(INFO_ABOUT_CLIENT* client, CLIENTS* clients, char message[1024])	{
	char buff[1024];
	int j = 0;
	char* analyzer = (char*)malloc(strlen(message)+1);
	strcpy(buff, message);
	analyzer = strtok(buff, " ");
    if(!strcmp(analyzer, "/reg")) {
		int j=0, fail=0;
		char *usr=NULL, *pwd=NULL;
        	while(1) {
            		analyzer = strtok(NULL, " ");
			if(analyzer == NULL) {
				break;
			}
			puts(analyzer);
			if(j == 0) {
				INFO_ABOUT_CLIENT tmp;
				if(find_by_login(analyzer, clients, &tmp) >= 0) {
					fail = 1;
				}
				usr = strdup(analyzer);
			} else {
				pwd = strdup(analyzer);
			}
            		j++;
        	}
		if(fail || !j) {
			printf("Login already in use: %s\n", usr);
			server_reply_to("Login already in use", client);
			return;
		}
		server_reply_to("Registered", client);
		printf("Registered new user <%s; %s>\n", usr, pwd);
		strcpy(client->login, usr);
		strcpy(client->password, pwd);
		client->logged = 1;
		if(usr)	free(usr);
		if(pwd) free(pwd);
		return;
	} else if(!strcmp(analyzer, "/color")) {
		puts("Setting color");
		char fail = 0;
    	if(analyzer = strtok('\0', " ")) {
        	puts(analyzer);
	        if(!strcmp(analyzer,"red")) {
	            	client->color = 1;
	        } else if(!strcmp(analyzer,"blue")) {
	            	client->color = 2;
	        } else if(!strcmp(analyzer,"green")) {
	            	client->color = 3;
	        } else if(!strcmp(analyzer,"reset")) {
	            	client->color = 0;
	        } else {
				printf("Unknown color %s\n", analyzer);
				fail = 1;
			}
			server_reply_to(fail?"Unknown color":"Color set", client);
		}
		return;
	} else if(!strcmp(analyzer, "/login"))	{
		int j=0, fail=0;
		char *usr=NULL, *pwd=NULL;
    	while(1) {
        		analyzer = strtok(NULL, " ");
		if(analyzer == NULL) {
			break;
		}
		puts(analyzer);
		if(j == 0) {
			usr = strdup(analyzer);
		} else {
			pwd = strdup(analyzer);
		}
        		j++;
    	}
    	INFO_ABOUT_CLIENT tmp;
    	int log_adr;
		if((log_adr = find_by_login(usr, clients, &tmp)) >= 0) {
    		if(!clients->client_list[log_adr].logged) {
    			if(!strcmp(clients->client_list[log_adr].password, pwd)) {
					strcpy(client->login,
						clients->client_list[log_adr].login);
					strcpy(client->password,
						clients->client_list[log_adr].password);
					//clients->client_list[log_adr].logged = 0;
					*clients->client_list[log_adr].login = 0;
					*clients->client_list[log_adr].password = 0;
					client->logged = 1;
					client->color = clients->client_list[log_adr].color;
					printf("Logged <%s; %s>\n", usr, pwd);
					server_reply_to("Logged in", client);
    			} else {
					server_reply_to("Incorrect password", client);
				}
    		} else {
				server_reply_to("User already online", client);
			}
		} else {
			server_reply_to("Incorrect login", client);
		}
		if(usr)	free(usr);
		if(pwd) free(pwd);
		return;
	} else if(!strcmp(analyzer, "/logout")) {
		if(client->logged) {
			printf("%s logged out\n", client->login);
			server_reply_to("Logged out", client);
			client->logged = 0;
		} else {
			server_reply_to("Tbl MyDAK", client);
		}
		return;
	}

	printf("In process_message() : %s\n", message);
	MY_MESSAGE msg = create_message(client, message);
	send_message(&msg, client, clients);
	puts("Sending...");
}



int find_by_login(char* log, CLIENTS* clients, INFO_ABOUT_CLIENT* cl)
{
	int i;
	for(i=0; i < clients->client_list_size; ++i)	{
		if(!strcmp(log, clients->client_list[i].login))	{
			*cl = clients->client_list[i];
			return i;
		}
	}
	return -1; // Returns -1 if not found
}



int server_reply_to(char* str, INFO_ABOUT_CLIENT* client)
{
	char message[1024] = "Server: ";
	strcat(message, str);
	return send(client->adress, message, strlen(message)+1, 0);
}
