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
   	topicEntry *tQueue;
    struct timeval timeStamp;
    int id;
    char topic[100];
	pthread_mutex_t mutex;
    int head;
    int tail;
	int len;
}topicQueue;

topicQueue topics[maxTopics];
topicEntry tQueues[maxTopics][maxEntries];
#define tQ_DEF(pos,id,tname,length)            \
    strcpy(topics[pos].topic, tname); \
    topics[pos].head = 0; \
    topics[pos].tQueue = tQueues[pos]; \
    topics[pos].id = id; \
    topics[pos].tail = 0; \
    topics[pos].len = length; \
    topics[pos].tQueue[length].entryNum = -1; \
    pthread_mutex_init(&topics[pos].mutex, NULL); \
     
     

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


int enqueue(char* topic, topicEntry* te) {
    int i = 0;
    for (i; i < maxTopics; i++){ 
        if (strcmp(topics[i].topic, topic) == 0) {
            if (topics[i].tQueue[topics[i].tail].entryNum == -1 || strlen(te->photoURL) == 0) {
                return 0; //not allowing empty tes to be inserted for simplicity
            }
            pthread_mutex_lock(&topics[i].mutex);  //lock process so only one thread can access
            gettimeofday(&te->timeStamp, 0); //set timestamp for topicentry
            te->entryNum = entryNumber; //set entrynum for topicentry
            entryNumber++;
            topics[i].tQueue[topics[i].tail] = *te;
            topics[i].tail++;
            if (topics[i].tail == topics[i].len + 1){
                topics[i].tail = 0;
            }
            pthread_mutex_unlock(&topics[i].mutex);
            return 1;
        }
    }
    return 0;
}

int getEntry(char* topic, int lastEntry, topicEntry* empty) {
    int i = 0;
    for (i; i < maxTopics; i++) { //check if queue exists
        if (strcmp(topics[i].topic, topic) == 0) {
            if (topics[i].head == topics[i].tail) {    //case 1 - queue is empty
                printf("%s is empty\n", topic);
                return 0;
            }
            int j = topics[i].head;
            while (j != topics[i].tail) {  //check if lastEntry + 1 is in queue
                if (topics[i].tQueue[j].entryNum == lastEntry + 1){
                    memcpy(empty, &topics[i].tQueue[j], sizeof(topicEntry)); //case 2
                    return 1;
                }
                j++;
                if (j == maxEntries + 1)
                    j = 0;
            }
            //case 3
            j = topics[i].head; 
            while (j != topics[i].tail) {
                if(topics[i].tQueue[j].entryNum > lastEntry + 1) {
                    memcpy(empty, &topics[i].tQueue[j], sizeof(topicEntry)); //case 3 part 2 
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
        if (strcmp(topics[i].topic, topic) == 0) {
            pthread_mutex_lock(&topics[i].mutex);
            rval = getEntry(topic, lastEntry, empty);
            pthread_mutex_unlock(&topics[i].mutex);
            sched_yield();
        }
    }    
    return rval; 
}
// vvv needs to be thread safe vvv
int dequeue(char* topic, topicEntry* empty) {
    int i = 0;
    for (i; i < maxTopics; i ++) {
        if (strcmp(topics[i].topic, topic) == 0){
            if (topics[i].head == topics[i].tail) {
                printf("%s appears to be empty\n", topic);
                return 0;
            }
            else {
                int j = topics[i].head;
                while (j != topics[i].tail) {
                    struct timeval curTime;
                    gettimeofday(&curTime, 0); //get current time
                    double elapsed = curTime.tv_sec - topics[i].tQueue[j].timeStamp.tv_sec;
                    elapsed = elapsed + ((curTime.tv_usec - topics[i].tQueue[j].timeStamp.tv_usec)/10000.0);
                    printf("Elapsed time: %f\n", elapsed);
                    if (elapsed > delta) { //check elapsed against the delta
                            printf("Dequeue'd something\n");
                            topics[i].tQueue[j] = *empty; //reset index
                            if (topics[i].head == 0) {
                                topics[i].tQueue[topics[i].len].entryNum = 0;
                            }               //release the old ends
                            else {
                                topics[i].tQueue[topics[i].head - 1].entryNum = 0;
                            }                           
                            topics[i].tQueue[topics[i].head].entryNum = -1; //create the new end
                            topics[i].head++; //increment head
                            if (topics[i].head == maxEntries + 1){
                                topics[i].head = 0;
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
    ;       
}
void* subscriber(void* args) {
    ;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Program requires a configuration file.\n");
        return 0;
    } 
    int pos = 0;
    FILE* fin;
    char* line = NULL;
    size_t len = 320;
    ssize_t nread;
    char* token;
    fin = fopen(argv[1], "r");
    if (fin == NULL) {exit(EXIT_FAILURE);}
    while ((nread = getline(&line, &len, fin) != -1)) {
        token = strtok(line, " ");
        int i = 0;
        char* inputs[len];
        while(token) {
            inputs[i] = token;
            token = strtok(NULL, " ");
            i++;
        }   
    if (strcmp(inputs[0], "create") + strcmp(inputs[1], "topic") == 0) {
        char* topic = malloc(sizeof(char*) * 30);
        strcpy(topic, inputs[3]);
        int id = atoi(inputs[2]);
        int len = atoi(inputs[4]);
        tQ_DEF(pos, id,topic,len); 
        pos++;   
    }
    }       
    int k;
    for (k = 0; k < 3; k++) {
        printf("%s\n", topics[k].topic);
    }   
    return 1;    
}
