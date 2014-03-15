#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


// this file contains all of the Buffer_* definitions
#include "buffer.c"
#define TRUE  1
#define FALSE 0
#define MAX_JOB_SIZE 10

// for keeping track of print jobs
Buffer *buffer;

// locks and conditions for the buffer
pthread_mutex_t buffer_lock   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  buffer_ready  = PTHREAD_COND_INITIALIZER; // buffer is not full or empty


// This is what the clients run
static void * clientThread(void *arg){
	int id = arg;

	while(TRUE){
		pthread_mutex_lock(&buffer_lock);

		int jobSize = rand() % MAX_JOB_SIZE;
		int push = Buffer_push(buffer, jobSize);

		if (push == BUFFER_FULL){
			printf("Client %d: Buffer is full\n", id);
			// wait for normal condition
			pthread_cond_wait(&buffer_ready, &buffer_lock);
			// push value onto the buffer
			Buffer_push(buffer, jobSize);
		}

		printf("Client %d: Requested a print job of %d pages\n", id, jobSize);
		pthread_mutex_unlock(&buffer_lock);

		if (Buffer_is_ready(buffer)){
			pthread_cond_signal(&buffer_ready);
		}

	}

	pthread_exit(NULL);
	return NULL;
}

static void * printerThread(void *arg){
	int id = arg;

	while(TRUE){ 
		pthread_mutex_lock(&buffer_lock);

		// get the number of pages to print
		int pages = Buffer_pop(buffer);

		if (pages == BUFFER_EMPTY){ // buffer empty
			printf("Printer %d: Buffer is empty\n", id);
			pthread_cond_wait(&buffer_ready, &buffer_lock); // wait until there is something to print
			pages = Buffer_pop(buffer); // get a new value after buffer is ready
		}

		printf("Printer %d: Printing %d pages\n", id, pages);
		pthread_mutex_unlock(&buffer_lock);

		if (Buffer_is_ready(buffer)){
			pthread_cond_signal(&buffer_ready);
		}

		sleep(pages);
		printf("Printer %d: Finished printing %d pages\n", id, pages);
	}

	pthread_exit(NULL);
	return NULL;
}



int main(int argc, char *argv[]){
	if (argc != 4){
		printf("Incorrect use of the program...\n");
		printf("You must input 3 arguments\n");
		exit(EXIT_FAILURE);
	}

	// [name, clients, printers, buffer]
	int clientSize  = atoi(argv[1]);
	int printerSize = atoi(argv[2]);
	int bufferSize  = atoi(argv[3]);

	buffer = Buffer_create(bufferSize);

	int i;
	pthread_t clients[clientSize];
	pthread_t printers[printerSize];

	// create clients
	for (i=0;i<clientSize;i++){
		pthread_create(&clients[i], NULL, clientThread, i);
	}

	// create printers
	for(i=0;i<printerSize;i++){
		pthread_create(&printers[i], NULL, printerThread, i);
	}

	printf("Main Thread: Threads launched\n");

	// for (i=0;i<clientSize;i++){
	// 	pthread_join(clients[i], NULL);
	// }

	// for(i=0;i<printerSize;i++){
	// 	pthread_join(printers[i], NULL);
	// }

	// printf("\nThreads are done\n");


	pthread_exit(NULL);
	return EXIT_SUCCESS;
}