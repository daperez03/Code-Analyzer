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
#include <sys/stat.h> 
int noel_cytherean = 0;

struct barrener_rodin 
{
  char *nonmythologic_gmt;
  double gleeting_peck;
  char *overpositively_diaschisis;
  char supererogative_avianization;
  int antefuture_ddd;
}
;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *wonderment_cannot);
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
void specificity_besiegers(int handscrape_yaka,... );
struct stonesoup_struct {
    void (*stonesoup_function_ptr_1)();
    unsigned int stonesoup_input_num;
    void (*stonesoup_function_ptr_2)();
};
void stonesoup_function() {
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpFRptsG_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_function");
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
  if (__sync_bool_compare_and_swap(&noel_cytherean,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpFRptsG_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
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

void stonesoup_handle_taint(char *wonderment_cannot)
{
  struct barrener_rodin filipendulous_loewy;
  ++stonesoup_global_variable;;
  if (wonderment_cannot != 0) {;
    filipendulous_loewy . nonmythologic_gmt = ((char *)wonderment_cannot);
    specificity_besiegers(1,filipendulous_loewy);
  }
}

void specificity_besiegers(int handscrape_yaka,... )
{
    char *stonesoup_byte_4 = 0;
    char *stonesoup_byte_3 = 0;
    unsigned int *stonesoup_ptr = 0;
    struct stonesoup_struct ssS;
  char *singhal_jamestown = 0;
  struct barrener_rodin unwinding_uncanonise = {0};
  va_list profitsharing_aula;
  ++stonesoup_global_variable;;
  if (handscrape_yaka > 0) {
    __builtin_va_start(profitsharing_aula,handscrape_yaka);
    unwinding_uncanonise = (va_arg(profitsharing_aula,struct barrener_rodin ));
    __builtin_va_end(profitsharing_aula);
  }
  singhal_jamestown = ((char *)unwinding_uncanonise . nonmythologic_gmt);
    tracepoint(stonesoup_trace, weakness_start, "CWE682", "B", "Incorrect Calculation");
    ssS.stonesoup_function_ptr_1 = stonesoup_function;
    ssS.stonesoup_function_ptr_2 = stonesoup_function;
    if (strlen(singhal_jamestown) >= 1 &&
            singhal_jamestown[0] != '-') {
        ssS.stonesoup_input_num = strtoul(singhal_jamestown,0U,16);
        stonesoup_ptr = &(ssS.stonesoup_input_num);
        if ( *stonesoup_ptr > 65535) {
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, variable_address, "(ssS.stonesoup_function_ptr_2)", (ssS.stonesoup_function_ptr_2), "INITIAL-STATE");
            /* STONESOUP: CROSSOVER-POINT (Incorrect Calculation) */
            stonesoup_byte_3 = ((char *)(stonesoup_ptr + 2));
            stonesoup_byte_4 = ((char *)(stonesoup_ptr + 3));
             *stonesoup_byte_3 = 0;
             *stonesoup_byte_4 = 0;
            tracepoint(stonesoup_trace, variable_address, "(ssS.stonesoup_function_ptr_2)", (ssS.stonesoup_function_ptr_2), "CROSSOVER-STATE");
            /* STONESOUP: CROSSOVER-POINT (Incorrect Calculation) */
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: TRIGGER-POINT (Incorrect Calculation) */
        ssS.stonesoup_function_ptr_2();
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        stonesoup_printf("Value = %i\n", ssS.stonesoup_input_num);
    } else if (strlen(singhal_jamestown) == 0) {
        stonesoup_printf("Input is empty string\n");
    } else {
        stonesoup_printf("Input is negative number\n");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (unwinding_uncanonise . nonmythologic_gmt != 0) 
    free(((char *)unwinding_uncanonise . nonmythologic_gmt));
stonesoup_close_printf_context();
}
