/*
 * Based on Sergio Johann {https://bitbucket.org/sjohann81/setpriority}
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>
#include <semaphore.h>

sem_t sem_buff;
char *buffer;
char *buffer_ptr;
int buff_size;
int thread_number;
/**
 * Prints the corresponding name for the policy id
 * @param policy THe policy id
 */
void print_sched(int policy)
{
	int priority_min, priority_max;

	switch(policy){
		case SCHED_DEADLINE:
			printf("SCHED_DEADLINE");
			break;
		case SCHED_FIFO:
			printf("SCHED_FIFO");
			break;
		case SCHED_RR:
			printf("SCHED_RR");
			break;
		case SCHED_NORMAL:
			printf("SCHED_OTHER");
			break;
		case SCHED_BATCH:
			printf("SCHED_BATCH");
			break;
		case SCHED_IDLE:
			printf("SCHED_IDLE");
			break;
		default:
			printf("unknown\n");
	}
	priority_min = sched_get_priority_min(policy);
	priority_max = sched_get_priority_max(policy);
	printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
}
/**
 * Sets the priority for a new thread
 * @param  th           The thread
 * @param  new_policy   The policy to be set
 * @param  new_priority The priority to be set
 * @return              1 for success, 0 otherwise
 */
int set_priority(pthread_t *th, int new_policy, int new_priority)
{
	int policy, ret;
	struct sched_param param;
	if (new_priority > sched_get_priority_max(new_policy) || new_priority < sched_get_priority_min(new_policy)) {
		printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(new_policy), sched_get_priority_max(new_policy));
		return -1;
	}
	
	pthread_getschedparam(*th, &policy, &param);
	printf("current: ");
	print_sched(policy);

	param.sched_priority = new_priority;
	ret = pthread_setschedparam(*th, new_policy, &param);
	if (ret != 0)
		perror("perror(): ");

	pthread_getschedparam(*th, &policy, &param);
	printf("new: ");
	print_sched(policy);


	return -1;
}

/**
 * Get Thread Name based on it's ID
 * @param  id The corresponding thread ID
 * @return    the corresponding character for it's name
 */
char get_th_name(int id)
{
	return 65 + id;
}
/**
 * Thread Generator
 * @param  id The thread ID
 * @return    void
 */
void *th_gen(void* id)
{
	const char th_name = get_th_name((int)id); //Get thread name
	// printf("Thread %c Started\n",th_name);
	while(1){
		sem_wait(&sem_buff);
		if(buffer_ptr == buffer+(buff_size-1)){ //Check if reached the end
			sem_post(&sem_buff);
			pthread_exit(NULL);
		}
		*buffer_ptr = th_name;
		buffer_ptr++;
		sem_post(&sem_buff);
	}
}

/**
 * Post process the Buffer aggregating the characters and counting each execution 
 */
void post_processing(int thread_number)
{
	int current_thread;
	char *aux_buff = buffer;
	// int thread_number
	while(*aux_buff != '\0') {
		aux_buff++;
	}
}

int main(int argc, const char** argv)
{
	if (argc != 5) {
		printf("Usage:\n");
		printf("%s <thread_number> <global_buffer_size_KB> <policy> <priority>\n",argv[0]);
		return -1;
	}
	thread_number = atoi(argv[1]);
	buff_size = atoi(argv[2]) * 1000;
	
	//FIX ME
	// const char* policy = argv[3];

	const int priority = atoi(argv[4]);

	//Allocate buffer memory
	buffer = malloc(sizeof(char) * (buff_size));
	buffer_ptr = &buffer[0];
	
	//Initializes all threads
	int i = 0;
	pthread_t th[thread_number];
	sem_init(&sem_buff,0,0);
	for (; i < thread_number; i++) {
		pthread_create(&th[i],NULL,th_gen,(void*)i);
	}
	//Allow each thread to start
	sem_post(&sem_buff);

	//Wait for all threads to exit
	for (i = 0; i < thread_number; i++) {
		pthread_join(th[i],NULL);
	}

	// post_processing(thread_number);

	// printf("Buffer = %s\n",buffer);
	free(buffer); // Free buffer memory
	return 0;
}










