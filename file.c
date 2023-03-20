#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t writer_mutex, reader_mutex;
int sharedCounterValue=0;
int flag = 0, readCnt = 0;

void *writerFunction(void *arg){
    int w;
    for(w = 0; w < 2500; w++){
        sem_wait(&writer_mutex);
        flag = 1;
        sharedCounterValue++;
        //printf("Counter Value increased by writer = %d\n", sharedCounterValue);
        flag = 0;
        sem_post(&writer_mutex);
    }
    printf("Writer done writing\n");

    return NULL;
    
    
}

void *readerFunction(void *arg){
    int id = *((int *) arg);
    free(arg);

    for(int j = 0; j < 250000000; j++ ){
            

        sem_wait(&reader_mutex);
        readCnt++;
        if(readCnt == 1){
            sem_wait(&writer_mutex);

        }
        sem_post(&reader_mutex);

        sem_wait(&reader_mutex);
        if(flag == 1){
            printf("Error, writer is in it's critical section");
        }
        sem_post(&reader_mutex);
        
        sharedCounterValue = sharedCounterValue;


        //printf("Reader %d reading %d\n", id+1, sharedCounterValue);

        sem_wait(&reader_mutex);
        readCnt--;
        if(readCnt == 0){
            sem_post(&writer_mutex);
        }
        sem_post(&reader_mutex);

    }
    printf("Reader %d Done reading\n", id+1);


    return NULL;

}


int main(int agrc, char* argv[]){
    int n;
    n = atoi(argv[1]);
    if(n >= 1 && n <= 14){
        pthread_t readerThreads[n], writer;
        sem_init(&writer_mutex, 0, 1);
        sem_init(&reader_mutex, 0, 1);
        int k,i;
        k = (int)(n/2);
        for(i = 0; i < k; i++)
        {
            int *id = malloc(sizeof(int));
            *id = i;
            pthread_create(&readerThreads[i], NULL , readerFunction, id);
        }
        /* Create the writer thread */
        pthread_create(&writer, NULL , writerFunction, NULL);
        for(i = k ; i < n ; i++)
        {
            int *id = malloc(sizeof(int));
            *id = i;
            pthread_create(&readerThreads[i], NULL , readerFunction, id);
        } 

        for(i=0;i<n;i++){
            pthread_join(readerThreads[i], NULL);
        }
        pthread_join(writer, NULL);

        sem_destroy(&writer_mutex);
        sem_destroy(&reader_mutex);

        


    }
    else{
        printf("Number of Threads entered %d is not in the range of 1 and 14\nPlease try again with a number that falls within the range", n);

    }
}
