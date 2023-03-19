#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

int nmax=14;
int counter=0;
int readmax=250000000;
int flag=0;



int main(int argc,char* argv[])
{
    int i,k,n;
    int attr[10000];
    n=atoi(argv[1]);
    //printf("%d\n",n);
    k = (int) (n/2);
    //printf("%d\n",n);

    if(argc==2)
    {
        //printf("okay\n");
        if(n>0 && n<15)
        {
            //printf("okay2\n");
            for(i=0;i<k;i++)
            {
                for(i = 0; i < k; i++)
                    {
                        //printf("%d\n",i);
                        pthread_create(&readers[i], &attr[0], reader_thread,(void*) i);
                    }

                    for(i = 0; i < k; i++)
                    {
                        //printf("%d\n",i);
                        pthread_join(&readers[i],NULL);
                    }

                    pthread_create(&writer[0], &attr[0], writer_thread,NULL);
                    pthread_join(&writer[0],NULL);

                    for(i = k ; i < n ; i++)
                        {
                            pthread_create(&readers[i], &attr[0], reader_thread,(void*) i);
                        }
                    for(i = k ; i < n ; i++)
                        {
                            pthread_join(&readers[i], NULL);
                        }


            }
        }
        else{
            printf("invalid : n should be more than 0 and less than 15\n");
        }
    }
    else{
        printf("enter valid input\n");
    }

    return 0;
}
