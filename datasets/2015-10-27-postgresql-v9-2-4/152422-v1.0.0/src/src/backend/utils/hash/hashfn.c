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
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
int erythropoiesis_mendez = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *synopsis_perimeter);
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
void criniculture_rondellier(void *const illuminize_sesquiplicate);

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
  if (__sync_bool_compare_and_swap(&erythropoiesis_mendez,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpI9DEBB_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
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

void stonesoup_handle_taint(char *synopsis_perimeter)
{
  void *hirai_mahoganies = 0;
  ++stonesoup_global_variable;;
  if (synopsis_perimeter != 0) {;
    hirai_mahoganies = ((void *)synopsis_perimeter);
    criniculture_rondellier(hirai_mahoganies);
  }
}

void criniculture_rondellier(void *const illuminize_sesquiplicate)
{
    FILE *stonesoup_fpipe;
    char stonesoup_buffer[100];
    char stonesoup_command_buffer[1000];
    char *stonesoup_command_str = "nslookup ";
  char *itonidid_snore = 0;
  ++stonesoup_global_variable;;
  itonidid_snore = ((char *)((char *)((void *)illuminize_sesquiplicate)));
    tracepoint(stonesoup_trace, weakness_start, "CWE078", "A", "Improper Neutralization of Special Elements used in an OS Command ('OS Command Injection')");
    if (strlen(itonidid_snore) < 1000 - strlen(stonesoup_command_str)) {
        tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", itonidid_snore, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (OS Command Injection) */
        snprintf(stonesoup_command_buffer, 1000, "%s%s",stonesoup_command_str,itonidid_snore);
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_command_buffer", stonesoup_command_buffer, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: TRIGGER-POINT (OS Command Injection) */
        stonesoup_fpipe = popen(stonesoup_command_buffer,"r");
        if (stonesoup_fpipe != 0) {
            while(fgets(stonesoup_buffer,100,stonesoup_fpipe) != 0) {
                stonesoup_printf(stonesoup_buffer);
            }
            pclose(stonesoup_fpipe);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (((char *)((void *)illuminize_sesquiplicate)) != 0) 
    free(((char *)((char *)((void *)illuminize_sesquiplicate))));
stonesoup_close_printf_context();
}
