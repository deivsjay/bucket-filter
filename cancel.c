#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include <pthread.h>
/* compile with -D_POSIX_PTHREAD_SEMANTICS on nunki */

#define NUM_THRS 10
pthread_t tid[NUM_THRS];  // 10 random child threads
sigset_t set; // 1 signal set

// when pthread_cleanup_push is called, it prints what threads is being cleaned
void cleanup(void *arg)
{
  int index=(int)(long)(arg);
  printf("Clean up thread %1d\n", index);
}

// sleeping infinitely until pthread_cleanup_push happens
void *child(void *arg)
{
  pthread_cleanup_push(cleanup, arg);
  for (;;) {
    usleep(lrand48()%1000000);
  }
  pthread_cleanup_pop(0);
  return 0;
}

// pthread_join for all child threads
void waitall()
{
  int i=0;
  for (i=0; i < NUM_THRS; i++)
    pthread_join(tid[i], 0);
}


void *monitor(void *arg) {
  int i=0, sig=0; 
  printf("Press <Cntrl+C>: ");
  fflush(stdout);
  sigwait(&set, &sig);
  printf("\nGot signal %1d\n", sig);

  // for as many child threads as they are, will infinitely check that thread to see if it's 0
  // as soon as child thread isn't 0, cancel thread
  for (i=0; i < NUM_THRS; i++) {
    while (tid[i] == 0) {
      usleep(100000);
    }
    pthread_cancel(tid[i]);
  }
  return 0;
}


int main(int argc, char *argv[])
{
  pthread_t thr;  // create signal thread                            
  int i=0;        // initialize for loop to come
  srand48(time(0)); // random time

  // bunch of signal stuff
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigprocmask(SIG_BLOCK, &set, 0);

  // initialize all other threads that aren't signal thread -> keeps sleeping until cleanup
  for (i=0; i < NUM_THRS; i++) {
    pthread_create(&tid[i], 0, child, (void*)(long)i);
  }

  // create signal thread
  pthread_create(&thr, 0, monitor, 0);

  // calling pthread_join on all child threads
  waitall();

  // calling pthread_join on signal thread
  pthread_join(thr, 0);
  return 0;
}