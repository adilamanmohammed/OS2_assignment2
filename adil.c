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
    n=atoi(argv[1]);
    //printf("%d\n",n);
    k=n/2;
    //printf("%d\n",n);

    if(argc==2)
    {
        //printf("okay\n");
        if(n>0 && n<15)
        {
            printf("okay2\n");
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
