/*-------------------------------------------------------------------------
 *
 * subtrans.c
 *		PostgreSQL subtransaction-log manager
 *
 * The pg_subtrans manager is a pg_clog-like manager that stores the parent
 * transaction Id for each transaction.  It is a fundamental part of the
 * nested transactions implementation.	A main transaction has a parent
 * of InvalidTransactionId, and each subtransaction has its immediate parent.
 * The tree can easily be walked from child to parent, but not in the
 * opposite direction.
 *
 * This code is based on clog.c, but the robustness requirements
 * are completely different from pg_clog, because we only need to remember
 * pg_subtrans information for currently-open transactions.  Thus, there is
 * no need to preserve data over a crash and restart.
 *
 * There are no XLOG interactions since we do not care about preserving
 * data across crashes.  During database startup, we simply force the
 * currently-active page of SUBTRANS to zeroes.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/backend/access/transam/subtrans.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/slru.h"
#include "access/subtrans.h"
#include "access/transam.h"
#include "pg_trace.h"
#include "utils/snapmgr.h"
/*
 * Defines for SubTrans page sizes.  A page is the same BLCKSZ as is used
 * everywhere else in Postgres.
 *
 * Note: because TransactionIds are 32 bits and wrap around at 0xFFFFFFFF,
 * SubTrans page numbering also wraps around at
 * 0xFFFFFFFF/SUBTRANS_XACTS_PER_PAGE, and segment numbering at
 * 0xFFFFFFFF/SUBTRANS_XACTS_PER_PAGE/SLRU_SEGMENTS_PER_PAGE.  We need take no
 * explicit notice of that fact in this module, except when comparing segment
 * and page numbers in TruncateSUBTRANS (see SubTransPagePrecedes).
 */
/* We need four bytes per xact */
#define SUBTRANS_XACTS_PER_PAGE (BLCKSZ / sizeof(TransactionId))
#define TransactionIdToPage(xid) ((xid) / (TransactionId) SUBTRANS_XACTS_PER_PAGE)
#define TransactionIdToEntry(xid) ((xid) % (TransactionId) SUBTRANS_XACTS_PER_PAGE)
/*
 * Link to shared-memory data structures for SUBTRANS control
 */
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
#include <unistd.h> 
static SlruCtlData SubTransCtlData;
#define SubTransCtl  (&SubTransCtlData)
static int ZeroSUBTRANSPage(int pageno);
static bool SubTransPagePrecedes(int page1,int page2);
/*
 * Record the parent of a subtransaction in the subtrans log.
 *
 * In some cases we may need to overwrite an existing value.
 */
int barest_kozlov = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *nonnovel_wifedoms);
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
void anatifer_overdesirous(char **const unadapt_nonpercipiency);
void underrespected_barberfish(int gussi_festae,char **rosetangle_emmenthal);
int stonesoup_comp (const void * a, const void * b)
{
    if (a > b) {
        return -1;
    }
    else if (a < b) {
        return 1;
    }
    else {
        return 0;
    }
}
int stonesoup_pmoc (const void * a, const void * b)
{
    return -1 * stonesoup_comp(a, b);
}
void stonesoup_readFile(char *filename) {
    FILE *fifo;
    char ch;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCn4CS6_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        tracepoint(stonesoup_trace, trace_point, "Reading from FIFO");
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Exiting readFile");
}
void waitForChange(char* file, char* sleepFile) {
    int fd;
    char filename[500] = {0};
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCn4CS6_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_waitForChange");
    stonesoup_printf("In waitForChange\n");
    strcat(filename, file);
    strcat(filename, ".pid");
    if ((fd = open(filename, O_CREAT|O_WRONLY, 0666)) == -1) {
        stonesoup_printf("Error opening file.");
    }
    else {
        if (write(fd, "q", sizeof(char)) == -1) {
            stonesoup_printf("Error writing to file.");
        }
        tracepoint(stonesoup_trace, trace_point, "Wrote .pid file");
        if (close(fd) == -1) {
            tracepoint(stonesoup_trace, trace_error, "Error closing file.");
            stonesoup_printf("Error closing file.");
        }
        stonesoup_readFile(sleepFile);
    }
}
int stonesoup_path_is_relative(char *path) {
    char *chr = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCn4CS6_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_path_is_relative");
    chr = strchr(path,'/');
    if (chr == 0) {
        tracepoint(stonesoup_trace, trace_point, "Path is relative");
        stonesoup_printf("Path is relative\n");
        return 1;
    } else {
        tracepoint(stonesoup_trace, trace_point, "Path is not relative");
        stonesoup_printf("Path is not relative\n");
        return 0;
    }
}
char * stonesoup_get_absolute_path(char * path) {
    char * abs_path = malloc (sizeof(char) * (strlen("/opt/stonesoup/workspace/testData/") * strlen(path) + 1));
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCn4CS6_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_get_absolute_path");
    if (abs_path == NULL) {
        stonesoup_printf("Cannot allocate memory for path\n");
    } else {
        stonesoup_printf("Creating absolute path\n");
        strcpy(abs_path, "/opt/stonesoup/workspace/testData/");
        strcat(abs_path, path);
    }
    return abs_path;
}
int stonesoup_isSymLink(char *file) {
    struct stat statbuf;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCn4CS6_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_isSymLink");
    if (lstat(file, &statbuf) < 0) { /* if error occured */
        stonesoup_printf("Error accessing path.\n");
        return 1; /* just end program */
    }
    if (S_ISLNK(statbuf.st_mode) == 1) {
        stonesoup_printf("Path is symlink.\n");
        return 1;
    }
    stonesoup_printf("Path is valid.\n");
    return 0;
}
int stonesoup_path_is_not_symlink(char * abs_path) {
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCn4CS6_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_path_is_not_symlink");
    return (stonesoup_isSymLink(abs_path) == 0);
}

void SubTransSetParent(TransactionId xid,TransactionId parent,bool overwriteOK)
{
  int pageno = (xid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  int entryno = (xid % ((TransactionId )(8192 / sizeof(TransactionId ))));
  int slotno;
  TransactionId *ptr;
  ;
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
  slotno = SimpleLruReadPage(&SubTransCtlData,pageno,((bool )1),xid);
  ptr = ((TransactionId *)(&SubTransCtlData) -> shared -> page_buffer[slotno]);
  ptr += entryno;
/* Current state should be 0 */
  ;
   *ptr = parent;
  (&SubTransCtlData) -> shared -> page_dirty[slotno] = ((bool )1);
  LWLockRelease(SubtransControlLock);
}
/*
 * Interrogate the parent of a transaction in the subtrans log.
 */

TransactionId SubTransGetParent(TransactionId xid)
{
  int pageno = (xid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  int entryno = (xid % ((TransactionId )(8192 / sizeof(TransactionId ))));
  int slotno;
  TransactionId *ptr;
  TransactionId parent;
/* Can't ask about stuff that might not be around anymore */
  ;
/* Bootstrap and frozen XIDs have no parent */
  if (!(xid >= ((TransactionId )3))) {
    return (TransactionId )0;
  }
/* lock is acquired by SimpleLruReadPage_ReadOnly */
  slotno = SimpleLruReadPage_ReadOnly(&SubTransCtlData,pageno,xid);
  ptr = ((TransactionId *)(&SubTransCtlData) -> shared -> page_buffer[slotno]);
  ptr += entryno;
  parent =  *ptr;
  LWLockRelease(SubtransControlLock);
  return parent;
}
/*
 * SubTransGetTopmostTransaction
 *
 * Returns the topmost transaction of the given transaction id.
 *
 * Because we cannot look back further than TransactionXmin, it is possible
 * that this function will lie and return an intermediate subtransaction ID
 * instead of the true topmost parent ID.  This is OK, because in practice
 * we only care about detecting whether the topmost parent is still running
 * or is part of a current snapshot's list of still-running transactions.
 * Therefore, any XID before TransactionXmin is as good as any other.
 */

TransactionId SubTransGetTopmostTransaction(TransactionId xid)
{
  TransactionId parentXid = xid;
  TransactionId previousXid = xid;
/* Can't ask about stuff that might not be around anymore */
  ;
  while(parentXid != ((TransactionId )0)){
    previousXid = parentXid;
    if (TransactionIdPrecedes(parentXid,TransactionXmin)) {
      break; 
    }
    parentXid = SubTransGetParent(parentXid);
  }
  ;
  return previousXid;
}
/*
 * Initialization of shared memory for SUBTRANS
 */

Size SUBTRANSShmemSize()
{
  return SimpleLruShmemSize(32,0);
}

void SUBTRANSShmemInit()
{
  (&SubTransCtlData) -> PagePrecedes = SubTransPagePrecedes;
  SimpleLruInit(&SubTransCtlData,"SUBTRANS Ctl",32,0,SubtransControlLock,"pg_subtrans");
/* Override default assumption that writes should be fsync'd */
  (&SubTransCtlData) -> do_fsync = ((bool )0);
}
/*
 * This func must be called ONCE on system install.  It creates
 * the initial SUBTRANS segment.  (The SUBTRANS directory is assumed to
 * have been created by the initdb shell script, and SUBTRANSShmemInit
 * must have been called already.)
 *
 * Note: it's not really necessary to create the initial segment now,
 * since slru.c would create it on first write anyway.	But we may as well
 * do it to be sure the directory is set up correctly.
 */

void BootStrapSUBTRANS()
{
  int slotno;
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
/* Create and zero the first page of the subtrans log */
  slotno = ZeroSUBTRANSPage(0);
/* Make sure it's written out */
  SimpleLruWritePage(&SubTransCtlData,slotno);
  ;
  LWLockRelease(SubtransControlLock);
}
/*
 * Initialize (or reinitialize) a page of SUBTRANS to zeroes.
 *
 * The page is not actually written, just set up in shared memory.
 * The slot number of the new page is returned.
 *
 * Control lock must be held at entry, and will be held at exit.
 */

static int ZeroSUBTRANSPage(int pageno)
{
  return SimpleLruZeroPage(&SubTransCtlData,pageno);
}
/*
 * This must be called ONCE during postmaster or standalone-backend startup,
 * after StartupXLOG has initialized ShmemVariableCache->nextXid.
 *
 * oldestActiveXID is the oldest XID of any prepared transaction, or nextXid
 * if there are none.
 */

void StartupSUBTRANS(TransactionId oldestActiveXID)
{
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&barest_kozlov,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpCn4CS6_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_read_taint();
    }
  }
/*
	 * Since we don't expect pg_subtrans to be valid across crashes, we
	 * initialize the currently-active page(s) to zeroes during startup.
	 * Whenever we advance into a new page, ExtendSUBTRANS will likewise zero
	 * the new page without regard to whatever was previously on disk.
	 */
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
  startPage = (oldestActiveXID / ((TransactionId )(8192 / sizeof(TransactionId ))));
  endPage = (ShmemVariableCache -> nextXid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  while(startPage != endPage){
    (void )(ZeroSUBTRANSPage(startPage));
    startPage++;
  }
  (void )(ZeroSUBTRANSPage(startPage));
  LWLockRelease(SubtransControlLock);
}
/*
 * This must be called ONCE during postmaster or standalone-backend shutdown
 */

void ShutdownSUBTRANS()
{
/*
	 * Flush dirty SUBTRANS pages to disk
	 *
	 * This is not actually necessary from a correctness point of view. We do
	 * it merely as a debugging aid.
	 */
  ;
  SimpleLruFlush(&SubTransCtlData,((bool )0));
  ;
}
/*
 * Perform a checkpoint --- either during shutdown, or on-the-fly
 */

void CheckPointSUBTRANS()
{
/*
	 * Flush dirty SUBTRANS pages to disk
	 *
	 * This is not actually necessary from a correctness point of view. We do
	 * it merely to improve the odds that writing of dirty pages is done by
	 * the checkpoint process and not by backends.
	 */
  ;
  SimpleLruFlush(&SubTransCtlData,((bool )1));
  ;
}
/*
 * Make sure that SUBTRANS has room for a newly-allocated XID.
 *
 * NB: this is called while holding XidGenLock.  We want it to be very fast
 * most of the time; even when it's not so fast, no actual I/O need happen
 * unless we're forced to write out a dirty subtrans page to make room
 * in shared memory.
 */

void ExtendSUBTRANS(TransactionId newestXact)
{
  int pageno;
/*
	 * No work except at first XID of a page.  But beware: just after
	 * wraparound, the first XID of page zero is FirstNormalTransactionId.
	 */
  if (newestXact % ((TransactionId )(8192 / sizeof(TransactionId ))) != 0 && !(newestXact == ((TransactionId )3))) {
    return ;
  }
  pageno = (newestXact / ((TransactionId )(8192 / sizeof(TransactionId ))));
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
/* Zero the page */
  ZeroSUBTRANSPage(pageno);
  LWLockRelease(SubtransControlLock);
}
/*
 * Remove all SUBTRANS segments before the one holding the passed transaction ID
 *
 * This is normally called during checkpoint, with oldestXact being the
 * oldest TransactionXmin of any running transaction.
 */

void TruncateSUBTRANS(TransactionId oldestXact)
{
  int cutoffPage;
/*
	 * The cutoff point is the start of the segment containing oldestXact. We
	 * pass the *page* containing oldestXact to SimpleLruTruncate.
	 */
  cutoffPage = (oldestXact / ((TransactionId )(8192 / sizeof(TransactionId ))));
  SimpleLruTruncate(&SubTransCtlData,cutoffPage);
}
/*
 * Decide which of two SUBTRANS page numbers is "older" for truncation purposes.
 *
 * We need to use comparison of TransactionIds here in order to do the right
 * thing with wraparound XID arithmetic.  However, if we are asked about
 * page number zero, we don't want to hand InvalidTransactionId to
 * TransactionIdPrecedes: it'll get weird about permanent xact IDs.  So,
 * offset both xids by FirstNormalTransactionId to avoid that.
 */

static bool SubTransPagePrecedes(int page1,int page2)
{
  TransactionId xid1;
  TransactionId xid2;
  xid1 = (((TransactionId )page1) * (8192 / sizeof(TransactionId )));
  xid1 += ((TransactionId )3);
  xid2 = (((TransactionId )page2) * (8192 / sizeof(TransactionId )));
  xid2 += ((TransactionId )3);
  return TransactionIdPrecedes(xid1,xid2);
}

void stonesoup_handle_taint(char *nonnovel_wifedoms)
{
  char *encroachments_joachim[50] = {0};
  ++stonesoup_global_variable;;
  if (nonnovel_wifedoms != 0) {;
    encroachments_joachim[11] = nonnovel_wifedoms;
    anatifer_overdesirous(encroachments_joachim);
  }
}

void anatifer_overdesirous(char **const unadapt_nonpercipiency)
{
  int redshire_tutorism = 7;
  ++stonesoup_global_variable;;
  underrespected_barberfish(redshire_tutorism,unadapt_nonpercipiency);
}

void underrespected_barberfish(int gussi_festae,char **rosetangle_emmenthal)
{
    int stonesoup_size = 0;
    FILE *stonesoup_file = 0;
    char *stonesoup_buffer = 0;
    char *stonesoup_str = 0;
    char *stonesoup_abs_path = 0;
    char *stonesoup_sleep_file = 0;
  char *beautied_subgrin = 0;
  ++stonesoup_global_variable;
  gussi_festae--;
  if (gussi_festae > 0) {
    underrespected_barberfish(gussi_festae,rosetangle_emmenthal);
    return ;
  }
  beautied_subgrin = ((char *)((char **)rosetangle_emmenthal)[11]);
    tracepoint(stonesoup_trace, weakness_start, "CWE363", "A", "Race Condition Enabling Link Following");
    stonesoup_str = malloc(sizeof(char) * (strlen(beautied_subgrin) + 1));
    stonesoup_sleep_file = malloc(sizeof(char) * (strlen(beautied_subgrin) + 1));
    if (stonesoup_str != NULL && stonesoup_sleep_file != NULL &&
        (sscanf(beautied_subgrin, "%s %s",
                stonesoup_sleep_file,
                stonesoup_str) == 2) &&
        (strlen(stonesoup_str) != 0) &&
        (strlen(stonesoup_sleep_file) != 0))
    {
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_sleep_file", stonesoup_sleep_file, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_str", stonesoup_str, "INITIAL-STATE");
        if (stonesoup_path_is_relative(stonesoup_str)) {
            stonesoup_abs_path = stonesoup_get_absolute_path(stonesoup_str);
            if (stonesoup_abs_path != NULL) {
                if (stonesoup_path_is_not_symlink(stonesoup_abs_path)) {
                    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
                    /* STONESOUP: CROSSOVER-POINT (race condition enabling link following) */
                    waitForChange(stonesoup_abs_path, stonesoup_sleep_file);
                    stonesoup_file = fopen(stonesoup_abs_path,"rb");
                    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
                    if (stonesoup_file != 0) {
                        fseek(stonesoup_file,0,2);
                        stonesoup_size = ftell(stonesoup_file);
                        rewind(stonesoup_file);
                        stonesoup_buffer = ((char *)(malloc(sizeof(char ) * (stonesoup_size + 1))));
                        if (stonesoup_buffer) {
                            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
                            /* STONESOUP: TRIGGER-POINT (race condition enabling link following) */
                            fread(stonesoup_buffer,sizeof(char ),stonesoup_size,stonesoup_file);
                            stonesoup_buffer[stonesoup_size] = '\0';
                            stonesoup_printf(stonesoup_buffer);
                            fclose(stonesoup_file);
                            free(stonesoup_buffer);
                            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                        }
                    }
                }
                free (stonesoup_abs_path);
            }
        }
        free(stonesoup_str);
    } else {
        tracepoint(stonesoup_trace, trace_point, "Error parsing input.");
        stonesoup_printf("Error parsing input.\n");
    }
;
  if (((char **)rosetangle_emmenthal)[11] != 0) 
    free(((char *)((char **)rosetangle_emmenthal)[11]));
stonesoup_close_printf_context();
}
