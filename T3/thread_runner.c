/*
 * Based on Sergio Johann {https://bitbucket.org/sjohann81/setpriority}
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>



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

void *th_gen(void* id)
{
	const char th_id = 65+(int)id; // A....Z
	printf("Thread %c says HI\n",th_id);
	pthread_exit(NULL);
}

int main(int argc, const char** argv)
{
	if (argc != 5) {
		printf("Usage:\n");
		printf("%s <thread_number> <global_buffer_size_KB> <policy> <priority>\n",argv[0]);
		return -1;
	}
	const int thread_number = atoi(argv[1]);
	const int buff_size = atoi(argv[2]);
	// const char* policy = argv[3];

	const int priority = atoi(argv[4]);
	
	int i = 0;
	pthread_t th[thread_number];

	for (; i < thread_number; i++) {
		pthread_create(&th[i],NULL,th_gen,(void*)i);
	}

	for (i = 0; i < thread_number; i++) {
		pthread_join(th[i],NULL);
	}

	return 0;
}










