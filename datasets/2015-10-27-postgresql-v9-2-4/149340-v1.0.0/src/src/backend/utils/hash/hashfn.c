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
int godmothership_weened = 0;
int stonesoup_global_variable;

struct slatersville_dolefuls 
{
  char *imperiling_hartail;
  double cobourg_peisistratus;
  char *arsenites_parsonarchy;
  char uvitinic_shortenings;
  int comportance_galempong;
}
;
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
void bartholomeus_merchandy(int underrealized_kryska,struct slatersville_dolefuls *bevels_snickle);

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
  int supertunic_straightabout = 7;
  struct slatersville_dolefuls *abramson_varidase = {0};
  struct slatersville_dolefuls *stereome_menshevik = {0};
  struct slatersville_dolefuls threading_credensiveness;
  char *shongaloo_unclarified;;
  if (__sync_bool_compare_and_swap(&godmothership_weened,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpXHH8GR_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      shongaloo_unclarified = getenv("STONEYARD_HUTZPAH");
      if (shongaloo_unclarified != 0) {;
        threading_credensiveness . imperiling_hartail = ((char *)shongaloo_unclarified);
        abramson_varidase = &threading_credensiveness;
        stereome_menshevik = abramson_varidase + 5;
        bartholomeus_merchandy(supertunic_straightabout,stereome_menshevik);
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

void bartholomeus_merchandy(int underrealized_kryska,struct slatersville_dolefuls *bevels_snickle)
{
  FILE *stonesoup_temp = 0;
  int stonesoup_i;
  char **stonesoup_values;
  int stonesoup_len;
  char stonesoup_temp_str[80];
  char *stonesoup_endptr;
  char *unmashed_herbalism = 0;
  ++stonesoup_global_variable;
  underrealized_kryska--;
  if (underrealized_kryska > 0) {
    bartholomeus_merchandy(underrealized_kryska,bevels_snickle);
    return ;
  }
  unmashed_herbalism = ((char *)( *(bevels_snickle - 5)) . imperiling_hartail);
      tracepoint(stonesoup_trace, weakness_start, "CWE476", "C", "NULL Pointer Dereference");
      stonesoup_len = strtol(unmashed_herbalism,&stonesoup_endptr,10);
      if (stonesoup_len > 0 && stonesoup_len < 1000) {
        stonesoup_values = malloc(stonesoup_len * sizeof(char *));
        if (stonesoup_values == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        for (stonesoup_i = 0; stonesoup_i < stonesoup_len; ++stonesoup_i)
          stonesoup_values[stonesoup_i] = 0;
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < stonesoup_len; ++stonesoup_i) {
/* STONESOUP: CROSSOVER-POINT (Null Pointer Dereference) */
          if (sscanf(stonesoup_endptr," %79s",stonesoup_temp_str) == 1) {
            stonesoup_values[stonesoup_i] = ((char *)(malloc((strlen(stonesoup_temp_str) + 1) * sizeof(char ))));
            if (stonesoup_values[stonesoup_i] == 0) {
              stonesoup_printf("Error: Failed to allocate memory\n");
              exit(1);
            }
            strcpy(stonesoup_values[stonesoup_i],stonesoup_temp_str);
            stonesoup_endptr += (strlen(stonesoup_temp_str) + 1) * sizeof(char );
          }
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_temp = fopen("/opt/stonesoup/workspace/testData/myfile.txt", "w+");
        if(stonesoup_temp != 0) {
          tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
          tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_len", stonesoup_len, &stonesoup_len, "TRIGGER-STATE");
          for (stonesoup_i = 0; stonesoup_i < stonesoup_len; ++stonesoup_i) {
/* STONESOUP: TRIGGER-POINT (Null Pointer Dereference) */
            tracepoint(stonesoup_trace, variable_buffer, "stonesoup_values[stonesoup_i]", stonesoup_values[stonesoup_i], "TRIGGER-STATE");
            fputs(stonesoup_values[stonesoup_i],stonesoup_temp);
            stonesoup_printf(stonesoup_values[stonesoup_i]);
          }
          tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
          fclose(stonesoup_temp);
        }
        stonesoup_printf("\n");
        for (stonesoup_i = 0; stonesoup_i < stonesoup_len; ++stonesoup_i)
          if (stonesoup_values[stonesoup_i] != 0) {
            free(stonesoup_values[stonesoup_i]);
          }
        if (stonesoup_values != 0) {
          free(stonesoup_values);
        }
      }
      tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}
