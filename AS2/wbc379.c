#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <math.h>

#include <openssl/evp.h>
#include <openssl/pem.h>


#define	MY_PORT	2223
#define BUFFERSIZE 100

jmp_buf readonly_memory;
int s;


void sigint_handler(int signo) {
   if (signo == SIGINT) {
       printf("\nexiting\n");
       close (s);
       exit(1);
       //int pthread_kill(pthread_t thread, int sig);
   }
}


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
unsigned char* encrypt(unsigned char* message, char* filename);

char encryption_addon[] = "CMPUT379 Whiteboard Encrypted v0\n";


// GLOBAL VARIABLES FOR ENCRYPTION
EVP_CIPHER_CTX ctx;
//unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

unsigned char iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//unsigned char *base64_encoded;
//unsigned char* base64_decoded_key ;
//unsigned char * encrypted_message_full;  //The string we will base-64 encode.
unsigned char *base64_encoded ;
unsigned char* base64_decoded_key ;
unsigned char * encrypted_message_full;



unsigned char outbuf[1024];
unsigned char debuf[1024];

int outlen, tmplen, delen, i, key_bytes;


int firstime = 0;

int main(int argc, char *argv[]) {

  base64_encoded = malloc(sizeof (unsigned char));
  base64_decoded_key = malloc(sizeof *base64_decoded_key);
  encrypted_message_full=  malloc(sizeof *encrypted_message_full);


  struct sigaction seg_act;

  seg_act.sa_handler = &sigint_handler;
  sigemptyset(&seg_act.sa_mask);
  sigaddset(&seg_act.sa_mask, SA_NODEFER);
  sigaction(SIGINT, &seg_act, NULL);
 
  EVP_CIPHER_CTX_init(&ctx);

  EVP_CIPHER_CTX_set_padding(&ctx, 0);


	int number, portnumber, x, n;
  	char  server_message[100];

	char* hostname;
	int entrynumber;
	char ctest, ftest, newlinetest, newlinetest2;
        int sizetest, entrytest;
  	int keyfile_flag = 0, encrypt_flag = 0;
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

    char buf[1024];
    char encryption_addon[] = "CMPUT379 Whiteboard Encrypted v0\n";
    bzero(&buf, strlen(buf));
    int len = read(s, buf, 1024);
    if(firstime == 0) {
      printf("%s\n", buf);
      firstime = 1;
    }

    if(len < 33) {
      printf("Inital handshake not complete. Exiting...\n");
      exit(0);
    }
    int i = 0; int whiteboard_size; char sizeStr[50]; sizeStr[49] = '\0';
    while(1) {
      if(buf[i] == '\n') {
        i++;
        int n = 0;
        while(1) {
          if(buf[i] == '\n') {
            whiteboard_size = strtol(sizeStr, NULL, 10);
            break;
          }
          sizeStr[n] = buf[i];
          n++;
          i++;
        }
        break;
      }
      i++;
    }


    char c[1000];

    memset(buf, 0, sizeof(buf));

    while (1) {
      while(1){
        printf("Options: \n 1: use whiteboard \n 2: exit \n Enter 1 or 2: ");
        scanf("%d", &n);

        if (n == 2) {
          close(s);
          printf("exiting\n");
          return 0;
        }
        if(n == 1){
          break;
        }
        printf("enter a valid option\n: ");
      }
      printf("Are you\n 1: viewing an entry \n 2: updating an entry\n");

      while(1){
        scanf("%d", &n);
        if(n == 1){
          char *p, t[100];
          while (fgets(t, sizeof(t), stdin)) {
            entrynumber = strtol(t, &p, 10);
            if (p == t || *p != '\n') {
              printf("Please enter an integer entry: ");
            } else {break};
          }
        sprintf(buf, "?%d\n", entrynumber);
        write (s, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
	
        int len = read(s, buf, sizeof(buf));
        /* error handling */
		
        sscanf(buf, "%c%d%c%d%c", &ftest, &entrytest, &ctest, &sizetest, &newlinetest);

        if (ftest != '!' || entrytest < 0 || newlinetest != '\n' || sizetest < 0) {
          printf("Improper server message, exiting");
        }  
        if(ctest != 'c' || ctest != 'e'|| ctest != 'p'){
          printf("Improper server message2, exiting");
        }	
        // decryption
        if(buf[2] == 'c'){
          printf("message is decrpyted\n");
          //bzero(&buf, strlen(buf));
          /*if(ctest != 'c' || ftest != '!' || newlinetest != '\n'){
          printf("bad protocol\n");
          return 0;
          } */
          if(argc != 4){
            printf("no keyfile to decode message\n");
            break;
          }
          sscanf(buf, "%c%d%c%d%c%s%c", &ftest, &entrytest, &ctest, &sizetest, &newlinetest, c, &newlinetest2);
          //printf("our message is: %s", c);
          printf("about to decrpy: %s\n", c);
          decrypt(c, keyfile_name);
          break;
        }
        int i = 0; //
        sscanf(buf, "%c%d%c%d%c", &ftest, &entrytest, &ctest, &sizetest, &newlinetest);
	
        int premessage = strlen(&ftest) +strlen(&ctest);
        int nDigits1 = floor(log10(abs(entrytest))) + 1;
        int nDigits2;

        if(sizetest != 0){
          nDigits2 = floor(log10(abs(sizetest))) + 1;
        }
        else{
          nDigits2 = 0;
        }

        //char *messagepointer;
        //messagepointer = buf;
        //messagepointer = messagepointer + premessage;
        
        //printf("%d, %d, %d", premessage, nDigits1, nDigits2);
        if(sizetest >0){
          sizetest--;	
        }
        premessage = premessage + nDigits1 + nDigits2 + sizetest;
	  
        printf("Here is the entry: [");
        while(i < premessage){
          printf("%c", buf[i]);
          i++;
        }
        printf("]\n");
        break;
      }
        if(n == 2){
          unsigned char tempstring[1000];
          memset(tempstring, 0, 1000);
          int ENTRY_NUMBER;

          //printf("What entry would you like to change: \n");
          //while(1) {
          ///scanf("%d", &ENTRY_NUMBER);
          //}
          char *p, t[100];
    	

          while (fgets(t, sizeof(t), stdin)) {
            ENTRY_NUMBER = strtol(t, &p, 10);
            if (p == t || *p != '\n') {
              printf("Please enter an integer entry: ");
            } else {break};
          }
          //printf("Enter a string to be sent\nEnter NULL for a blank entry:\n");
          char *text = calloc(1,1), buffer[BUFFERSIZE];
          //getchar();
    
          printf("Enter a message: Ctrl D to exit, Last Enter is ignored:\n");
          while( fgets(buffer, BUFFERSIZE , stdin) ) /* break with ^D or ^Z */ {
            text = realloc( text, strlen(text)+1+strlen(buffer) );
            //if(strcmp(buffer, "quit\n") ==0) {
            //break;			 	 
            //}/* error handling */
            strcat( text, buffer ); /* note a '\n' is appended here everytime */
            //printf("%s", buffer);
        }
        char* pointer;
        pointer = text;
        //pointer++;
        //char deststring[100];
        //printf("%s", pointer);	

        memcpy(tempstring, pointer, strlen(text)-1);

        //printf("\ntext:%s",tempstring);
        //printf("test2");
        //scanf("%s",tempstring);
	  
        int flagNULL = 0;
        if(tempstring[0] == 'N' && tempstring[1] == 'U' && tempstring[2] == 'L' && tempstring[3] == 'L'){
          flagNULL = 1;
          bzero(tempstring, strlen(tempstring));
        }
        if(flagNULL != 1) {
          printf("please enter 0 for non-encrypted and 1 for encrypted: ");
          scanf("%d", &n);
        }
        else{
          n = 0;
          flagNULL = 0;
        }
        if(n == 1){
          pointer = tempstring;
          printf("encrypting message %s\n", tempstring);
          unsigned char* encoded_message = encrypt(pointer, keyfile_name);
          printf("final encrypted message is= %s\n", encoded_message );

          //move our message into temp string
          //memset(tempstring,0,sizeof(tempstring));
          //memcpy(tempstring, encoded_message, sizeof(encoded_message));
          int xy = strlen(encoded_message);
          sprintf(buf, "@%dc%d\n%s\n", ENTRY_NUMBER, xy, encoded_message);
        }
        else {
          int xy = strlen(tempstring);
          sprintf(buf, "@%dp%d\n%s\n", ENTRY_NUMBER, xy, tempstring);
        }
        write (s, buf, strlen(buf));
        bzero(buf, sizeof(buf));
        read(s, buf, sizeof(buf));
        printf("Reponse: %s", buf);
        break;
        }

        else {
          printf("enter a valid option\n: ");
          break;
        }
    }
      i++;
      continue;
  }
  if(n < 0){
    perror ("ERROR reading from socket");
    exit(1);
  }

}

// based off of decrypt.c

unsigned char* encrypt(unsigned char* message, char* filename) {

  memset(outbuf, 0, strlen(outbuf));
  //memset(message, 0, sizeof(encrypted_message_full));

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  //printf("getting key");

  fp = fopen (filename,"r");
  if (fp == NULL) {
	printf("can't encrypt without keyfile");
        exit(EXIT_FAILURE);
  }

  char testkey[128];
  char c;

  

  int i =0;

  while((c=fgetc(fp))!=EOF){

       // try the key
       if(c == '\n'){



         key_bytes = strlen(testkey);
         //printf("%s\n",line);
         base64_decoded_key = base64decode(testkey, key_bytes);
         printf("our key = %s\n", base64_decoded_key);
         break;
       }
       testkey[i] = c;
       i++;

       // found no key that works
     }




	//EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, base64_decoded_key, iv);

        //memcpy(encrypted_message_full, message, strlen(message)); // co


	memset(encrypted_message_full, 0, strlen(encrypted_message_full));

	//printf("adding on %s", encrypted_message_full);

  	memmove(encrypted_message_full, encryption_addon, strlen(encryption_addon));

  	strcat(encrypted_message_full, message);

	//encrypted_message_full[strlen(encrypted_message_full)] = 0;

    	printf("new message = %s\n", encrypted_message_full);

	if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, encrypted_message_full, strlen(encrypted_message_full)))
    {
        /* Error */
	printf("Error encoding");
        return 0;
    }

	if(!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen))
    {
	printf("Error encoding");
        /* Error */
        return 0;
    }

	outlen += tmplen;



	EVP_CIPHER_CTX_cleanup(&ctx);

	// buffer should contain ciphertext

        printf("our encrypted message = %s\n", outbuf);

  	int bytes_to_encode = outlen; //Number of bytes in string to base64 encode.

    	base64_encoded = base64encode(outbuf, bytes_to_encode);   //Base-64 encoding.

    fclose(fp);
    return base64_encoded;
}


int decrypt(char *encrpyted_message, char* filename ) {


    unsigned char testkey[128];
    FILE *fp;
    memset(testkey, 0, sizeof(testkey));

    fp = fopen (filename,"r");
    
    int remainingBytes;
    char c;

    int x;

    int bytes_to_decode = strlen(encrpyted_message); //Number of bytes in string to base64 decode.
    //unsigned char *base64_decoded = malloc(sizeof (unsigned char));
    unsigned char *base64_decoded = base64decode(encrpyted_message, bytes_to_decode);   //Base-64 decoding.

    printf("decoded message= %s\n",base64_decoded);

    //outlen = strlen(base64_decoded);

	


    //bzero(debuf,1024);
    //memset(testkey, 0, sizeof(testkey));
	


 while((c=fgetc(fp))!=EOF){


       // try the key
       if(c == '\n'){

         key_bytes = strlen(testkey);
         //printf("%s\n",line);
         base64_decoded_key = base64decode(testkey, key_bytes);
         printf("test key: = %s, our key = %s\n", testkey, base64_decoded_key);

	  delen = 0;

          memset(debuf, 0, strlen(debuf));

          EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, base64_decoded_key, iv);

	  
        
          bzero (&debuf, sizeof(base64_decoded));

          if(!EVP_DecryptUpdate(&ctx, debuf, &delen, base64_decoded, outlen))
          {printf("Error1\n");return 0;}

	  printf("first decrypt = %s", debuf);

	    for (x = 0; i< 25; i++){
              printf("%c", debuf[i]);
            }

           if(!EVP_DecryptFinal_ex(&ctx, debuf + delen, &remainingBytes)){
             memset(testkey, 0, sizeof(testkey));
             printf("decrypt failed\n");
	     ERR_print_errors_fp(stderr);
             
             //continue;
           }

            delen+=remainingBytes;
            EVP_CIPHER_CTX_cleanup(&ctx);
            

            char check[34];

            for(x = 0; i < delen; i++){
              check[i] = debuf[i];
            }

	    printf("attempted decrpytion produces = ");

            /*if (check != "CMPUT379 Whiteboard Encrypted v0\n") {
             printf("no key was able to decrypt the message\n");
              return 0;
            }*/

            for (x = 0; i< 25; i++){
              printf("%c", debuf[i]);
            }

	    memset(testkey, 0, strlen(testkey));
            i=0;	
           continue;
       }
       else 
	{
	 testkey[i] = c;
         testkey[i+1] = '\0';
       }
	i++;

       // found no key that works

     }
     
	      fclose(fp);
       printf("no key was able to decrypt the message\n");

    /*
    i =0;
    int x;
    char t;
    memset(testkey, 0, sizeof(testkey));

    while((c=fgetc(fp))!=EOF){
	 
	  

         // try the key
         if(c == '\n'){
	
           //Todo Needs to conver base64decode into a char array

          key_bytes = strlen(testkey);

          base64_decoded_key = base64decode(testkey, key_bytes);
          //printf("decoded= %s\n", base64_decoded);
	  //printf("our key = %s\n", base64_decoded_key);
	  printf("our key = %s\n", base64_decoded_key);

	  delen = 0;

          memset(debuf, 0, strlen(debuf));

          EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, base64_decoded_key, iv);

	  
        
          bzero (&debuf, sizeof(base64_decoded));

          if(!EVP_DecryptUpdate(&ctx, debuf, &delen, base64_decoded, outlen))
          {printf("Error1\n");return 0;}

	  printf("first decrypt = %s", debuf);

	    for (x = 0; i< 25; i++){
              printf("%c", debuf[i]);
            }

           if(!EVP_DecryptFinal_ex(&ctx, debuf + delen, &remainingBytes)){
             memset(testkey, 0, sizeof(testkey));
             printf("decrypt failed\n");
	     ERR_print_errors_fp(stderr);
             
             //continue;
           }

            delen+=remainingBytes;
            EVP_CIPHER_CTX_cleanup(&ctx);
            

            char check[34];

            for(x = 0; i < delen; i++){
              check[i] = debuf[i];
            }

	    printf("attempted decrpytion produces = ");

            if (check != "CMPUT379 Whiteboard Encrypted v0\n") {
             printf("no key was able to decrypt the message\n");
              return 0;
            }

            for (x = 0; i< 25; i++){
              printf("%c", debuf[i]);
            }
	    memset(testkey, 0, strlen(testkey));
            i=0;
            continue;
         }
         //printf("key value = %c, %d ", c, i);
         testkey[i] = c;
         //printf("testkey %s ", testkey);
         i++;


         // found no key that works
       }
      fclose(fp);
       printf("no key was able to decrypt the message\n");
	*/

}
