#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <malloc.h>
 
#define NUMLIDO 1
#define NUMESCRITO 2
#define BUFFERSIZE 4
#define COUNT_MAX 4
 
pthread_t lido[NUMLIDO];
pthread_t escrito[NUMESCRITO];
pthread_mutex_t buffer_mutex;
int buffer[BUFFERSIZE];
int currentidx;
sem_t buffer_full, buffer_empty;
static pthread_key_t thread_log_key;
int count;
 

 
void *escritor(void *arg) {
    int n;
    while(count < COUNT_MAX) {
        n = rand()%1000;
        sem_wait(&buffer_full);
        pthread_mutex_lock(&buffer_mutex);
        count = count +1; 
        buffer[currentidx++] = n;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&buffer_empty);
        printf("Thread 1 escrevendo numero %d\n", n);
        sleep((1 + rand()%5));
    }
    pthread_exit(NULL);
}
 
void *leitor(void *arg) {
    int n;
    while(count < COUNT_MAX) {
        sem_wait(&buffer_empty);
        pthread_mutex_lock(&buffer_mutex);
        count = count +1; 

        n = buffer[--currentidx];
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&buffer_full);
        printf("Thread 2 lendo numero %d\n", n);
        sleep(1 + (rand()%5));
    }
    pthread_exit(NULL);

}
 void write_to_thread_log (const char* message)
{
  FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);
  fprintf (thread_log, "%s\n", message);
}

void close_thread_log (void* thread_log)
{
  fclose ((FILE*) thread_log);
}
void* thread_function (void* args)
{
  char thread_log_filename[20];
  FILE* thread_log;

  /* Generate the filename for this thread.s log file. */
  sprintf (thread_log_filename, "thread%d.log", (int) pthread_self ());


  thread_log = fopen (thread_log_filename, "w");

  /* Store the file pointer in thread-specific data under thread_log_key. */
  pthread_setspecific (thread_log_key, thread_log);
  write_to_thread_log ("Thread starting.");
 
}
 
int main(int argc, char **argv) {
    int i;    
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&buffer_full, 0, BUFFERSIZE);
    sem_init(&buffer_empty, 0, 0);
    pthread_t threads[i];
 
    for(i=0; i<NUMLIDO; i++)
        pthread_create(&(lido[i]), NULL, escritor, NULL);
 
    for(i=0; i<NUMESCRITO; i++)
        pthread_create(&(escrito[i]), NULL, leitor, NULL);
 
    for(i=0; i<NUMLIDO; i++)
        pthread_join(lido[i], NULL);
 
    for(i=0; i<NUMESCRITO; i++)
        pthread_join(escrito[i], NULL);

        /* Create a key to associate thread log file pointers in
   * thread-specific data. Use close_thread_log to clean up the file
   * pointers. */
  pthread_key_create (&thread_log_key, close_thread_log);

  /* Create threads . */
  for (i = 0; i < COUNT_MAX; ++i)
    pthread_create (&(threads[i]), NULL, thread_function, NULL);


  for (i = 0; i < COUNT_MAX; ++i)
    pthread_join (threads[i], NULL);
  return 0;

    
}
