//
// Created by konrad on 16.05.17.
//

#ifndef MULTHITHREADINGCONSUMERSNPRODUCERS_QUEUESTRUCTURE_H
#define MULTHITHREADINGCONSUMERSNPRODUCERS_QUEUESTRUCTURE_H

#include <pthread.h>

#define SIZE_OF_QUEUE 15

struct QueueStructure {
    int headOfQueue;
    int tailOfQueue;
    int dataOfQueue[SIZE_OF_QUEUE];

/**
 * For consumers, information to await some iterations to consume
 */
    pthread_cond_t *isNotEmpty;
/**
 *For producers, information to await some iterations to produce
 */
    pthread_cond_t *isNotFull;
/**
 * Protecting data of queue
 */
    pthread_mutex_t * mutex;

    bool isFull;
    bool isEmpty;


    /****METHODS*****/

    QueueStructure* create();
    void deleteAndFreeSpace();
    void addData(int);
    void removeData(int*);
};


#endif
