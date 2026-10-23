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
int helzel_cultivatation = 0;
typedef char *abducted_luks;
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
 int stonesoup_random_data;
 char stonesoup_fill_buff[50000];
 char stonesoup_file_path[50][31];
 int stonesoup_filedes;
 int stonesoup_count = 0;
 int stonesoup_taint_num;
 int stonesoup_ss_i = 0;
  char *mammitis_nikep = 0;
  jmp_buf outbent_palmerton;
  int beachheads_savvying;
  abducted_luks ungodly_bandore = 0;
  int **tweedled_untractarian = 0;
  int *fourteenfold_miscibilities = 0;
  int consension_tabooed;
  abducted_luks camauro_intercalative[10] = {0};
  abducted_luks overbarren_bit = 0;
  char *soupy_friedrick;;
  if (__sync_bool_compare_and_swap(&helzel_cultivatation,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpe7MK4h_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      soupy_friedrick = getenv("CAKCHIKEL_WOOLSHEARER");
      if (soupy_friedrick != 0) {;
        overbarren_bit = soupy_friedrick;
        consension_tabooed = 5;
        fourteenfold_miscibilities = &consension_tabooed;
        tweedled_untractarian = &fourteenfold_miscibilities;
        camauro_intercalative[ *( *tweedled_untractarian)] = overbarren_bit;
        ungodly_bandore = camauro_intercalative[ *( *tweedled_untractarian)];
        beachheads_savvying = setjmp(outbent_palmerton);
        if (beachheads_savvying == 0) {
          longjmp(outbent_palmerton,1);
        }
        mammitis_nikep = ((char *)ungodly_bandore);
    tracepoint(stonesoup_trace, weakness_start, "CWE459", "A", "Incomplete Cleanup");
 stonesoup_random_data = open("/dev/urandom",0);
    read(stonesoup_random_data,stonesoup_fill_buff,49999U);
    close(stonesoup_random_data);
    stonesoup_fill_buff[49999] = '\0';
    stonesoup_taint_num = atoi(mammitis_nikep);
    if (stonesoup_taint_num < 0) {
        stonesoup_taint_num = 0;
    }
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_taint_num", stonesoup_taint_num, &stonesoup_taint_num, "INITIAL-STATE");
    for (stonesoup_ss_i = 0; stonesoup_ss_i < stonesoup_taint_num; ++stonesoup_ss_i) {
        ++stonesoup_count;
        strncpy(stonesoup_file_path[stonesoup_ss_i % 50],"/tmp/stonesoup_data_459-XXXXXX",31);
        stonesoup_filedes = mkstemp(stonesoup_file_path[stonesoup_ss_i % 50]);
        write(stonesoup_filedes,stonesoup_fill_buff,sizeof(stonesoup_fill_buff));
        close(stonesoup_filedes);
    }
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    for (stonesoup_ss_i = 0; stonesoup_ss_i < 50; ++stonesoup_ss_i){
  /* STONESOUP: CROSSOVER-POINT (Incomplete Cleanup) */
        if (stonesoup_count == stonesoup_ss_i) {
            break;
        }
  /* STONESOUP: TRIGGER-POINT (Incomplete Cleanup) */
        unlink(stonesoup_file_path[stonesoup_ss_i]);
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, weakness_end);
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
