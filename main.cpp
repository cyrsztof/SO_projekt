//
// Created by krzysztof-c on 10.05.17.
//
#include <pthread.h>
#include <iostream>
#include <cstdlib>

/* pthread_create (thread, attr, start_routine, arg)
 *
 * pthread_create creates new thread,
 * "thread" - unique identifier for the new thread
 * "attr" - opaque attribute object to set thread attributes, NULL for default
 * "start_routine" - the C++ routine that the thread will execute once it is created
 * "arg" - single argument that may be passed to start_routine. It must be passed by
 *         reference as a pointer cast of type void. NULL may be used if no argument is to be passed.
 *
 * */

#define NUM_THREADS     5

void *PrintHello(void *threadID) {
    long tid;
    tid = (long)threadID;
    std::cout << "Hello World! Thread ID, " << tid << std::endl;
    pthread_exit(NULL);
}
int main() {

    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    for( i=0; i < NUM_THREADS; i++ ){
        std::cout << "main() : creating thread, " << i << std::endl;
        rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);

        if (rc){
            std::cout << "Error:unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }

    pthread_exit(NULL);
}
