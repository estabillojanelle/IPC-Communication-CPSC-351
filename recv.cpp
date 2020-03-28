/*AUTHORS
--------------------------------------
Janelle Estabillo
estabillojanelle@csu.fullerton.edu
CPSC 351-02

Nidhi Shah
nidhi989@csu.fullerton.edu
CPSC 351-02 */
//Reciever create and connect with shared memory and message queue,
//and wait until the sender message as data is ready on shared memory
//and read the data from shared memory and store into the text file.
//than, send the signal to sender that data received and wait for next message
//once it received the shared data size 0 in sender message,
//deallocate shared memory and message queue.
//and closed the recv file.
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
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
void *sharedMemPtr;

/* The name of the received file */
const char recvFileName[] = "recvfile";


/**
 * Sets up the shared memory segment and message queue
 * @param shmid - the id of the allocated shared memory
 * @param msqid - the id of the shared memory
 * @param sharedMemPtr - the pointer to the shared memory
 */

void init(int& shmid, int& msqid, void*& sharedMemPtr)
{

			/* TODO: 1. Create a file called keyfile.txt containing string "Hello world" (you may do
		 		    so manually or from the code).
			         2. Use ftok("keyfile.txt", 'a') in order to generate the key.
				 3. Use the key in the TODO's below. Use the same key for the queue
				    and the shared memory segment. This also serves to illustrate the difference
				    between the key and the id used in message queues and shared memory. The id
				    for any System V object (i.e. message queues, shared memory, and sempahores)
				    is unique system-wide among all System V objects. Two objects, on the other hand,
				    may have the same key.
			 */

		  	//generating key for keyfile.txt for further use
			 key_t key = ftok("keyfile.txt",'a');
			 //checking error
			 if(key == -1)
			 {
			    perror("EROR:: generating key");
					exit(1);
			 }


			/* TODO: Allocate a piece of shared memory. The size of the segment must be SHARED_MEMORY_CHUNK_SIZE. */
			/* TODO: Attach to the shared memory */
			/* TODO: Create a message queue */
			/* Store the IDs and the pointer to the shared memory region in the corresponding parameters */

		   	//get shmid from shmget function.. with the new creation of shared memory, will have id related to the key. if the segment is already exit,
			//will recieve -1 as error
			shmid = shmget(key, SHARED_MEMORY_CHUNK_SIZE, IPC_CREAT|0666);

			//if it already exit, than get the id related to that key
			if(shmid == -1)
			 {
				 perror("ERROR:: shared segment already exist for this key");
				 //just get shared memory id
				 shmid = shmget(key, SHARED_MEMORY_CHUNK_SIZE,0666);
				 exit(1);
			 }

			//get msqid from mdgget fuction.. with new creation of message queue, will have id related to the key. if the segment is already exit,
			//will recieve -1 as error
			msqid = msgget(key,IPC_CREAT|0666);

			//if it already exit, than get the id related to that key
			if(msqid == -1)
			{
				perror("ERROR:: message queue already exist for this key");
				//just get the msg id
				msqid = msgget(key,0666);
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
 * The main loop
 */
void mainLoop()
{
	cout<< "in main loop"<<endl;
	/* The size of the mesage */
	int msgSize = 1;

	/* A buffer to store message we will send to the receiver. */
	message sndMsg;


	/* A buffer to store message received from the receiver. */
	message rcvMsg;

	/* Open the file for writing */
	FILE* fp = fopen(recvFileName, "w");

	/* Error checks */
	if(!fp)
	{
		perror("fopen");
		exit(-1);
	}

    /* TODO: Receive the message and get the message size. The message will
     * contain regular information. The message will be of SENDER_DATA_TYPE
     * (the macro SENDER_DATA_TYPE is defined in msg.h).  If the size field
     * of the message is not 0, then we copy that many bytes from the shared
     * memory region to the file. Otherwise, if 0, then we close the file and
     * exit.
     *
     * NOTE: the received file will always be saved into the file called
     * "recvfile"
     */

	/* Keep receiving until the sender set the size to 0, indicating that
 	 * there is no more data to send
 	 */


	while(msgSize != 0)
	{

			/* If the sender is not telling us that we are done, then get to work */
			//receive a message type of message is SENDER_DATA_TYPE
			if (msgrcv(msqid, &rcvMsg, sizeof(struct message) - sizeof(long), SENDER_DATA_TYPE, 0) == -1)
			{
					//error checking
				 perror("msgrcv");
				 exit(-1);
		  }

			//getting the message size
			msgSize = rcvMsg.size;
			if(msgSize != 0)
			{

					/* Save the shared memory to file */
					if(fwrite(sharedMemPtr, sizeof(char), msgSize, fp) < 0)
					{
						//checking error
						perror("fwrite");
					}

					/* TODO: Tell the sender that we are ready for the next file chunk.
		 			 * I.e. send a message of type RECV_DONE_TYPE (the value of size field
		 			 * does not matter in this case).
		 			 */

					 //setting the type of sending Message
					 sndMsg.mtype = RECV_DONE_TYPE;

					//now sending the message that we recieved first one and we are ready for the next one
					 if (msgsnd(msqid, &sndMsg, sizeof(struct message) - sizeof(long), 0) == -1)
					 {
						 //checking error
							perror("msgsnd");
				   }

			}
			/* We are done */
			else
			{
				/* Close the file */
				fclose(fp);
			}
	}
}



/**
 * Perfoms the cleanup functions
 * @param sharedMemPtr - the pointer to the shared memory
 * @param shmid - the id of the shared memory segment
 * @param msqid - the id of the message queue
 */

void cleanUp(const int& shmid, const int& msqid, void* sharedMemPtr)
{
	/* TODO: Detach from shared memory */
	if (shmdt(sharedMemPtr) == -1) {
 	 perror("shmdt"); // If Shared memory failed to detach
 	 exit(-1);
  }
	/* TODO: Deallocate the shared memory chunk */
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl"); //If Shared memory isn't destroyed
		exit(-1); // Exits Program
	}
	/* TODO: Deallocate the message queue */
	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl"); //If Message Queue isn't destroyed
		exit(-1); // Exits program
	}
}

/**
 * Handles the exit signal
 * @param signal - the signal type
 */

void ctrlCSignal(int signal)
{
	/* Free system V resources */
	cleanUp(shmid, msqid, sharedMemPtr);
}

int main(int argc, char** argv)
{

		/* TODO: Install a singnal handler (see signaldemo.cpp sample file).
	 	 * In a case user presses Ctrl-c your program should delete message
	 	 * queues and shared memory before exiting. You may add the cleaning functionality
	 	 * in ctrlCSignal().
	 	 */
	  signal(SIGINT, ctrlCSignal);
		/* Initialize */
		init(shmid, msqid, sharedMemPtr);

		/* Go to the main loop */
		mainLoop();

		/** TODO: Detach from shared memory segment, and deallocate shared memory and message queue (i.e. call cleanup) **/
		ctrlCSignal(0);

		return 0;
}
