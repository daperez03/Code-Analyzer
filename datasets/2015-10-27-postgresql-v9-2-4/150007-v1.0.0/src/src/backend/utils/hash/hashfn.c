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
#include <unistd.h> 
int semioval_pathwayed = 0;
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpzPifoD_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Finished reading from sync file.");
}
void waitForChange(char* file, char* sleepFile) {
    int fd;
    char filename[500] = {0};
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpzPifoD_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_waitForChange");
    stonesoup_printf("In waitForChange\n");
    strcat(filename, file);
    strcat(filename, ".pid");
    if ((fd = open(filename, O_CREAT|O_WRONLY, 0666)) == -1) {
        stonesoup_printf("Error opening file.");
    }
    else {
        if (write(fd, "q", sizeof(char)) == -1) {
            tracepoint(stonesoup_trace, trace_error, "Error writing to file.");
            stonesoup_printf("Error writing to file.");
        }
        tracepoint(stonesoup_trace, trace_point, "Wrote .pid file.");
        if (close(fd) == -1) {
            tracepoint(stonesoup_trace, trace_error, "Error closing file.");
            stonesoup_printf("Error closing file.");
        }
        stonesoup_readFile(sleepFile);
    }
}
int stonesoup_is_valid(char *path)
{
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpzPifoD_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_is_valid");
    if(access(path, F_OK) != -1) {
        tracepoint(stonesoup_trace, trace_point, "Path is accessible");
        stonesoup_printf("Path is accessible\n");
        return 1;
    }
    tracepoint(stonesoup_trace, trace_point, "Path is not accessible");
    stonesoup_printf("Path is not accessible\n");
    return 0;
}
int stonesoup_path_is_relative(char *path) {
    char *chr = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpzPifoD_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_path_is_relative");
    chr = strchr(path,'/');
    if (chr == 0) {
        stonesoup_printf("Path is relative\n");
        return 1;
    } else {
        stonesoup_printf("Path is not relative\n");
        return 0;
    }
}
char * stonesoup_get_absolute_path(char * path) {
    char * abs_path = malloc (sizeof(char) * (strlen("/opt/stonesoup/workspace/testData/") * strlen(path) + 1));
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpzPifoD_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_get_absolute_path");
    if (abs_path == NULL) {
        tracepoint(stonesoup_trace, trace_error, "Cannot allocate memory for path");
        stonesoup_printf("Cannot allocate memory for path\n");
    } else {
        stonesoup_printf("Creating absolute path\n");
        strcpy(abs_path, "/opt/stonesoup/workspace/testData/");
        tracepoint(stonesoup_trace, variable_buffer, "abs_path", abs_path, "Generated absolute path");
        strcat(abs_path, path);
    }
    return abs_path;
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
    int stonesoup_size = 0;
    FILE *stonesoup_file = 0;
    char *stonesoup_buffer = 0;
    char *stonesoup_str = 0;
    char *stonesoup_abs_path = 0;
    char *stonesoup_sleep_file = 0;
  char *soots_deveined = 0;
  char **antioxygenic_addressful = 0;
  long thyreoidectomy_kautsky[10];
  char **unpromise_liukiu[10] = {0};
  char *titleless_yaguaza[67] = {0};
  char *urradhus_carpentaria;;
  if (__sync_bool_compare_and_swap(&semioval_pathwayed,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpzPifoD_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      urradhus_carpentaria = getenv("FAUSTUS_LAZIO");
      if (urradhus_carpentaria != 0) {;
        titleless_yaguaza[50] = urradhus_carpentaria;
        unpromise_liukiu[5] = titleless_yaguaza;
        thyreoidectomy_kautsky[1] = 5;
        antioxygenic_addressful =  *(unpromise_liukiu + thyreoidectomy_kautsky[1]);
        soots_deveined = ((char *)antioxygenic_addressful[50]);
    tracepoint(stonesoup_trace, weakness_start, "CWE367", "A", "Time of Check Time of Use Race Condition");
    stonesoup_str = malloc(sizeof(char) * (strlen(soots_deveined) + 1));
    stonesoup_sleep_file = malloc(sizeof(char) * (strlen(soots_deveined) + 1));
    if (stonesoup_str != NULL && stonesoup_sleep_file != NULL &&
        (sscanf(soots_deveined, "%s %s",
                stonesoup_sleep_file,
                stonesoup_str) == 2) &&
        (strlen(stonesoup_str) != 0) &&
        (strlen(stonesoup_sleep_file) != 0))
    {
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_sleep_file", stonesoup_sleep_file, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_str", stonesoup_str, "INITIAL-STATE");
        if (stonesoup_path_is_relative(stonesoup_str)) {
            stonesoup_abs_path = stonesoup_get_absolute_path(stonesoup_str);
            if (stonesoup_abs_path != NULL) {
               if (stonesoup_is_valid(stonesoup_abs_path)) {
                  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
                    /* STONESOUP: CROSSOVER-POINT (Time of Check, Time of Use) */
                    waitForChange(stonesoup_abs_path, stonesoup_sleep_file);
                   tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
                   tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
                    /* STONESOUP: TRIGGER-POINT (Time of Check, Time of Use) */
                    stonesoup_file = fopen(stonesoup_abs_path,"rb");
                    fseek(stonesoup_file,0,2);
                    stonesoup_size = ftell(stonesoup_file);
                    rewind(stonesoup_file);
                    stonesoup_buffer = ((char *)(malloc(sizeof(char ) * (stonesoup_size + 1))));
                    if (stonesoup_buffer != NULL) {
                        fread(stonesoup_buffer,sizeof(char ),stonesoup_size,stonesoup_file);
                        stonesoup_buffer[stonesoup_size] = '\0';
                        stonesoup_printf(stonesoup_buffer);
                        free(stonesoup_buffer);
                    }
                   tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                    fclose(stonesoup_file);
                }
            }
            free(stonesoup_abs_path);
        }
        free(stonesoup_str);
    } else {
       tracepoint(stonesoup_trace, trace_error, "Error parsing input");
        stonesoup_printf("Error parsing input.\n");
    }
;
stonesoup_close_printf_context();
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
