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
#include <sys/stat.h> 
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <math.h> 
#include <pthread.h> 
#include <signal.h> 
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
int transnationally_molten = 0;
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
void proembryonic_minify(char *accompanimental_gecr);
struct stonesoup_data {
    int data_size;
    char *data;
    char *file1;
};
struct stonesoup_data *stonesoupData;
pthread_mutex_t stonesoup_mutex;
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp4nqCbV_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Finished reading sync file");
}
void waitForSig(char* sleepFile) {
    int fd;
    char outStr[25] = {0};
    char filename[500] = {0};
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp4nqCbV_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "waitForSig");
    stonesoup_printf("In waitForSig\n");
    sprintf(outStr, "%d.pid", getpid());
    strcat(filename, "/opt/stonesoup/workspace/testData/");
    strcat(filename, outStr);
    if ((fd = open(filename, O_CREAT|O_WRONLY, 0666)) == -1) {
        tracepoint(stonesoup_trace, trace_error, "Error opening file.");
        stonesoup_printf("Error opening file.");
    }
    else {
        if (write(fd, "q", sizeof(char)) == -1) {
            stonesoup_printf("Error writing to file.");
        }
        tracepoint(stonesoup_trace, trace_point, "Wrote .pid file.");
        if (close(fd) == -1) {
            stonesoup_printf("Error closing file.");
        }
        stonesoup_readFile(sleepFile);
    }
}
void delNonAlpha (void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*) data;
    int i = 0;
    int j = 0;
    char* temp = malloc(sizeof(char) * (stonesoupData->data_size + 1));
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp4nqCbV_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "delNonAlpha");
    stonesoup_printf("Grabbing lock\n");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    /* STONESOUP: TRIGGER-POINT (nonreentrant signal handler) */
    pthread_mutex_lock(&stonesoup_mutex); /* mutex lock causes deadlock on re-entrance */
    tracepoint(stonesoup_trace, trace_point, "mutex locked");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    while(stonesoupData->data[i] != '\0') {
        if((stonesoupData->data[i] >= 'A' && stonesoupData->data[i] <= 'Z') ||
           (stonesoupData->data[i] >= 'a' && stonesoupData->data[i] <= 'z')) {
            temp[j++] = stonesoupData->data[i];
        }
        i++;
    }
    temp[j++] = '\0';
    stonesoupData->data_size = j;
    free(stonesoupData->data);
    stonesoupData->data = temp;
    waitForSig(stonesoupData->file1); /* Deadlock */
    stonesoup_printf("Realeasing lock\n");
    pthread_mutex_unlock(&stonesoup_mutex);
    tracepoint(stonesoup_trace, trace_point, "mutex unlocked");
}
void sig_handler (int sig) {
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp4nqCbV_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "sig_handler");
    /* STONESOUP: CROSSOVER-POINT (nonreentrentsighandler) */
    if (stonesoupData != NULL) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        delNonAlpha(stonesoupData); /* call non-reentrant function - deadlock */
    }
    signal(SIGUSR1, SIG_IGN);
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
#define IUUS_FORDHAM(x) proembryonic_minify((char *) x)
/*
 * This must be called ONCE during postmaster or standalone-backend startup,
 * after StartupXLOG has initialized ShmemVariableCache->nextXid.
 *
 * oldestActiveXID is the oldest XID of any prepared transaction, or nextXid
 * if there are none.
 */

void StartupSUBTRANS(TransactionId oldestActiveXID)
{
  char *inofficiously_paramagnetism = 0;
  long tuque_hipper[10];
  char *serosa_coverlets[10] = {0};
  int rowty_nagle = 0;
  char *hyperacuteness_candyh = 0;
  char *trichophoric_scuttle;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&transnationally_molten,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp4nqCbV_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&trichophoric_scuttle,"ZEBEC_TYMPANOCERVICAL");
      if (trichophoric_scuttle != 0) {;
        rowty_nagle = ((int )(strlen(trichophoric_scuttle)));
        hyperacuteness_candyh = ((char *)(malloc(rowty_nagle + 1)));
        if (hyperacuteness_candyh == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(hyperacuteness_candyh,0,rowty_nagle + 1);
        memcpy(hyperacuteness_candyh,trichophoric_scuttle,rowty_nagle);
        if (trichophoric_scuttle != 0) 
          free(((char *)trichophoric_scuttle));
        serosa_coverlets[5] = hyperacuteness_candyh;
        tuque_hipper[1] = 5;
        inofficiously_paramagnetism =  *(serosa_coverlets + tuque_hipper[1]);
	IUUS_FORDHAM(inofficiously_paramagnetism);
      }
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

void proembryonic_minify(char *accompanimental_gecr)
{
  char *denounced_unpulverize = 0;
  ++stonesoup_global_variable;;
  denounced_unpulverize = ((char *)accompanimental_gecr);
    tracepoint(stonesoup_trace, weakness_start, "CWE479", "A", "Signal Handler Use of a Non-reentrant Function");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(denounced_unpulverize) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(denounced_unpulverize) + 1));
        if (stonesoupData->data && stonesoupData->file1) {
            if ((sscanf(denounced_unpulverize, "%s %s",
                        stonesoupData->file1,
                        stonesoupData->data) == 2) &&
                (strlen(stonesoupData->data) != 0) &&
                (strlen(stonesoupData->file1) != 0))
            {
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "INITIAL-STATE");
                stonesoupData->data_size = strlen(stonesoupData->data);
                if (pthread_mutex_init(&stonesoup_mutex, NULL) != 0) {
                    stonesoup_printf("Mutex failed to initilize.");
                 }
                if (signal(SIGUSR1, sig_handler) == SIG_ERR) { /* setup signal handler */
                    tracepoint(stonesoup_trace, trace_error, "Error setting up sig handler for SIGUSR1");
                    stonesoup_printf ("Error catching SIGUSR1!\n");
                }
                delNonAlpha(stonesoupData);
                signal(SIGUSR1, SIG_IGN); /* 'deregister' signal hander befor returning to base program */
            } else {
                tracepoint(stonesoup_trace, trace_error, "Error parsing data");
                stonesoup_printf("Error parsing data\n");
            }
            free(stonesoupData->data);
        }
        free(stonesoupData);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (accompanimental_gecr != 0) 
    free(((char *)accompanimental_gecr));
stonesoup_close_printf_context();
}
