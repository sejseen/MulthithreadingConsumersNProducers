#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "WorkerService.h"
#include "QueueStructure.h"
#include "Process.h"
#include <curses.h>
#include <string.h>
#include <zconf.h>
/*
 * Define constants for how big the shared queue should be and how
 * much total work the produceers and consumers should perform
 */
#define ITEM_TO_PRODUCE_AND_CONSUME 10

/*
 * These constants specify how much CPU bound work the producer and
 * consumer do when processing an item. They also define how long each
 * blocks when producing an item.
 */
#define PRODUCER_CPU   25
#define PRODUCER_BLOCK 5000
#define CONSUMER_CPU   25
#define CONSUMER_BLOCK 5000

static const char *const MENU_OPTION_FIRST_ENTER = "Start multithreading program\n";
static const char *const MENU_OPTION_SECOND_QUIT = "Exit from the program\n";
static const char *const FAIL_ALERT_MESSAGE = "Colors are not available in terminal\n";
static const char *const HEADER_TOP_MENU = "\n\t\t\t\t\t\t\tMENU FOR CONS&PROS";

/*Static constant for path of saved content*/
static const char *const pathForProducersContent = "producerData.txt";
/*Static constant for path of saved content*/
static const char *const pathForConsumersContent = "consumerData.txt";

/*GUI open Method*/
WINDOW *initMenuDialog(const char *header);
/*GUI close Method*/
void closeGui();

void *produceElement(void*);
void *consumeElement(void*);

int main (int argc, char *argv[]) {

    int inputChoice;
    int highlighted = 0;

    char image[] = "\t\t\t\t\t\t\t______________¶¶¶\n"
            "\t\t\t\t\t\t\t_____________¶¶_¶¶¶¶\n"
            "\t\t\t\t\t\t\t____________¶¶____¶¶¶\n"
            "\t\t\t\t\t\t\t___________¶¶¶______¶¶\n"
            "\t\t\t\t\t\t\t___________¶¶¶_______¶¶\n"
            "\t\t\t\t\t\t\t__________¶¶¶¶________¶¶\n"
            "\t\t\t\t\t\t\t__________¶_¶¶_________¶¶\n"
            "\t\t\t\t\t\t\t__________¶__¶¶_________¶¶____¶¶\n"
            "\t\t\t\t\t\t\t__________¶__¶¶__________¶¶¶¶¶¶¶\n"
            "\t\t\t\t\t\t\t_________¶¶__¶¶¶______¶¶¶¶¶¶___¶\n"
            "\t\t\t\t\t\t\t_________¶¶___¶¶__¶¶¶¶¶¶__¶¶\n"
            "\t\t\t\t\t\t\t_______¶¶_¶____¶¶¶¶________¶¶\n"
            "\t\t\t\t\t\t\t______¶¶__¶¶___¶¶__________¶¶\n"
            "\t\t\t\t\t\t\t_____¶¶____¶¶___¶¶__________¶¶\n"
            "\t\t\t\t\t\t\t___¶¶_______¶¶___¶¶_________¶¶\n"
            "\t\t\t\t\t\t\t___¶¶¶¶¶¶¶¶¶¶¶¶¶__¶¶_________¶\n"
            "\t\t\t\t\t\t\t_¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶_¶¶________¶¶\n"
            "\t\t\t\t\t\t\t¶¶__¶¶¶¶¶¶____¶¶¶¶¶¶¶¶¶______¶¶\n"
            "\t\t\t\t\t\t\t¶¶¶¶¶___¶______¶___¶¶¶¶¶_____¶¶\n"
            "\t\t\t\t\t\t\t________¶¶¶¶¶¶¶¶______¶¶¶¶¶_¶¶\n"
            "\t\t\t\t\t\t\t______¶¶¶¶¶¶¶¶¶¶¶________¶¶¶¶\n"
            "\t\t\t\t\t\t\t______¶¶¶¶¶¶¶¶¶¶¶¶\n"
            "\t\t\t\t\t\t\t______¶__¶¶_¶¶¶¶¶¶\n"
            "\t\t\t\t\t\t\t_____¶¶______¶___¶\n"
            "\t\t\t\t\t\t\t_____¶¶_____¶¶___¶\n"
            "\t\t\t\t\t\t\t_____¶______¶¶___¶\n"
            "\t\t\t\t\t\t\t____¶¶______¶¶___¶¶\n"
            "\t\t\t\t\t\t\t____¶¶______¶¶___¶¶\n"
            "\t\t\t\t\t\t\t___¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶\n"
            "\t\t\t\t\t\t\t__¶¶¶¶¶¶¶¶¶_¶¶¶¶¶¶¶¶\n"
            "\t\t\t\t\t\t\t__¶¶________¶¶¶____¶¶\n"
            "\t\t\t\t\t\t\t____¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶\n";

    //Get args

    pthread_t *consumerThread;
    int numberOfConsumers = atoi(argv[1]);
    int *consumerCounter;

    QueueStructure *queueStructure;
    int i;

    pthread_t *producerThread;
    int *producerCounter;
    int numberOfProducers = atoi(argv[2]);

    Process *threadArguments;

    WINDOW *inputWindow = initMenuDialog(image);

    const char *menuOption[] = {
            MENU_OPTION_FIRST_ENTER,
            MENU_OPTION_SECOND_QUIT,
    };

    while (1) {
        for (i = 0; i < 2; i++) {
            if (i == highlighted)
                wattron(inputWindow, A_REVERSE);
            mvwprintw(inputWindow, i + 1, 1, menuOption[i]);
            if (i == highlighted)
                wattroff(inputWindow, A_REVERSE);
        }

        inputChoice = wgetch(inputWindow);

        switch (inputChoice) {
            case KEY_UP:
                highlighted--;
                if (highlighted < 0) highlighted = 0;
                break;
            case KEY_DOWN:
                highlighted++;
                if (highlighted > 1) highlighted = 1;
                break;
            default:
                break;
        }

        if (inputChoice == 10) break;
    }

    if(menuOption[highlighted] == MENU_OPTION_SECOND_QUIT) {
    printw("\nYour choice was:%s\n", menuOption[highlighted]);
    refresh();

    /* Wait for user to press enter to exit */
    closeGui();

    } else if (menuOption[highlighted] == MENU_OPTION_FIRST_ENTER) {

        //For present an ncurses bug with threads, doesnt printing from more than 2 threads
//        wclear(inputWindow);
//        clear();
//
//        scrollok(stdscr,TRUE);
//        attron( A_BOLD );
//
//        refresh();
    endwin();

    if (numberOfConsumers < 1 || numberOfProducers < 1) {
        fprintf(stderr, "Something wrong, Number of cons or pros less than 1");
        exit(1);
    }

        /*
         * Create the shared queue
         */
        queueStructure = queueStructure->create();
        if (queueStructure == NULL) {
            fprintf(stderr, "main: Queue Init failed.\n");
            exit(1);
        }


        /**
        *Counters for how many items were produced/consumed shared for all workers
         */
        consumerCounter = (int *) malloc(sizeof(int));
        producerCounter = (int *) malloc(sizeof(int));

        /*
         * Create arrays of thread structures, one for each producer and
         * consumer
         */
        producerThread = (pthread_t *) malloc(sizeof(pthread_t) * numberOfProducers);
        if (producerThread == NULL) {
            fprintf(stderr, "pros\n");
            exit(1);
        }

        consumerThread = (pthread_t *) malloc(sizeof(pthread_t) * numberOfConsumers);
        if (consumerThread == NULL) {
            fprintf(stderr, "cons\n");
            exit(1);
        }

        /*
         * Create the specified number of producers
         */
        for (i = 1; i <= numberOfProducers; i++) {

            threadArguments = (Process *) malloc(sizeof(Process));

            threadArguments->setQueue(queueStructure);
            if (threadArguments->getQueue() == NULL) {
                fprintf(stderr, "Something wrong, Setting queue for thread not success");
            }

            threadArguments->setThreadCounter(producerCounter);
            threadArguments->setProcessID(i);
            pthread_create(&producerThread[i], NULL, produceElement, threadArguments);
        }

        /*
         * Create the specified number of consumers
         */
        for (i = 1; i <= numberOfConsumers; i++) {
            /*
             * Allocate space for next consumer's args
             */
            threadArguments = (Process *) malloc(sizeof(Process));
            if (threadArguments == NULL) {
                fprintf(stderr, "main: Thread_Args Init failed.\n");
                exit(1);
            }

            /*
             * Fill them in and create the thread
             */
            threadArguments->setQueue(queueStructure);
            threadArguments->setThreadCounter(consumerCounter);
            threadArguments->setProcessID(i);
            pthread_create(&consumerThread[i], NULL, consumeElement, threadArguments);
        }

        /*
         The pthread_join() function suspends execution of the calling thread until the
         target thread terminates, unless the target thread has already terminated
         int pthread_join(pthread_t thread, void **value_ptr);
         */
        for (int i = 0; i < numberOfProducers; i++) {
            pthread_join(producerThread[i], NULL);
        }
        for (int j = 0; j < numberOfConsumers; j++) {
            pthread_join(consumerThread[j], NULL);
        }

        queueStructure->deleteAndFreeSpace();
    }

    return 0;
}

WINDOW *initMenuDialog(const char *header) {
    int widthOfTerminal, lengthOfTerminal;

    initscr(); //Inizjalizacja całości ncurses, kolory itp
    raw();
    noecho();
    keypad(stdscr, TRUE);
    attron( A_BOLD );
    start_color();
    init_pair(1,COLOR_GREEN, COLOR_BLACK);


    getmaxyx(stdscr, widthOfTerminal, lengthOfTerminal);
    /**Terminal on main area**/
    move(widthOfTerminal / 2, (lengthOfTerminal - strlen(header)) / 2);

    WINDOW *inputWindow = newwin(7, lengthOfTerminal - 12, widthOfTerminal - 9, 6);
    box(inputWindow, 0, 0);

    printw(header);
    refresh();

    attron(COLOR_PAIR(1));
    mvprintw(32, 30,HEADER_TOP_MENU);
    attroff(COLOR_PAIR(1));
    refresh();

    refresh();
    wrefresh(inputWindow);

    if (has_colors() == false || start_color() != OK) {
        endwin();
        puts(FAIL_ALERT_MESSAGE);
        exit(EXIT_FAILURE);
    }

    keypad(inputWindow, TRUE);
    return inputWindow;
}

void *produceElement (void *producerArg)
{
    Process *producerData;
    int currentProducedData;
    producerData = (Process *) producerArg;
    WorkerService workerService;

    QueueStructure *fifoQueue = producerData->getQueue();
    int individualId = producerData->getProcessID();
    int *summaryProduced = producerData->getThreadCounter();

    /*
     * Continue producing until the total produced reaches the
     * configured maximum
     */
    while (true) {
        /*
         * Do work to produce an item. Tthe get a slot in the queue for
         * it. Finally, at the end of the loop, outside the critical
         * section, announce that we produced it.
         */
        workerService.simulateWork(PRODUCER_CPU, PRODUCER_BLOCK);
        workerService.savingDataToFile(pathForProducersContent, "produceSth\n");

        /*
         * If the queue is full, we have no place to put anything we
         * produce, so wait until it is not full.
         */
        pthread_mutex_lock(fifoQueue->mutex );
        while (fifoQueue->isFull && *summaryProduced < ITEM_TO_PRODUCE_AND_CONSUME) {

            pthread_cond_wait(fifoQueue->isNotFull,fifoQueue->mutex);

            printf("No place to produce anything, PRODUCER_%d waitin'\n", individualId);
        }

        /*
         * Check to see if the total produced by all producers has reached
         * the configured maximum, if so, we can quit.
         */
        if (*summaryProduced >= ITEM_TO_PRODUCE_AND_CONSUME) {
            pthread_mutex_unlock(fifoQueue->mutex);
            break;
        }

        /*
         * OK, so we produce an item. Increment the counter of total
         * produced, and add the new ID, its number, to the
         * queue.
         */
        currentProducedData = (*summaryProduced)++;
        fifoQueue->addData(currentProducedData);
        pthread_mutex_unlock(fifoQueue->mutex );
        pthread_cond_signal(fifoQueue->isNotEmpty);


        /*
         * Announce the production outside the critical section
         */
        printf("Announce produced item: %d by PRODUCER_%d.\n", currentProducedData, individualId);
    }

    printf("Nothing to do for me PRODUCER_%d:  See ya\n", individualId);

    return (NULL);
}

void *consumeElement (void *consumerArg)
{
    Process *consumerData;
    int currentConsumedData;
    consumerData = (Process*) consumerArg;
    WorkerService workerService;

    QueueStructure *fifoQueue = consumerData->getQueue();
    int individualId = consumerData->getProcessID();
    int *summaryConsume = consumerData->getThreadCounter();

    /*
     * Continue producing until the total consumed by all consumers
     * reaches the configured maximum
     */
    while (true) {
        /*
         * If the queue is empty, there is nothing to do, so wait until it
         * si not empty.
         */
        pthread_mutex_lock(fifoQueue->mutex );
        while (fifoQueue->isEmpty && *summaryConsume <= ITEM_TO_PRODUCE_AND_CONSUME) {
            printf("Queue is empty, consumer %d just waitin' :  EMPTY QUEUE.\n", individualId);
            pthread_cond_wait(fifoQueue->isNotEmpty,fifoQueue->mutex);
        }

        /*
         *Stop the creating items, exit the loop and finish the program
         */
        if (*summaryConsume >= ITEM_TO_PRODUCE_AND_CONSUME) {
            pthread_mutex_unlock(fifoQueue->mutex);
            break;
        }
        /*
         * Remove the next item from the queue. Increment the count of the
         * total consumed.
         */
        fifoQueue->removeData(&currentConsumedData);
        (*summaryConsume)++;
        pthread_mutex_unlock(fifoQueue->mutex );
        pthread_cond_signal(fifoQueue->isNotFull);


        /*
         * Do work outside the critical region to consume the item
         * obtained from the queue and then announce its consumption.
         */
        workerService.simulateWork(CONSUMER_CPU,CONSUMER_BLOCK);

        printf("Announce consuming item: %d by CONSUMER_%d.\n",currentConsumedData, individualId);

    }

    printf("Damn nothing to eat. I'm out! CONSUMER_%d \n", individualId);
    return (NULL);
}

void closeGui() {
    clear();
    attron( A_BOLD );
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvprintw(4, 50,"SYSTEMY OPERACYJNE 2 - Projekt");
    mvprintw(6, 50,"Symulacja pracy konsumentów oraz producentów");
    mvprintw(12, 50,"Autor: Konrad Tyma");
    mvprintw(13, 50,"Numer indeksu: 218700");
    mvprintw(16, 50,"Prowadzacy inz. mgr Szymon Datko");
    mvprintw(19, 50,"");
    attroff(COLOR_PAIR(1));
    refresh();

    getch();
    endwin();
}
