

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

int decrypt(char *encrpyted_message, char* filename);



// GLOBAL VARIABLES FOR ENCRYPTION
EVP_CIPHER_CTX ctx;
unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
unsigned char iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

unsigned char outbuf[1024];
unsigned char debuf[1024];

int outlen, tmplen, delen, i;



int main(int argc, char *argv[]) {

	int	s, number, portnumber, x, n;
  char entrynumber[1000];


	char* hostname;

  int keyfile_flag = 0;
  char* keyfile_name;

	struct	sockaddr_in	server;
	struct	hostent		*host;

	char message[256];
  unsigned char intext[200];

  FILE *fp;

  unsigned char testkey[16];

	host = gethostbyname ("localhost");

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}


    if (argc == 4) {
      //hostname = argv[1];
      //portnumber = atoi(argv[2]);	// get portnumber
      keyfile_flag = 1;
      keyfile_name = argv[3];
      fp = fopen(keyfile_name, "r");
      if (fp == NULL){
        printf("File not created\n");
        return 1;
      }
      fclose(fp);

    }

    if(argc != 3 && keyfile_flag ==0){
      printf("needs hostname & portnumber\n");
      return 0;
    }

	   hostname = argv[1];	// get hostname
     portnumber = atoi(argv[2]);	// get portnumber



		while (1) {

		s = socket (AF_INET, SOCK_STREAM, 0);  		// create connection

		if (s < 0) {
			perror ("Client: cannot open socket");
			exit (1);
		}

		bzero (&server, sizeof (server));		//all values in a buffer to zero
		//bcopy (host->h_addr, & (server.sin_addr), host->h_length); // set fields in serv+adder

		server.sin_family = host->h_addrtype;
		server.sin_addr.s_addr = inet_addr(hostname);
		server.sin_port = htons (portnumber);

		if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
			perror ("Client: cannot connect to server");
			exit (1);
		}

		unsigned char key2[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    char buf[1024];
    char c[1000];

    char encryption_addon[] = "CMPUT379 Whiteboard Encrypted v0\n";

    unsigned char *keyp;
    keyp = key;
    memset(buf, 0, sizeof(buf));

    printf("Options: \n 1: use whiteboard \n 2:exit \n Enter 1 or 2: ");
    scanf("%d", &n);

    if (n == 2) {
      close(s);
      printf("exiting\n");
      return 0;

    }

    printf("Are you\n 1: viewing an entry \n 2: updating an entry ");
    scanf("%d", &n);

    if(n == 1){

      printf("What entry would you like to see: \n");
      scanf("%s", entrynumber);

      //printf("you entered %s\n",entrynumber);
      buf[0] = '?';

      // add on the entry number
      strcat(buf, entrynumber);
      //printf("first concat = %s\n", buf );
      buf[strlen(buf)] = '\n';

      write (s, buf, sizeof(buf));


    }

    else{

    char tempstring[1000];
    printf("What entry would you like to see: \n");
    scanf("%s", entrynumber);

    sprintf(tempstring,"Enter a string to be sent\n");


    write(1,tempstring,strlen(tempstring));

		read(0,intext,199);
    intext[strlen(intext) - 1] = 0;


    printf("please enter 0 for non-encrypted and 1 for encrypted: ");
    scanf("%d", &n);

    if(n == 1){
      // call encryption
    }

    buf[0] = '@';
    strcat(buf, entrynumber);
    char stringlength[0];

    stringlength[0] = (char)strlen(tempstring);
    strcat(buf, stringlength);
    buf[strlen(buf)] = '\n';
    strcat(buf, tempstring);
    buf[strlen(buf)] = '\n';


    write (s, buf, sizeof(buf));
    }

    // recv the message
    int abc = recv(s,c,999,0);

    if (strlen(c) == 0){
      close(s);
      printf("connect with server was terminated");
      return 0;
    }

    int message_length;
    char message_length_c[1000];

    i =0, x=0 ;
    memset(message_length_c, 0, sizeof(message_length_c));

    while(1){

    // handle regular responses
    if(c[i] == 'p'){

      while (1) {
        if(c[i] == '\n'){
          break;
        }

        char cx[0];
        cx[0] = c[i+1];
        strcat(message_length_c, cx);
        i++;
      }

      message_length = atoi(message_length_c);

      for(x=0; x < message_length+i; x++){
        printf("%c", c[x]); // print all values in
      }

      break;

    }

    // handle error message
    if(c[i] == 'e'){

      while (1) {
        if(c[i] == '\n'){
          break;
        }

        char cx[0];
        cx[0] = c[i+1];
        strcat(message_length_c, cx);
        i++;
      }

      message_length = atoi(message_length_c);

      // print thre recieved message
      for(x=0; x< message_length + i; x++){
        printf("%c", c[x]);
      }

      break;
    }



    if(c[i] == 'c'){
      decrypt(c, keyfile_name);
      break;
    }

    i++;

    }


		if(n < 0){
			perror ("ERROR reading from socket");
			exit(1);
		}


	}













}


// based off of decrypt.c

char* encrypt(unsigned char message[]) {

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


    // 64 bit encode it
   	 unsigned char * data_to_encode;  //The string we will base-64 encode.
   	 data_to_encode = malloc(outlen); // allocate size of string
   	 memcpy(data_to_encode,outbuf,outlen); // copy string to pointer
  	 int bytes_to_encode = outlen; //Number of bytes in string to base64 encode.
   	 unsigned char *base64_encoded = base64encode(data_to_encode, bytes_to_encode);   //Base-64 encoding.

    return base64_encoded;
}


int decrypt(char *encrpyted_message, char* filename ) {

    FILE *fp;

    fp = fopen (filename,"r");
    unsigned char testkey[20];
    int remainingBytes;
    char c;

    int bytes_to_decode = strlen(encrpyted_message); //Number of bytes in string to base64 decode.
    unsigned char *base64_decoded = base64decode(encrpyted_message, bytes_to_decode);   //Base-64 decoding.

    outlen = strlen(base64_decoded);

    //bzero(debuf,1024);
    memset(testkey, 0, sizeof(testkey));

    while((c=fgetc(fp))!=EOF){

         // try the key
         if(c == '\n'){

           //Todo Needs to conver base64decode into a char array
          int key_bytes = strlen(testkey);
          unsigned char* base64_decoded_key = base64decode(testkey, bytes_to_decode);
          printf("decoded= %s\n", base64_decoded);

          EVP_CIPHER_CTX_init(&ctx);
         	EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, base64_decoded_key, iv);

          if(!EVP_DecryptUpdate(&ctx, debuf, &delen, base64_decoded, outlen))
          {printf("Error1\n");return 0;}

           if(!EVP_DecryptFinal_ex(&ctx, debuf + delen, &remainingBytes)){
             memset(testkey, 0, sizeof(testkey));
             i=0;
             continue;
           }

            delen+=remainingBytes;
            EVP_CIPHER_CTX_cleanup(&ctx);
            int i;
            for (i = 0; i< delen; i++){
              printf("%c", debuf[i]);
            }

            return 0;
         }
         testkey[i] = c;
         i++;

         // found no key that works
       }
       printf("no key was able to decrypt the message\n");


  //outlen = strlen(base64_decoded); // size of new base64 string
	//free(base64_encoded);		// clear the mem




}
