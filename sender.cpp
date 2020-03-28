/*AUTHORS
--------------------------------------
Janelle Estabillo
estabillojanelle@csu.fullerton.edu
CPSC 351-02

Nidhi Shah
nidhi989@csu.fullerton.edu
CPSC 351-02 */
//Sender connect with shared memory and message queue, get the data from file and put on shared memory,
//and notify the receiver the data on shared memory is ready by message Queue with size of data.
//and wait until the receiver message as data received and send the message again to receiver with size 0
//to allow deallocate shared memory and message queue. Itselft also deallocate shared memory and message queue
//and closed the file.
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "msg.h"    /* For the message struct */
#include <iostream>

using namespace std;

/* The size of the shared memory chunk */
#define SHARED_MEMORY_CHUNK_SIZE 1000

/* The ids for the shared memory segment and the message queue */
int shmid, msqid;

/* The pointer to the shared memory */
void* sharedMemPtr;

/**
 * Sets up the shared memory segment and message queue
 * @param shmid - the id of the allocated shared memory
 * @param msqid - the id of the shared memory
 */

void init(int& shmid, int& msqid, void*& sharedMemPtr)
{
	/* TODO:
        1. Create a file called keyfile.txt containing string "Hello world" (you may do
 		    so manually or from the code).
	    2. Use ftok("keyfile.txt", 'a') in order to generate the key.
		3. Use the key in the TODO's below. Use the same key for the queue
		    and the shared memory segment. This also serves to illustrate the difference
		    between the key and the id used in message queues and shared memory. The id
		    for any System V objest (i.e. message queues, shared memory, and sempahores)
		    is unique system-wide among all SYstem V objects. Two objects, on the other hand,
		    may have the same key.
	 */

      	//generating key for keyfile.txt for further use
      	key_t key = ftok("keyfile.txt",'a');
        //checking key error
  			if(key == -1)
  	 	 {
  	 	    perror("EROR:: generating key");
  	 			exit(1);
  	 	 }

    	/* TODO: Get the id of the shared memory segment. The size of the segment must be SHARED_MEMORY_CHUNK_SIZE */
    	/* TODO: Attach to the shared memory */
    	/* TODO: Attach to the message queue */
    	/* Store the IDs and the pointer to the shared memory region in the corresponding parameters */

    	//getting id of segment
    	shmid = shmget(key, SHARED_MEMORY_CHUNK_SIZE,0666);
      //checking shared memory is exist or not..with error checking
    	if(shmid == -1)
    	{
    		perror("ERROR:: shared segment ");

    		exit(1);
    	}

    	//get id of message queue
    	msqid = msgget(key,0666);
      //checking error
    	if(msqid == -1)
    	{
    		perror("ERROR:: message queue");

    		exit(1);
    	}

    	//attach with segment with reading and writing permission
      sharedMemPtr = shmat(shmid,NULL,0);
        //checking error
    	if(sharedMemPtr == (char*)-1)
    	{
    		perror("ERROR:: not attached with segment");
    		exit(1);
    	}
}

/**
 * Performs the cleanup functions
 * @param sharedMemPtr - the pointer to the shared memory
 * @param shmid - the id of the shared memory segment
 * @param msqid - the id of the message queue
 */

void cleanUp(const int& shmid, const int& msqid, void* sharedMemPtr)
{
  	/* TODO: Detach from shared memory */
  	// Detaching from our Shared Memory
  	if (shmdt(sharedMemPtr) == -1) {
  		perror("shmdt"); // If Shared memory failed to detach
  		exit(-1);
  	}
}

/**
 * The main send function
 * @param fileName - the name of the file
 */
void send(const char* fileName)
{
  	/* Open the file for reading */
  	FILE* fp = fopen(fileName, "r");


  	/* A buffer to store message we will send to the receiver. */
  	message sndMsg;
  	sndMsg.mtype = SENDER_DATA_TYPE;

  	/* A buffer to store message received from the receiver. */
  	message rcvMsg;


  	/* Was the file open? */
  	if(!fp)
    	{
        //checking file open or not
    		perror("fopen");
    		exit(-1);
    	}

  	/* Read the whole file */
  	while(!feof(fp))
  	{
      		/* Read at most SHARED_MEMORY_CHUNK_SIZE from the file and store them in shared memory.
       		 * fread will return how many bytes it has actually read (since the last chunk may be less
       		 * than SHARED_MEMORY_CHUNK_SIZE).
       		 */
      		if((sndMsg.size = fread(sharedMemPtr, sizeof(char), SHARED_MEMORY_CHUNK_SIZE, fp)) < 0)
      		{
              //checking error
        			perror("fread");
        			exit(-1);
      		}

      		/* TODO: Send a message to the receiver telling him that the data is ready
       		 * (message of type SENDER_DATA_TYPE)
       		 */
      		 if (msgsnd(msqid, &sndMsg, sizeof(struct message) - sizeof(long), 0) == -1)
           {
                //checking error
      			    perror("msgsnd");
      	   }

  		/* TODO: Wait until the receiver sends us a message of type RECV_DONE_TYPE telling us
   		 * that he finished saving the memory chunk.
   		 */
  		 if (msgrcv(msqid, &rcvMsg, sizeof(struct message) - sizeof(long), RECV_DONE_TYPE, 0) == -1)
        {
              //checking error
        			perror("msgrcv");
        			exit(-1);
  	  	}
   }

      	/** TODO: once we are out of the above loop, we have finished sending the file.
       	  * Lets tell the receiver that we have nothing more to send. We will do this by
       	  * sending a message of type SENDER_DATA_TYPE with size field set to 0.
      	  */
      		sndMsg.size = 0;

       	if (msgsnd(msqid, &sndMsg, sizeof(struct message) - sizeof(long) , 0) == -1)
        {
          //checking error
       		perror("msgsnd");
       	}

      	/* Close the file */
      	fclose(fp);

}


int main(int argc, char** argv)
{

	/* Check the command line arguments */
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME>\n", argv[0]);
		exit(-1);
	}

	/* Connect to shared memory and the message queue */
	init(shmid, msqid, sharedMemPtr);

	/* Send the file */
	send(argv[1]);

	/* Cleanup */
  cleanUp(shmid, msqid, sharedMemPtr);

	return 0;
}
