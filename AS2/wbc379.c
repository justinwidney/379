

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/pem.h>


#define	MY_PORT	2223


// GLOBAL VARIABLES FOR ENCRYPTION
EVP_CIPHER_CTX ctx;
unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
unsigned char iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

unsigned char outbuf[1024];
unsigned char debuf[1024];

int outlen, tmplen, delen, i;



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


// based off of decrypt.c

int encrypt(unsigned char message[]) {

	
	
	
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);

	message[strlen(message) -1] = 0;

	if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, message, strlen(message)))
    {
        /* Error */
        return 0;
    }

	if(!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen))
    {
        /* Error */
        return 0;
    }

	outlen += tmplen;

	EVP_CIPHER_CTX_cleanup(&ctx);

	// buffer should contain ciphertext
}


int decrypt(char *encrpyted_message) {
	int remainingBytes;

	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);

	

	if(!EVP_DecryptFinal_ex(&ctx, debuf + delen, &remainingBytes)) {
        /* Error */
        return 0;
    }

	delen+=remainingBytes; // length till last block + number of bytes that are not padded

   	EVP_CIPHER_CTX_cleanup(&ctx);

}










