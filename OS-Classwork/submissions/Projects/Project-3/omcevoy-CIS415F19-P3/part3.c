#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#define maxTopics 3
#define maxEntries 5
#define MAXPROXIES 5 
int entryNumber= 1;
int pubID = 100; 
float delta = 0.0;
int masterKey = 1;
//----------------Structs & Definitions -------------------
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

// --------- Thread Struct & Declarations ----------------------
typedef struct tArg {
    char file[150];
}tArg;

tArg subArgs[MAXPROXIES];
tArg pubArgs[MAXPROXIES];
pthread_t pubs[MAXPROXIES];
pthread_t subs[MAXPROXIES];
pthread_t janitor; 
pthread_attr_t attr;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t con_mtx;
// ----------------------Misc. Functions---------------------------------------
char* stringStrip(char str[150]) { //function removes quotations from string 
    int len = strlen(str);
    char* newStr = str;
    newStr++;
    newStr[len - 2] = '\0';
    return newStr; 
}
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
    pthread_mutex_lock(&mtx);
    pthread_cond_wait(&con_mtx, &mtx); //ensures the threads wait until start
    pthread_mutex_unlock(&mtx);
/*    tArg* pub = (tArg*) args;
    char* newFile = stringStrip(pub->file);
    strcpy(pub->file, newFile);
    FILE* pubFile = fopen(pub->file, "r");
    char* line = NULL;
    char* token;
    char* dummy; 
    size_t len = 720;
    ssize_t nread;
    if (pubFile == NULL) {exit(EXIT_FAILURE);} //ensures there is a file to read
    while ((nread = getline(&line, &len, pubFile) != -1)) {
        token = strtok_r(line, " ", &dummy);
        int i = 0;
        char* inputs[len];
        while (token) {
            inputs[i] = token;
            i++;
            token = strtok_r(NULL, " ", &dummy);
        }
        if (strcmp(inputs[0], "put") == 0) {
            int topicId = atoi(inputs[1]); 
            for (i = 0; i < maxTopics; i++) {
                if (topicId == topics[i].id) {
                    topicEntry newEntry = teInit(pubID, inputs[2], inputs[3]);
                    pubID++;
                    topicEntry* new = &newEntry;
                    enqueue(topics[i].topic, new);
                }
            }                                        
        }
        else if (strcmp(inputs[0], "sleep") == 0) {
            int sleepTime = atoi(inputs[1]);
            usleep(sleepTime);
        }
        else if (strcmp(inputs[0], "stop\n") == 0) {
            return;
        }
    } */ 
    printf("Proxy Thread %d\t Publisher\n", pthread_self());
    return;
}
void* subscriber(void* args) {
    pthread_mutex_lock(&mtx);
    pthread_cond_wait(&con_mtx, &mtx);
    pthread_mutex_unlock(&mtx);  
    printf("Proxy Thread %d\t Subscriber\n", pthread_self());
    return;
}

void* clean_up() {
    pthread_mutex_lock(&mtx);
    pthread_cond_wait(&con_mtx, &mtx);
    pthread_mutex_unlock(&mtx);
    printf("Proxy Thread %d\t Clean Up\n", pthread_self());
    return;
}
int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Program requires a configuration file.\n");
        return 0;
    } 
    int pos = 0;
    int pubC = 0;
    int subC = 0;
    FILE* fin;
    char* line = NULL;
    char** strtk; 
    size_t len = 320;
    ssize_t nread;
    char* token;
    fin = fopen(argv[1], "r");
    if (fin == NULL) {exit(EXIT_FAILURE);}
    while ((nread = getline(&line, &len, fin) != -1)) {
        token = strtok_r(line, " ", strtk);
        int i = 0;
        char* inputs[len];
        while(token) {
            inputs[i] = token;
            token = strtok_r(NULL, " ", strtk);
            i++;
        }//creating topic queues
        if (strcmp(inputs[0], "create") + strcmp(inputs[1], "topic") == 0) {
            char* topic = malloc(sizeof(char*) * 30);
            strcpy(topic, inputs[3]);
            int id = atoi(inputs[2]);
            int len = atoi(inputs[4]);
            tQ_DEF(pos,id,topic,len); 
            pos++;   
        }//printing topics, pubs, & subs
        else if (strcmp(inputs[0], "query") == 0) {
            int argLen = strlen(inputs[1]) - 1;
            if (inputs[1][argLen] == '\n')
                inputs[1][argLen] = '\0';
            if (strcmp(inputs[1], "topics") == 0) {
                for (i = 0; i < pos; i++) {
                    printf("%s\n", topics[i].topic);
                }
            }
            if (strcmp(inputs[1], "publishers") == 0) {
                for (i = 0; i < pubC; i++) {
                    printf("Publisher Thread: %d\t File: %s\n", pubs[i], pubArgs[i].file);
                }
            }
            if (strcmp(inputs[1], "subscribers") == 0) {
                for (i = 0; i < subC; i ++) {
                    printf("Subscriber Thread: %d\t File: %s\n", subs[i], subArgs[i].file);
                }
            }
        }//creating and adding pubs/subs
        else if(strcmp(inputs[0], "add") == 0) {
            if(strcmp(inputs[1], "subscriber") == 0) {
                strcpy(subArgs[subC].file, inputs[2]);
                int argLen = strlen(subArgs[subC].file) - 1;
                if (subArgs[subC].file[argLen] == '\n')
                    subArgs[subC].file[argLen] = '\0';
                pthread_create(&subs[subC], &attr, subscriber, (void*) &subArgs[subC]);
                subC++;
            }
            if (strcmp(inputs[1], "publisher") == 0) {
                strcpy(pubArgs[pubC].file, inputs[2]);
                int argLen = strlen(pubArgs[pubC].file) - 1;
                if (pubArgs[pubC].file[argLen] == '\n')
                    pubArgs[pubC].file[argLen] = '\0';
                pthread_create(&pubs[pubC], &attr, publisher, (void*) &pubArgs[pubC]);
                pubC++;
            } 
        } //set the delta
        else if (strcmp(inputs[0], "delta") == 0) {
            int argLen = strlen(inputs[1]) - 1;
            if (inputs[1][argLen] == '\n')
                inputs[1][argLen] = '\0';
            delta = atof(inputs[1]);
            printf("%f\n", delta);
        }//release the threads from their wait
        else if (strcmp(inputs[0], "start\n") == 0) {
            pthread_create(&janitor, &attr, clean_up, NULL);
            sleep(1);
            pthread_mutex_lock(&mtx);
            pthread_cond_broadcast(&con_mtx);
            pthread_mutex_unlock(&mtx);
        }
        else {
            printf("%s is not a recognized input\n", inputs[0]);
        }
    }
    fclose(fin); 
    int i = 0; //join the publisher threads
    for (i; i < pubC; i++) {
        pthread_join(pubs[i], NULL);
    }   //join the subscriber threads
    for(i = 0; i < subC; i++) {
        pthread_join(subs[i], NULL);
    }          
    pthread_join(janitor, NULL);
    return 1;    
}
