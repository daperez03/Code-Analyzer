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
#include <mongoose.h> 
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <pthread.h> 
#include <sys/stat.h> 
int shillelagh_schtick = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *seduction_unbuilt);
void* stonesoup_printf_context;
void stonesoup_setup_printf_context() {
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    // mg_send_header(stonesoup_printf_context, "Content-Type", "text/plain");
    va_start(argptr, format);
    mg_vprintf_data((struct mg_connection*) stonesoup_printf_context, format, argptr);
    va_end(argptr);
}
void stonesoup_close_printf_context() {
}
static int stonesoup_exit_flag = 0;
static int stonesoup_ev_handler(struct mg_connection *conn, enum mg_event ev) {
  char * ifmatch_header;
  char* stonesoup_tainted_buff;
  int buffer_size = 1000;
  int data_size = 0;
  if (ev == MG_REQUEST) {
    ifmatch_header = (char*) mg_get_header(conn, "if-match");
    if (strcmp(ifmatch_header, "weak_taint_source_value") == 0) {
        while (1) {
            stonesoup_tainted_buff = (char*) malloc(buffer_size * sizeof(char));
            /* STONESOUP: SOURCE-TAINT (Socket Variable) */
            data_size = mg_get_var(conn, "data", stonesoup_tainted_buff, buffer_size * sizeof(char));
            if (data_size < buffer_size) {
                stonesoup_exit_flag = 1;
                break;
            }
            buffer_size = buffer_size * 2;
            free(stonesoup_tainted_buff);
        }
        stonesoup_printf_context = conn;
        stonesoup_handle_taint(stonesoup_tainted_buff);
        /* STONESOUP: INJECTION-POINT */
    }
    return MG_TRUE;
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  } else {
    return MG_FALSE;
  }
}
void stonesoup_read_taint(void) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
    struct mg_server *stonesoup_server = mg_create_server(NULL, stonesoup_ev_handler);
    mg_set_option(stonesoup_server, "listening_port", "8887");
    while (1) {
      if (mg_poll_server(stonesoup_server, 1000) == 0 && stonesoup_exit_flag == 1) {
          break;
      }
    }
    mg_destroy_server(&stonesoup_server);
  }
}
void uncompassionate_josephus(int plantations_psilotaceous,... );
void drillstock_censor(void *helleborein_kenogenetic);
struct stonesoup_data {
    int qsize;
    int data_size;
    char *data;
    char *file1;
    char *file2;
};
struct stonesoup_data2 {
    int qsize;
    int data_size;
    int data_size2;
    char *data;
    char *data2;
};
pthread_mutex_t stonesoup_mutex;
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpPjuu5T_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Finished reading sync file.");
}
struct stonesoup_data2 *ssD2 = 0;
struct stonesoup_data2 *init_stonesoup_data2 (struct stonesoup_data *ssD) {
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpPjuu5T_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "init_stonesoup_data2");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    stonesoup_printf("Checking for initalization\n");
    /* STONESOUP: CROSSOVER-POINT (double-check locking) */
    if (ssD2 == NULL) {
        pthread_mutex_lock(&stonesoup_mutex);
        if (ssD2 == NULL) {
            stonesoup_printf("Initializing\n");
            ssD2 = calloc(1, sizeof(struct stonesoup_data2));
            stonesoup_readFile(ssD->file2);
            ssD2->data = ssD->data;
            ssD2->qsize = ssD->qsize;
            ssD2->data_size = ssD->data_size;
            ssD2->data2 = ssD->data;
            ssD2->data_size2 = ssD->data_size;
        } else {
            stonesoup_printf("No need to initialize\n");
        }
        pthread_mutex_unlock(&stonesoup_mutex);
    } else {
        stonesoup_printf("Data is already initialized\n");
    }
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    return ssD2;
}
void *doStuff(void *ssD) {
    struct stonesoup_data2 *ssD2;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpPjuu5T_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "doStuff");
    stonesoup_printf("Inside doStuff\n");
    ssD2 = init_stonesoup_data2((struct stonesoup_data*)ssD);
    return NULL;
}
void *doStuff2(void *stonesoupData) {
    struct stonesoup_data2 *ssD2;
    struct stonesoup_data *ssD = stonesoupData;
    int stonesoup_i;
    int *stonesoup_arr;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpPjuu5T_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "doStuff2");
    stonesoup_printf("Inside doStuff2\n");
    stonesoup_arr = malloc(sizeof(int) * ssD->qsize);
    for (stonesoup_i = 0; stonesoup_i < ssD->qsize; stonesoup_i++) {
        stonesoup_arr[stonesoup_i] = ssD->qsize - stonesoup_i;
    }
    qsort(stonesoup_arr, ssD->qsize, sizeof(int), &stonesoup_comp);
    free(stonesoup_arr);
    stonesoup_readFile(ssD->file1);
    ssD2 = init_stonesoup_data2((struct stonesoup_data*)ssD);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, variable_signed_integral, "ssD2->qsize", ssD2->qsize, &(ssD2->qsize), "ssD2 - TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "ssD2->data", ssD2->data, "ssD2 - TRIGGER-STATE");
    /* STONESOUP: TRIGGER-POINT (double-check locking) */
    if (ssD2->data2[0] != '\0') {
        stonesoup_printf("%s\n", ssD2->data2);
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
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
{;
  if (__sync_bool_compare_and_swap(&shillelagh_schtick,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpPjuu5T_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_read_taint();
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

void stonesoup_handle_taint(char *seduction_unbuilt)
{
  void *drawbores_dealers = 0;
  ++stonesoup_global_variable;;
  if (seduction_unbuilt != 0) {;
    drawbores_dealers = ((void *)seduction_unbuilt);
    uncompassionate_josephus(1,drawbores_dealers);
  }
}
#define LESCHE_HEMINA(x) drillstock_censor((void *) x)

void uncompassionate_josephus(int plantations_psilotaceous,... )
{
  void *unencrypted_spicier = 0;
  va_list hepatopancreas_kumis;
  ++stonesoup_global_variable;;
  if (plantations_psilotaceous > 0) {
    __builtin_va_start(hepatopancreas_kumis,plantations_psilotaceous);
    unencrypted_spicier = (va_arg(hepatopancreas_kumis,void *));
    __builtin_va_end(hepatopancreas_kumis);
  }
	LESCHE_HEMINA(unencrypted_spicier);
}

void drillstock_censor(void *helleborein_kenogenetic)
{
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data *stonesoupData;
  char *diapsidan_moonset = 0;
  ++stonesoup_global_variable;;
  diapsidan_moonset = ((char *)((char *)helleborein_kenogenetic));
    tracepoint(stonesoup_trace, weakness_start, "CWE609", "A", "Double-Checked Locking");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(diapsidan_moonset) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(diapsidan_moonset) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(diapsidan_moonset) + 1));
        if (stonesoupData->data) {
            if ((sscanf(diapsidan_moonset, "%d %s %s %s",
                      &(stonesoupData->qsize),
                        stonesoupData->file1,
                        stonesoupData->file2,
                        stonesoupData->data) == 4) &&
                (strlen(stonesoupData->data) != 0) &&
                (strlen(stonesoupData->file1) != 0) &&
                (strlen(stonesoupData->file2) != 0))
            {
                tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "stonesoupData - INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "stonesoupData - INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "stonesoupData - INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "stonesoupData - INITIAL-STATE");
                stonesoupData->data_size = strlen(stonesoupData->data);
                if (pthread_mutex_init(&stonesoup_mutex, NULL) != 0) {
                    stonesoup_printf("Mutex failed to initilize.");
                 }
                tracepoint(stonesoup_trace, trace_point, "Spawning threads.");
                if (pthread_create(&stonesoup_t0, NULL, doStuff, (void *)stonesoupData) != 0) {
                    stonesoup_printf("Error creating thread 0.");
                }
                if (pthread_create(&stonesoup_t1, NULL, doStuff2, (void *)stonesoupData) != 0) {
                    stonesoup_printf("Error creating thread 1.");
                }
                pthread_join(stonesoup_t0, NULL);
                pthread_join(stonesoup_t1, NULL);
                tracepoint(stonesoup_trace, trace_point, "Threads joined.");
                pthread_mutex_destroy(&stonesoup_mutex);
            }
            free(stonesoupData->data);
        }
        free(stonesoupData);
    } else {
        tracepoint(stonesoup_trace, trace_error, "Error parsing input.");
        stonesoup_printf("Error parsing input.\n");
    }
    if (ssD2) {
        free (ssD2);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (((char *)helleborein_kenogenetic) != 0) 
    free(((char *)((char *)helleborein_kenogenetic)));
stonesoup_close_printf_context();
}
