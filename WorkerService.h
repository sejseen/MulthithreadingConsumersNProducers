//
// Created by konrad on 13.05.17.
//

#ifndef SO_PROJECT_WORKERSERVICE_H
#define SO_PROJECT_WORKERSERVICE_H


struct WorkerService {
    int memoryArea[1000000];

    void sleep(unsigned int miliSecunds);
    void simulateWork(int numberOfCPUiterations, int timeForBlock);
};

#endif //MULTHITHREADINGCONSUMERSNPRODUCERS_WORKERSERVICE_H
