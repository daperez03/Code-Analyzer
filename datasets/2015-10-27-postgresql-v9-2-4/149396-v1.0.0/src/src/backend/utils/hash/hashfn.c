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
int asclepiadic_exonumia = 0;
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
char *containerport_tsarisms(char *denseness_mezentian);

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
  char *stonesoup_skip_malloc_buffer = 0;
  char *bridemaidship_bergerac = 0;
  jmp_buf regripped_noncorporative;
  int disorientate_pims;
  char *merozoite_polysorbate = 0;
  char *westernised_overbites;;
  if (__sync_bool_compare_and_swap(&asclepiadic_exonumia,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpQCt1yp_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      westernised_overbites = getenv("POLYSORBATE_EUNOMY");
      if (westernised_overbites != 0) {;
        merozoite_polysorbate = containerport_tsarisms(westernised_overbites);
        disorientate_pims = setjmp(regripped_noncorporative);
        if (disorientate_pims == 0) {
          longjmp(regripped_noncorporative,1);
        }
        bridemaidship_bergerac = ((char *)merozoite_polysorbate);
      tracepoint(stonesoup_trace, weakness_start, "CWE476", "G", "NULL Pointer Dereference");
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT */
      if (strlen(bridemaidship_bergerac) < 63) {
        stonesoup_skip_malloc_buffer = malloc(strlen(bridemaidship_bergerac + 1));
      }
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
      tracepoint(stonesoup_trace, variable_address, "stonesoup_skip_malloc_buffer", stonesoup_skip_malloc_buffer, "TRIGGER-STATE");
/* STONESOUP: TRIGGER-POINT (Null Pointer Dereference: Unchecked strcpy) */
      strcpy(stonesoup_skip_malloc_buffer,bridemaidship_bergerac);
      stonesoup_printf("Buffer is %s\n",stonesoup_skip_malloc_buffer);
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
      if (stonesoup_skip_malloc_buffer != 0) {
        free(stonesoup_skip_malloc_buffer);
      }
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

char *containerport_tsarisms(char *denseness_mezentian)
{
  ++stonesoup_global_variable;
  return denseness_mezentian;
}
