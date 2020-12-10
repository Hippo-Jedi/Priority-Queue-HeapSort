/*
 * Name: Michael Smith
 * Email: smitmic5@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "pq.h"
#include "dynarray.h"

struct pq {
  struct dynarray* values;
  struct dynarray* priors;
};

struct node {
  int val;
  int prior;
};

/*
 * These are declarations of the percolation and swap functions.
 */
void perc_down(struct pq* pq, int idx);
void swap(struct pq* pq, int one, int two);
void perc_up(struct pq* pq, int idx);

/*
 * This function allocates and initializes an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pq = malloc(sizeof(struct pq));

  pq->values = dynarray_create();
  pq->priors = dynarray_create();
  return pq;
}


/*
 * This function frees the memory allocated to a given priority queue.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  assert(pq);

  dynarray_free(pq->values);
  dynarray_free(pq->priors);
  free(pq);
}


/*
 * This function returns 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  assert(pq);

  return (dynarray_size(pq->values) == 0);
}


/*
 * This function inserts a given element into a priority queue with a
 * specified priority value. 
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  assert(pq);
  int last;
  int* prior = malloc(sizeof(int));

  *prior = priority;
  last = dynarray_size(pq->values);
  dynarray_insert(pq->values, value);
  dynarray_insert(pq->priors, prior);
  perc_up(pq, last);
}

/*
 * This function returns the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  assert(pq);

  return dynarray_get(pq->values, 0);
}


/*
 * This function returns the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  assert(pq);
  int prior;

  prior = *((int*) dynarray_get(pq->priors, 0));
  return prior;
}


/*
 * This function returns the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  assert(pq);
  void* first;
  int last;

  first = dynarray_get(pq->values, 0);
  last = dynarray_size(pq->values) - 1;
  swap(pq, 0, last);
  free(dynarray_get(pq->priors, last));
  dynarray_remove(pq->values, last);
  dynarray_remove(pq->priors, last);
  if (dynarray_size(pq->values) > 0){
    perc_down(pq, 0);
  }
  return first;
}

void swap(struct pq* pq, int one, int two){
  assert(pq);
  void* valTemp = NULL;
  void* priorTemp = NULL;

  valTemp = dynarray_get(pq->values, one);
  priorTemp = dynarray_get(pq->priors, one);
  dynarray_set(pq->values, one, dynarray_get(pq->values, two));
  dynarray_set(pq->priors, one, dynarray_get(pq->priors, two));
  dynarray_set(pq->values, two, valTemp);
  dynarray_set(pq->priors, two, priorTemp);
}

void perc_down(struct pq* pq, int idx){
  assert(pq);
  int left;
  int right;
  int leftP;
  int rightP;
  int min;

  left = 2 * idx + 1;
  right = 2 * idx + 2;
  if (right >= dynarray_size(pq->values)) {
    if (left >= dynarray_size(pq->values)) {
      return;
    } else {
      min = left;
    }
  } else {
    leftP = (*((int*) dynarray_get(pq->priors, left)));
    rightP = (*((int*) dynarray_get(pq->priors, right)));
    if(leftP <= rightP){
      min = left;
    }else{
      min = right;
    }
  }
  if ((*((int*) dynarray_get(pq->priors, idx))) > (*((int*) dynarray_get(pq->priors, min)))) {
    swap(pq, idx, min);
    perc_down(pq, min);
  }

}

void perc_up(struct pq* pq, int idx){
  assert(pq);
  int parent;
  int parentP;
  int currP;

  if (idx != 0) {
    parent = (idx - 1) / 2;
    parentP = (*((int*) dynarray_get(pq->priors, parent)));
    currP = (*((int*) dynarray_get(pq->priors, idx)));
    if (parentP > currP) {
      swap(pq, parent, idx);
      perc_up(pq, parent);
    }
  }
}