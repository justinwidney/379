
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
int quit_request =0;

jmp_buf readonly_memory;
int flag = 0;

void sigint_handler(int signo) {
   if (signo == SIGINT) {
       close (snew);
       flag = 1;
       quit_request =1;
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
    entries[i-1].entry[0] = 0;
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
    if (entry > WHITEBOARD_SIZE) {
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
	int sock = *(int *)acc_socket;
	int message_size;
	//char f_message[]= {("CMPUT379 Whiteboard Server v0\%d\n", WHITEBOARD_SIZE)};
  char f_message[100];
  sprintf(f_message, "CMPUT379 Whiteboard Server v0\n%d\n", WHITEBOARD_SIZE);
  //unsigned char WHITEBOARD_SIZE_CHAR[1];

  //printf("%d\n", WHITEBOARD_SIZE);
 // WHITEBOARD_SIZE_CHAR[0] = WHITEBOARD_SIZE + '0';

  //f_message[strlen(f_message)] = '\n';
  //strcat(f_message, WHITEBOARD_SIZE_CHAR);
 // f_message[strlen(f_message)] = '\n';

	char *message, client_message[1000], server_message[1000];
  char mode;

	unsigned char temp[1], temp2[1], temp3[1];
  char cpointer;

	/*
	** Create function to get whiteboard size
	*/

	// first message
	write(sock, f_message, strlen(f_message)+1);

	// continous loop
	while(1) {

    	message_size = read(sock, client_message, sizeof(client_message));
      if(client_message[0] == '?'){
        pthread_mutex_lock(&mutexg);
        temp[0] = client_message[1];
        int i = 1; char entryStr[20]; int entryNumber;
        while(1) {
          if(client_message[i] == '\n') {
            entryNumber = strtol(entryStr, NULL, 10);
            break;
          }
          entryStr[i-1] = client_message[i];
          i++;
        }

        char *fishedentry = getNEntry(entryNumber);
        int len = 0;
        while(len < strlen(fishedentry)) {
          len += write(sock, fishedentry, strlen(fishedentry));
          printf("%d\n", len);


      pthread_mutex_unlock(&mutexg);

        }
        pthread_mutex_unlock(&mutexg);
      }



     if(client_message[0] == '@'){
       pthread_mutex_lock(&mutexr);
     	  b++;
      	 if (b==1) {pthread_mutex_lock(&mutexg);}
      	 pthread_mutex_unlock(&mutexr);


	          // entry
	         temp[0] = client_message[1];
	         int x = atoi(temp);

	           temp2[0] = client_message[2];

	            if(temp2[0] == 'p') {
	               mode = 'p';
	              }

	            if(temp2[0] == 'c') {
	                mode = 'c';

	              }

	               // length
          	temp3[0] = client_message[3];
	          int y = atoi(temp3);

         printf("entry number = %d length = %d mode = %c\n message = %s",x,y,mode,client_message);
         updateEntry(x, mode, y, client_message);
         memset(server_message, 0, sizeof(server_message));

         // replies
         sprintf(server_message, "!%de0\n\n", x);


      	 pthread_mutex_lock(&mutexr);
      	 b--;
      	 if (b==0) {pthread_mutex_unlock(&mutexg);}
      	 pthread_mutex_unlock(&mutexr);

     }


	   // reply with message
   	 write(sock, server_message, strlen(server_message));

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

int i;

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


  }
  else {
    printf("Invalid argument format! Only './wbs379 \"portnumber\" {-f \"statefile\" | -n \"entries\"}' is accepted.\n");
    exit(0);
  }
    pid_t pid = 0;
    pid_t sid = 0;
    FILE *fp= NULL;

    /* ADD DAEMON PRCOESS
    pid = fork();

    if (pid < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }

    if (pid > 0)
    {
    	// in the parent
       printf("pid of child process %d \n", pid);
       exit(0);
    }

    umask(0);

	// open a log file
    fp = fopen ("logfile.log", "w+");
    if(!fp){
    	printf("cannot open log file");
    }

    // create new process group -- don't want to look like an orphan
    sid = setsid();
    if(sid < 0)
    {
    	fprintf(fp, "cannot create new process group");
        exit(1);
    }

    if ((chdir("/")) < 0) {
      printf("Could not change working directory to /\n");
      exit(1);
    }

	   // close standard fds
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    */


  	/////////////////// push at end of program

  	//fclose(STATEFILE);
  int	sock, fromlength, number, outnum, a;

	struct	sockaddr_in	master, client;

	pthread_t thread_id;

	struct sigaction seg_act;


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

	int c = sizeof(struct sockaddr_in);

	while (snew = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)) {

    sigsetjmp(readonly_memory,1);

    if(flag == 1 || quit_request == 1) {
      printf("Closing\n");
      close (sock);

      /*
      for(i =0; i< threadcount; i++) {
      //(void) pthread_join(thread[i], NULL);
      }
      */

      exit(1);
    }


		// accept the connection

		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}

		// create the thread
    printf("Creating thread %d\n", i);
		pthread_create(&thread_id, NULL, thread_connections, (void *) &snew);
    i++;


		//returnmessages(snew);

		//write (snew, &outnum, sizeof (outnum));
		//close (snew);

	}
  free(entries);
}
