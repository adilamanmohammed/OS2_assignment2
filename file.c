/*
Name : Adil Aman Mohammed
description : the following code is the implementation of readers and writers problem where the writer is going to update shared counter for 25000 and
                reader is going to loop for 250M times (relax and spend).
References : Lecture 6 ppt 
*/

//required libraries
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//global declaration of variables
int SC=0;  //shared counter 
int wflag = 0, rcount = 0;  //writer flag and reader counter


sem_t wrt,rd;  //writer and reader semaphore declaration

//relax and spend function declaration
void relaxandspendtime()
{
int i;
for(i = 0; i < 250000000; i++) i=i;
}


//writer function declaration
void *wrtfunc(void *arg){
    int i=0;

    //while loop for looping 25k times
    while(i<25000){
        
        //entry section
        //writer semaphore locked 
        sem_wait(&wrt);

        //giving a signal that writer is in critical section 
        wflag = 1;

        //critical section
        SC++;
        
        //exit section 
        //giving a signal that writer is out of critical section
        wflag = 0;
        
        //writer semaphore unlocked
        sem_post(&wrt);

        //remainder section
        i++;

    }

    //printing the counter value and writer is done
    printf("Writer done writing : ");
    printf("counter = %d increased by writer\n", SC);

    return NULL;
    
    
}

void *rdfunc(void *value){

    //entery section
    //initializing j
    int j=0;
    int x = *((int *) value); //converting pointer value to integer 
        
        //if reader enters writers giving an error
        if(wflag == 1)
        {
            printf("Error, writer in critical section");
        }
       
        //critical section
       for(j=0;j<250000000;j++)
       {
        sem_wait(&rd);

        rcount++;

        if(rcount == 1)
        {
            sem_wait(&wrt);

        }
        sem_post(&rd);

        SC=SC;
        //relaxandspendtime();

        sem_wait(&rd);

        rcount--;

        if(rcount == 0)
        {
            sem_post(&wrt);
        }

        sem_post(&rd);
       }
    
    //exit section
    printf("Reader %d Done reading\n", x+1);

    //remainder section
    return NULL;

}

//main function declaration 
int main(int argc, char* argv[])
{
    //variable declaration
    int n;
    n = atoi(argv[1]);

    //loop to check that if given number of arguments in comment line is only 1 value
    if(argc==2)
    {   
        //loop to check that if the n value is in the range of 1 to 14
        if(n >= 1 && n <= 14)
        {           
            //variable declaration
                    int k,i;
                    k = (int)(n/2);

                    //declaration of reader and writer threads
                    pthread_t reader[n], writer;

                    //initialization of writer and reader semaphore 
                    sem_init(&wrt, 0, 1);
                    sem_init(&rd, 0, 1); 

                    //creating 0 to k reader threads
                    for(i = 0; i < k; i++)
                        {
                            int *x = malloc(sizeof(int));
                            *x = i;
                            pthread_create(&reader[i], NULL , rdfunc, x);
                        }


                    //creating writer threads
                    pthread_create(&writer, NULL , wrtfunc, NULL);

                    //creating k to n reader threads
                    for(i = k ; i < n ; i++)
                        {
                            int *x = malloc(sizeof(int));
                            *x = i;
                            pthread_create(&reader[i], NULL , rdfunc, x);
                        } 

                    //joining 0 to n reader threads
                    for(i=0;i<n;i++)
                        {
                            pthread_join(reader[i], NULL);
                        }
                    
                    //joining writer thread
                        pthread_join(writer, NULL);

                    //killing the semaphores
                        sem_destroy(&wrt);
                        sem_destroy(&rd);
        }
        else
        {
        //printing message if condition not satisfied
        printf("n shound be n>0 and n<15\n");

        }
    }
    else
    {
    //printing message if arguments in comment line are more than 1
    printf("invalid input\n");
    }
    return 0;
}
