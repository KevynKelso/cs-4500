#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 1024

int threadCount = 0;
int total = 0;
int n1, n2;
char *s1, *s2;
FILE *fp;

struct ThreadArgs {
  int index;
};

int readf(FILE *fp) {
  if ((fp = fopen("strings.txt", "r")) == NULL) {
    printf("ERROR: can't open string.txt!\n");
    return 0;
  }
  s1 = (char *)malloc(sizeof(char) * MAX);
  if (s1 == NULL) {
    printf("ERROR: Out of memory!\n");
    return -1;
  }
  s2 = (char *)malloc(sizeof(char) * MAX);
  if (s2 == NULL) {
    printf("ERROR: Out of memory\n");
    return -1;
  }
  /*read s1 s2 from the file*/
  s1 = fgets(s1, MAX, fp);
  s2 = fgets(s2, MAX, fp);
  n1 = strlen(s1) - 1; /*length of s1*/
  n2 = strlen(s2) - 1; /*length of s2*/

  if (s1 == NULL || s2 == NULL || n1 < n2) /*when error exit*/
    return -1;

  return 0;
}

void *threadFunc(void *arg) {
  threadCount++;
  struct ThreadArgs *passedArgs = ((struct ThreadArgs *)arg);
  int i = passedArgs->index;
  int j, k;
  int count = 0;
  for (j = i, k = 0; k < n2;
       j++, k++) { /*search for the next string of size of n2*/
    if (*(s1 + j) != *(s2 + k)) {
      break;
    } else
      count++;
    if (count == n2)
      total++; /*find a substring in this step*/
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t threadId;
  int err, i, j, k;

  readf(fp);

  for (i = 0; i <= (n1 - n2); i++) {
    struct ThreadArgs *argsToPass =
        (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    argsToPass->index = i;

    err = pthread_create(&threadId, NULL, &threadFunc, (void *)argsToPass);
    if (err) {
      printf("Something went wrong creating a thread");
      return 1;
    }
  }

  err = pthread_join(threadId, NULL);
  if (err) {
    printf("Something went wrong joining the threads");
    return 1;
  }

  printf("The number of substrings is: %d\n", total);
  printf("Thread count: %d\n", threadCount);

  return 0;
}
