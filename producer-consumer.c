// Circular Queue implementation in C, taken from
// https://www.programiz.com/dsa/circular-queue

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 8

pthread_mutex_t mutex;
sem_t semEmpty;
sem_t semFull;

char items[QUEUE_SIZE];
int front = -1, rear = -1;
FILE *fp;

// Check if the queue is full
int isFull() {
  if ((front == rear + 1) || (front == 0 && rear == QUEUE_SIZE - 1))
    return 1;
  return 0;
}

// Check if the queue is empty
int isEmpty() {
  if (front == -1)
    return 1;
  return 0;
}

// Adding an element
void enQueue(char element) {
  if (!isFull()) {
    if (front == -1)
      front = 0;
    rear = (rear + 1) % QUEUE_SIZE;
    items[rear] = element;
    /*printf("\n Inserted -> %d", element);*/
  }
}

// Removing an element
char deQueue() {
  char element;
  if (isEmpty()) {
    /*printf("\n Queue is empty !! \n");*/
    return '\0';
  } else {
    element = items[front];
    if (front == rear) {
      front = -1;
      rear = -1;
    }
    // Q has only one element, so we reset the
    // queue after dequeing it. ?
    else {
      front = (front + 1) % QUEUE_SIZE;
    }
    /*printf("\n Deleted element -> %d \n", element);*/
    return (element);
  }
}

// Display the queue
void display() {
  int i;
  if (isEmpty())
    printf(" \n Empty Queue\n");
  else {
    printf("\n Front -> %d ", front);
    printf("\n Items -> ");
    for (i = front; i != rear; i = (i + 1) % QUEUE_SIZE) {
      printf("%c ", items[i]);
    }
    printf("%c ", items[i]);
    printf("\n Rear -> %d \n", rear);
  }
}

void *producer(void *args) {
  char item;

  while (1) {
    item = fgetc(fp);
    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutex);

    enQueue(item);

    pthread_mutex_unlock(&mutex);
    sem_post(&semFull);
    if (item == EOF)
      return NULL; // we are done
  }
}

void *consumer(void *args) {
  char item;

  while (1) {
    sem_wait(&semFull);
    pthread_mutex_lock(&mutex);

    item = deQueue();

    pthread_mutex_unlock(&mutex);
    sem_post(&semEmpty);

    if (item == EOF)
      return NULL;

    if (item != '\0') {
      printf("%c", item); // consume item
    }
  }
}

int main() {
  pthread_t threadId;
  int err;

  fp = fopen("strings.txt", "r");

  pthread_mutex_init(&mutex, NULL);
  sem_init(&semEmpty, 0, QUEUE_SIZE);
  sem_init(&semFull, 0, 0);

  err = pthread_create(&threadId, NULL, &producer, NULL);
  if (err) {
    printf("Something went wrong creating producer thread");
    return 1;
  }

  err = pthread_create(&threadId, NULL, &consumer, NULL);
  if (err) {
    printf("Something went wrong creating consumer thread");
    return 1;
  }

  err = pthread_join(threadId, NULL);
  if (err) {
    printf("Something went wrong joining the threads");
    return 1;
  }
  pthread_mutex_destroy(&mutex);
  sem_destroy(&semEmpty);
  sem_destroy(&semFull);

  return 0;
}
