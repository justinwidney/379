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
#include <signal.h>
#include <setjmp.h>


#define	MY_PORT	2223
#define encrypted c
#define plaintext p

// mutex lock
pthread_mutex_t mutexg;
int WHITEBOARD_SIZE = 0;
int MY_PORT = 2222;
pthread_mutex_t mutexr;
int b = 0;
FILE * STATEFILE;
int snew;

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



struct Entry {
  int entryNumber;
  char * entry;
  char mode;
  int length;
} entry;

struct Entry *entries;

void fillWhiteboardFromFile(FILE *fp) {
  // fill whiteboard struct from file
  rewind(fp);
  char line[2000]; int entryNum = 0;
  while(fgets(line, sizeof(line), fp) != NULL) {
    if(line[0] == '!') {
      // new entry
      // get the entry number
      int i = 1; char entryNumber[10] = ""; entryNumber[9] = '\0';
      while(1) {
        if(line[i] == '\n') {
          printf("This statefile is not in proper order. At entry %d. Exiting...\n", entryNum+1);
          exit(0);
        }
        if (line[i] == 'p' || line[i] == 'c') {
          entries[entryNum].entryNumber = strtol(entryNumber, NULL, 10);;
          break;
        }
        entryNumber[i-1] = line[i];
        i++;
      }
      entries[entryNum].mode = line[i]; // get mode
      // get entry length
      i++;
      int n = 0; char entryLength[10] = ""; entryLength[9] = '\0';
      while(1) {
        if(!isdigit(line[i])) { if(line[i] != '\n') {
          printf("Non-digit found in entry length, statefile is not correct. At entry %d. Exiting...\n", entries[entryNum].entryNumber);
          exit(0);
        }}
        if(line[i] == '\n') {
          entries[entryNum].length = strtol(entryLength, NULL, 10);
          break;
        }
        entryLength[n] = line[i];
        i++; n++;
      }

      int j; char * message = malloc(sizeof(char)*entries[entryNum].length);
      if(message == NULL) {
        printf("Error aquiring memory for entry %d. Exiting...\n", entries[entryNum].entryNumber);
      }
      for(j = 0; j < entries[entryNum].length; j++) {
        message[j] = fgetc(fp);
        if(message[j] < 0 || message[j] == '!') {
          printf("Got entry shorter than it was specified. At entry %d. Exiting...\n", entries[entryNum].entryNumber);
          exit(0);
        }
      }
      entries[entryNum].entry = message;
      entryNum++;
    }
  }
}

void fillWhiteboardBlank(int numEntries) {
  int i;
  for(i = 1; i <= numEntries; i++) {
    entries[i-1].entryNumber = i;
    entries[i-1].entry = malloc(6*sizeof(char));
    if(entries[i-1]. entry == NULL) {
      printf("Error in entry memory allocation. Exiting...\n");
      exit(0);
    }
    sprintf(entries[i-1].entry, "!%dp0\n\n", i);
    entries[i-1].mode = 'p';
    entries[i-1]. length = strlen(entries[i-1].entry);
  }
}

int getEntryLimit() {
	int count = 0;
	char c;
	for (c = getc(STATEFILE); c != EOF; c = getc(STATEFILE)) {
        if (c == '!')  {
            count = count + 1;
        }
   }
	return count;
}


int makeWhiteboardFile(int numEntries) {
	int i;
	for(i = 1; i <= numEntries; i++) {
		fprintf(STATEFILE, "!%dp0\n\n", i);
	}
}


char *getNEntry(int entry) {
  int i = 0;
  while(1) {
    if (i > WHITEBOARD_SIZE) {
      // can't find entry
      char * error = malloc(50); sprintf(error, "!%de14\nNo such entry!\n", entry);
      return error;
    }
    if(entries[i].entryNumber == entry) {
      char * message = malloc(sizeof(int)*2+strlen(entries[i].entry)+4);
      if(message == NULL) {
        printf("Failed to allocate memory when responding to query!. Reponse not possible\n");
        // return error
        char * error = malloc(50); sprintf(error, "!%de36\nThere are memory problems on server!\n",  entry);
        return error;
      }
      sprintf(message, "!%d%c%d\n%s\n", entries[i].entryNumber, entries[i].mode, entries[i].length, entries[i].entry);
      return message;
    }
    i++;
  }
}


char *updateEntry(int entry, char mode, int length, char *message) {
  int i = 0;
  while(1) {
    if (i > WHITEBOARD_SIZE) {
      // can't find entry
      char * error = malloc(50); sprintf(error, "!%de17\nFailed to update!\n", entry);
      return error;
    }
    if(entries[i].entryNumber == entry) {
      entries[i].mode = mode;
      entries[i].length = length;
      entries[i].entry = message;
      char * error = malloc(50); sprintf(error, "!%de0\n\n", entry);
      return error;
    }
    i++;
  }
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
	write(sock, f_message, strlen(message));

	// continous loop
	while(1) {

      sigsetjmp(readonly_memory,1);

      if(flag == 1) {
   printf("Closing\n");
   close (sock);
   exit(1);
}


    	message_size = read(sock, client_message, sizeof(client_message));


   	 pthread_mutex_lock(&mutexg);

    /*
    ** Update Whiteboard
    */

   	 pthread_mutex_unlock(&mutexg);


   	 pthread_mutex_lock(&mutexr);
  	  b++;
   	 if (b==1) {pthread_mutex_lock(&mutexg);}
   	 pthread_mutex_unlock(&mutexr);

    /*
    ** Respond function
    */

   	 pthread_mutex_lock(&mutexr);
   	 b--;
   	 if (b==0) {pthread_mutex_unlock(&mutexg);}
   	 pthread_mutex_unlock(&mutexr);

	// reply with message
   	 write(sock, client_message, strlen(client_message));

	// clear the buffer
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

    entries = realloc(entries, WHITEBOARD_SIZE * sizeof(entry));
    char line[256];

    if (entries == NULL) {
      printf("Error in whiteboard memory allocation, exiting...\n");
    }
    fillWhiteboardBlank(WHITEBOARD_SIZE);
  }
  else if (strcmp("-f", argv[2]) == 0) {
    fileName = argv[3];
    STATEFILE = fopen(fileName, "r+");
    if(STATEFILE == NULL) {
      printf("Could not open file %s, make sure it exists and that its readable. Exiting..\n", argv[3]);
      exit(0);
    }
    WHITEBOARD_SIZE = getEntryLimit();
    entries = realloc(entries, WHITEBOARD_SIZE * sizeof(entry));
    if (entries == NULL) {
      printf("Error in whiteboard memory allocation, exiting...\n");
    }
    fillWhiteboardFromFile(STATEFILE);
    int i;
    for(i = 0; i < WHITEBOARD_SIZE; i++) {
      printf("Entry: %d, Mode: %c, Length: %d, Message: %s\n", entries[i].entryNumber, entries[i].mode, entries[i].length, entries[i].entry);
    }

  }
  else {
    printf("Invalid argument format! Only './wbs379 \"portnumber\" {-f \"statefile\" | -n \"entries\"}' is accepted.\n");
    exit(0);
  }

  	/////////////////// push at end of program
    free(entries);
  	//fclose(STATEFILE);
  int	sock, fromlength, number, outnum, a;

	struct	sockaddr_in	master, from;

	pthread_t thread_id;



  seg_act.sa_handler = &sigint_handler;
  sigemptyset(&seg_act.sa_mask);
  sigaddset(&seg_act.sa_mask, SA_NODEFER);
  sigaction(SIGINT, &seg_act, NULL);

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

	c = sizeof(struct sockaddr_in);

	while (snew = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c))) {


		// accept the connection

		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}

		// create the thread
    printf("Creating thread %d\n",i);
		pthread_create(&thread_id, NULL, thread_connections, (void *) &snew);


		//returnmessages(snew);

		write (snew, &outnum, sizeof (outnum));
		close (snew);

	}
}
