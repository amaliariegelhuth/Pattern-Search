#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int searchPosition(int i);
void *myThread();

#define TEXT_LEN 1000000
#define NUM_THREADS 2

// The pattern and text strings are global variables
char *pattern;
char text[TEXT_LEN];
int indx;
int found;
int result;

pthread_mutex_t lock;
pthread_cond_t cond;

int main(int argc, char *argv[]) {
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);

   // Step 1: Extract the pattern string from the command line.   
   
   pattern = argv[1];
   int patlen = strlen(pattern);

   // Step 2: Create and populate the text string.
   // Note that it isn't a proper "string" in the C sense,
   // because it doesn't end in '\0'.

   int textlen = TEXT_LEN; // will be overridden by the actual length
   int count = 0;
   while (count < TEXT_LEN) {
       int status = scanf("%c", &text[count]);
       count++;
       if (status == EOF) {
          textlen = count;
          break;
       }
   }

   // Step 3: Search for the pattern in the text string

 	result = -1; // a positive value indicates the location of the match (I have made it a global variable)
   	int pos;
	indx = 0;
	found = 0; // similar to result but it acts as a boolean to check if the pattern has been found
	int i;
	int *ptr;
	pthread_t p[NUM_THREADS]; // can create as many threads as is desirable
   for (pos=0; pos<=textlen-patlen; pos++) {
		for (i = 0; i < NUM_THREADS; i++){
			ptr = malloc(sizeof(int));
			*ptr = i; // to free up more memory
			pthread_create(&p[i], NULL, myThread, ptr); // create threads to search
		}
		for (i = 0; i < NUM_THREADS; i++){
			pthread_join(p[i], NULL); //join the threads
		}
		if (found == 1) { // stop creating threads if found
			break;
		}
   }


   // Step 4: Determine the result and print it

   if (result == -1)
       printf("Pattern not found\n");
   else
       printf("Pattern begins at character %d\n", result);
}

void *myThread(void *vargp) {
	found = searchPosition(indx);
	if (found == 1) { 
		result = indx;
	}
	pthread_mutex_lock(&lock); // this is the critical section we need to lock to prevent a data race
	indx = indx + 1; 
	pthread_mutex_unlock(&lock);
	free(vargp);
}

int searchPosition(int i) {
   int j;
   for (j=0;j<strlen(pattern); j++) 
       if (text[i+j] != pattern[j])
          return 0;
   return 1;
}
				

