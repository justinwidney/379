#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <pthread.h>
#include <signal.h>
#include <setjmp.h>

#define	MY_PORT	2222

/* ---------------------------------------------------------------------
 This	is  a sample server which opens a stream socket and then awaits
 requests coming from client processes. In response for a request, the
 server sends an integer number  such	 that  different  processes  get
 distinct numbers. The server and the clients may run on different ma-
 chines.
 --------------------------------------------------------------------- */
int snew;
pthread_t thread_id;
jmp_buf readonly_memory;
int flag = 0;

 void sigint_handler(int signo) {
    if (signo == SIGINT) {
        close (snew);
        flag = 1;
        siglongjmp(readonly_memory,1);
        //int pthread_kill(pthread_t thread, int sig);
    }
    return;
}


 void *thread_connections( void* acc_socket) {


 	int sock = *(int *)acc_socket;
 	int message_size, i;

 	char *message, client_message[1000];
	char c[1000];


 	/*
 	** Create function to get whiteboard size
 	*/

	printf("not sure how this prints");
 	// continous loop
 	while(1)
	{
    sigsetjmp(readonly_memory,1);


    if(flag == 1) {
      printf("Closing\n");
      close (sock);
      exit(1);
    }

		bzero(c,1000);
		recv(sock,c,999,0);

		//printf("Printing character array c as a string is: %s\n",c);
		//printf("Character array c has the following byte values:\n");
		//for (i = 0; i < 25; i++){
		//printf("c[%d] = %d\n",i,c[i]);
		//}

		write (sock, c, 999);
		return 0;

 		}

 	// client disconnected
 	//if(message_size == 0) {
		//close (sock);
 //	}


 	return 0;
 }


int main()
{
	int	sock,  fromlength, number, outnum, a;

  struct sigaction seg_act;

	pid_t pid = 0;
	pid_t sid = 0;


  seg_act.sa_handler = &sigint_handler;
  sigemptyset(&seg_act.sa_mask);
  sigaddset(&seg_act.sa_mask, SA_NODEFER);
  sigaction(SIGINT, &seg_act, NULL);


	struct	sockaddr_in	master, client;
	unsigned char outbuf[1024];
	unsigned char debuf[8];
	int outlen, tmplen, delen, i=0;

	unsigned char intext[200];
	bzero(intext,200);
	char buf[200];


	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = INADDR_ANY;
	master.sin_port = htons (MY_PORT);

	if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	number = 0;

	listen (sock, 5);

	int c = sizeof(struct sockaddr_in);


	while((snew = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c))){

		// accept the connection
		//snew = accept (sock, (struct sockaddr *)&client, (socklen_t*)&c);

		if (snew < 0) {
			//perror ("Server: accept failed");
			exit (1);
		}

		printf("test 3 \n");
		// create the thread
		printf("Creating thread %d\n",i);
		pthread_create(&thread_id, NULL, thread_connections, (void *) &snew);
		//outnum = htonl (number);
		i++;



	}
}
