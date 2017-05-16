//
// Created by konrad on 16.05.17.
//

#ifndef MULTHITHREADINGCONSUMERSNPRODUCERS_PROCESS_H
#define MULTHITHREADINGCONSUMERSNPRODUCERS_PROCESS_H


#include "QueueStructure.h"

class Process {
    /*
     * Id for consumer/producer process
     */
    int processID;
    /*
     * Pointer for thread counter, to track how much have been done by thread.
     * How many have been produced/consumed
     */
    int *threadCounter;
    /*
     * Pointer to shared Queue
     */
    QueueStructure *queue;

public:
    int getProcessID() const {
        return processID;
    }

    void setProcessID(int processID) {
        Process::processID = processID;
    }

    int *getThreadCounter() const {
        return threadCounter;
    }

    void setThreadCounter(int *threadCounter) {
        Process::threadCounter = threadCounter;
    }

    QueueStructure *getQueue() const {
        return queue;
    }

    void setQueue(QueueStructure *queue) {
        Process::queue = queue;
    }
};



#endif //MULTHITHREADINGCONSUMERSNPRODUCERS_PROCESS_H
