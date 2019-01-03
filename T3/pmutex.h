/**
 * 	Autor: Felipe Pfeifer Rubin & Ian Aragon Escobar
 *  Email: felipe.rubin;ian.escobar@acad.pucrs.br
 *  Description: Peterson Algorithm Implementation N > 2
 *  The Filter lock Algorithm
 */
#ifndef PMUTEX_H
#define PMUTEX_H
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

/* Simple Linked List */
struct list_t{
	void* data;
	struct list_t* next;
};
/* Information about a thread */
struct pnode_t{
	int level; //Level it currently resides
	pthread_t *owner; //Owner thread
};

/* The structure that acts as a mutex */
struct pmutex_t{
	int size; //How many levels/threads allowed to use this mutex
	struct list_t *onhold; //list of 1 sacrifice per level
	struct list_t *pnode; //list of pnode_t 
};
/*
	Wants to lock, busy wait till it can
	Undefined behavior if the accessing thread
	was not added with pmutex_add().
 */
int lock(struct pmutex_t *p);

/*
	Unlocks critical section
 */
int unlock(struct pmutex_t *p);

/*
	Create pmutex, it does NOT allocate it
	An already allocated pmutex_t pointer should be passed
 */
int pmutex_create(struct pmutex_t *p);

/*
	Add Thread that is allowed to use this mutex
 */
int pmutex_add(struct pmutex_t *p, pthread_t *owner);

/*
	Free resources of pmutex
 */
int pmutex_destroy(struct pmutex_t *p);

#endif
