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
int ketway_morrowmass = 0;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_env_var_name) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        char* stonesoup_tainted_file_name = 0;
        FILE * stonesoup_tainted_file = 0;
        size_t stonesoup_result = 0;
        long stonesoup_lsize = 0;
        stonesoup_tainted_file_name = getenv(stonesoup_env_var_name);
        stonesoup_tainted_file = fopen(stonesoup_tainted_file_name,"rb");
        if (stonesoup_tainted_file != 0) {
            fseek(stonesoup_tainted_file,0L,2);
            stonesoup_lsize = ftell(stonesoup_tainted_file);
            rewind(stonesoup_tainted_file);
            *stonesoup_tainted_buff = ((char *)(malloc(sizeof(char ) * (stonesoup_lsize + 1))));
            if (*stonesoup_tainted_buff != 0) {
                /* STONESOUP: SOURCE-TAINT (File Contents) */
                stonesoup_result = fread(*stonesoup_tainted_buff,1,stonesoup_lsize,stonesoup_tainted_file);
                (*stonesoup_tainted_buff)[stonesoup_lsize] = '\0';
            }
        }
        if (stonesoup_tainted_file != 0) {
            fclose(stonesoup_tainted_file);
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
void reiterating_otogyps(int iodyrite_bores,void *acquiescing_coforeknown);
struct stonesoup_data {
    int qsize;
    char *file1;
    char *file2;
    char *data;
    int data_size;
};
pthread_t stonesoup_t0, stonesoup_t1;
sem_t stonesoup_sem;
int stonesoup_comp (const void * a, const void * b)
{
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
int stonesoup_pmoc (const void * a, const void * b)
{
    return -1 * stonesoup_comp(a, b);
}
void stonesoup_readFile(char *filename) {
    FILE *fifo;
    char ch;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpNVUjdR_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
}
void *to1337(void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*)data;
    int qsize;
    int random;
    char temp;
    char *temp_str;
    int i = 0;
    int *stonesoup_arr;
    int semValue = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpNVUjdR_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "to1337");
    stonesoup_printf("Entering to1337\n");
    /* slow things down to make correct thing happen in good cases */
    stonesoup_arr = malloc(sizeof(int)*stonesoupData->qsize);
    if (stonesoup_arr != NULL) {
        for (i = 0; i < stonesoupData->qsize; i++) {
            stonesoup_arr[i] = stonesoupData->qsize - i;
        }
        qsort(stonesoup_arr, stonesoupData->qsize, sizeof(int), &stonesoup_comp);
        free (stonesoup_arr);
        stonesoup_arr = NULL;
    }
    temp_str = malloc(sizeof(char)*(stonesoupData->data_size + 1));
    qsize = stonesoupData->qsize;
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "to1337: Locking semaphore");
    tracepoint(stonesoup_trace, trace_point, "to1337: Locking semaphore");
    sem_wait(&stonesoup_sem);
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, trace_point, "to1337: Locked semaphore");
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "to1337: Locked semaphore");
    i = 0;
    while(stonesoupData->data[i] != '\0') {
        random = (int)(rand() / (double)RAND_MAX + 0.5); /* add .5 before truncation to round */
        switch(stonesoupData->data[i]) { /* 1337 s<r1p7 i5 f0r h4x0r5 */
            case 'c':
                if (random == 0)
                    temp = '<';
                else
                    temp = 'c';
                break;
            case 'e':
                if (random == 0)
                    temp = '3';
                else
                    temp = 'e';
                break;
            case 'i':
                if (random == 0)
                    temp = '1';
                else
                    temp = 'i';
                break;
            case 'l':
                if (random == 0)
                    temp = '1';
                else
                    temp = 'l';
                break;
            case 'o':
                if (random == 0)
                    temp = '0';
                else
                    temp = 'o';
                break;
            case 's':
                if (random == 0)
                    temp = '5';
                else
                    temp = 's';
                break;
            case 't':
                if (random == 0)
                    temp = '7';
                else
                    temp = 't';
                break;
            default:
                temp = stonesoupData->data[i];
                break;
        }
        temp_str[i] = temp;
        i++;
    }
    temp_str[i] = '\0';
    free(stonesoupData->data);
    stonesoupData->data = NULL; /* setting free()'d ptrs to null is good practice yo */
    tracepoint(stonesoup_trace, variable_address, "stonesoupData->data", stonesoupData->data, "TRIGGER-STATE: SET");
    stonesoup_printf("Set ptr to null\n");
    tracepoint(stonesoup_trace, trace_point, "to1337: Reading file");
    /* execute second */
    stonesoup_readFile(stonesoupData->file2);
    tracepoint(stonesoup_trace, trace_point, "to1337: Read file");
    stonesoup_printf("Set ptr to NON null\n");
    stonesoupData->data = temp_str;
    tracepoint(stonesoup_trace, variable_address, "stonesoupData->data", stonesoupData->data, "TRIGGER-STATE: UNSET");
    tracepoint(stonesoup_trace, trace_point, "to1337: Unlocking semaphore");
    sem_post(&stonesoup_sem);
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "to1337: Unlocked semaphore");
    tracepoint(stonesoup_trace, trace_point, "to1337: Unlocked semaphore");
    return NULL;
}
void *reverseStr(void * data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*)data;
    int i = 0;
    char *temp_str;
    int semValue = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpNVUjdR_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "reverseStr");
    stonesoup_printf("Entering reverseStr\n");
    /* execute first */
    stonesoup_readFile(stonesoupData->file1);
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "reverseStr: Locking semaphore");
    tracepoint(stonesoup_trace, trace_point, "reverseStr: Locking semaphore");
    sem_wait(&stonesoup_sem); /* if weakness has been triggered, */
                                                                    /* too many resource copies available */
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, trace_point, "reverseStr: Locked semaphore");
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "reverseStr: Locked semaphore");
    temp_str = malloc(sizeof(char)* (stonesoupData->data_size + 1));
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, variable_address, "stonesoupData->data", stonesoupData->data, "TRIGGER-STATE: ACCESS");
    for (i = 0; i < stonesoupData->data_size; i++) {
        /* STONESOUP: TRIGGER-POINT (multipleunlocks) */
        stonesoup_printf("Dereferencing ptr\n");
        temp_str[stonesoupData->data_size - 1 - i] = stonesoupData->data[i]; /* null ptr dereference */
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    temp_str[stonesoupData->data_size] = '\0';
    free(stonesoupData->data);
    stonesoupData->data = NULL;
    stonesoupData->data = temp_str;
    tracepoint(stonesoup_trace, trace_point, "reverseStr: Unlocking semaphore");
    sem_post(&stonesoup_sem);
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "reverseStr: Unlocked semaphore");
    tracepoint(stonesoup_trace, trace_point, "reverseStr: Unlocked semaphore");
    return NULL;
}
void toLower (struct stonesoup_data * stonesoupData) {
    int i = 0;
    int semValue = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpNVUjdR_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "toLower");
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "toLower: Locking semaphore");
    tracepoint(stonesoup_trace, trace_point, "toLower: Locking semaphore");
    sem_wait(&stonesoup_sem);
    stonesoup_printf("Entering toLower\n");
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, trace_point, "toLower: Locked semaphore");
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "toLower: Locked semaphore");
    for (i = 0; i < strlen(stonesoupData->data) - 1; i++) { /* all caps to lower */
        if (stonesoupData->data[i] >= 'A' &&
            stonesoupData->data[i] <= 'Z') {
            stonesoupData->data[i] += 32;
        }
    }
    tracepoint(stonesoup_trace, trace_point, "toLower: Unlocking semaphore (01)");
    sem_post(&stonesoup_sem);
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "toLower: Unlocked semaphore (01)");
    tracepoint(stonesoup_trace, trace_point, "toLower: Unlocked semaphore (01)");
    tracepoint(stonesoup_trace, trace_point, "toLower: CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "toLower: Unlocking semaphore (02)");
    /* STONESOUP: CROSSOVER-POINT (multipleunlocks) */
    sem_post(&stonesoup_sem); /* oops, extra unlock */
    sem_getvalue(&stonesoup_sem, &semValue);
    tracepoint(stonesoup_trace, variable_signed_integral, "semaphore", semValue, &semValue, "toLower: Unlocked semaphore (02)");
    tracepoint(stonesoup_trace, trace_point, "toLower: Unlocked semaphore (02)");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "CROSSOVER-STATE");
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
  int overinvolves_crystic = 7;
  void *misbusy_logisticians = 0;
  int **wampums_bathymetrically = 0;
  int *fos_swinge = 0;
  int pycnosis_nonspecious;
  void *skogbolite_laocoon[10] = {0};
  void *panse_nocturn = 0;
  char *epichoric_resor;;
  if (__sync_bool_compare_and_swap(&ketway_morrowmass,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpNVUjdR_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&epichoric_resor,"MOID_FUGITIVE");
      if (epichoric_resor != 0) {;
        panse_nocturn = ((void *)epichoric_resor);
        pycnosis_nonspecious = 5;
        fos_swinge = &pycnosis_nonspecious;
        wampums_bathymetrically = &fos_swinge;
        skogbolite_laocoon[ *( *wampums_bathymetrically)] = panse_nocturn;
        misbusy_logisticians = skogbolite_laocoon[ *( *wampums_bathymetrically)];
        reiterating_otogyps(overinvolves_crystic,misbusy_logisticians);
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

void reiterating_otogyps(int iodyrite_bores,void *acquiescing_coforeknown)
{
    int hasCap = 0;
    int stonesoup_i = 0;
    struct stonesoup_data *stonesoupData;
  char *upsy_displeasures = 0;
  ++stonesoup_global_variable;
  iodyrite_bores--;
  if (iodyrite_bores > 0) {
    reiterating_otogyps(iodyrite_bores,acquiescing_coforeknown);
    return ;
  }
  upsy_displeasures = ((char *)((char *)acquiescing_coforeknown));
    tracepoint(stonesoup_trace, weakness_start, "CWE-765", "A", "Multiple Unlocks of a Critical Resource");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(upsy_displeasures) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(upsy_displeasures) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(upsy_displeasures) + 1));
        if (stonesoupData->data) {
            if ((sscanf(upsy_displeasures, "%d %s %s %s",
               &(stonesoupData->qsize),
                 stonesoupData->file1,
                 stonesoupData->file2,
                 stonesoupData->data) == 4) &&
                (strlen(stonesoupData->data) != 0))
            {
                tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "INITIAL-STATE");
               sem_init(&stonesoup_sem, 0, 1);
                stonesoupData->data_size = strlen(stonesoupData->data);
                while (stonesoupData->data[stonesoup_i] != '\0') { /* if input has capital */
                    if (stonesoupData->data[stonesoup_i] >= 'A' && /*   call function that contains */
                        stonesoupData->data[stonesoup_i] <= 'Z') { /*   weakness */
                        hasCap = 1;
                    }
                    stonesoup_i++;
                }
               tracepoint(stonesoup_trace, variable_signed_integral, "hasCap", hasCap, &hasCap, "toLower() gate");
                if (hasCap == 1) {
                    toLower(stonesoupData);
                }
                tracepoint(stonesoup_trace, trace_point, "Creating threads");
                if (pthread_create(&stonesoup_t0, NULL, reverseStr, (void *)stonesoupData) != 0) {
                    stonesoup_printf("Error creating thread 0.");
                }
                if (pthread_create(&stonesoup_t1, NULL, to1337, (void *)stonesoupData) != 0) {
                    stonesoup_printf("Error creating thread 1.");
                }
                tracepoint(stonesoup_trace, trace_point, "Joining threads");
                tracepoint(stonesoup_trace, trace_point, "Joining thread-01");
                pthread_join(stonesoup_t0, NULL);
                tracepoint(stonesoup_trace, trace_point, "Joined thread-01");
                tracepoint(stonesoup_trace, trace_point, "Joining thread-02");
                pthread_join(stonesoup_t1, NULL);
                tracepoint(stonesoup_trace, trace_point, "Joined thread-02");
                tracepoint(stonesoup_trace, trace_point, "Joined threads");
                tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "FINAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "FINAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "FINAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "FINAL-STATE");
                stonesoup_printf("After joins.\n");
                stonesoup_printf("String: %s\n", stonesoupData->data);
            }
            free(stonesoupData->data);
        }
        free(stonesoupData);
    } else {
        stonesoup_printf("Error parsing input.\n");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (((char *)acquiescing_coforeknown) != 0) 
    free(((char *)((char *)acquiescing_coforeknown)));
stonesoup_close_printf_context();
}
