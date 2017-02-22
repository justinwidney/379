#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define	MY_PORT	2223


void returnmessages(int);


int main(int argc, char *argv[])
{
	int	sock, snew, fromlength, number, outnum;

	struct	sockaddr_in	master, from;


	//if (argc< 2) {return 0;}
	

	
	

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

	

	listen (sock, 5);

	while (1) {
		fromlength = sizeof (from);
		snew = accept (sock, (struct sockaddr*) & from, & fromlength);
		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}
		
		returnmessages(snew);

		write (snew, &outnum, sizeof (outnum));
		close (snew);
		
	}
}

// handle the return messages
void returnmessages( int sock)

{

	int n;
	char buffer[256];

	bzero(buffer, 256);
	n = read(sock, buffer, 255);
	if (n<0) perror("Error");

	printf("%s\n",buffer);

	if(buffer[0] = '?'){
	printf("responsetest");
	}

	if(buffer[0] = '!'){
	printf("responsetest2");
	}

	if(buffer[0] = '@'){
	printf("responsetes3");
	}




}








