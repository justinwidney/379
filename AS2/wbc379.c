

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

char *base64encode (const void *b64_encode_this, int encode_this_many_bytes){
    BIO *b64_bio, *mem_bio;      //Declares two OpenSSL BIOs: a base64 filter and a memory BIO.
    BUF_MEM *mem_bio_mem_ptr;    //Pointer to a "memory BIO" structure holding our base64 data.
    b64_bio = BIO_new(BIO_f_base64());                      //Initialize our base64 filter BIO.
    mem_bio = BIO_new(BIO_s_mem());                           //Initialize our memory sink BIO.
    BIO_push(b64_bio, mem_bio);            //Link the BIOs by creating a filter-sink BIO chain.
    BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);  //No newlines every 64 characters or less.
    BIO_write(b64_bio, b64_encode_this, encode_this_many_bytes); //Records base64 encoded data.
    BIO_flush(b64_bio);   //Flush data.  Necessary for b64 encoding, because of pad characters.
    BIO_get_mem_ptr(mem_bio, &mem_bio_mem_ptr);  //Store address of mem_bio's memory structure.
    BIO_set_close(mem_bio, BIO_NOCLOSE);   //Permit access to mem_ptr after BIOs are destroyed.
    BIO_free_all(b64_bio);  //Destroys all BIOs in chain, starting with b64 (i.e. the 1st one).
    BUF_MEM_grow(mem_bio_mem_ptr, (*mem_bio_mem_ptr).length + 1);   //Makes space for end null.
    (*mem_bio_mem_ptr).data[(*mem_bio_mem_ptr).length] = '\0';  //Adds null-terminator to tail.
    return (*mem_bio_mem_ptr).data; //Returns base-64 encoded data. (See: "buf_mem_st" struct).
}

char *base64decode (const void *b64_decode_this, int decode_this_many_bytes){
    BIO *b64_bio, *mem_bio;      //Declares two OpenSSL BIOs: a base64 filter and a memory BIO.
    char *base64_decoded = calloc( (decode_this_many_bytes*3)/4+1, sizeof(char) ); //+1 = null.
    b64_bio = BIO_new(BIO_f_base64());                      //Initialize our base64 filter BIO.
    mem_bio = BIO_new(BIO_s_mem());                         //Initialize our memory source BIO.
    BIO_write(mem_bio, b64_decode_this, decode_this_many_bytes); //Base64 data saved in source.
    BIO_push(b64_bio, mem_bio);          //Link the BIOs by creating a filter-source BIO chain.
    BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);          //Don't require trailing newlines.
    int decoded_byte_index = 0;   //Index where the next base64_decoded byte should be written.
    while ( 0 < BIO_read(b64_bio, base64_decoded+decoded_byte_index, 1) ){ //Read byte-by-byte.
        decoded_byte_index++; //Increment the index until read of BIO decoded data is complete.
    } //Once we're done reading decoded data, BIO_read returns -1 even though there's no error.
    BIO_free_all(b64_bio);  //Destroys all BIOs in chain, starting with b64 (i.e. the 1st one).
    return base64_decoded;        //Returns base-64 decoded data with trailing null terminator.
}





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


	
   	 unsigned char * data_to_encode;  //The string we will base-64 encode.
   	 data_to_encode = malloc(outlen); // allocate size of string
   	 memcpy(data_to_encode,outbuf,outlen); // copy string to pointer
  	 int bytes_to_encode = outlen; //Number of bytes in string to base64 encode.
   	 unsigned char *base64_encoded = base64encode(data_to_encode, bytes_to_encode);   //Base-64 encoding.

}


int decrypt(char *encrpyted_message) {
	int remainingBytes;

	
  	int bytes_to_decode = strlen(base64_encoded); 	//Number of bytes in string to base64 decode.
  	unsigned char *base64_decoded = base64decode(base64_encoded, bytes_to_decode);   //Base-64 decoding.

   	outlen = strlen(base64_decoded); // size of new base64 string
	free(base64_encoded);		// clear the mem



	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);

	

	if(!EVP_DecryptFinal_ex(&ctx, debuf + delen, &remainingBytes)) {
        /* Error */
        return 0;
    }

	delen+=remainingBytes; // length till last block + number of bytes that are not padded

   	EVP_CIPHER_CTX_cleanup(&ctx);

	

}










