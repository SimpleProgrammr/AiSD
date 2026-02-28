#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    if(a==NULL) {
        std::cerr << "Out of memory!" << std::endl;
    }
    int x,i;

    for(i=0; i<N;i++)
        a[i]=rand()%N;


    x = rand()%(N-1);


    long double time_sz = 0.0, time_wt = 0.0, time = 0.0;
    int its = 0;
    int limit = 20;
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
    }while(its<limit);

    time_sz /= its;
    time_wt /= its;

    std::cout << time_sz<<"\t"<<time_wt  << std::endl<<std::endl;


    std::ofstream SAVE("times.txt");

    for (int l = 0; l < log10(N); l++ ) {
        long long size = N/pow(10, l);
        int secs = 0;
        time_sz =0;
        time_wt =0;

        int iters = 0;
        do{
            x = rand()%(100000)+100000;
            gettimeofday(&c1,NULL);
            szuk(x,size,a);
            gettimeofday(&c2, NULL);
            secs = c2.tv_sec - c1.tv_sec;
            time = c2.tv_usec - c1.tv_usec + secs*1000000;
            //printf("czas - %Lg", time);
            time_sz += time/1000;


            gettimeofday(&c1,NULL);
            szuk_wart(x,size,a);
            gettimeofday(&c2,NULL);
            secs = c2.tv_sec - c1.tv_sec;
            time = c2.tv_usec - c1.tv_usec + secs*1000000;
            //printf("|%Lg\n", time);
            time_wt += time/1000;

            iters++;
        }while(iters<limit);

        time_sz /= its;
        time_wt /= its;

        std::cout << time_sz<<"\t"<<time_wt  << std::endl;
        SAVE << time_sz<<"\t"<<time_wt  << std::endl;
    }

    SAVE.close();

    free(a);
}