
#include<stdio.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>

int nmax=14;
int Readmax=250000000;

int counter=0;
int wflag=0;
sem_t mutex,wrt;


void RnS()
{
    int i;
    for(i=0;i<250000000;i++)
    {
        i=i;
    }
    return 0;
}

void* readerfunc()
{
    int i=0;
    /*for(i=0;i<Readmax;i++)
    {
        sem_wait(&mutex);
        if(flag==1)
    }
    */
    printf("reader okay\n");
}

void* writerfunc()
{
    
}





int main(int argc, char* argv[])
{
    int n=atoi(argv[1]),i;
    int k=n/2;

    printf("c=%d n=%d\n",argc,n);

    sem_init(&mutex,0,1);
    sem_init(&wrt,0,1);
    pthread_t R[n], writer;




    if(argc==2){
        //printf("\ngood\n");
        if(n>0 && n<15)
        {
            //printf("\nn is okay\n");
            
            for(i=0;i<k;i++)
            {
                pthread_create(&R[i],NULL,readerfunc,NULL);
            }


            pthread_create(&writer,NULL,writerfunc,NULL);


            for(i=k;i<n;i++)
            {
                pthread_create(&R[i],NULL,readerfunc,NULL);
            }
            

            for(i=0;i<n;i++)
            {
                pthread_join(&R[i],NULL);
            }
            pthread_join(&writer,NULL);

            sem_destroy(&mutex);
            sem_destroy(&wrt);


        }
        else{
            printf("\natleast 1 reader or max of 14 readers\n");
        }
    }
    else
    {
        printf("\ninvalid arguments\n");
        exit(0);
    }
    return 0;
}
