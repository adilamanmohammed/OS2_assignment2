#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int SC=0;  //shared counter
int wflag = 0, rcount = 0;  //writer flag and reader counter

sem_t wrt,rd;  //writer and reader semaphore declaration


void relaxandspendtime()
{
int i;
for(i = 0; i < 250000000; i++) i=i;
}



void *wrtfunc(void *arg){
    int i=0;
    while(i<25000){

        sem_wait(&wrt);
        wflag = 1;


        SC++;
        
        
        wflag = 0;
        i++;
        sem_post(&wrt);

    }
    printf("counter increased by %d\n", SC);
    printf("Writer done writing\n");

    return NULL;
    
    
}

void *rdfunc(void *arg){
    int j=0;
    int x = *((int *) arg);

    while(j<250000){
            

        sem_wait(&rd);

        rcount++;
        if(rcount == 1)
        {
            sem_wait(&wrt);

        }
        sem_post(&rd);

        sem_wait(&rd);
        if(wflag == 1){
            printf("Error, writer in critical section");
        }
        sem_post(&rd);
        
        SC=SC;  //relax and spendtime
        

        sem_wait(&rd);

        rcount--;
        if(rcount == 0)
        {
            sem_post(&wrt);
        }

        j++;
        sem_post(&rd);

    }

    printf("Reader %d Done reading\n", x+1);


    return NULL;

}


int main(int argc, char* argv[])
{
    int n;
    n = atoi(argv[1]);
    if(argc==2)
    {
        if(n >= 1 && n <= 14)
        {           
                    int k,i;
                    k = (int)(n/2);

                    pthread_t reader[n], writer;

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
        printf("n shound be n>0 and n<15\n");

        }
    }
    else
    {
    printf("invalid input\n");
    }
    return 0;
}


