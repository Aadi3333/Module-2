#include <Arduino.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <time.h>
#include<unistd.h>
#include<string>
using namespace std;

#define NUM_THREADS 8
#define MAX 1000000
/// <summary>
/// initialize mutex for thread safety
/// </summary>
pthread_mutex_t mutx; 

int data[MAX];
long global_sum = 0;

/// <summary>
/// function to be executed by threads
/// </summary>
/// <param name="threadid"></param>
/// <returns></returns>
void *sum(void *threadid) 
{
   long tid;
   long sum = 0;
   tid = (long)threadid;
   int range = MAX/NUM_THREADS;
   /// <summary>
   /// calculate the start index for each thread
   /// </summary>
   /// <param name="threadid"></param>
   /// <returns></returns>
   int start = tid * range ; 

   /// <summary>
   /// calculate the end index for each thread
   /// </summary>
   /// <param name="threadid"></param>
   /// <returns></returns>
   int end = start + range ;


   /// <summary>
   /// compute the sum for the elements in the thread's range
   /// </summary>
   /// <param name="threadid"></param>
   /// <returns></returns>
   for(int i = start ; i < end ; i++) {
        sum += data[i];
   }

   /// <summary>
   /// locking the threads to calculate the sum 
   /// when a integer is sent to first thread then it will lock
   /// </summary>
   /// <param name="threadid"></param>
   /// <returns></returns>
   pthread_mutex_lock(&mutx);

   /// <summary>
   /// update the global sum using mutex to avoid race conditions
   /// </summary>
   /// <param name="threadid"></param>
   /// <returns></returns>
   global_sum += sum;

   /// <summary>
   /// after updating the global sum values it will unlock the threads 
   /// </summary>
   /// <param name="threadid"></param>
   /// <returns></returns>
   pthread_mutex_unlock(&mutx);

   /// <summary>
   /// exit the thread
   /// </summary>
   /// <param name="threadid"></param>
   /// <returns></returns>
   pthread_exit(NULL); 
}

int main(int argc, char *argv[])
{ 
    int cores = std::thread::hardware_concurrency();

    /// <summary>
    /// initialize mutex
    /// </summary>
    /// <param name="argc"></param>
    /// <param name="argv"></param>
    /// <returns></returns>
    pthread_mutex_init(&mutx, NULL);

    /// <summary>
    /// will print the number of cores used in the code from machine
    /// </summary>
    /// <param name="argc"></param>
    /// <param name="argv"></param>
    /// <returns></returns>
    cout<<" the number of cores on this machine = "<<cores<<endl;
   
   /// <summary>
   ///  generate random data for the array from 0 to MAX 
   /// </summary>
   /// <param name="argc"></param>
   /// <param name="argv"></param>
   /// <returns></returns>
   for(int i = 0; i < MAX; i++) {
       data[i] = rand() % 20; 
   }

   /// <summary>
   /// Assigning the number of threads 
   /// In this case we are using 8 threads
   /// </summary>
   /// <param name="argc"></param>
   /// <param name="argv"></param>
   /// <returns></returns>
   pthread_t threads[NUM_THREADS];
   long tid = 0;

   /// <summary>
   /// create threads and assign them to work on different parts of the array
   /// this loop will iterate from 0 to 8 for calculating the sun
   /// </summary>
   /// <param name="argc"></param>
   /// <param name="argv"></param>
   /// <returns></returns>
   for(tid = 0; tid < NUM_THREADS; tid++) {
       pthread_create(&threads[tid], NULL, sum, (void *)tid);
   }

   /// <summary>
   /// wait for all the threads to finish execution
   /// </summary>
   /// <param name="argc"></param>
   /// <param name="argv"></param>
   /// <returns></returns>
   for(tid = 0; tid < NUM_THREADS; tid++) {
       pthread_join(threads[tid], NULL);
   }

   /// <summary>
   ///  this method will print the global sum
   /// </summary>
   /// <param name="argc"></param>
   /// <param name="argv"></param>
   /// <returns></returns>
   cout<<"the final sum ="<<global_sum<<endl; 
}
