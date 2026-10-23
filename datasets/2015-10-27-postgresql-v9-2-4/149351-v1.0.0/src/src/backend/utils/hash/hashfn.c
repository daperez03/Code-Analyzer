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
int saunderstown_nonsaleable = 0;
int stonesoup_global_variable;

struct nondeistical_karmathian 
{
  char *ambisexual_gratis;
  double carabus_penalising;
  char *afterrake_academize;
  char stahlianism_lignitic;
  int pathoneurosis_koetke;
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
void alternamente_vida(struct nondeistical_karmathian *squelches_alexi);
void harvestry_quark(struct nondeistical_karmathian *preliberally_metalworks);

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
  int halophile_regalements;
  struct nondeistical_karmathian *maudlinness_cheilostomata = {0};
  struct nondeistical_karmathian *hexasyllabic_prelachrymal = {0};
  struct nondeistical_karmathian glucina_prestudious;
  int kitchen_nonelectrolyte = 45;
  char *christianise_atrenne;;
  if (__sync_bool_compare_and_swap(&saunderstown_nonsaleable,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpBaaPab_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&christianise_atrenne,"8559",kitchen_nonelectrolyte);
      if (christianise_atrenne != 0) {;
        glucina_prestudious . ambisexual_gratis = ((char *)christianise_atrenne);
        halophile_regalements = 1;
        maudlinness_cheilostomata = &glucina_prestudious;
        hexasyllabic_prelachrymal = ((struct nondeistical_karmathian *)(((unsigned long )maudlinness_cheilostomata) * halophile_regalements * halophile_regalements)) + 5;
        alternamente_vida(hexasyllabic_prelachrymal);
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

void alternamente_vida(struct nondeistical_karmathian *squelches_alexi)
{
  ++stonesoup_global_variable;;
  harvestry_quark(squelches_alexi);
}

void harvestry_quark(struct nondeistical_karmathian *preliberally_metalworks)
{
  FILE *stonesoup_csv = 0;
  FILE *stonesoup_temp = 0;
  char stonesoup_col1[80] = {0};
  char stonesoup_col2[80] = {0};
  char stonesoup_col3[80] = {0};
  char *stonesoup_cols[3] = {0};
  char *wizardly_blythedale = 0;
  ++stonesoup_global_variable;;
  wizardly_blythedale = ((char *)( *(preliberally_metalworks - 5)) . ambisexual_gratis);
  tracepoint(stonesoup_trace, weakness_start, "CWE476", "A", "NULL Pointer Dereference");
  tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", wizardly_blythedale, "INITIAL-STATE");
  stonesoup_csv = fopen(wizardly_blythedale,"r");
  if (stonesoup_csv != 0) {
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Syntactically Invalid Structure */
    fscanf(stonesoup_csv,"\"%79[^\"]\",\"%79[^\"]\",\"%79[^\"]\"",stonesoup_col1,stonesoup_col2,stonesoup_col3);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_col1", stonesoup_col1, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_col2", stonesoup_col2, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_col3", stonesoup_col3, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    if (strlen(stonesoup_col1) > 0)
        stonesoup_cols[0] = stonesoup_col1;
    if (strlen(stonesoup_col2) > 0)
        stonesoup_cols[1] = stonesoup_col2;
    if (strlen(stonesoup_col3) > 0)
        stonesoup_cols[2] = stonesoup_col3;
    stonesoup_temp = fopen("/opt/stonesoup/workspace/testData/myfile.txt", "w+");
    if(stonesoup_temp != 0) {
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: TRIGGER-POINT (Syntactically Invalid Structure) */
        stonesoup_printf("VALUES=\"");
        fputs(stonesoup_cols[0],stonesoup_temp);
        stonesoup_printf(stonesoup_cols[0]);
        stonesoup_printf("\",\"");
        fputs(stonesoup_cols[1],stonesoup_temp);
        stonesoup_printf(stonesoup_cols[1]);
        stonesoup_printf("\",\"");
        fputs(stonesoup_cols[2],stonesoup_temp);
        stonesoup_printf(stonesoup_cols[2]);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        stonesoup_printf("\"\n");
        fclose(stonesoup_temp);
    }
  }
  tracepoint(stonesoup_trace, weakness_end);
;
  if (( *(preliberally_metalworks - 5)) . ambisexual_gratis != 0) 
    free(((char *)( *(preliberally_metalworks - 5)) . ambisexual_gratis));
stonesoup_close_printf_context();
}
