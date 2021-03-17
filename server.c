/*
 * socket_exam.c
 * derived from Advanced Programming in the UNIX Environment, 3rd Edition
 *
 * Eric McGregor 02/27/20
 */

#include <string.h>	
#include <stdio.h>	
#include <stdlib.h> 
#include <unistd.h>	
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>  
#include <signal.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>	
#include <sys/socket.h> 
#include <pthread.h>
//#include "box.h"

#define QLEN 128

#ifndef HOST_NAME_MAX 
	#define HOST_NAME_MAX 256 
#endif

#define MAX_THREADS 1000000L

struct box
{
	int height;
	int length;
	int width;
};
extern int errno;
int token = 1700; 
int descri[1096];
int size=0;
void * thread_do(void *arg);
void printids(const char *s);
void remove_semaphore_set(char* user, int semid); 

int main(int argc, char *argv[]) 
{
	char *host_name = malloc(HOST_NAME_MAX);
	memset(host_name, 0, HOST_NAME_MAX);

	if (gethostname(host_name, HOST_NAME_MAX) < 0) {
		perror("gethostname error");
		return(-1);
	}

	printf("host name: %s\n", host_name);

	struct addrinfo 	*host_ai;
	struct addrinfo 	hint;

	hint.ai_flags = 0;
    	hint.ai_family = 0;
    	hint.ai_socktype = SOCK_STREAM;
    	hint.ai_protocol = 0;
    	hint.ai_addrlen = 0;
    	hint.ai_canonname = NULL;
    	hint.ai_addr = NULL;
    	hint.ai_next = NULL;

	int err;
    	if ((err = getaddrinfo(host_name, "tokenserver", &hint, &host_ai)) != 0) {
        	printf("getaddrinfo error: %s\n", gai_strerror(err));
        	return 0;
    	}

   	int host_fd;
	if ((host_fd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		perror("unable to create socket"); 
	}
	printf("socket created [%d]\n", host_fd);

    	if (bind(host_fd, host_ai->ai_addr, host_ai->ai_addrlen) < 0) {
   	     perror("unable to bind to socket");
    	}
	printf("bind returned success\n");

	freeaddrinfo(host_ai);

    	if (listen(host_fd, QLEN) < 0) {
        	perror("listen failed");
    	}
	printf("listen returned success\n");

	struct sockaddr client_sockaddr;
	socklen_t client_sockaddr_len = sizeof(client_sockaddr);
	
	for (;;) {

		printf("waiting for connection ...\n");
    	int clfd = accept(host_fd, &client_sockaddr, &client_sockaddr_len);
       	if (clfd < 0) {
			perror("accept error");
		}
		printf("accepted connection, socket [%d]\n", clfd);

		pthread_t tid;	
		if (pthread_create(&tid, NULL, thread_do, &clfd) != 0) {
			perror("Error creating thread");
		}
				
		printf("running thread %d\n", tid);	
		if (pthread_detach(tid) != 0) {
			perror("detach error");
		}


		
		
	} 
}



void * thread_do(void *fd) 
{
	
	int clfd = *(int*) fd;
	
	descri[size]= clfd;
	size++;
	
	while(1)
	{
		char name[30];
		memset(name,'\0',30);
		recv(clfd,name,30,0);
	
		int i ;
		for (i=0;i<size;i++)
		{
			
			send(descri[i],&clfd,4,0);	
			printf("%d\n",i);	
			if (send(descri[i],name,30,0)==30)
			{
				printf("send 30");
			}
		}
		//printf("loop down");
	
		}

	close(clfd);
}

// END OF FILE
