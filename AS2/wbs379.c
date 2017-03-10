#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


// mutex lock
pthread_mutex_t mutex;
int WHITEBOARD_SIZE = 0;
int MY_PORT = 2222;
FILE * STATEFILE;

int getEntryLimit() {
	int count = 0;	
	char c;	
	for (c = getc(STATEFILE); c != EOF; c = getc(STATEFILE)) {
        if (c == '\n') {// Increment count if this character is newline
            count = count + 1;
        }
   }
   rewind(STATEFILE);
	return count/2; 
}


int makeWhiteboardFile(int entries) {
	int i;
	for(i = 1; i <= entries; i++) {	
		fprintf(STATEFILE, "!%dp0\n\n", i);
	}
}


void getNEntry(entry) {
	rewind(STATEFILE);	
	char buf[200];
	char msg[20];
	sprintf(msg, "!%d", entry);
	printf("%c%c\n", msg[0], msg[1]);
	while(strcmp(buf, msg) != 0) {
		fgets(buf, 200, STATEFILE);
		if(strcmp(buf, "test10\n") != 0) {
			printf("%s\n", buf);
		}}
	fgets(buf, 200, STATEFILE); 
	printf("%s\n", buf);
}


// all functionability in this function
void *thread_connections( void* acc_socket) {
  printf("got here\n");
	int sock = *(int *)acc_socket;
	int message_size;

	char f_message[]= {("CMPUT379 Whiteboard Server v0\%d\n", WHITEBOARD_SIZE)};
	char *message, client_message[1000];
 
	/* 
	** Create function to get whiteboard size
	*/
	
	// first message
	int written = write(sock, f_message, strlen(f_message));
  if(written==-1) {
    printf("Could not write! %s\n", strerror(errno));
  } 

	// continous loop
	while(message_size = read(sock, client_message, sizeof(client_message)) > 0) {
    message_size = read(sock, client_message, sizeof(client_message));
    pthread_mutex_lock(&mutex);

    /*
    ** Update Whiteboard
    */
  
    pthread_mutex_unlock(&mutex);
  
    /*
    ** Respond function
    */
  
    write(sock, client_message, strlen(client_message));
  
    memset(client_message, 0, 2000);
	}
	
	// client disconnected
	if(message_size == 0) {
	
	}

	else if(message_size == -1) {
    perror("can't recieve message");
	}

	return 0;
}


int main(int argc, char *argv[])
{
  if (argc < 4) {
    printf("Too few arguments! Exiting...\n");
    exit(0);
  }
  char * fileName;
  
  MY_PORT = strtol(argv[1], NULL, 10);
  if(MY_PORT == 0) {
    printf("Invalid port number! Exiting...\n");
    exit(0);
  }
  
  if(strcmp("-n", argv[2]) == 0) {
    WHITEBOARD_SIZE = strtol(argv[3], NULL, 10);
    if(WHITEBOARD_SIZE == 0) {
      printf("Invalid whiteboard size! Exiting...\n");
      exit(0);
    }
    STATEFILE = fopen("whiteboard.all", "w");
    if(STATEFILE == NULL) {
      printf("Could not create new whiteboard file! Exiting...\n");
      exit(0);
    }
    // fill whiteboard file with empty entries
    makeWhiteboardFile(WHITEBOARD_SIZE);
  }
  else if (strcmp("-f", argv[2]) == 0) {
    fileName = argv[3];
    STATEFILE = fopen(fileName, "r+");
    if(STATEFILE == NULL) {
      printf("Could not open file %s, make sure it exists and that its readable. Exiting..\n", argv[3]);
      exit(0);
    }
    WHITEBOARD_SIZE = getEntryLimit();
    
  }
  else {
    printf("Invalid argument format! Only './wbs379 \"portnumber\" {-f \"statefile\" | -n \"entries\"}' is accepted.\n");
    exit(0);
  }
  
	 getNEntry(2);
  
  
  
  	/////////////////// push at end of program
  	fclose(STATEFILE);
  
  int	sock, snew, fromlength, number, outnum, a;

	struct	sockaddr_in	master, from;

	pthread_t thread_id;

	pid_t pid = 0;
	pid_t sid = 0;

	// now Daemon process
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
	
	//puts();
	a = sizeof(struct sockaddr_in);

	//puts();
	a = sizeof(struct sockaddr_in);

	while (1) {
		fromlength = sizeof (from);

		// accept the connection 
		snew = accept (sock, (struct sockaddr*) & from, & fromlength);

		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}

		// create the thread
		pthread_create(&thread_id, NULL, thread_connections, (void *) &snew);


		//returnmessages(snew);

		//write (snew, &outnum, sizeof (outnum));
		//close (snew);
		
	}
}