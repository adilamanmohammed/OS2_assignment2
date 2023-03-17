
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>


struct shared_dat
   {
   int value;     /* shared variable to store result*/
   };


struct shared_dat  *counter;      //shared variable
sem_t writer_lock;                  //writes lock
pthread_mutex_t mutex;              //mutex used to protect no_of_readers varible in reader_thread.
int no_of_readers ;                 //no of readers in CS.
pthread_mutex_t flag;                 //stop upcoming reader from entrying crtitcal section
bool check = true;                    //check if writer_thread is writing.
int n;                                  //no of readers


//reader function
void relaxandspendtime() 
{
    
        for(int i = 0; i < 250000000; i++) counter->value = counter->value;
}


void *reader_thread (void *arg)
{
        int line = 0;
        int  x = *(int *) arg;
    
    //this code will stop upcoming reader from entering critical section when writer_thread is updating.
        while (pthread_mutex_trylock(&flag) != 0){
        }
        pthread_mutex_unlock(&flag);
    
    //this code will lock the writer if no_of_readers is more than 1 and also lock the reader if writer_thread is updating the varible.
    pthread_mutex_lock(&mutex);
    no_of_readers++;
    if(no_of_readers == 1 ) {
        sem_wait(&writer_lock);
    }
    pthread_mutex_unlock(&mutex);
    
    /*if writer_thread is updating the varible and reader_thread will try to access that varible then it will display error*/
    if(check == false)
        printf("Error: Write is writing");
    
    relaxandspendtime();
    
    //this code will unlock the writer if no_of_readers is equal to 0.
    pthread_mutex_lock(&mutex);
    no_of_readers--;
    if(no_of_readers == 0) {
        sem_post(&writer_lock);
    }
    pthread_mutex_unlock(&mutex);

        printf(" Reader %d   Done    25M  ,  last value read  = %d\n",x, counter->value); 
    return(NULL);
}

void *writer_thread(void *arg)
{
        int line = 0;   
        pthread_mutex_lock(&flag);    //flag lock
    sem_wait(&writer_lock);      //semaphore lock
        while (line < 25000)
    {   
        check = false;              //writer_thread is updating
        line++;
        counter->value = counter->value + 1;
        check = true;               //writer_thread has finished updating.
        }
    pthread_mutex_unlock(&flag);   //flag unlock
    sem_post(&writer_lock);      //semaphore unlock
         
        printf(" Writer   Done    25K   ,   last value read  = %d\n" , counter->value); 
    return(NULL);
}

int main(int argc,char* argv[]) 
{ 
    
    if(argc  != 2 ) 
       { printf("\nNo input is entered"); return 1;}
    if(argc == 2) 
    { 
       n =  atoi(argv[1]); 
       if(n < 2 || n > 16){
           printf("invalid input");
           return 1;
       }
    } 

    pthread_t   readers[n];             //readers pthread array
    pthread_t writer[1];                //writer pthread array
    
    pthread_attr_t      attr[1];     /*attribute pointer array */

        /*semaphore and mutex initialize*/
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&flag, NULL);
        sem_init(&writer_lock, 0, 1);   
    counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));
    
    int k = (int) (n/2);
    int i;
    int temp[n];
    
    /* Create the writer and reader thread */
    for(i = 0; i < k; i++){
        temp[i] = i;
        pthread_create(&readers[i], NULL,reader_thread,  (void *) &temp[i]);
    }
    pthread_create(&writer[0], NULL, writer_thread, NULL);
    for(i = k ; i < n ; i++) {
       temp[i] = i;
    pthread_create(&readers[i],NULL,reader_thread, &temp[i] );
    }
    
    //waiting for all the thread to finish
    for(i = 0; i < n; i++){
        pthread_join(readers[i], NULL);
    }
    pthread_join(writer[0],NULL);
    
    // releasing free space
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&flag);
    sem_destroy(&writer_lock);
    
        return 0; 
} 
