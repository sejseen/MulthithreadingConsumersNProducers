//
// Created by konrad on 13.05.17.
//

#include <time.h>
#include <cstdio>
#include "WorkerService.h"

/**
 * Implementation of sleep method
 *
 * @param miliSecunds Time to sleep in Milisecund
 */
void WorkerService::sleep(unsigned int miliSecunds) {
    struct timespec req = {0};
    time_t seconds;

    seconds = (miliSecunds/1000);
    miliSecunds = miliSecunds - (seconds * 1000);

    //Convert for setting up a timespec structure
    req.tv_sec = seconds;
    req.tv_nsec = miliSecunds * 1000000L;

    //Sleep thread for required period of time. First parametr specifies how long
    //second is neccessary because, thread could be awakened before the period is over
    while (nanosleep(&req, &req) == -1) {
        printf("Sleepin'");
        continue;
    }

}

/**
 * Method for simulate some work in cpu, per thread
 * @param numberOfCPUiterations
 * @param timeForBlock
 */
void WorkerService::simulateWork(int numberOfCPUiterations, int timeForBlock) {
    int copyVariable;
    for(int i = 0; i < numberOfCPUiterations;i++) {
        for(int j =  0; j< 1000 ; j++) {
            copyVariable = this->memoryArea[j];
        }
    }

    if(timeForBlock > 0) {
        sleep(timeForBlock);
    }
}
