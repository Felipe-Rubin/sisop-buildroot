/*
 * Simple disc I/O generator
 * Felipe Rubin & Guilherme Girotto
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/types.h>
#include<sys/wait.h> //Added to use wait(&child)
#include <semaphore.h>
#define BUFFER_LENGTH 512
#define DISK_SZ	1073741824

sem_t s0;
sem_t s1;
/*
 * This is a simple task that request 100 writes from disk.
 * The semaphores are used to increase the chance of populating the request_queue,
 * otherwise the process scheduller could just run one request at a time, so the
 * I/O scheduller would never have more than one request at a time to select.
 * 
*/
void *write_task(void *arg)
{
	
	printf("Starting %d\n",(int)arg);
	int ret, fd, pid, i;
	unsigned int pos;
	char buf[BUFFER_LENGTH];
	srand(getpid());

	fd = open("/dev/sdb", O_RDWR);
	if (fd < 0){
		perror("Failed to open the device...");
		pthread_exit(NULL);
	}

	strcpy(buf, "hello world!");
	sem_post(&s1);
	sem_wait(&s0);	
	for (i = 0; i < 100; i++){
		pos = (rand() % (DISK_SZ >> 9));
		/* Set position */
		lseek(fd, pos * 512, SEEK_SET);
		/* Peform write. */
		write(fd, buf, 100);
	}
	close(fd);

	pthread_exit(NULL);
}

int main(){


	printf("Starting sector write example...\n");

	printf("Cleaning disk cache...\n");
	system("echo 3 > /proc/sys/vm/drop_caches");
	sem_init(&s0,0,0);
	sem_init(&s1,0,0);
	int procs = 50;
	
	int i;
	for (i = 0; i < procs; i++) {
		pthread_t th;
		pthread_create(&th,NULL,write_task,(void*)i);
	}
	
	/*Wait all procs be ready*/
	for(i = 0; i < procs; i++){
		sem_wait(&s1);
	}
	/* Let all the procs begin */
	for(i = 0; i < procs; i++){
		sem_post(&s0);
	}
	/* Wait for them to finish*/
	pthread_exit(NULL);

	return 0;
}
