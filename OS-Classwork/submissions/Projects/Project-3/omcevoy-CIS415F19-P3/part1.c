#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define maxTopics 3
#define maxEntries 5
 
int entryNumber= 1;

typedef struct{
	int entryNum;
	struct timeval timeStamp;
	int pubID;
	char photoURL[255];
	char photoCaption[500]; 
}topicEntry;

topicEntry teInit(int pubID, char* url, char* caption) {
    topicEntry te;
    te.pubID = pubID;
    strcpy(te.photoURL, url);
    strcpy(te.photoCaption, caption);
    return te;
}


typedef struct {
   	topicEntry *const tQueue;
    struct timeval timeStamp;
    char topic[100];
	int head;
    int tail;
	const int len;
}topicQueue;

#define tQ_DEF(var, tname)      \
    topicEntry var##_tQueue[maxEntries + 1] = {[maxEntries].entryNum = -1};\
    topicQueue var = {          \
        .topic = tname,         \
        .tQueue = var##_tQueue,    \
        .head = 0,             \
        .tail = 0,             \
        .len = maxEntries,   \
    }

topicQueue* topics[maxTopics];
tQ_DEF(mountains, "Mountains");

int enqueue(char* topic, topicEntry* te) {
    int i = 0;
    for (i; i < maxTopics; i++){ 
        if (strcmp(topics[i]->topic, topic) == 0) {
            if (topics[i]->tQueue[topics[i]->tail].entryNum == -1) {
                printf("It appears %s is full\n", topic);
                return 0;
            }
            gettimeofday(&te->timeStamp, 0); //set timestamp for topicentry
            te->entryNum = entryNumber; //set entrynum for topicentry
            entryNumber++;
            topics[i]->tQueue[topics[i]->tail] = *te;
            topics[i]->tail++;
            if (topics[i]->tail == topics[i]->len + 1)
                topics[i]->tail = 0;
            return 1;
        }
    }
    printf("%s does not appear to be an existing topic\n", topic);
    return 0;
}

int getEntry(char* topic, int lastEntry, topicEntry* empty) {
    int i = 0;
    for (i; i < maxTopics; i++) { //check if queue exists
        if (strcmp(topics[i]->topic, topic) == 0) {
            if (topics[i]->head == topics[i]->tail) {    //case 1 - queue is empty
                printf("%s is empty\n", topic);
                return 0;
            }
            int j = topics[i]->head;
            while (j != topics[i]->tail) {  //check if lastEntry + 1 is in queue
                if (topics[i]->tQueue[j].entryNum == lastEntry + 1){
                    memcpy(empty, &topics[i]->tQueue[j], sizeof(topicEntry)); //case 2
                    return 1;
                }
                j++;
                if (j == maxEntries + 1)
                    j = 0;
            }
            //case 3
            j = topics[i]->head; 
            while (j != topics[i]->tail) {
                if(topics[i]->tQueue[j].entryNum > lastEntry + 1) {
                    memcpy(empty, &topics[i]->tQueue[j], sizeof(topicEntry)); //case 3 part 2 
                    return 1;
                }
                j++; 
                if (j == maxEntries + 1)
                    j = 0;
            }
            printf("Case 3 part 1\n");
            return 0; //case 3 part 1 (lastEntry + 1 is not in queue and nothing in queue is > than it
        }
    }   
    printf("%s does not appear to be an existing topic\n", topic);
    return 0;
}
int dequeue(char* topic, topicEntry* empty) {
    int i = 0;
    for (i; i < maxTopics; i ++) {
        if (strcmp(topics[i]->topic, topic) == 0){
            if (topics[i]->head == topics[i]->tail) {
                printf("%s appears to be empty\n", topic);
                return 0;
            }
            else {
                int j = topics[i]->head;
                while (j != topics[i]->tail) {
                    struct timeval curTime;
                    gettimeofday(&curTime, 0); //get current time
                    double elapsed = curTime.tv_sec - topics[i]->tQueue[j].timeStamp.tv_sec;
                    elapsed = elapsed + ((curTime.tv_usec - topics[i]->tQueue[j].timeStamp.tv_usec)/10000.0);
                    printf("Elapsed time: %f\n", elapsed);
                    double delta = 3.0;
                    if (elapsed > delta) { //check elapsed against the delta
                            printf("Dequeue'd something\n");
                            topics[i]->tQueue[j] = *empty; //reset index
                            if (topics[i]->head == 0) {
                                topics[i]->tQueue[topics[i]->len].entryNum = 0;
                            }               //release the old ends
                            else {
                                topics[i]->tQueue[topics[i]->head - 1].entryNum = 0;
                            }                           
                            topics[i]->tQueue[topics[i]->head].entryNum = -1; //create the new end
                            topics[i]->head++; //increment head
                            if (topics[i]->head == maxEntries + 1){
                                topics[i]->head = 0;
                            }
                            return 1;
                        }
                    j++;
                    if (j == maxEntries + 1)
                        j = 0;
                }
                printf("Nothing to dequeue\n");
                return 0;
            }               
        }
    }
    printf("%s does not appear to be an existing topic\n", topic);
    return 0;
}

int main() {
    topicEntry ne = teInit(69, "lol.com", "swag hehe");
    topicEntry* a = &ne;
    topicEntry ne1 = teInit(69, "ftb", "captionnne");
    topicEntry* a1 = &ne1;
    topicEntry ne2 = teInit(420, "do you believe", "in magic");
    topicEntry* a2 = &ne2;
    topicEntry ne3 = teInit(69420, "anything", "can happen hehe");
    topicEntry* a3 = &ne;
    topicEntry ne4 = teInit(100, "mt.fuji", "cant hehe");
    topicEntry* a4 = &ne4;

    topics[0] = &mountains;
    enqueue("Mountains", a);
    enqueue("Mountains", a2);
    enqueue("Mountains", a2);
    enqueue("Mountains", a3);
    enqueue("Mountains", a4); 
    sleep(5);
    topicEntry emp;
    topicEntry* empty = &emp;
    printf( "%s\n", topics[0]->tQueue[0].photoCaption);
    int k = 0;
    while(dequeue("Mountains", empty)){
        sleep(1);
    }
    
    enqueue("Mountains", a);
    enqueue("Mountains", a2);
    enqueue("Mountains", a2);
    enqueue("Mountains", a3);
    enqueue("Mountains", a4); 
    sleep(4);
    while(dequeue("Mountains", empty)){
        sleep(1);
    }
    return 1;    
}
