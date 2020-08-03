#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxQueues 4
#define maxName 50

int tickNum = 0;
typedef struct {
	int ticketNum;
	char dish[maxName];
}mealTicket; 

mealTicket mtInit(int tNum, char* dname) {
	mealTicket mt;
	mt.ticketNum = tNum;
	strcpy(mt.dish, dname);
    tickNum++;
	return mt;
}
typedef struct {
	char name[maxName];
	mealTicket *const buffer;
	int head;
	int tail;
	const int len;	
}MTQ;

MTQ mtqInit(char* name, const int length, mealTicket last) {
	MTQ mtq;
	strcpy(mtq.name, name);
	mtq.head = 0;
	mtq.tail = 0;
	*(int *) &mtq.len = length;
	*(mealTicket **) &mtq.buffer = malloc(sizeof(mealTicket) * (length + 1));
	mtq.buffer[mtq.len] = last;
	return mtq;
}
MTQ* registry[maxQueues];

void freeMTQs() {
    int i = 0;
    for (i; i < maxQueues; i++) {
        free(registry[i]->buffer);
    }
    return;
}
int enqueue(char* mtq_id, mealTicket* mt) {
	int i = 0;
	for (i; i < maxQueues; i++) {
        if (strcmp(registry[i]->name, mtq_id) == 0) {
            if(registry[i]->buffer[registry[i]->tail].ticketNum != -1) {
                registry[i]->buffer[registry[i]->tail] = *mt;
                registry[i]->tail++;
                if(registry[i]->buffer[registry[i]->tail].ticketNum == -1 && registry[i]->head ) {
                    registry[i]->tail = 0; //wraparound case
                }
                return 1;            
            }
            else {
                printf("it appears that the %s queue is full\n", mtq_id);
                return 0;
            }        
        }    
	}
    
    printf("This queue, %s, does not appear to exist\n", mtq_id);
    return 0;
}

int dequeue(char* mtq_id, int ticketNum, mealTicket* mt) {
    int i = 0;
    for (i; i < maxQueues; i++) {
        if (strcmp(registry[i]-> name, mtq_id) == 0) {
            if(registry[i]->head == -1 || registry[i]->tail == registry[i]->head !=0) {
                printf("%s is empty\n", mtq_id);
                return 0;
            }
            
            int j = 0;
            for (j; j < registry[i]->len; j++){
                if (ticketNum == registry[i]->buffer[j].ticketNum) {
                    mt->ticketNum = registry[i]->buffer[j].ticketNum;
                    strcpy(mt->dish, registry[i]->buffer[j].dish);
                    registry[i]->buffer[j].ticketNum = 0;
                    registry[i]->buffer[registry[i]->head].ticketNum = -1;
                    registry[i]->head++;
                    return 1;
                }
            }
            printf("Ticket #: %d does not appear to exist\n", ticketNum);
            return 0;   
        }
    }    
    printf("this queue, %s, does not appear to exist\n", mtq_id);
    return 0;
}

int main() {
	mealTicket last = mtInit(-1, "Grrr");
	MTQ mtq = mtqInit("Dinner", 5, last);	
	MTQ mtq2 = mtqInit("Lunch", 5, last);
	MTQ mtq3 = mtqInit("Breakfast", 5, last); //create 4 queues and the pointers for those queus
	MTQ mtq4 = mtqInit("Bar", 5, last);
	MTQ* dinner = &mtq;
	MTQ* lunch = &mtq2;
	MTQ* breakfast = &mtq3;
	MTQ* bar = &mtq4;

	registry[0] = dinner;	//add *MTQs to registry
	registry[1] = lunch;
	registry[2] = breakfast;
	registry[3] = bar;

	mealTicket d1 = mtInit(tickNum, "pasta"); //1
	mealTicket d2 = mtInit(tickNum, "steak"); //2
    mealTicket d3 = mtInit(tickNum, "Brisket"); //3
    mealTicket* spag = &d1;	
    mealTicket* steak = &d2;
    mealTicket* brisk = &d3;
    enqueue("Dinner", spag);
    enqueue("Dinner", steak);
    enqueue("Dinner", brisk);

    mealTicket l1 = mtInit(tickNum, "club sub"); //4
    mealTicket l2 = mtInit(tickNum, "tomato bisque"); //5
    mealTicket l3 = mtInit(tickNum, "salad"); //6
    mealTicket* club = &l1;
    mealTicket* tbisq = &l2;
    mealTicket* salad = &l3;
    enqueue("Lunch", club);
    enqueue("Lunch", tbisq);
    enqueue("Lunch", salad);

    mealTicket b1 = mtInit(tickNum, "pancake"); //7
    mealTicket b2 = mtInit(tickNum, "bacon"); //8
    mealTicket b3 = mtInit(tickNum, "eggs"); //9
    mealTicket* pcake = &b1;
    mealTicket* bacon = &b2;
    mealTicket* eggs = &b3;
    enqueue("Breakfast", pcake);
    enqueue("Breakfast", bacon);
    enqueue("Breakfast", eggs);

    mealTicket a1 = mtInit(tickNum, "Whiskey"); //10
    mealTicket a2 = mtInit(tickNum, "Vodka"); //11
    mealTicket a3 = mtInit(tickNum, "Scotch"); //12
    mealTicket* whisk = &a1;
    mealTicket* vodk = &a2;
    mealTicket* sco = &a3;
    enqueue("Bar", whisk);
    enqueue("Bar", vodk);
    enqueue("Bar", sco);
    
    int j = 0;
    for (j; j < 3; j++){
        mealTicket emp1 = mtInit(-10, "None");
        mealTicket emp2 = mtInit(-11, "None");
        mealTicket emp3 = mtInit(-12, "None");
        mealTicket emp4 = mtInit(-13, "None");
        mealTicket *e1 = &emp1;
        mealTicket *e2 = &emp2;
        mealTicket *e3 = &emp3;
        mealTicket *e4 = &emp4;
        dequeue("Dinner", j + 1, e1);
        printf("Queue: %s - Ticket Number: %d - Dish: %s\n", registry[0]->name, e1->ticketNum, e1->dish);
        dequeue("Lunch", j + 4, e2);
        printf("Queue: %s - Ticket Number: %d - Dish: %s\n", registry[1]->name, e2->ticketNum, e2->dish);
        dequeue("Breakfast", j + 7, e3);
        printf("Queue: %s - Ticket Number: %d - Dish: %s\n", registry[2]->name, e3->ticketNum, e3->dish);
        dequeue("Bar", j + 10, e4);
        printf("Queue: %s - Ticket Number: %d - Dish: %s\n", registry[3]->name, e4->ticketNum, e4->dish);
    }

    printf("\n\n Test Cases \n\n");
    
    MTQ testQ = mtqInit("Test", 5, last);
    MTQ* testr = &testQ;
    free(registry[0]->buffer);
    registry[0] = testr;

    mealTicket test= mtInit(25, "None");
    mealTicket *t1 = &test;

    mealTicket emp1 = mtInit(tickNum, "None");
    mealTicket emp2 = mtInit(tickNum, "None");
    mealTicket emp3 = mtInit(tickNum, "None");
    mealTicket emp4 = mtInit(40, "None");
    mealTicket emp5 = mtInit(tickNum, "None");
    mealTicket *e1 = &emp1;
    mealTicket *e2 = &emp2;
    mealTicket *e3 = &emp3;
    mealTicket *e4 = &emp4;
    mealTicket *e5 = &emp5;
    printf("Test Case: A - Result: %d\n", dequeue("Test", 40, t1)); 
    printf("Test Case: D - Result: %d\n", enqueue("Test", e1));
    enqueue("Test", e2);
    enqueue("Test", e3);
    enqueue("Test", e4);
    enqueue("Test", e5);
    
    mealTicket emp6 = mtInit(tickNum, "None");
    mealTicket *e6 = &emp6;
    printf("Test Case: C - Result: %d\n", enqueue("Test", e6));
    printf("Test Case: B - Result: %d\n", dequeue("Test", 40, t1)); 
    freeMTQs();
    return 1;
}

