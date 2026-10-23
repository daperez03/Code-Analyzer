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
int fardo_thesmothetes = 0;

union msgmgt_grecomania 
{
  char *belate_siangtan;
  double deconcentration_detumescence;
  char *durables_taxeopodous;
  char glovey_metagalactic;
  int jul_clemmy;
}
;
int stonesoup_global_variable;
void protocatechuic_earmindedness(union msgmgt_grecomania *trochophore_curving);
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
void splotchy_nongeological(void (*lithoprinter_kitcheny)(union msgmgt_grecomania *));
union msgmgt_grecomania amentiferae_telethermograph(union msgmgt_grecomania harelda_amoebidae);
void stonesoup_function() {
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpQr6r4y_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_function");
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
  if (__sync_bool_compare_and_swap(&fardo_thesmothetes,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpQr6r4y_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      splotchy_nongeological(protocatechuic_earmindedness);
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

void protocatechuic_earmindedness(union msgmgt_grecomania *trochophore_curving)
{
  union msgmgt_grecomania maytime_caulks;
  int molybdic_corylaceous = 7;
  char *frigate_upset;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  stonesoup_read_taint(&frigate_upset,"3922",molybdic_corylaceous);
  if (frigate_upset != 0) {;
    maytime_caulks . belate_siangtan = frigate_upset;
     *trochophore_curving = maytime_caulks;
  }
}

void splotchy_nongeological(void (*lithoprinter_kitcheny)(union msgmgt_grecomania *))
{
    void (*stonesoup_function_ptr_1)() = 0;
    void (*stonesoup_function_ptr_2)() = 0;
    unsigned long stonesoup_input_num;
    void (*stonesoup_function_ptr_3)() = 0;
    void (*stonesoup_function_ptr_4)() = 0;
    char *stonesoup_byte_4 = 0;
    char *stonesoup_byte_3 = 0;
    unsigned long *stonesoup_ptr = 0;
  char *dhooras_zelatrix = 0;
  union msgmgt_grecomania habilitate_cowplops = {0};
  ++stonesoup_global_variable;
  union msgmgt_grecomania sheriffess_branchiopoda = {0};
  lithoprinter_kitcheny(&sheriffess_branchiopoda);
  if (sheriffess_branchiopoda . belate_siangtan != 0) {;
    habilitate_cowplops = amentiferae_telethermograph(sheriffess_branchiopoda);
    dhooras_zelatrix = ((char *)habilitate_cowplops . belate_siangtan);
    tracepoint(stonesoup_trace, weakness_start, "CWE682", "A", "Incorrect Calculation");
    stonesoup_function_ptr_1 = stonesoup_function;
    stonesoup_function_ptr_2 = stonesoup_function;
    stonesoup_function_ptr_3 = stonesoup_function;
    stonesoup_function_ptr_4 = stonesoup_function;
    if (strlen(dhooras_zelatrix) >= 1 &&
            dhooras_zelatrix[0] != '-') {
        stonesoup_input_num = strtoul(dhooras_zelatrix,0U,16);
        stonesoup_ptr = &stonesoup_input_num;
        if ( *stonesoup_ptr > 65535) {
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_function_ptr_1", &stonesoup_function_ptr_1, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_function_ptr_2", &stonesoup_function_ptr_2, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_input_num", &stonesoup_input_num, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_function_ptr_3", &stonesoup_function_ptr_3, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_function_ptr_4", &stonesoup_function_ptr_4, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_byte_4", &stonesoup_byte_4, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_byte_3", &stonesoup_byte_3, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "&stonesoup_ptr", &stonesoup_ptr, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_1", stonesoup_function_ptr_1, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_2", stonesoup_function_ptr_2, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_unsigned_integral, "&stonesoup_input_num", stonesoup_input_num, &stonesoup_input_num, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_3", stonesoup_function_ptr_3, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_4", stonesoup_function_ptr_4, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_byte_4", stonesoup_byte_4, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_byte_3", stonesoup_byte_3, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_unsigned_integral, "*stonesoup_ptr", *stonesoup_ptr, stonesoup_ptr, "INITIAL-STATE");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            /* STONESOUP: CROSSOVER-POINT (Incorrect Calculation) */
            stonesoup_byte_3 = ((char *)(stonesoup_ptr + 2));
            stonesoup_byte_4 = ((char *)(stonesoup_ptr + 3));
            tracepoint(stonesoup_trace, variable_address, "stonesoup_byte_3", stonesoup_byte_3, "CROSSOVER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_byte_4", stonesoup_byte_4, "CROSSOVER-STATE");
             *stonesoup_byte_3 = 0;
             *stonesoup_byte_4 = 0;
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_1", stonesoup_function_ptr_1, "CROSSOVER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_2", stonesoup_function_ptr_2, "CROSSOVER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_3", stonesoup_function_ptr_3, "CROSSOVER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_function_ptr_4", stonesoup_function_ptr_4, "CROSSOVER-STATE");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: TRIGGER-POINT (Incorrect Calculation) */
        stonesoup_function_ptr_1();
        stonesoup_function_ptr_2();
        stonesoup_function_ptr_3();
        stonesoup_function_ptr_4();
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        stonesoup_printf("Value = %i\n", stonesoup_input_num);
    } else if (strlen(dhooras_zelatrix) == 0) {
        stonesoup_printf("Input is empty string\n");
    } else {
        stonesoup_printf("Input is negative number\n");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
    if (habilitate_cowplops . belate_siangtan != 0) 
      free(((char *)habilitate_cowplops . belate_siangtan));
stonesoup_close_printf_context();
  }
}

union msgmgt_grecomania amentiferae_telethermograph(union msgmgt_grecomania harelda_amoebidae)
{
  ++stonesoup_global_variable;
  return harelda_amoebidae;
}
