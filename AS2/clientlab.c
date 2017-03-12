#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <strings.h>
#include <string.h>
#include <errno.h>

#define	 MY_PORT  2222

/* ---------------------------------------------------------------------
 This is a sample client program for the number server. The client and
 the server need not run on the same machine.
 --------------------------------------------------------------------- */
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



int main()
{
	int	s, number, outnum;
  int i;
	struct	sockaddr_in	server;
  char c;


	struct	hostent		*host;

	host = gethostbyname ("localhost");

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}

  // if (argc == 3)

  FILE *fp;
  fp = fopen ("keys.txt","r");
  if (fp == NULL) {
     printf ("File not created okay, errno = %d\n", errno);
     return 1;
   }
   unsigned char testkey[16];
   i =0;
   while((c=fgetc(fp))!=EOF){

     if (c == '\n') {
       memset(members, 0, sizeof members);
       i=0;
     }


         testkey[i] = c;
         i++;



     }
    //printf("No possible decryption");
    for (i=0; i< 17; i++)
    printf("%c\n",testkey[i]);

    i = 0;

    fclose(fp);

		while (1) {
    char buf[1024];

    sprintf(buf,"Enter a string to be encrypted\n");

		s = socket (AF_INET, SOCK_STREAM, 0);

		if (s < 0) {
			perror ("Client: cannot open socket");
			exit (1);
		}

		bzero (&server, sizeof (server));
		bcopy (host->h_addr, & (server.sin_addr), host->h_length);
		server.sin_family = host->h_addrtype;
		server.sin_port = htons (MY_PORT);

		if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
			perror ("Client: cannot connect to server");
			exit (1);
		}

		unsigned char outbuf[1024];
		unsigned char debuf[1024];
		int outlen, tmplen, delen, i;
		unsigned char key2[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    unsigned char key[] = {3,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    unsigned char *keyp;
    keyp = key;

		unsigned char iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
		EVP_CIPHER_CTX ctx;
		//unsigned char intext[] = "Mark Stevens is my friend for life man.";
		unsigned char intext[200];


		char c[1024];


		write(1,buf,strlen(buf));
		read(0,intext,199);
		intext[strlen(intext) - 1] = 0;
		EVP_CIPHER_CTX_init(&ctx);
		EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key2, iv);
		//printf("\nintext is %d bytes in length\n\n",strlen(intext));
		if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, intext, strlen(intext)))
		{
				/* Error */
				return 0;
		}
		bzero(buf,1024);
		bzero(intext,200);
		bzero(outbuf,1024);
		bzero(debuf,1024);

		/* Buffer passed to EVP_EncryptFinal() must be after data just
		 * encrypted to avoid overwriting it.
		 */
		if(!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen))
		{
				/* Error */
				return 0;
		}
		outlen += tmplen;
		EVP_CIPHER_CTX_cleanup(&ctx);

		unsigned char * data_to_encode;  //The string we will base-64 encode.
		data_to_encode = malloc(outlen);
		memcpy(data_to_encode,outbuf,outlen);
		int bytes_to_encode = outlen; //Number of bytes in string to base64 encode.

		printf("bytes to encode = %d\n", bytes_to_encode);
		printf("Entered string = %s\n", outbuf );


		//unsigned char *base64_encoded = base64encode(data_to_encode, bytes_to_encode);   //Base-64 encoding.
		//printf("encoded %s\n", base64_encoded);

		//for (i = 0; i < 25; i++){
		//printf("c[%d] = %d\n",i,base64_encoded[i]);
		//}

		write (s, outbuf, sizeof(outbuf));

		bzero(c,1000);
		recv(s,c,999,0);

		bzero(buf,200);
		//for (i = 0; i < 24; i++){
		//printf("c[%d] = %d\n",i,c[i]);
		//buf[i] = c[i];
		//}

		//printf("recvied from socket = %s\n",c);


		int bytes_to_decode = strlen(c); //Number of bytes in string to base64 decode.
		//printf("bytes = %d\n", bytes_to_decode );
    //unsigned char *base64_decoded = base64decode(c, bytes_to_decode);   //Base-64 decoding.

		//printf("%s\n", base64_decoded);

  	//outlen = strlen(base64_decoded);
		//printf("outlen = %d\n", outlen );
   	//free(base64_decoded);                //Frees up the memory holding our base64 decoded data.
    EVP_CIPHER_CTX_init(&ctx);
		//EVP_CIPHER_CTX_set_padding(&ctx, 0);

    while(1) {
    int a = EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, keyp, iv);
    //printf("\n%d\n", a);

    if(!EVP_DecryptUpdate(&ctx, debuf, &delen, c, outlen))
    {
        /* Error */
				printf("Error1\n");

        return 0;
    }
    int remainingBytes;
		//printf("%d\n", delen);
		printf("string = %s\n", debuf);

    if(!EVP_DecryptFinal_ex(&ctx, debuf + delen, &remainingBytes))
    {
        /* Error */
				ERR_print_errors_fp(stdout);
				printf("Error2\n");
        keyp = key2;

        continue;
        return 0;
    }
    delen+=remainingBytes;
    EVP_CIPHER_CTX_cleanup(&ctx);
    break;
    }
		//printf("%s", debuf);

	}
}
