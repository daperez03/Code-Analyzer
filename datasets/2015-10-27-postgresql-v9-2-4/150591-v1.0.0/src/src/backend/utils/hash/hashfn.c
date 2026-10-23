/*-------------------------------------------------------------------------
 *
 * hashfn.c
 *		Hash functions for use in dynahash.c hashtables
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/hash/hashfn.c
 *
 * NOTES
 *	  It is expected that every bit of a hash function's 32-bit result is
 *	  as random as every other; failure to ensure this is likely to lead
 *	  to poor performance of hash tables.  In most cases a hash
 *	  function should use hash_any() or its variant hash_uint32().
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/hash.h"
/*
 * string_hash: hash function for keys that are NUL-terminated strings.
 *
 * NOTE: this is the default hash function if none is specified.
 */
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <pthread.h> 
int paracusic_jesuitizing = 0;
int stonesoup_global_variable;
void* stonesoup_printf_context = NULL;
void stonesoup_setup_printf_context() {
    struct stat st = {0};
    char * ss_tc_root = NULL;
    char * dirpath = NULL;
    int size_dirpath = 0;
    char * filepath = NULL;
    int size_filepath = 0;
    int retval = 0;
    ss_tc_root = getenv("SS_TC_ROOT");
    if (ss_tc_root != NULL) {
        size_dirpath = strlen(ss_tc_root) + strlen("testData") + 2;
        dirpath = (char*) malloc (size_dirpath * sizeof(char));
        if (dirpath != NULL) {
            sprintf(dirpath, "%s/%s", ss_tc_root, "testData");
            retval = 0;
            if (stat(dirpath, &st) == -1) {
                retval = mkdir(dirpath, 0700);
            }
            if (retval == 0) {
                size_filepath = strlen(dirpath) + strlen("logfile.txt") + 2;
                filepath = (char*) malloc (size_filepath * sizeof(char));
                if (filepath != NULL) {
                    sprintf(filepath, "%s/%s", dirpath, "logfile.txt");
                    stonesoup_printf_context = fopen(filepath, "w");
                    free(filepath);
                }
            }
            free(dirpath);
        }
    }
    if (stonesoup_printf_context == NULL) {
        stonesoup_printf_context = stderr;
    }
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stonesoup_printf_context, format, argptr);
    va_end(argptr);
    fflush(stonesoup_printf_context);
}
void stonesoup_close_printf_context() {
    if (stonesoup_printf_context != NULL &&
        stonesoup_printf_context != stderr) {
        fclose(stonesoup_printf_context);
    }
}
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_envKey, int stonesoup_shmsz) {
    int stonesoup_shmid;
 key_t stonesoup_key;
 char *stonesoup_shm, *stonesoup_s;
 char* stonesoup_envSize = NULL;
 *stonesoup_tainted_buff = NULL;
    if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
        strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        if(stonesoup_envKey != NULL) {
            if(sscanf(stonesoup_envKey, "%d", &stonesoup_key) > 0) {
                if ((stonesoup_shmid = shmget(stonesoup_key, stonesoup_shmsz, 0666)) >= 0) {
                    if ((stonesoup_shm = shmat(stonesoup_shmid, NULL, 0)) != (char *) -1) {
                        *stonesoup_tainted_buff = (char*)calloc(stonesoup_shmsz, sizeof(char));
                        /* STONESOUP: SOURCE-TAINT (Shared Memory) */
                        for (stonesoup_s = stonesoup_shm; *stonesoup_s != (char)0; stonesoup_s++) {
                            (*stonesoup_tainted_buff)[stonesoup_s - stonesoup_shm] = *stonesoup_s;
                        }
                    }
                }
            }
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
void lamellary_birder(char *semitism_wil);
struct stonesoup_data {
    int qsize;
    int data_size;
    char *data;
};
pthread_mutex_t stonesoup_mutex;
int stonesoup_comp (const void * a, const void * b) {
    if (a > b) {
        return -1;
    }
    else if (a < b) {
        return 1;
    }
    else {
        return 0;
    }
}
int stonesoup_pmoc (const void * a, const void * b) {
    return -1 * stonesoup_comp(a, b);
}
void arrFunc (struct stonesoup_data *stonesoupData) {
    int *stonesoup_arr = malloc(sizeof(int) * stonesoupData->qsize);
    int stonesoup_i;
    int stonesoup_j;
    FILE *fp;
    static int stonesoup_ctr;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpucZ8Nr_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "arrFunc");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    stonesoup_ctr = 0; /* static var will reset to 0 */
    for(stonesoup_i = 0; /* and count twice when second thread is in func */
        stonesoup_i < stonesoupData->data_size; /* which will blow the free() struct away */
        stonesoup_i++, stonesoup_ctr++)
    {
        /* STONESOUP: CROSSOVER-POINT (nonreentrant function in multi-threaded context) */
        stonesoupData->data[stonesoup_ctr] = '\0';
        stonesoup_printf("I: %d, C: %d\n", stonesoup_i, stonesoup_ctr);
        if (stonesoupData->qsize > 5) {
            fp = fopen("asdfqwer1234", "w+");
            fprintf(fp, "%d", stonesoup_i);
            fclose(fp);
            for (stonesoup_j = 0; stonesoup_j < stonesoupData->qsize; stonesoup_j++) {
                stonesoup_arr[stonesoup_j] = stonesoupData->qsize - stonesoup_j;
            }
            qsort(stonesoup_arr, stonesoupData->qsize, sizeof(int), &stonesoup_comp);
        }
    }
    free(stonesoup_arr);
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->data_size", stonesoupData->data_size, &stonesoupData->data_size, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ctr", stonesoup_ctr, &stonesoup_ctr, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
}
void *replaceSymbols(void *data) {
    struct stonesoup_data *stonesoupData = data;
    int i;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpucZ8Nr_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "replaceSymbols");
    pthread_mutex_lock(&stonesoup_mutex);
    for(i = 0; i < stonesoupData->data_size; i++) {
        if (((stonesoupData->data[i] >= '!' && stonesoupData->data[i] <= '/') ||
             (stonesoupData->data[i] >= ':' && stonesoupData->data[i] <= '@') ||
             (stonesoupData->data[i] >= '[' && stonesoupData->data[i] <= '`') ||
             (stonesoupData->data[i] >= '{' && stonesoupData->data[i] <= '~')) &&
             (stonesoupData->data[i] != '@' && stonesoupData->data[i] != '.'))
        {
            stonesoupData->data[i] = '_';
        }
    }
    pthread_mutex_unlock(&stonesoup_mutex);
    arrFunc(stonesoupData);
    return NULL;
}
void *toCaps(void *data) {
    struct stonesoup_data *stonesoupData = data;
    int threadTiming = 500000;
    int stonesoup_j;
    int *stonesoup_arr;
    int i;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpucZ8Nr_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "toCaps");
    /* slow things down to make correct thing happen in good cases */
    stonesoup_arr = malloc(sizeof(int)*threadTiming);
    if (stonesoup_arr != NULL) {
        for (stonesoup_j = 0; stonesoup_j < threadTiming; stonesoup_j++) {
            stonesoup_arr[stonesoup_j] = threadTiming - stonesoup_j;
        }
        qsort(stonesoup_arr, threadTiming, sizeof(int), &stonesoup_comp);
        free (stonesoup_arr);
        stonesoup_arr = NULL;
    }
    pthread_mutex_lock(&stonesoup_mutex);
    for(i = 0; i < stonesoupData->data_size; i++) {
        if(stonesoupData->data[i] >= 'a' && stonesoupData->data[i] <= 'z') {
            stonesoupData->data[i] -= 'a' - 'A';
        }
    }
    pthread_mutex_unlock(&stonesoup_mutex);
    arrFunc(stonesoupData);
    return NULL;
}

uint32 string_hash(const void *key,Size keysize)
{
/*
	 * If the string exceeds keysize-1 bytes, we want to hash only that many,
	 * because when it is copied into the hash table it will be truncated at
	 * that length.
	 */
  Size s_len = strlen(((const char *)key));
  s_len = (s_len < keysize - 1?s_len : keysize - 1);
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )s_len)))) & 0xffffffff);
}
/*
 * tag_hash: hash function for fixed-size tag values
 */

uint32 tag_hash(const void *key,Size keysize)
{
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )keysize)))) & 0xffffffff);
}
/*
 * oid_hash: hash function for keys that are OIDs
 *
 * (tag_hash works for this case too, but is slower)
 */

uint32 oid_hash(const void *key,Size keysize)
{
  void (*wean_finings)(char *) = lamellary_birder;
  int eudoxia_presufficiently = 51;
  char *seamlike_ratement;;
  if (__sync_bool_compare_and_swap(&paracusic_jesuitizing,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpucZ8Nr_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&seamlike_ratement,"1885",eudoxia_presufficiently);
      if (seamlike_ratement != 0) {;
        wean_finings(seamlike_ratement);
      }
    }
  }
  ;
  ;
  return (uint32 )(((Datum )(hash_uint32(((uint32 )( *((const Oid *)key)))))) & 0xffffffff);
}
/*
 * bitmap_hash: hash function for keys that are (pointers to) Bitmapsets
 *
 * Note: don't forget to specify bitmap_match as the match function!
 */

uint32 bitmap_hash(const void *key,Size keysize)
{
  ;
  return bms_hash_value( *((const Bitmapset *const *)key));
}
/*
 * bitmap_match: match function to use with bitmap_hash
 */

int bitmap_match(const void *key1,const void *key2,Size keysize)
{
  ;
  return !bms_equal( *((const Bitmapset *const *)key1), *((const Bitmapset *const *)key2));
}

void lamellary_birder(char *semitism_wil)
{
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data *stonesoupData = malloc(sizeof(struct stonesoup_data));
  char *dalecarlian_reassigning = 0;
  ++stonesoup_global_variable;;
  dalecarlian_reassigning = ((char *)semitism_wil);
    tracepoint(stonesoup_trace, weakness_start, "CWE663", "A", "Use of a Non-reentrant Function in a Concurrent Context");
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(dalecarlian_reassigning)+ 1));
        if (stonesoupData->data &&
            (sscanf(dalecarlian_reassigning, "%d %s", &stonesoupData->qsize, stonesoupData->data) == 2) &&
            (strlen(stonesoupData->data) != 0)) {
            pthread_mutex_init(&stonesoup_mutex, NULL);
            stonesoupData->data_size = strlen(stonesoupData->data);
            tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &stonesoupData->qsize, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "INTIAL-STATE");
            tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->data_size", stonesoupData->data_size, &stonesoupData->data_size, "INTIAL-STATE");
            tracepoint(stonesoup_trace, trace_point, "Spawning threads");
            if (pthread_create(&stonesoup_t0, NULL, replaceSymbols, (void*)stonesoupData) != 0) {
                stonesoup_printf("Error initilizing thread 0.");
            }
            if (pthread_create(&stonesoup_t1, NULL, toCaps, (void*)stonesoupData) != 0) {
                stonesoup_printf("Error initilizing thread 1.");
            }
            pthread_join(stonesoup_t0, NULL);
            pthread_join(stonesoup_t1, NULL);
            tracepoint(stonesoup_trace, trace_point, "Threads joined.");
            pthread_mutex_destroy(&stonesoup_mutex);
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            /* STONESOUP: TRIGGER-POINT (nonreentrant function in multi-threaded context) */
            free(stonesoupData->data);
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        }
        else {
            stonesoup_printf("Error parsing input.\n");
        }
        free(stonesoupData);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (semitism_wil != 0) 
    free(((char *)semitism_wil));
stonesoup_close_printf_context();
}
