TARGET=CHAT
CC=gcc
libs=-lpthread
CFLAGS= -g -Wall

all:&(TARGET)

cl:	cli.c
	 $(CC) $(CFLAGS) -pthread  -o cl cli.c

myfunctions.o: myfunctions.h mystructs.h myfunctions.c
	gcc -g myfunctions.c -c
serv: 	myfunctions.o
	gcc -g myfunctions.o  myserv.c -o serv
clean:
	rm cl serv -f
rebuild: clean all



&(TARGET): cl serv 

