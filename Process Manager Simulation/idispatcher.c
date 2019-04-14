/* Nicholas Nolan
 * 0914780
 * March 2019
 * CIS*3110 Assignment 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cpu {
	struct node * current;
	unsigned long idle;
} CPU;

typedef struct node {
	struct node * previous;
	struct node * next;

	unsigned long processID;
	unsigned long running;
	unsigned long ready;
	unsigned long blocked;
} Node;

typedef struct queue {
	struct node * head;
	struct node * tail;
} Queue;

Node * createNode();
Queue * createQueue();
void deleteQueue(Queue * queue);
Node * getNode(Queue * queue, unsigned long id);
int isEmpty(Queue * queue);
Node * pop(Queue * queue);
void push(Queue * queue, Node * toPush);

int main(void) {
	CPU cpu;
	cpu.idle = 0;
	cpu.current = NULL;

	Queue * waiting = createQueue();
	Queue * resource1 = createQueue();
	Queue * resource2 = createQueue();
	Queue * resource3 = createQueue();
	Queue * resource4 = createQueue();
	Queue * resource5 = createQueue();

	Node * temp;

	char description[21];
	char * ptr;
	char event;
	unsigned long lastArrival = 0;
	int elapsed = 0;
	int maxID = 1;
	char completed[200][30];

	for(int i = 0; i < 200; i++) {
		completed[i][0] = '\0';
	}

	unsigned long arrival = 0;
	unsigned long resource = 0;
	unsigned long id = 0;

	for(;;) {
		//Read input from stdin and parse
		fgets(description, 20, stdin);
		if(strlen(description) < 2) {
			break;
		}
		arrival = strtoul(description, &ptr, 10);
		ptr++;
		event = ptr[0];
		ptr++;
		if(event == 'R' || event == 'I') {
			resource = strtoul(ptr, &ptr, 10);
			ptr++;
		} else {
			resource = 0;
		}
		if(event != 'T') {
			id = strtoul(ptr, &ptr, 10);
		} else {
			id = 0;
		}

		//Increment the timers
		elapsed = arrival - lastArrival;
		lastArrival = arrival;

		if(cpu.current == NULL) {
			cpu.idle += elapsed;
		} else {
			cpu.current->running += elapsed;
		}

		temp = waiting->head;
		while(temp != NULL) {
			temp->ready += elapsed;
			temp = temp->next;
		}

		temp = resource1->head;
		while(temp != NULL) {
			temp->blocked += elapsed;
			temp = temp->next;
		}
		temp = resource2->head;
                while(temp != NULL) {
                        temp->blocked += elapsed;
                        temp = temp->next;
                }
		temp = resource3->head;
                while(temp != NULL) {
                        temp->blocked += elapsed;
                        temp = temp->next;
                }
		temp = resource4->head;
                while(temp != NULL) {
                        temp->blocked += elapsed;
                        temp = temp->next;
                }
		temp = resource5->head;
                while(temp != NULL) {
                        temp->blocked += elapsed;
                        temp = temp->next;
                }

		//Update the state of the machine
		switch(event) {
			case 'C':
				//Create new process
				;//C requires an empty statement here
				Node * newNode = createNode();
				newNode->processID = id;
				if(cpu.current == NULL) {
					cpu.current = newNode;
				} else {
					push(waiting, newNode);
				}

				maxID++;
				break;
			case 'E':
				//Exit running process and record running data
				if(cpu.current != NULL && cpu.current->processID == id) {
					temp = cpu.current;
					sprintf(completed[id], "%lu %lu %lu %lu\n", id,
					temp->running, temp->ready, temp->blocked);
					free(temp);
					cpu.current = pop(waiting);
				} else {
					fprintf(stderr, "Invalid input\n");
					return EXIT_FAILURE;
				}
				break;
			case 'R':
				//Process blocked by request for resource
				if(cpu.current != NULL && cpu.current->processID == id) {
					switch(resource) {
						case 1:
							push(resource1, cpu.current);
							break;
						case 2:
							push(resource2, cpu.current);
							break;
						case 3:
							push(resource3, cpu.current);
							break;
						case 4:
							push(resource4, cpu.current);
							break;
						case 5:
							push(resource5, cpu.current);
							break;
						default:
							fprintf(stderr, "Invalid input\n");
							return EXIT_FAILURE;
					}
					cpu.current = NULL;
				} else {
					fprintf(stderr, "Invalid input\n");
				}
				cpu.current = pop(waiting);
				break;
			case 'I':
				//Process unblocked by resource
				//Search resource blocks rather than pop the head
				switch(resource) {
					case 1:
						push(waiting, getNode(resource1, id));
						break;
					case 2:
						push(waiting, getNode(resource2, id));
						break;
					case 3:
						push(waiting, getNode(resource3, id));
						break;
					case 4:
						push(waiting, getNode(resource4, id));
						break;
					case 5:
						push(waiting, getNode(resource5, id));
						break;
					default:
						fprintf(stderr, "Invalid input\n");
						return EXIT_FAILURE;
				}
				if(cpu.current == NULL) {
					cpu.current = pop(waiting);
				}
				break;
			case 'T':
				//Timer interrupt; move process to waiting queue
				if(cpu.current != NULL) {
					push(waiting, cpu.current);
					cpu.current = pop(waiting);
				}
				break;
			default:
				fprintf(stderr, "Invalid input\n");
				return EXIT_FAILURE;
		}
	}

	//Print data about the processes
	printf("0 %lu\n", cpu.idle);
	for(int i = 1; i < 200; i++) {
		if(completed[i][0] != '\0') {
			printf("%s", completed[i]);
		}
	}		

	//Free memory and exit
	deleteQueue(waiting);
	deleteQueue(resource1);
	deleteQueue(resource2);
	deleteQueue(resource3);
	deleteQueue(resource4);
	deleteQueue(resource5);
	return EXIT_SUCCESS;
}

Node * createNode() {
	Node * new = malloc(sizeof(Node));
	new->previous = NULL;
	new->next = NULL;
	new->processID = 0;
	new->running = 0;
	new->ready = 0;
	new->blocked = 0;
	return new;
}

/* createQueue
 * mallocs the memory for a queue and sets the pointers to zero
 */
Queue * createQueue() {
	Queue * new = malloc(sizeof(Queue));
	new->head = NULL;
	new->tail = NULL;
	return new;
}

/* deleteQueue
 * frees the memory associated with a queue
 */
void deleteQueue(Queue * queue) {
	if(queue == NULL) {
		return;
	}
	free(queue);
}

/* getNode
 * searches for the node with processID equal to id in a queue
 * returns NULL if not found, otherwise returns that node and
 * removes it from the queue
 * helps to service requests out of order
 */
Node * getNode(Queue * queue, unsigned long id) {
	if(queue == NULL || id <= 0 || queue->head == NULL) {
		return NULL;
	}

	Node * temp = queue->head;
	while(temp != NULL) {
		if(temp->processID == id) {
			if(temp == queue->head) {
				queue->head = queue->head->next;
				return temp;
			} else if(temp == queue->tail) {
				queue->tail = queue->tail->previous;
				queue->tail->next = NULL;
				return temp;
			} else {
				temp->next->previous = temp->previous;
				temp->previous->next = temp->next;
				return temp;
			}
		} else {
			temp = temp->next;
		}
	}
	return NULL;
}

/* isEmpty
 * returns 1 if queue is empty or does not exist
 * returns 0 if queue exists and is not empty
 */
int isEmpty(Queue * queue) {
	if(queue == NULL) {
		return 1;
	} else if(queue->head == NULL) {
		return 1;
	} else {
		return 0;
	}
}

/* pop
 * returns the head of the queue and sets the head to the next node in the queue
 * returns NULL if the queue does not exist or is empty
 */
Node * pop(Queue * queue) {
	if(queue == NULL) {
		return NULL;
	} else if(queue->head == NULL) {
		return NULL;
	}
	Node * temp = queue->head;
	queue->head = queue->head->next;
	temp->next = NULL;
	temp->previous = NULL;
	return temp;
}

/* push
 * pushes node to the back of queue
 * fails silently if queue or node do not exist
 */
void push(Queue * queue, Node * toPush) {
	if(queue == NULL || toPush == NULL) {
		return;
	}

	if(queue->head == NULL) {
		queue->head = toPush;
		queue->tail = toPush;
	} else {
		queue->tail->next = toPush;
		toPush->previous = queue->tail;
		queue->tail = toPush;
	}
}

