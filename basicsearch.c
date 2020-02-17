// basicsearch.c: Search for a pattern in a text string.  
//                Uses a single thread with a brute force algorithm.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int searchPosition(int i);

#define TEXT_LEN 1000000

// The pattern and text strings are global variables
char *pattern;
char text[TEXT_LEN];

int main(int argc, char *argv[]) {

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

   int result = -1; // a positive value indicates the location of the match
   int pos;
   for (pos=0; pos<=textlen-patlen; pos++) {
       if (searchPosition(pos) == 1) {
          result = pos; 
          break;
       }
   }

   // Step 4: Determine the result and print it

   if (result == -1)
       printf("Pattern not found\n");
   else
       printf("Pattern begins at character %d\n", result);
}


int searchPosition(int i) {
   int j;
   for (j=0;j<strlen(pattern); j++) 
       if (text[i+j] != pattern[j])
          return 0;
   return 1;
}
				

