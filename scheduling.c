#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#define CHUNK 10

int main(int argc, char *argv[]){
   int p, i, n;
   p = atoi(argv[1]);
   n = atoi(argv[2]);
   
   #pragma omp parallel for private(i) schedule(dynamic, CHUNK) num_threads(p)
   for(i = 0; i < n; i++)
   {
      // aspetta un numero pari ad i secondi
      sleep(0.1);
      printf("Il thread %d ha completato iterazione %d.\n", omp_get_thread_num(), i);
   }
   printf("Tutti hanno finito!\n");
   
   return 0;
}

