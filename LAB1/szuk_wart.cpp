#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
long long N= 1000000000;

int szuk_sort_wart(int x, int n, int *a)
{
    int i=0;
    a[n]=x;
    while(x>a[i])
        i++;
    if (a[i]!=x) return n;
    return i;
}

int szuk_sort(int x, int n, int *a)
{
    int i=0;

    while(i<n && x>a[i])
        i++;
    if (i>=n) return n;
    if (a[i]==x) return i;
    return n;
}

int szuk_wart(int x, int n, int *a)
{
    int i=0;
    a[n]=x;
    while(x!=a[i])
        i++;
    return i;
}

int szuk(int x, int n, int *a)
{
    int i=0;

    while(i<n && x!=a[i])
        i++;
    return i;
}

int main(int argc, char *argv[])
{
    struct timeval c1, c2;
    int* a = static_cast<int *>(calloc(N, sizeof(int)));
    int x,i;

    for(i=0; i<N;i++)
        a[i]=rand()%1000000;


    x = rand()%10000;


    long double time_sz = 0.0, time_wt = 0.0, time = 0.0;
    int its = 0;
    do{

        gettimeofday(&c1,NULL);
        szuk(x,N,a);
        gettimeofday(&c2, NULL);
        time = c2.tv_usec - c1.tv_usec;
        //printf("czas - %Lg", time);
        time_sz += time/1000;


        gettimeofday(&c1,NULL);
        szuk_wart(x,N,a);
        gettimeofday(&c2,NULL);
        time = c2.tv_usec - c1.tv_usec;
        //printf("|%Lg\n", time);
        time_wt += time/1000;

        its++;
    }while(its<50);

    time_sz /= its;
    time_wt /= its;

    std::cout << time_sz<<"\t"<<time_wt  << std::endl<<std::endl;


    for (int l = 0; l < log10(N); l++ ) {
        long long size = N/pow(10, l);
        time_sz =0;
        time_wt =0;

        int iters = 0;
        do{
x = rand()%N/100000+100000;
            gettimeofday(&c1,NULL);
            szuk(x,N,a);
            gettimeofday(&c2, NULL);
            time = c2.tv_usec - c1.tv_usec;
            //printf("czas - %Lg", time);
            time_sz += time/1000;


            gettimeofday(&c1,NULL);
            szuk_wart(x,N,a);
            gettimeofday(&c2,NULL);
            time = c2.tv_usec - c1.tv_usec;
            //printf("|%Lg\n", time);
            time_wt += time/1000;

            iters++;
        }while(iters<10);

        time_sz /= iters;
        time_wt /= iters;

        std::cout << time_sz<<"\t"<<time_wt  << std::endl;
    }

    free(a);
}