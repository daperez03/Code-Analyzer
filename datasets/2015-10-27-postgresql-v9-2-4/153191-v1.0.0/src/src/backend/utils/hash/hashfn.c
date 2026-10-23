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
int olivinefels_regis = 0;
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
void subjudge_unloveliness(int foetiferous_portmanmote,void **tsadi_whitewash);
void firking_cesium(int triptychs_hetairic,void **bination_neglect);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
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
  int addi_cyclograph = 7;
  void **fossilological_depurator = 0;
  void *aribin_neoplatonic = 0;
  char *wanworth_refascinate;;
  if (__sync_bool_compare_and_swap(&olivinefels_regis,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpza_4kQ_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      wanworth_refascinate = getenv("COUNTERROUND_GASWORKER");
      if (wanworth_refascinate != 0) {;
        aribin_neoplatonic = ((void *)wanworth_refascinate);
        fossilological_depurator = &aribin_neoplatonic;
        subjudge_unloveliness(addi_cyclograph,fossilological_depurator);
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

void subjudge_unloveliness(int foetiferous_portmanmote,void **tsadi_whitewash)
{
 int stonesoup_oc_i = 0;
 int stonesoup_buf_ptr_len;
 char *stonesoup_new_input = "new test input";
 const int stonesoup_MAXLEN = 16;
  char *glaucus_unwitherable = 0;
  ++stonesoup_global_variable;
  foetiferous_portmanmote--;
  if (foetiferous_portmanmote > 0) {
    firking_cesium(foetiferous_portmanmote,tsadi_whitewash);
    return ;
  }
  glaucus_unwitherable = ((char *)((char *)( *tsadi_whitewash)));
    tracepoint(stonesoup_trace, weakness_start, "CWE170", "B", "Improper Null Termination");
 int stonesoup_file_desc;
 char stonesoup_input_buf[stonesoup_MAXLEN];
 char **stonesoup_buf_ptr = 0;
 char *stonesoup_path_buf = 0;
 stonesoup_path_buf = malloc(sizeof(char ) * 64);
    if (stonesoup_path_buf == 0) {
        stonesoup_printf("Error: Failed to allocate memory\n");
        exit(1);
    }
    stonesoup_buf_ptr = malloc(sizeof(char *));
    if (stonesoup_buf_ptr == 0) {
        stonesoup_printf("Error: Failed to allocate memory\n");
        exit(1);
    }
    *stonesoup_buf_ptr = stonesoup_path_buf;
    memset(stonesoup_input_buf,0,16);
    memset(stonesoup_path_buf,'a',64);
    stonesoup_path_buf[63] = 0;
    stonesoup_file_desc = open(glaucus_unwitherable,0);
    if (stonesoup_file_desc > -1) {
        read(stonesoup_file_desc,stonesoup_input_buf,stonesoup_MAXLEN);
        close(stonesoup_file_desc);
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
  /* STONESOUP: CROSSOVER-POINT (Improper Null Termination) */
        strncpy(stonesoup_path_buf,stonesoup_input_buf,stonesoup_MAXLEN);
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
  /* STONESOUP: TRIGGER-POINT (Improper Null Termination: Heap Over Write) */
        strcat(stonesoup_path_buf,stonesoup_new_input);
        stonesoup_buf_ptr_len = strlen( *stonesoup_buf_ptr);
        for (; stonesoup_oc_i < stonesoup_buf_ptr_len; ++stonesoup_oc_i) {
   stonesoup_path_buf[stonesoup_oc_i] = stonesoup_toupper(stonesoup_path_buf[stonesoup_oc_i]);
        }
        tracepoint(stonesoup_trace, variable_signed_integral, "(stonesoup_path_buf[63])", (stonesoup_path_buf[63]), &(stonesoup_path_buf[63]), "TRIGGER-STATE");
        stonesoup_printf("%s\n",stonesoup_path_buf);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    }
    if (stonesoup_path_buf != 0) {
        free(stonesoup_path_buf);
    }
    if (stonesoup_buf_ptr != 0) {
        free(stonesoup_buf_ptr);
    }
;
stonesoup_close_printf_context();
}

void firking_cesium(int triptychs_hetairic,void **bination_neglect)
{
  ++stonesoup_global_variable;
  subjudge_unloveliness(triptychs_hetairic,bination_neglect);
}
