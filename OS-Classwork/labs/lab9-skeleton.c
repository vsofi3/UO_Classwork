/*=============================================================================
 * Program Name: lab8
 * Author: Jared Hall, <your name>
 * Date: <date>
 * Description:
 *     A simple program that implements a thread-safe queue of meal tickets
 *
 * Notes:
 *     1. DO NOT COPY-PASTE MY CODE INTO YOUR PROJECTS.
 *===========================================================================*/

//========================== Preprocessor Directives ==========================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
//=============================================================================

//================================= Constants =================================
#define MAXNAME 15
#define MAXQUEUES 4
#define MAXTICKETS 3
#define MAXDISH 20
#define MAXPUBs 3
#define MAXSUBS 4
//=============================================================================

//============================ Structs and Macros =============================
typedef struct mealTicket{
	int ticketNum;
	char *dish;
} mealTicket;

//TODO: Declare a mutex in the struct. (e.g. add pthread_mutex_t ...)
typedef struct MTQ {
	char name[MAXNAME];
	struct mealTicket *buffer;
	int head;
	int tail;
	int length;
	int ticket;
} MTQ;

//TODO: Add a mutex init to this function (e.g registry[pos].mutex = PTHREAD_MUTEX_INITIALIZER)
//TODO: If you use the null ticket trick then add a null ticket to the end of the buffer.
//      If you use the heads pos-1 to determine empty/full case then you can skip this.
void init(int pos, char *MTQ_ID) {
	strcpy(registry[pos].name, MTQ_ID);
	registry[pos].buffer = (struct mealTicket *) malloc(sizeof(struct mealTicket) *MAXTICKETS);
	registry[pos].head = 0;
	registry[pos].tail = 0;
	registry[pos].length = MAXTICKETS;
	registry[pos].ticket = 0;
	
	//add null ticket or use head-1 to split empty/full case. Up to you.
}

void freeMTQ(int pos, char *MTQ_ID) {
	free(registry[pos].buffer);
}
	

MTQ registry[MAXQUEUES]; //INFO: Changed to be an array of structs
//=============================================================================

//================================= Functions =================================
int enqueue(char *MTQ_ID, mealTicket *MT) {
	int ret = 0; 
	int i, flag = 0;
	
	//Step-1: Find registry
	for(i=0;i<MAXQUEUES;i++) { 
		if(strcmp(MTQ_ID, registry[i]->name) == 0) { flag = 1; break; }
	}

	//STEP-2: Enqueue the ticket
	if(flag) {
		//TODO: Aquire the lock if it's available. Otherwise, wait until it is.
		//HINT: Use conditional variables and sched_yield.
		int tail = registry[i]->tail;
		if(registry[i]->buffer[tail].ticketNum != -1) {
			MT->ticketNum = TICKET;
			TICKET++;
			registry[i]->buffer[tail] = *MT;
			if(tail == registry[i]->length) { registry[i]->tail = 0; }
			else { registry[i]->tail++; }
			ret = 1;
		}
		//TODO: Release the lock.
	}
	return ret;
}

int dequeue(char *MTQ_ID, int ticketNum, mealTicket *MT) {
	int ret = 0;
	int i, flag = 0;
	
	//Step-1: Find registry
	for(i=0;i<MAXQUEUES;i++) { 
		if(strcmp(MTQ_ID, registry[i]->name) == 0) { flag = 1; break; }
	}
	
	//Step-2: Dequeue the ticket
	if(flag) {
		//TODO: Aquire the lock if it's available. Otherwise, wait until it is.
		//HINT: Use conditional variables and sched_yield.
		int head = registry[i]->head;
		int tail = registry[i]->tail;
		
		if(head != tail && ticketNum == registry[i]->buffer[head].ticketNum) {
			//copy the ticket
			MT->ticketNum = registry[i]->buffer[head].ticketNum;
			strcpy(MT->dish, registry[i]->buffer[head].dish);
			
			//change the null ticket to empty
			if(head == 0) { 
				registry[i]->buffer[registry[i]->length].ticketNum = 0;
			} else { 
			registry[i]->buffer[head-1].ticketNum = 0;
			}
			
			//change the current ticket to null
			registry[i]->buffer[head].ticketNum = -1;
			
			//increment the head
			if(head == registry[i]->length+1) { registry[i]->head = 0; }
			else { registry[i]->head++; }
			ret = 1;
		}
		//TODO: Release the lock.
	}
	return ret;
}

void *publisher(void *args) {
	/* TODO: The publisher will receive the following in the struct args:
	*        1. An array of mealTickets to push to the queue.
	*        2. For each meal ticket, an MTQ_ID. (Note: a publisher can push to multiple queues)
	*        3. The thread ID
	* The publisher will then print its type and thread ID on startup. Then it will push a meal ticket to
	* its appropriate queue before sleeping for 1 second. It will do this until there are no more meal tickets to push.
	*/
}

void *subscriber(void *args) {
	/* TODO:The subscriber will take the following:
	* 1. The MTQ_ID it will pull from.
	* 2. The thread ID
	* 3. An empty meal ticket.
	* The subscriber will print its type and thread ID on startup. Then it will pull a ticket from its queue
	* and print it. If the queue is empty then it will print an empty message along with its
	* thread ID and wait for 1 second. If the thread is not empty then it will pop a ticket and 
	* print it along with the thread id.
	*/
}
//=============================================================================

//=============================== Program Main ================================
int main(int argc, char argv[]) {
	//Variables Declarations
	char *qNames[] = {"Breakfast", "Lunch", "Dinner", "Bar"};
	char *bFood[] = {"Eggs", "Bacon", "Steak"};
	char *lFood[] = {"Burger", "Fries", "Coke"};
	char *dFood[] = {"Steak", "Greens", "Pie"};
	char *brFood[] = {"Whiskey", "Sake", "Wine"};
	int i, j, t = 1;
	int test[4];
	char dsh[] = "Empty";
	mealTicket bfast[3] = {[0].dish = bFood[0], [1].dish = bFood[1], [2].dish = bFood[2]};
	mealTicket lnch[3] = {[0].dish = lFood[0], [1].dish = lFood[1], [2].dish = lFood[2]};
	mealTicket dnr[3] = {[0].dish = dFood[0], [1].dish = dFood[1], [2].dish = dFood[2]};
	mealTicket br[3] = {[0].dish = brFood[0], [1].dish = brFood[1], [2].dish = brFood[2]};
	mealTicket ticket = {.ticketNum=0, .dish=dsh};
	
	//STEP-1: Initialize the registry
	
	//STEP-2: Create the publisher thread-pool
	
	//STEP-3: Create the subscriber thread-pool

	//STEP-4: Join the thread-pools
	
	//STEP-5: Free the registry
	
	return EXIT_SUCCESS;
}
//=============================================================================