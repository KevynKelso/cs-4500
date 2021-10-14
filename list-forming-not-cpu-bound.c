/*
  list-forming.c:
  Each thread generates a data node, attaches it to a global list. This is
  reapeated for K times. There are num_threads threads. The value of
  "num_threads" is input by the student.
*/

#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/time.h>
#include <unistd.h>

int K = 200;

struct Node {
  int data;
  struct Node *next;
};

struct list {
  struct Node *header;
  struct Node *tail;
};

pthread_mutex_t mutex_lock;

struct list *List;

/*void bind_thread_to_cpu(int cpuid) {*/
/*cpu_set_t mask;*/
/*CPU_ZERO(&mask);*/

/*CPU_SET(cpuid, &mask);*/
/*if (sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {*/
/*fprintf(stderr, "sched_setaffinity");*/
/*exit(EXIT_FAILURE);*/
/*}*/
/*}*/

struct Node *generate_data_node() {
  struct Node *ptr;
  ptr = (struct Node *)malloc(sizeof(struct Node));

  if (ptr) {
    ptr->next = NULL;
  } else {
    printf("Node allocation failed!\n");
  }
  return ptr;
}

void *producer_thread(void *arg) {
  struct list *localList;

  localList = (struct list *)malloc(sizeof(struct list));
  if (NULL == localList) {
    printf("End here\n");
    exit(0);
  }
  localList->header = localList->tail = NULL;

  struct Node *ptr, tmp;
  int counter = 0;

  /* generate and attach K nodes to the local list */
  while (counter < K) {
    ptr = generate_data_node();

    if (ptr) {
      /* access the critical region and add a node to the local list */
      ptr->data = 1; // generate data
      /* attache the generated node to the local list */
      if (localList->header == NULL) {
        localList->header = localList->tail = ptr;
      } else {
        localList->tail->next = ptr;
        localList->tail = ptr;
      }
    }
    ++counter;
  }

  /*appends local list to global list*/
  pthread_mutex_lock(&mutex_lock);

  if (List->header == NULL) {
    List->header = localList->header;
    List->tail = localList->tail;
  } else {
    List->tail->next = localList->header;
    List->tail = localList->tail;
  }
  pthread_mutex_unlock(&mutex_lock);

  return NULL;
}

int main(int argc, char *argv[]) {
  int i, num_threads, err;

  int NUM_PROCS; // number of CPU
  int *cpu_array = NULL;

  struct Node *tmp, *next;
  struct timeval starttime, endtime;

  if (argc < 3) {
    printf("USAGE: ./a.out <num_threads> <K>");
    exit(1);
  }

  num_threads = atoi(argv[1]); // read num_threads from user
  K = atoi(argv[2]);

  pthread_t threadId;

  /*NUM_PROCS = sysconf(_SC_NPROCESSORS_CONF); // get number of CPU*/
  /*if (NUM_PROCS > 0) {*/
  /*cpu_array = (int *)malloc(NUM_PROCS * sizeof(int));*/
  /*if (cpu_array == NULL) {*/
  /*printf("Allocation failed!\n");*/
  /*exit(0);*/
  /*} else {*/
  /*for (i = 0; i < NUM_PROCS; i++)*/
  /*cpu_array[i] = i;*/
  /*}*/
  /*}*/

  pthread_mutex_init(&mutex_lock, NULL);

  List = (struct list *)malloc(sizeof(struct list));
  if (NULL == List) {
    printf("End here\n");
    exit(0);
  }
  List->header = List->tail = NULL;

  gettimeofday(&starttime, NULL); // get program start time

  for (i = 0; i < num_threads; i++) {
    err = pthread_create(&threadId, NULL, (void *)producer_thread, NULL);
    if (err) {
      printf("Something went wrong creating the thread");
      return 1;
    }
  }

  err = pthread_join(threadId, NULL);
  if (err) {
    printf("Something went wrong joining the threads");
    return 1;
  }

  gettimeofday(&endtime, NULL); // get the finish time

  if (List->header != NULL) {
    next = tmp = List->header;
    while (tmp != NULL) {
      next = tmp->next;
      free(tmp);
      tmp = next;
    }
  }
  if (cpu_array != NULL)
    free(cpu_array);
  /* calculate program runtime */
  printf("Total run time is %ld microseconds.\n",
         (endtime.tv_sec - starttime.tv_sec) * 1000000 +
             (endtime.tv_usec - starttime.tv_usec));
  return 0;
}
