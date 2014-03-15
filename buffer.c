#include <stdio.h>
#include <stdlib.h>

#define BUFFER_FULL 1
#define BUFFER_EMPTY -1
#define BUFFER_SUCCESS 0

typedef struct Buffer {
	int *data;
	int pos; // indicates the next element to be removed
	int maxSize;
	int size;
} Buffer;

/*
Creates a buffer of a given size
*/
Buffer* Buffer_create(int maxSize){
	Buffer *buffer  = malloc(sizeof(Buffer));
	buffer->maxSize = maxSize;
	buffer->size    = 0;
	buffer->pos     = 0;
	buffer->data    = malloc(maxSize * (sizeof(int)));
	return buffer;
}

/* 
Adds and Elements to the buffer
returns 1 if successful
returns zero if failure
*/
int Buffer_push(Buffer *buffer, int value){
	 // is buffer full?
	if (buffer->size == buffer->maxSize) return BUFFER_FULL;

	// calculate next open spot and increment size
	int nextLocation = (buffer->pos + buffer->size) % buffer->maxSize;
	buffer->data[nextLocation] = value;
	buffer->size++;

	return BUFFER_SUCCESS;
}

/*
Returns the top element in the buffer
returns 0 if the buffer is empty
*/
int Buffer_pop(Buffer *buffer){
	if (buffer->size == 0) return BUFFER_EMPTY;

	int topValue = buffer->data[buffer->pos];

	// decrement the size and position of the buffer
	buffer->size--;
	buffer->pos = (buffer->pos + 1) % buffer->maxSize;

	// roll the position back to the end if if has become negative
	// if (buffer->pos == -1) buffer->pos = buffer->maxSize - 1;

	return topValue;
}

/* prints out the contents of the buffer */
void Buffer_print(Buffer *buffer){
	int i;

	printf("Printing the Buffer Contents: \n");
	printf("Max Size is: %d \n", buffer->maxSize);
	printf("Current Size is: %d \n", buffer->size);

	for(i=0;i<buffer->maxSize;i++){
		printf("Element %d: %d\n", i, buffer->data[i]);
	}
}

/* returns true(1) if the buffer is not full or empty and false(0) otherwise */
int Buffer_is_ready(Buffer *buffer){
	return (buffer->size > 0 && buffer->size < buffer->maxSize);
}

/*
Better-than-nothing test of the buffer.
Note: This function terminates the program
*/
void Buffer_test(){
	Buffer *buffer = Buffer_create(3);

	Buffer_push(buffer, 10);

	printf("First Test\n");
	if (Buffer_pop(buffer) != 10){
		printf("Ahhhh the first test failed\n");
		exit(EXIT_FAILURE);
	}

	printf("Second Test\n");
	if (Buffer_pop(buffer) != 0){
		printf("Ahhh the second test failed\n");
		exit(EXIT_FAILURE);
	}

	Buffer_push(buffer, 10);
	Buffer_push(buffer, 20);
	Buffer_push(buffer, 30);

	printf("Third Test\n");
	if (Buffer_pop(buffer) != 10) {
		printf("First value was incorrect\n");
		exit(EXIT_FAILURE);
	}
	if (Buffer_pop(buffer) != 20) {
		printf("Second value was incorrect\n");
		exit(EXIT_FAILURE);
	}
	if (Buffer_pop(buffer) != 30) {
		printf("Third value was incorrect\n");
		exit(EXIT_FAILURE);
	}

	printf("All Buffer Tests Pass\n");
	exit(EXIT_SUCCESS);
}