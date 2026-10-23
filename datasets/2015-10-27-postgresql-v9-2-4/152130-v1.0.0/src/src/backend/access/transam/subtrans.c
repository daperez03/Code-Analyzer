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
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
static SlruCtlData SubTransCtlData;
#define SubTransCtl  (&SubTransCtlData)
static int ZeroSUBTRANSPage(int pageno);
static bool SubTransPagePrecedes(int page1,int page2);
/*
 * Record the parent of a subtransaction in the subtrans log.
 *
 * In some cases we may need to overwrite an existing value.
 */
int repletely_coaction = 0;
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
 unsigned int **stonesoup_buffer_array = 0;
    unsigned int stonesoup_i;
    unsigned int stonesoup_size;
    int stonesoup_num;
    unsigned int stonesoup_trace_counter = 0;
  char *vaudevillist_mrsr = 0;
  int jawn_squirrels;
  int muffy_observingly;
  char *(**********apishamore_goetic)[91] = 0;
  char *(*********yelped_hewitt)[91] = 0;
  char *(********reason_intestate)[91] = 0;
  char *(*******mispatch_lymphation)[91] = 0;
  char *(******federalising_preweigh)[91] = 0;
  char *(*****tidily_motors)[91] = 0;
  char *(****uncountermanded_magically)[91] = 0;
  char *(***specificity_salten)[91] = 0;
  char *(**carrotage_zealousnesses)[91] = 0;
  char *(*diapausing_pollaiuolo)[91] = 0;
  char **alcibiades_glimes = 0;
  char *chitkara_gearwheels[91] = {0};
  int papistical_jacobina = 7;
  char *fotched_sarangousty;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&repletely_coaction,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmps7qXqY_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&fotched_sarangousty,"2556",papistical_jacobina);
      if (fotched_sarangousty != 0) {;
        chitkara_gearwheels[43] = fotched_sarangousty;
        diapausing_pollaiuolo = &chitkara_gearwheels;
        carrotage_zealousnesses = &diapausing_pollaiuolo;
        specificity_salten = &carrotage_zealousnesses;
        uncountermanded_magically = &specificity_salten;
        tidily_motors = &uncountermanded_magically;
        federalising_preweigh = &tidily_motors;
        mispatch_lymphation = &federalising_preweigh;
        reason_intestate = &mispatch_lymphation;
        yelped_hewitt = &reason_intestate;
        apishamore_goetic = &yelped_hewitt;
        muffy_observingly = 5;
        while(1 == 1){
          muffy_observingly = muffy_observingly * 2;
          muffy_observingly = muffy_observingly + 2;
          if (muffy_observingly > 1000) {
            break; 
          }
        }
        jawn_squirrels = muffy_observingly;
        vaudevillist_mrsr = ((char *)( *( *( *( *( *( *( *( *( *( *apishamore_goetic))))))))))[43]);
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "A", "Uncontrolled Resource Consumption");
    stonesoup_num = atoi(vaudevillist_mrsr);
    tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_num", stonesoup_num, &stonesoup_num, "INITIAL-STATE");
    if (stonesoup_num > 0 && stonesoup_num < 4294967295U / sizeof(unsigned int **)) {
        stonesoup_size = 100000;
        stonesoup_buffer_array = ((unsigned int **)(malloc(stonesoup_num * sizeof(unsigned int *))));
        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_size", stonesoup_size, &stonesoup_size, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array", stonesoup_buffer_array, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i) {
            stonesoup_buffer_array[stonesoup_i] = 0;
            stonesoup_printf("evaluating input\n");
            while(stonesoup_buffer_array[stonesoup_i] == 0){
    /* STONESOUP: CROSSOVER-POINT (Resource Exhaustion) */
    /* STONESOUP: TRIGGER-POINT (Resource Exhaustion) */
    stonesoup_buffer_array[stonesoup_i] = ((unsigned int *)(malloc(stonesoup_size * sizeof(unsigned int ))));
                if (stonesoup_trace_counter % 1000 == 0) {
                    if (stonesoup_buffer_array[stonesoup_i] != 0) {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "CROSSOVER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "CROSSOVER-STATE");
                    } else {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "TRIGGER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "TRIGGER-STATE");
                    }
       }
       stonesoup_trace_counter ++;
   }
   if (stonesoup_buffer_array[stonesoup_i] != 0) {
    memset(stonesoup_buffer_array[stonesoup_i],0,stonesoup_size * sizeof(unsigned int ));
   }
   else
    break;
  }
 }
 tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
 tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
 tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
 stonesoup_printf("finished evaluating\n");
    for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i)
        if (stonesoup_buffer_array[stonesoup_i] != 0)
            free(stonesoup_buffer_array[stonesoup_i]);
    if (stonesoup_buffer_array != 0) {
        free(stonesoup_buffer_array);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (( *( *( *( *( *( *( *( *( *( *apishamore_goetic))))))))))[43] != 0) 
          free(((char *)( *( *( *( *( *( *( *( *( *( *apishamore_goetic))))))))))[43]));
stonesoup_close_printf_context();
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
