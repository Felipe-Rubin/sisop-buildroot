/*
 * Based on Sergio Johann {https://bitbucket.org/sjohann81/setpriority}
 * 
 */
// #define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>
#include <semaphore.h>

sem_t sem_buff;
char *buffer = NULL;
char *buffer_ptr = NULL;
int buff_size = 0;


int get_sched_id(char *policy)
{
	if (strcmp(policy,"SCHED_DEADLINE") == 0) return 6;
	if (strcmp(policy,"SCHED_FIFO") == 0) return 1;
	if (strcmp(policy,"SCHED_RR") == 0) return 2;
	if (strcmp(policy,"SCHED_OTHER") == 0) return 0;
	if (strcmp(policy,"SCHED_BATCH") == 0) return 3;
	if (strcmp(policy,"SCHED_IDLE") == 0) return 5;
	else return atoi(policy);
	return -1;
}


/**
 * Get the corresponding name for the policy id
 * @param policy The policy id
 * @return	the corresponding scheduler name
 */
char* get_sched_name(int policy)
{
	switch(policy){
		case SCHED_DEADLINE: return "SCHED_DEADLINE";
		case SCHED_FIFO: return "SCHED_FIFO";
		case SCHED_RR: return "SCHED_RR";
		case SCHED_NORMAL: return "SCHED_OTHER";
		case SCHED_BATCH: return "SCHED_BATCH";
		case SCHED_IDLE: return "SCHED_IDLE";
		default: return "unknown";
	}
}
/**
 * Sets the priority for a new thread
 * @param  th           The thread
 * @param  new_policy   The policy to be set
 * @param  new_priority The priority to be set
 * @return              1 for succesps, 0 otherwise
 */
int set_priority(pthread_t *th, int new_policy, int new_priority)
{
	int policy, ret;
	struct sched_param param;
	
	if (new_priority > sched_get_priority_max(new_policy) || new_priority < sched_get_priority_min(new_policy)) {
		printf("Invalid priority: %s MIN: %d, MAX: %d\n",get_sched_name(new_policy), sched_get_priority_min(new_policy), sched_get_priority_max(new_policy));
		return -1;
	}

	// printf("Ok\n");	
	// printf("%d %d \n",new_policy,new_priority);
	ret = pthread_getschedparam(*th, &policy, &param);
    if (ret != 0) {
        perror("perror(): ");
        return -1;
    }


	// printf("current: ");
	// print_sched(policy);
	// printf("Policy: ");
	// print_sched(policy);

	param.sched_priority = new_priority;
	ret = pthread_setschedparam(*th, new_policy, &param);
	if (ret != 0) {
		perror("perror(): ");
		return -1;
	}

	pthread_getschedparam(*th, &policy, &param);
	// printf("new: ");
	// print_sched(policy);


	return -1;
}

/**
 * Get Thread ID based on it's Name
 * @param  name The corresponding thread name
 * @return      The corresponding ID for it's name
 */
int get_th_id(char name)
{	if(name > 90) name-=7;
	return name - 65;
}
/**
 * Get Thread Name based on it's ID
 * @param  id The corresponding thread ID
 * @return    The corresponding name for it's ID
 */
char get_th_name(int id)
{
	if(id > 25) id+=7;
	return 65 + id;
}


void write_to_buffer(char th_name)
{
	*buffer_ptr = th_name;
	buffer_ptr++;
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
		if(buffer_ptr == buffer+buff_size-1){ //Check if reached the end
			sem_post(&sem_buff);
			pthread_exit(NULL);
		}
		write_to_buffer(th_name);
		// *buffer_ptr = th_name;
		// buffer_ptr++;
		sem_post(&sem_buff);
	}
}

/**
 * Post process the Buffer aggregating the characters and counting each execution 
 * @param thread_number The number of threads
 */
void post_processing(int thread_number)
{
	int *th_occurrences = calloc(thread_number,sizeof(int)); //Ocurrences of each thread
	char *curr_buff = buffer; //pointer to walk on the buffer
	int j = 0;
	//Allocate some first space, will need to realloc if it gets bigger
	int seq_buff_size = thread_number;
	char *seq_buff = malloc(sizeof(char) * seq_buff_size);
	seq_buff[0] = '\0';

	while(curr_buff != buffer+buff_size-1) {
		if(j == 0 || seq_buff[j-1] != *curr_buff){
			if(j == seq_buff_size){
				seq_buff_size*=2;
				seq_buff = realloc(seq_buff,seq_buff_size * sizeof(char));
			} //realloc it
			seq_buff[j] = *curr_buff;
			j++;
		}
		th_occurrences[get_th_id(*curr_buff)]++;
		curr_buff++;
	}
	printf("\n%s\n\n",buffer);
	int i = 0;	
	for(; i < j; i++) printf("%c",seq_buff[i]);
	
	printf("\n\n");
	for (j = 0; j < thread_number; j++) {
		printf("%c = %d\n",get_th_name(j),th_occurrences[j]);
	}
	free(seq_buff);
	free(th_occurrences);
}

// Ver pq round robin real-time Ñ funciona
// priority eh setada no codigo, tirar o parametro
/*
0 = SCHED_OTHER PRI_MIN: 0 PRI_MAX: 0
1 = SCHED_FIFO PRI_MIN: 1 PRI_MAX: 99
2 = SCHED_RR PRI_MIN: 1 PRI_MAX: 99
3 = SCHED_BATCH PRI_MIN: 0 PRI_MAX: 0
5 = SCHED_IDLE PRI_MIN: 0 PRI_MAX: 0
6 = SCHED_DEADLINE PRI_MIN: 0 PRI_MAX: 0
 */

int main(int argc, const char** argv)
{
	int k = 0;
	if (argc != 5) {
		printf("Usage:\n");
		printf("%s <thread_number> <global_buffer_size_KB> <policy> <priority>\n",argv[0]);
		return -1;
	}
	const int thread_number = atoi(argv[1]);
	printf("Total Threads: %d\n",thread_number);
	buff_size = (atoi(argv[2]) * 1024)+1;

	
	int* policies = calloc(thread_number,sizeof(int));
	int* priorities = calloc(thread_number,sizeof(int));
	const char* delim_0 = ",";

	char *policies_buff = strtok(argv[3],delim_0);
	while (policies_buff != NULL) {
		policies[atoi(policies_buff)] = get_sched_id((policies_buff = strtok(NULL,delim_0)));
		policies_buff = strtok(NULL,delim_0);
	}


	char *priorities_buff = strtok(argv[4],delim_0);
	while (priorities_buff != NULL) {
		priorities[atoi(priorities_buff)] = atoi((priorities_buff = strtok(NULL,delim_0)));
		priorities_buff = strtok(NULL,delim_0);
	}



	//Allocate buffer memory initializing it
	buffer = calloc(buff_size,sizeof(char));
	buffer_ptr = &buffer[0];
	//Start all threads

	pthread_t th[thread_number];
	sem_init(&sem_buff,0,0);
	int i = 0;
	for (; i < thread_number; i++) {
		printf("Thread %d: Policy: %s Priority: %d\n",i, get_sched_name(policies[i]),priorities[i]);
		// print_sched(policies[i]);
		// printf(" Priority: %d\n",priorities[i]);

		pthread_create(&th[i],NULL,th_gen,(void*)i);
		set_priority(&th[i],policies[i],priorities[i]);
	}
	
	//Allow each thread to start
	sem_post(&sem_buff);

	//Wait for all threads to exit
	for (i = 0; i < thread_number; i++) {
		pthread_join(th[i],NULL);
	}

	// printf("Buffer = %s\n\n",buffer);
	post_processing(thread_number);
	free(buffer); // Free buffer memory
	free(policies);
	free(priorities);
	return 0;
}










