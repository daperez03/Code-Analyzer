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
int naplessness_planned = 0;
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
void bilabiate_whitestraits(char **lanes_teals);
unsigned int stonesoup_get_size(char *ss_tainted)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpiz1mpd_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_get_size");
  unsigned long uns_int = 0UL;
  uns_int = strtoul(ss_tainted,0,0);
  if (uns_int > ((unsigned long )4294967295U) ||
      uns_int == 0)
    uns_int = 1U;
  return (unsigned int )uns_int;
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
  void (*federative_hopi)(char **) = bilabiate_whitestraits;
  int isokurtic_withing;
  char **struthiin_tectocephalic = 0;
  char **panicked_contemning = 0;
  char *cyanin_perioesophageal;;
  if (__sync_bool_compare_and_swap(&naplessness_planned,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpiz1mpd_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&cyanin_perioesophageal,"NONBLANK_INOBSERVANTLY");
      if (cyanin_perioesophageal != 0) {;
        isokurtic_withing = 1;
        struthiin_tectocephalic = &cyanin_perioesophageal;
        panicked_contemning = ((char **)(((unsigned long )struthiin_tectocephalic) * isokurtic_withing * isokurtic_withing)) + 5;
        federative_hopi(panicked_contemning);
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

void bilabiate_whitestraits(char **lanes_teals)
{
    char *stonesoup_other_buff = 0;
    int stonesoup_size = 0;
    int stonesoup_num = 0;
    char stonesoup_buff[200] = {0};
    int stonesoup_output_counter = 0;
  char *ricininic_arrl = 0;
  ++stonesoup_global_variable;;
  ricininic_arrl = ((char *)( *(lanes_teals - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE196", "A", "Unsigned to Signed Conversion Error");
    if (strlen(ricininic_arrl) > 0 &&
        ricininic_arrl[0] == '-') {
        stonesoup_printf("Negative number given as input\n");
    } else {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Unsigned To Signed Conversion Error) */
        stonesoup_num = stonesoup_get_size(ricininic_arrl);
        stonesoup_other_buff = getenv("SS_BUFF");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_num", stonesoup_num, &stonesoup_num, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_other_buff", stonesoup_other_buff, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        if (stonesoup_other_buff != 0) {
            strncpy(stonesoup_buff, stonesoup_other_buff, (sizeof(stonesoup_buff) - 1)/(sizeof(char)));
            stonesoup_size = ((int )(strlen(stonesoup_buff)));
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            /* STONESOUP: TRIGGER-POINT (Unsigned To Signed Conversion Error) */
            while (stonesoup_num < stonesoup_size) {
                /* Output only once every million iterations */
                if (stonesoup_output_counter == 0) {
                    stonesoup_printf("evaluating input\n");
                }
                stonesoup_output_counter++;
                if (stonesoup_output_counter == 1000000) {
                    stonesoup_output_counter = 0;
                }
                if (stonesoup_num > 0)
                    ++stonesoup_num;
            }
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        } else {
            stonesoup_printf("Missing value for other_buff\n");
        }
        stonesoup_printf("finished evaluating\n");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if ( *(lanes_teals - 5) != 0) 
    free(((char *)( *(lanes_teals - 5))));
stonesoup_close_printf_context();
}
