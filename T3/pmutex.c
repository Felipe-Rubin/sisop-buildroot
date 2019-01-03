/**
 * 	Autor: Felipe Pfeifer Rubin & Ian Aragon Escobar
 *  Email: felipe.rubin;ian.escobar@acad.pucrs.br
 *  Description: Peterson Algorithm Implementation N > 2
 *  The Filter lock Algorithm
 */
#include "pmutex.h"

/*
	Behavior is undefined if the thread using lock() 
	was not added with pmutex_add
 */
int lock(struct pmutex_t *p)
{
	int i,j,flg;
	struct list_t *listaux = p->pnode;
	pthread_t me = pthread_self();
	struct pnode_t *mypnode = NULL;
	struct pnode_t *curr = NULL;
	/*Acquire pointer to my node*/
	while(listaux != NULL){
		curr = ((struct pnode_t*)(listaux->data));
		if(pthread_equal(*curr->owner,me)){
			mypnode = curr;
			break;
		}
		listaux = listaux->next;
	}
	/*The actual Filter algorithm*/
	struct list_t *holdlist = p->onhold;
	for(i = 1; i < p->size; i++){
		mypnode->level = i;
		holdlist->data = (void*)mypnode->owner;
		flg = 1;
		while(pthread_equal(*((pthread_t*)(holdlist->data)),*(mypnode->owner)) && flg){
			flg = 0;
			listaux = p->pnode;
			for(j = 0; j < p->size; j++){
				if(pthread_equal(*((struct pnode_t*)(listaux->data))->owner,*mypnode->owner)){
					listaux = listaux->next;
 					continue;
				}
				if(((struct pnode_t*)(listaux->data))->level >= i){
					flg = 1;
					break;
				} 
				listaux = listaux->next;
			}
		}
		holdlist = holdlist->next;
	}
	return 1;
}
int unlock(struct pmutex_t *p)
{
	struct list_t *listaux = p->pnode;
	pthread_t me = pthread_self();
	struct pnode_t *curr;
	//Acquire pointer to my node
	while(listaux != NULL){
		curr = ((struct pnode_t*)(listaux->data));
		if(pthread_equal(*curr->owner,me)){
			curr->level = 0;
			break;
		}
		listaux = listaux->next;
	}
	return 1;
}
int pmutex_create(struct pmutex_t *p)
{
	p->size = 0;
	p->onhold = NULL;
	p->pnode = NULL;
	return 1;
}
int pmutex_add(struct pmutex_t *p, pthread_t *owner)
{
	//Create information structure
	struct pnode_t *pnew = malloc(sizeof(struct pnode_t));
	pnew->level = 0;
	pnew->owner = owner;

	//Create Node for the list
	struct list_t *plist = malloc(sizeof(struct list_t));
	//Cast as void
	plist->data = (void*)pnew;
	//Append as first elem of the list
	plist->next = p->pnode;
	p->pnode = plist;
	//Creating another level and append it as first element
	struct list_t *holdp = malloc(sizeof(struct list_t));
	holdp->next = p->onhold;
	p->onhold = holdp;

	//Increment size variable
	p->size++;
	return 1;
}
int pmutex_destroy(struct pmutex_t *p)
{
	struct list_t *plist;
	struct pnode_t *auxnode;
	while(p->pnode != NULL){
		plist = p->pnode;
		p->pnode = p->pnode->next;
		free(plist->data);
		plist->next = NULL;
		free(plist);
	}
	p->pnode = NULL;
	//Only free the list_t *node, the data
	//pointed to the same pnode that was freed
	//in p->pnode
	while(p->onhold != NULL){
		plist = p->onhold;
		p->onhold = p->onhold->next;
		free(plist);
	}
	p->onhold = NULL;

	return 1;
}




