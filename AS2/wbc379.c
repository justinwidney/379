

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define	MY_PORT	2223


int main(int argc, char *argv[]) {

	int	s, number, portnumber;

	char* hostname;


	struct	sockaddr_in	server;

	struct	hostent		*host;

	char message[256];

	host = gethostbyname ("localhost");

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}



	/*if(argc != 3) { return 0;}
	
	hostname = argv[1];	// get hostname

	portnumber = atoi(argv[2]);	// get portnumber 
	*/


	
		while (1) {

		s = socket (AF_INET, SOCK_STREAM, 0);  		// create connection 

		if (s < 0) {
			perror ("Client: cannot open socket");
			exit (1);
		}

		bzero (&server, sizeof (server));		//all values in a buffer to zero
		bcopy (host->h_addr, & (server.sin_addr), host->h_length); // set fields in serv+adder

		server.sin_family = host->h_addrtype;
		server.sin_port = htons (MY_PORT);
		//server.sin_port = htons (portnumber);

		if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
			perror ("Client: cannot connect to server");
			exit (1);
		}

		bzero(message, 256);
		fgets(message, 255, stdin);	
		
		int n = read (s, message, sizeof (number));
		if(n < 0){
			perror ("ERROR reading from socket");
			exit(1);
		}

		close (s);
		fprintf (stderr, "Process %d gets number %d\n", getpid (),
			ntohl (number));
		sleep (2);
	}
	

	










}
