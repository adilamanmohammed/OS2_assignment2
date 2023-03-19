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
    while(i<2500){

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

    while(j<25000){
            

        sem_wait(&rd);
        rcount++;
        if(rcount == 1){
            sem_wait(&wrt);

        }
        sem_post(&rd);

        sem_wait(&rd);
        if(wflag == 1){
            printf("Error, writer is in it's critical section");
        }
        sem_post(&rd);
        
        SC=SC;  //relax and spendtime
        

        sem_wait(&rd);
        rcount--;
        if(rcount == 0){
            sem_post(&wrt);
        }
        j++;
        sem_post(&rd);

    }

    printf("Reader %d Done reading\n", x+1);


    return NULL;

}


int main(int agrc, char* argv[]){
    int n;
    n = atoi(argv[1]);
    if(n >= 1 && n <= 14){
        pthread_t reader[n], writer;
        sem_init(&wrt, 0, 1);
        sem_init(&rd, 0, 1);
        int k,i;
        k = (int)(n/2);
        for(i = 0; i < k; i++)
        {
            int *x = malloc(sizeof(int));
            *x = i;
            pthread_create(&reader[i], NULL , rdfunc, x);
        }
        /* Create the writer thread */
        pthread_create(&writer, NULL , wrtfunc, NULL);
        for(i = k ; i < n ; i++)
        {
            int *x = malloc(sizeof(int));
            *x = i;
            pthread_create(&reader[i], NULL , rdfunc, x);
        } 

        for(i=0;i<n;i++){
            pthread_join(reader[i], NULL);
        }
        pthread_join(writer, NULL);

        sem_destroy(&wrt);
        sem_destroy(&rd);

        


    }
    else{
        printf("n shound be n>0 and n<15\n");

    }
}






