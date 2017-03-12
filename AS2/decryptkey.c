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

char *base64decode (const void *b64_decode_this, int decode_this_many_bytes){

    printf(" does it recieve anything %s\n", (char*)b64_decode_this);
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




int main(){
      unsigned char outbuf[1024];
      unsigned char debuf[1024];
      unsigned char * data_to_decode;  //The string we will base-64 encode.
      unsigned char* data_to_encode;
      int i;
      char c;

      FILE *fp;
      fp = fopen ("keys.txt","r");
      if (fp == NULL) {
         printf ("File not created okay, errno = %d\n", errno);
         return 1;
       }


      unsigned char testkey[20];


      unsigned char *base64_decoded;

      size_t outlen;

      while((c=fgetc(fp))!=EOF){

        if (c == '\n') {
          //testkey[i+1] = '\0';
          //printf("our key = %s\n",testkey);
          unsigned char *base64_encoded = base64encode((const void*)testkey, strlen(testkey));

          printf("our encoded key = %s,\n", base64_encoded);


          i=0;
          int bytes_to_decode = strlen(testkey); //Number of bytes in string to base64 decode.
          //printf("bytes = %d\n",bytes_to_decode );
          base64_decoded = base64decode(testkey, bytes_to_decode);   //Base-64 decoding.
          printf("decoded= %s\n", base64_decoded);
          unsigned char *base64_decoded2 = base64decode(base64_encoded, strlen(base64_encoded));
          //printf("decoded v2= %s\n", base64_decoded2);

          memset(testkey, 0, sizeof(testkey));
          printf("\n\n");
          continue;
        }


            testkey[i] = c;
            i++;


        }


return 0;

}
