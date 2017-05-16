//
// Created by konrad on 13.05.17.
//

#include <cstdlib>
#include "QueueStructure.h"


/**
 * Allocate memory for all variables, and initialize them by empty/nullable states
 *
 * @return new instance of queue structure with empty state
 */
QueueStructure* QueueStructure::create(){
    QueueStructure *createdQueue;

    createdQueue = (QueueStructure*) malloc (sizeof (QueueStructure));
    createdQueue->mutex = (pthread_mutex_t*) malloc(sizeof (pthread_mutex_t));
    createdQueue->isNotEmpty = (pthread_cond_t*) malloc(sizeof (pthread_cond_t));
    createdQueue->isNotFull = (pthread_cond_t*) malloc(sizeof (pthread_cond_t));

    //Starting states of queue[empty state]
    createdQueue->tailOfQueue = 0;
    createdQueue->headOfQueue = 0;
    createdQueue->isFull = false;
    createdQueue->isEmpty = true;
    pthread_mutex_init(createdQueue->mutex, NULL);
    pthread_cond_init(createdQueue->isNotFull, NULL);
    pthread_cond_init(createdQueue->isNotEmpty, NULL);

    return createdQueue;
}

/**
 * Destroy shared memory of conditions and mutex
 */
void QueueStructure::deleteAndFreeSpace() {
    pthread_mutex_destroy(this->mutex);
    pthread_cond_destroy(this->isNotEmpty);
    pthread_cond_destroy(this->isNotFull);

    free(this->mutex);
    free(this->isNotEmpty);
    free(this->isNotFull);

    //Dealoacte memory of structure
    free(this);
}

/**
 *Just adding an element to the queue, shifting tail variable, setting empty flag as false;
 *
 * @param content
 */
void QueueStructure::addData(int content){
    //Add element and shift tail of queue by one
    this->dataOfQueue[this->tailOfQueue] = content;
    this->tailOfQueue++;

    this->isEmpty = false;

    //Set zero for value of tailQueue if reached the end of data array
    if(this->tailOfQueue == SIZE_OF_QUEUE) {
        this->tailOfQueue = 0;
    }

    if(this->tailOfQueue == this->headOfQueue) {
        this->isFull = true;
    }

    return;
}

/**
 * Removing an element from the queue, setting full parameter as false
 *
 * @param content, it is an out element, getting and removing from the queue
 */
void QueueStructure::removeData(int *content) {
    *content = this->dataOfQueue[this->headOfQueue];
    this->headOfQueue++;

    this->isFull = false;

    if (this->headOfQueue == SIZE_OF_QUEUE) {
        this->headOfQueue = 0;
    }

    if (this->headOfQueue == this->tailOfQueue) {
        this->isEmpty = true;
    }
}

