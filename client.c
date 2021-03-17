#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

extern int errno;
static int sockfd;
void * msg_print(void *fd);
void * msg_type(void *fd); 
void print_error(char *);

int main(int argc, char *argv[]) {
	struct addrinfo *host_ai;
	struct addrinfo hint;
	int err;
    
	if (argc != 2) {
        	print_error("usage: client hostname");
    }

	hint.ai_flags = 0;
    	hint.ai_family = AF_INET;
    	hint.ai_socktype = SOCK_STREAM;
    	hint.ai_protocol = 0;
    	hint.ai_addrlen = 0;
    	hint.ai_canonname = NULL;
    	hint.ai_addr = NULL;
    	hint.ai_next = NULL;

	if ((err = getaddrinfo(argv[1], "tokenserver", &hint, &host_ai)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(err));
		return 0;
	}

	printf("creating socket\n");
	if ((sockfd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("Error creating socket");
	}
	printf("socket created\n");

	printf("attempting Connection\n");
	if (connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen) != 0) {
		close(sockfd);
		printf("can't connect to %s\n", argv[1]);
		print_error("Error connecting to server");
	}
	printf("connection made...\n"); 
	freeaddrinfo(host_ai);
	
	pthread_t tid;
	if(pthread_create(&tid,NULL,msg_print,&sockfd)!=0)
	{
		perror("Error creating thread");
	}

	printf("running thread %d\n", tid);
       if (pthread_detach(tid) != 0)
	 {
        	  perror("detach error");
         }
 	while(1)
        {
                char sim[30];
                memset(sim,'\0',30);
                fgets(sim,30,stdin);
		sim[strlen(sim)-1]= '\0';
		if (send(sockfd,sim,30,0)!=30)
                {
                      print_error("Error\n");
                }
}
}

void * msg_print(void *fd)
{
	
	int sockfd= *(int *) fd;

		
	
		
	while(1)
		{
		
			int clfd;
			memset(&clfd,'\0',4);
			recv(sockfd,&clfd,4,0);
		
			char name[30];
			memset(name,'\0',30);
			if (recv(sockfd,name,30,0)!=30)
			{
			//	print_error("Error\n");				
			}
			printf("User %d says: %s\n",clfd, name);
		}
}		


//void * msg_type(void *fd)
//{
//	int sockfd= *(int *) fd;
 //	while(1)
//	{
//		char sim[30];
//		memset(sim,'\0',30);
//		printf("Please input your name:");
//		scanf("%s",sim);
//		if (send(sockfd,sim,30,0)!=30)
//		{
//			print_error("Error\n")
//		}
		
//	}
		

//}


void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
	exit(1);
}


// END OF FILE	
