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

#define encrypted c
#define plaintext p

// mutex lock
pthread_mutex_t mutex;
int WHITEBOARD_SIZE = 0;
int MY_PORT = 2222;
FILE * STATEFILE;

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


char *getNEntry(entry) {
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
  
  
    char * message = getNEntry(6);
    printf("%s\n", message);
  	/////////////////// push at end of program
    free(entries);
  	//fclose(STATEFILE);
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
