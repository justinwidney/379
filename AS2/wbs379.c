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

#define	MY_PORT	2223

// mutex lock
pthread_mutex_t mutex;


int main(int argc, char *argv[])
{
	int	sock, snew, fromlength, number, outnum, a;

	struct	sockaddr_in	master, from;

	pthread_t thread_id;

	pid_t pid = 0;
	pid_t sid = 0;


	//if (argc< 2) {return 0;}
	
	/*
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

    // Change the current working directory //
	if ((chdir("/")) < 0) {
		printf("Could not change working directory to /\n");
      		exit(1);
    }

	

	// close standard fds
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);


	while(1) {

	sleep(1);
	}

	exit(EXIT_SUCCESS);

	*/ // now Daemon process


	
	

	

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
	
	puts();
	a = sizeof(struct sockaddr_in);

	puts();
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

		write (snew, &outnum, sizeof (outnum));
		close (snew);
		
	}
}


// all functionability in this function
void *thread_connections( void* acc_socket) {

	int sock = *(int *)acc_socket;
	int message_size;

	char f_message[] = "CMPUT379 Whiteboard Server v0\n"
	char *message, client_message[1000];



	/* 
	** Create function to get whiteboard size
	*/
	
	// first message
	write(sock, f_message, strlen(message));

	// continous loop
	while(message_size = read(sock, client_message, sizeof(client_message)) > 0) {

	message_size = read(sock, client_message, sizeof(client_message))

	
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










