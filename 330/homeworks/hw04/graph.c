#include "graph.h"


// This function initializes an adjacency list for
// a graph.
// 
// Note that adj list is an array of """adj_node_t*""" which is passed
// in by reference.
// That is, you are passing in a pointer (i.e, *) to an array (i.e., *)
// of adj_node_t*, which is why list is of type adj_node_t***
void init_adj_list(adj_node_t ***list, int rows)
{
    // Part 2 - Fill in this function
    *list = malloc(sizeof(adj_node_t*) * rows);
    for (int i = 0; i < rows; i++){
       (*list)[i] = NULL;
    }
}


// This function creates a new adj_node_t node
// and initializes it with node->vid = vid
// and node->next = NULL;
// The function then returns this node
adj_node_t *create_node(int vid)
{
    adj_node_t *newNode = (adj_node_t*)malloc(sizeof(adj_node_t));
    newNode->vid = vid;
    newNode->next = NULL;

    return newNode;
    // Part 3 - Fill in this function
}


// Pass in the list and the row to which you need to add a new node
// First check that the adjacency list for the current row is not
// empty (i.e., NULL). If it is NULL, it is the first adjacent node.
// Otherwise, traverse the list until you reach the end, and then add
// the new node
void add_node(adj_node_t** list, int row, adj_node_t* node)
{
    // Part 4 - Fill in this function
    
    adj_node_t* nextUp = list[row];
    if (list[row] == NULL) {
    	list[row] = node;
	return;
    }
    else{
        while (nextUp->next != NULL){
            nextUp = nextUp->next;
   	}
        nextUp->next = node;
    }
}

// deqeueu a node from a queue
// and return the vertex id of the first member
// when list returns, it points to the next member in the queue
int remove_node(adj_node_t **list)
{
    // Part 6 - Implement this function
    if(list == NULL) {
	    return -1;
    }
    adj_node_t* ret = *list;	//stores first node on the list
    int tempRet = ret->vid;
    (*list) = (*list)->next; //starts list at next node
    free(ret);
    return tempRet; //returns the vertex id of first number
}


// This function constructs an adjacency list for a graph.
//
// adj_mat is a 2D array that represents the adjacency matrix
//
// list is passed in by reference from the main function so that
// it can be malloc'd via the init_adj_list function (see aobve)
//
// After initializing it go through each row and add its adjacent 
// nodes
void construct_adj_list(int **adj_mat, int rows, int cols, adj_node_t ***list)
{
    // verify that the adj matrix is correct
    if(rows != cols) {
        printf("Adjacency matrix is not square\n");
        exit(EXIT_FAILURE);
    }

    init_adj_list(list, rows);
    // Part 1 - Fill in the rest of this function
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++) {
            if (adj_mat[i][j] == 1) {
                adj_node_t* newNode = create_node(j);
                add_node(*list, i, newNode);
            }
        }
    }
    // You will need to implement create_node() and
    // add_node() and use them to implement this function
}

// This takes in an adjacency ilst and prints out the list
void print_adj_list(adj_node_t **list, int rows)
{
    assert(list);

    printf("---- Print Adj. List ----\n");
    for(int i = 0; i < rows; i++) {
        printf("|%d| -> ", i);
        adj_node_t* next = list[i];
        while(next != NULL) {
            printf("%d -> ", next->vid);
            next = next->next;
        }
        printf("END\n");
    }
    printf("--------\n\n");
}

void free_adj_list(adj_node_t **list, int rows)
{
   for (int i = 0; i<rows; i++){
	adj_node_t* head = list[i];
	adj_node_t* tmp;
	while(head){
		tmp = head->next;
		free(head);
		head = tmp;
	}
   } 
   free(list);
}

void print_bfs_result(int rows, int *color, int *distance, int *parent)
{
    assert(color);
    assert(distance);
    assert(parent);

    printf("---- Print BFS Result ----\n");
    printf("Vert\tCol\tDis\tParent\n");
    for(int i = 0; i < rows; i++) {
        printf("%d\t%d\t%d\t%d\n", i, color[i], distance[i], parent[i]);
    }
    printf("--------\n\n");
}


// Do BFS here, given the source node and the
// graph's adjacency list
int bfs(adj_node_t **list, int rows, int source,
        int *color, int *distance, int *parent)
{
    // Make sure the source is a valid vertex
    if(source >= rows) {
        printf("Invalid source vertex\n");
        return 0;
    }
    // Make sure the adjacency list is not empty
    if(list == NULL) {
        printf("There is nothing in the adjacency list\n");
        return 0;
    }
    // Make sure all these are properly allocated
    assert(color);
    assert(distance);
    assert(parent);

    // Part 5 - Fill in the rest of this function
    for (int i = 0; i < rows; i++){
	color[i] = 0;
	distance[i] = -1;
	parent[i] = -1;
    }
    color[source] = 1;
    distance[source] = 0;
    parent[source] = -1;

    adj_node_t* queue = NULL;
    adj_node_t* origin = create_node(source);
    add_node(&queue, 0, origin); 
    
    while(queue){
   	int u = remove_node(&queue);
	adj_node_t* next = list[u];
	for (; next != NULL; next=next->next){
		int ID = next->vid;
		if (color[ID] == 0){
			color[ID] = 1;
			distance[ID] = distance[u] + 1; 
			parent[ID] = u;
			adj_node_t* upNext = create_node(ID);
			add_node(&queue, 0, upNext);
			
		}
		color[next->vid] = 2;
	}
    }
     
    #if DEBUG
    print_bfs_result(rows, color, distance, parent);
    #endif

    return 0;
}






