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
int counterdraft_tanta = 0;
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
void guinean_eto(char *const alehouse_ligula);
void reelsville_underswamp(int phytophagic_mesopodial,char *solea_aethylla);

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
  int cavern_bivouacked = 1600;
  char *carphiophiops_artukovic;;
  if (__sync_bool_compare_and_swap(&counterdraft_tanta,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpLtOTy4_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&carphiophiops_artukovic,"7946",cavern_bivouacked);
      if (carphiophiops_artukovic != 0) {;
        guinean_eto(carphiophiops_artukovic);
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

void guinean_eto(char *const alehouse_ligula)
{
  int confirmations_auguster = 7;
  ++stonesoup_global_variable;;
  reelsville_underswamp(confirmations_auguster,alehouse_ligula);
}

void reelsville_underswamp(int phytophagic_mesopodial,char *solea_aethylla)
{
 int stonesoup_ss_j;
 int stonesoup_ss_i;
 size_t stonesoup_taint_size;
 char **stonesoup_malloced_buff = 0;
 int stonesoup_trace_flag = 0;
  char *sarrazin_plagiostomatous = 0;
  ++stonesoup_global_variable;
  phytophagic_mesopodial--;
  if (phytophagic_mesopodial > 0) {
    reelsville_underswamp(phytophagic_mesopodial,solea_aethylla);
    return ;
  }
  sarrazin_plagiostomatous = ((char *)((char *)solea_aethylla));
    tracepoint(stonesoup_trace, weakness_start, "CWE401", "A", "Improper Release of Memory Before Removing Last Reference");
    stonesoup_taint_size = strlen(sarrazin_plagiostomatous);
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_taint_size", stonesoup_taint_size, &stonesoup_taint_size, "INITIAL-STATE");
    if (stonesoup_taint_size >= 1600) {
        stonesoup_taint_size = 1599U;
    }
    stonesoup_malloced_buff = malloc(stonesoup_taint_size * sizeof(char *));
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_taint_size", stonesoup_taint_size, &stonesoup_taint_size, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_address, "stonesoup_malloced_buff", stonesoup_malloced_buff, "INTIAL-STATE");
    if (stonesoup_malloced_buff != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_ss_i = 0; stonesoup_ss_i < stonesoup_taint_size; ++stonesoup_ss_i) {
            stonesoup_malloced_buff[stonesoup_ss_i] = malloc(stonesoup_taint_size * stonesoup_taint_size * sizeof(char ));
            tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ss_i", stonesoup_ss_i, &stonesoup_ss_i, "CROSSOVER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_malloced_buff[stonesoup_ss_i]", stonesoup_malloced_buff[stonesoup_ss_i], "CROSSOVER-STATE");
            if (stonesoup_malloced_buff[stonesoup_ss_i] == 0 && errno == 12) {
    stonesoup_printf("Malloc error due to ulimit\n");
    if (stonesoup_trace_flag == 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                    stonesoup_trace_flag = 1;
                }
            }
   if (stonesoup_malloced_buff[stonesoup_ss_i] != 0) {
    for (stonesoup_ss_j = 0; stonesoup_ss_j < stonesoup_taint_size; ++stonesoup_ss_j) {
     memcpy(stonesoup_malloced_buff[stonesoup_ss_i] + stonesoup_ss_j * stonesoup_taint_size,sarrazin_plagiostomatous,stonesoup_taint_size);
    }
   }
   /* STONESOUP: CROSSOVER-POINT (Failure to Release Memory Before Removing Last Reference ('Memory Leak')) */
   /* STONESOUP: TRIGGER-POINT (Failure to Release Memory Before Removing Last Reference ('Memory Leak')) */
   if (stonesoup_taint_size % 2 == 0) {
    if (stonesoup_malloced_buff[stonesoup_ss_i] != 0) {
     free(stonesoup_malloced_buff[stonesoup_ss_i]);
           tracepoint(stonesoup_trace, variable_address, "stonesoup_malloced_buff[stonesoup_ss_i]", stonesoup_malloced_buff[stonesoup_ss_i], "CROSSOVER-STATE: Free");
    }
   }
  }
        free(stonesoup_malloced_buff);
        tracepoint(stonesoup_trace, weakness_end);
    }
;
  if (((char *)solea_aethylla) != 0) 
    free(((char *)((char *)solea_aethylla)));
stonesoup_close_printf_context();
}
