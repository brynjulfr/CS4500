/*
* Project 3b
* John Foster and Joel Judd
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <pthread.h>
#include <sys/time.h>

//NPAD to align to 64 byte cacheline
#define NPAD 7
//working set size
#define SET 400
//# of threads
#define THREADS 4

long shared[32] = {0};

pthread_mutex_t lock;

struct read_t{
   struct read_t *next;
   long pad[NPAD];
};

//set stack size
int setStackSize()
{
   const rlim_t stackSize = 64*1024*1024; //64MB
   struct rlimit r1;
   int res = getrlimit(RLIMIT_STACK, &r1);
   if (res == 0)
   {
      r1.rlim_cur = stackSize;
      res = setrlimit(RLIMIT_STACK, &r1);
      if (res != 0)
      {
         fprintf(stderr,"setrlimit returned %d\n", res);
         return 1;
      }
      return 0;
   } else
      return 1;
}

//worker thread
void *worker(void *t)
{
   int i=0,j=0,l=0,r=0;
   struct read_t *head, *curr;

   //build head of linked list
   head = (struct read_t*)malloc(sizeof(struct read_t));
   head->next == NULL;
   srand(time(NULL));
   for(i=0;i<NPAD;i++)
      head->pad[i] = (rand() % 2147483645);

   //build linked list
   curr = head;
   for(i=0;i<SET;i++)
   {
      curr->next = (struct read_t*)malloc(sizeof(struct read_t));
      curr = curr->next;
      for(j=0;j<NPAD;j++)
         curr->pad[i] = (rand() % 2147483645);
      curr->next = NULL;
   }

   //read list random # of times
   for(i=0;i<((rand() % 5000)+2500);i++)
   {
      curr = head;
      l = (rand() % SET);
      r = (rand() % NPAD);
      for(j=0;j<l;j++)
         curr = curr->next;
      for(j=0;j<NPAD;j++)
         curr->pad[j]; //access each element
      if (curr->pad[r] == 0) //we found 0, ignore
         i--;
   }

   //Shared array manipulation
   pthread_mutex_lock(&lock);
   for(i=0,i<28;i++;)
      shared[i]++;
   pthread_mutex_unlock(&lock);

   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   int s1 = setStackSize();
   if (s1 == 0) //only work if we succeeded in setting stack size
   {
      printf("Size of struct read_t: %d\n", sizeof(struct read_t));
      pthread_t threads[THREADS];
      long pids[THREADS];
      int i;
      struct timeval starttime, endtime;
      gettimeofday(&starttime,NULL); //get program start time

      //start threads
      pthread_mutex_init(&lock, NULL);
      for(i=0;i<THREADS;i++)
      {
         pids[i] = (i+1);
         pthread_create(&threads[i],NULL,worker,(void *)pids[i]);
         pthread_join(threads[i],NULL);
      }

      gettimeofday(&endtime,NULL); //get the finish time
      long time = (endtime.tv_sec-starttime.tv_sec) * 1000000+(endtime.tv_usec-starttime.tv_usec);
      printf("Total run time is %ld microseconds ~= %ld seconds.\n", time, time/10000);
      printf("Threads: %d, Working Set Size: %d\n", THREADS, SET);
   } else
      printf("Couldn't set new stack size, exiting");
   

   //cleanup
   pthread_mutex_destroy(&lock);
   pthread_exit(NULL);
}

