#ifndef MY_STRUCTS
#define MY_STRUCTS

#define MAX_CLIENTS	1000
#define MAX_REG_LIST 	1000
#define TRUE		1
#define FALSE  		0
#define PORT 		8888


typedef struct 	{
	char login[1024];
	char password[40];
	int adress;
	int logged;
	int color;
} INFO_ABOUT_CLIENT;

typedef struct	{
	INFO_ABOUT_CLIENT client_list[MAX_REG_LIST];
	int client_list_size;
	int current_client_maximum;
} CLIENTS;

typedef struct	{
	char message[1024];
	char login[100];
	int color;
} MY_MESSAGE;


#endif
