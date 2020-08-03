#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#define maxTopics 3
#define maxEntries 5
#define delta 3.0
#define MAXPROXIES 3

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
	pthread_mutex_t mutex;
    int head;
    int tail;
	const int len;
}topicQueue;

#define tQ_DEF(var, tname)      \
    topicEntry var##_tQueue[maxEntries + 1] = {[maxEntries].entryNum = -1};\
    topicQueue var = {          \
        .topic = tname,         \
        .mutex = PTHREAD_MUTEX_INITIALIZER, \
        .tQueue = var##_tQueue,    \
        .head = 0,             \
        .tail = 0,             \
        .len = maxEntries,   \
    }

// --------------- Threads  --------------------------------------
typedef struct pArg {
    int id;
    topicEntry mt[3];
    char topic[50]; 
} pArg;

typedef struct sArg {
    char topic[50];
    topicEntry empty;   
} sArg;

pArg pubArgs[MAXPROXIES];
sArg subArgs[MAXPROXIES];
pthread_t pubs[MAXPROXIES];
pthread_t subs[MAXPROXIES];
pthread_attr_t attr;
// ----------------------------------------------------------------

topicQueue* topics[maxTopics];
tQ_DEF(mountains, "Mountains");
tQ_DEF(rivers, "Rivers");
tQ_DEF(trees, "Trees");

int enqueue(char* topic, topicEntry* te) {
    int i = 0;
    for (i; i < maxTopics; i++){ 
        if (strcmp(topics[i]->topic, topic) == 0) {
            if (topics[i]->tQueue[topics[i]->tail].entryNum == -1 || strlen(te->photoURL) == 0) {
                return 0; //not allowing empty tes to be inserted for simplicity
            }
            pthread_mutex_lock(&topics[i]->mutex);  //lock process so only one thread can access
            gettimeofday(&te->timeStamp, 0); //set timestamp for topicentry
            te->entryNum = entryNumber; //set entrynum for topicentry
            entryNumber++;
            topics[i]->tQueue[topics[i]->tail] = *te;
            topics[i]->tail++;
            if (topics[i]->tail == topics[i]->len + 1){
                topics[i]->tail = 0;
            }
           pthread_mutex_unlock(&topics[i]->mutex);
            return 1;
        }
    }
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
            return 0; //case 3 part 1 (lastEntry + 1 is not in queue and nothing in queue is > than it
        }
    }   
    printf("%s does not appear to be an existing topic\n", topic);
    return 0;
}
//thread safe method for calling getEntry()
int ts_getEntry(char* topic, int lastEntry, topicEntry* empty) {
    int i = 0;
    int rval = 0;                   
    for (i; i < maxTopics; i++) { //check if queue exists
        if (strcmp(topics[i]->topic, topic) == 0) {
            pthread_mutex_lock(&topics[i]->mutex);
            rval = getEntry(topic, lastEntry, empty);
            pthread_mutex_unlock(&topics[i]->mutex);
            sched_yield();
        }
    }    
    return rval; 
}
// vvv needs to be thread safe vvv
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
    return 0;
}
void* publisher(void* args) {
    pArg* arg = (pArg*) args;
    int until = 1;
    int k = 0;
    while (until) {
        topicEntry* temp = &(arg->mt[k]);
        until = enqueue(arg->topic, temp);
        k++;
        sleep(1);
    }
        
}
void* subscriber(void* args) {
    sArg* arg = (sArg*) args;
    int until = 1;
    int le = entryNumber;
    while(le) {
        topicEntry* temp = &arg->empty;
        ts_getEntry(arg->topic, le, temp);
        printf("Got entry: %d\t %s\n", le, temp->photoURL);
        le --;       
        sleep(1);
    }  
}

int main() {
    topicEntry whitney = teInit(69, "lol.com", "swag hehe");
//    topicEntry* m1 = &ne;
    topicEntry fuji = teInit(420, "ftb", "captionnne");
  //  topicEntry* m2 = &ne1;
    topicEntry bachelor = teInit(420, "do you believe", "in magic");
    //topicEntry* m3 = &ne2;
    topicEntry amazon = teInit(69420, "anything", "can happen hehe");
  //  topicEntry* r1 = &ne;
    topicEntry nile= teInit(100, "mt.fuji", "cant hehe");
  //  topicEntry* r2 = &ne4;
    topicEntry  will = teInit(123, "Willammete", "ha"); 
   // topicEntry* r3 = &riv;
    topicEntry birch = teInit(6869, "Birch", "Tree");
   // topicEntry* t1 = &birch;
    topicEntry oak = teInit(1344, "Oak", "ley");
   // topicEntry* t2 = &oak;
    topicEntry willow = teInit(567, "Willow", "tree");
  //  topicEntry* t3 = &willow;  
    topics[0] = &mountains;
    topics[1] = &rivers;
    topics[2] = &trees;
    topicEntry montanas[3] = {whitney, fuji, bachelor};
    topicEntry rios[3] = {amazon, nile, will };
    topicEntry arbol[3] = {birch, oak, willow};
    memcpy(pubArgs[0].mt, montanas, sizeof(montanas));
    memcpy(pubArgs[1].mt, rios, sizeof(rios));
    memcpy(pubArgs[2].mt, arbol, sizeof(arbol));
    topicEntry empty1, empty2, empty3; 
    topicEntry empties[3] = {empty1, empty2, empty3};

    int k = 0;
    for (k; k < MAXPROXIES; k++) { //create the publisher threads
        pubArgs[k].id = k;
        strcpy(pubArgs[k].topic, topics[k]->topic);
        pthread_create(&pubs[k], &attr, publisher, &pubArgs[k]);       
        printf("Publisher thread %d created\n", pubs[k]);
    }   //create the subscriber threads
    sleep(5);
    for (k = 0; k < MAXPROXIES; k++) {
        strcpy(subArgs[k].topic, topics[k]->topic);
        subArgs[k].empty = empties[k];       
        pthread_create(&subs[k], &attr, subscriber, &subArgs[k]);
        printf("Subscriber thread %d created\n", subs[k]);
    }                                        //join the threads
    for (k = 0; k < MAXPROXIES; k++) {
        pthread_join(subs[k], NULL);
        pthread_join(pubs[k], NULL);
    }
 
    return 1;    
}
