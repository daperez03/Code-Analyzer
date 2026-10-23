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
#include <stonesoup/stonesoup_trace.h> 
#include <pthread.h> 
#include <semaphore.h> 
int mulishnesses_sulfured = 0;
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
void diplomatology_breviate(void **womenfolks_neohipparion);
sem_t stonesoup_sem;
pthread_t stonesoup_t0, stonesoup_t1;
char *stonesoup_global_str;
int stonesoup_isspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}
void *replaceSpace () {
    int stonesoup_i = 0;
    stonesoup_printf("Replacing spaces\n");
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpH4hxdx_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "replaceSpace");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    /* STONESOUP: TRIGGER-POINT (multiple locks) */
    sem_wait(&stonesoup_sem); /* multiple locks - deadlock */
    sem_wait(&stonesoup_sem);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    while(stonesoup_global_str[stonesoup_i] != '\0') {
        if (stonesoup_isspace(stonesoup_global_str[stonesoup_i]) != 0) {
            stonesoup_global_str[stonesoup_i] = '_';
        }
        stonesoup_i++;
    }
    sem_post(&stonesoup_sem);
    return NULL;
}
void *toCap () {
    int stonesoup_i = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpH4hxdx_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "toCap");
    tracepoint(stonesoup_trace, trace_point, "Before sem_wait in toCap");
    stonesoup_printf("Capitalizing input\n");
    sem_wait(&stonesoup_sem);
    tracepoint(stonesoup_trace, trace_point, "After sem_wait in toCap");
    while(stonesoup_global_str[stonesoup_i] != '\0') {
        if (stonesoup_global_str[stonesoup_i] > 'a' && stonesoup_global_str[stonesoup_i] < 'z') {
            stonesoup_global_str[stonesoup_i] -= 'a' - 'A';
        }
        stonesoup_i++;
    }
    sem_post(&stonesoup_sem);
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
  void **udt_actinia = 0;
  void *sherrymoor_cardale = 0;
  char *pinkany_steelie;;
  if (__sync_bool_compare_and_swap(&mulishnesses_sulfured,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpH4hxdx_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      pinkany_steelie = getenv("BESTUDDING_BANQUETE");
      if (pinkany_steelie != 0) {;
        sherrymoor_cardale = ((void *)pinkany_steelie);
        udt_actinia = &sherrymoor_cardale;
        diplomatology_breviate(udt_actinia);
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

void diplomatology_breviate(void **womenfolks_neohipparion)
{
    int stonesoup_hasSpaces = 0;
    int stonesoup_i = 0;
  char *niemoeller_dasyproctine = 0;
  ++stonesoup_global_variable;;
  niemoeller_dasyproctine = ((char *)((char *)( *womenfolks_neohipparion)));
    tracepoint(stonesoup_trace, weakness_start, "CWE764", "A", "Multiple Locks of a Critical Resource");
    sem_init(&stonesoup_sem, 0, 1);
    while(niemoeller_dasyproctine[stonesoup_i] != '\0') { /* if the input contains spaces */
        if (stonesoup_isspace(niemoeller_dasyproctine[stonesoup_i++]) != 0) { /* we will call the deadlocking function */
            stonesoup_hasSpaces = 1;
        }
    }
    tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", niemoeller_dasyproctine, "INITIAL-STATE");
    stonesoup_global_str = malloc(sizeof(char) * strlen(niemoeller_dasyproctine) + 1);
    strcpy(stonesoup_global_str, niemoeller_dasyproctine);
    if (stonesoup_hasSpaces == 1) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (multiple locks) */
        if (pthread_create(&stonesoup_t0, NULL, replaceSpace, NULL) != 0) {
            stonesoup_printf("Thread 0 failed to spawn.");
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    if (pthread_create(&stonesoup_t1, NULL, toCap, NULL) != 0) {
        stonesoup_printf("Thread 1 failed to spawn.");
    }
    if (stonesoup_hasSpaces == 1) {
        pthread_join(stonesoup_t0, NULL);
    }
    pthread_join(stonesoup_t1, NULL);
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}
