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
#include <pthread.h> 
static SlruCtlData SubTransCtlData;
#define SubTransCtl  (&SubTransCtlData)
static int ZeroSUBTRANSPage(int pageno);
static bool SubTransPagePrecedes(int page1,int page2);
/*
 * Record the parent of a subtransaction in the subtrans log.
 *
 * In some cases we may need to overwrite an existing value.
 */
int cliffed_spinder = 0;
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
void cothy_quomodos(int wizes_rectorates,char ***********hurtleberry_belvedered);
struct stonesoup_list {
    int data;
    struct stonesoup_list *previous;
    struct stonesoup_list *next;
};
struct stonesoup_queue {
    pthread_mutex_t lock;
    pthread_cond_t is_empty;
    pthread_cond_t is_full;
    int size;
    int capacity;
    struct stonesoup_list *head;
    struct stonesoup_list *tail;
};
struct stonesoup_data {
    int qsize;
    int data;
    char* file1;
    char* file2;
};
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpkbPlH8_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpkbPlH8_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "Finished reading sync file.");
}
int enqueue(struct stonesoup_queue *ssQ, int data) {
    int rtnval = 0;
    if (ssQ != NULL) {
        struct stonesoup_list *elem = malloc(sizeof(struct stonesoup_list));
        pthread_mutex_lock(&(ssQ->lock));
        while (ssQ->size >= ssQ->capacity) {
            pthread_cond_wait(&(ssQ->is_full), &(ssQ->lock));
        }
        elem->next = NULL;
        elem->previous = ssQ->tail;
        elem->data = data;
        if (ssQ->tail != NULL) {
            ssQ->tail->next = elem;
        }
        ssQ->tail = elem;
        ssQ->size++;
        if (ssQ->head == NULL) {
            ssQ->head = elem;
        }
        pthread_mutex_unlock(&(ssQ->lock));
        pthread_cond_broadcast(&(ssQ->is_empty));
        }
    else {
        rtnval = -1;
    }
    return rtnval;
}
int dequeue(struct stonesoup_queue *ssQ) {
    int val = -1;
    if (ssQ != NULL) {
        struct stonesoup_list *elem;
        pthread_mutex_lock(&(ssQ->lock));
        while (ssQ->size <= 0) {
            pthread_cond_wait(&(ssQ->is_empty), &(ssQ->lock));
        }
        elem = ssQ->head;
        ssQ->head = elem->next;
        if(ssQ->head != NULL) {
            ssQ->head->previous = NULL;
        }
        else {
            ssQ->tail = NULL;
        }
        val = elem->data;
        ssQ->size--;
        free(elem);
        pthread_mutex_unlock(&(ssQ->lock));
        pthread_cond_broadcast(&(ssQ->is_full));
    }
    return val;
}
struct stonesoup_queue *get_instance (char* file2) {
    static struct stonesoup_queue *ssQ = NULL;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpkbPlH8_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "get_instance");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (singletonpatternwithoutsync) */
    if (ssQ == NULL) {
        if (file2 != NULL) {
            stonesoup_readFile(file2);
        }
        ssQ = (struct stonesoup_queue *)calloc(1, sizeof(struct stonesoup_queue));
        pthread_mutex_init(&(ssQ->lock), NULL);
        pthread_cond_init(&(ssQ->is_empty), NULL);
        pthread_cond_init(&(ssQ->is_full), NULL);
        ssQ->size = 0;
        ssQ->capacity = 30;
        ssQ->head = NULL;
        ssQ->tail = NULL;
    }
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, variable_address, "ssQ", ssQ, "CROSSOVER-STATE");
    return ssQ;
}
void *stonesoup_print_data (void *data) {
    struct stonesoup_data *ssD = (struct stonesoup_data *)data;
    struct stonesoup_queue *ssQ = get_instance(ssD->file2);
    int i;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpkbPlH8_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_print_data");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    /* STONESOUP: TRIGGER-POINT (singletonpatternwithoutsync) */
    while ((i = dequeue(ssQ)) != -1) {
        stonesoup_printf("Data: %d\n", i);
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    return NULL;
}
void *stonesoup_calc_data (void *data) {
    struct stonesoup_data *ssD = (struct stonesoup_data *)data;
    struct stonesoup_queue *ssQ;
    int *qsort_arr;
    int i;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpkbPlH8_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_calc_data");
    qsort_arr = malloc(sizeof(int)*ssD->qsize);
        if (qsort_arr != NULL) {
            for (i = 0; i < ssD->qsize; i++) {
                qsort_arr[i] = ssD->qsize - i;
            }
            qsort(qsort_arr, ssD->qsize, sizeof(int), &stonesoup_comp);
            free (qsort_arr);
            qsort_arr = NULL;
        }
    stonesoup_readFile(ssD->file1);
    ssQ = get_instance(NULL);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT2: BEFORE");
    for (i = 0; i < ssD->data; i++) {
        /* STONESOUP2: TRIGGER-POINT (singletonpatternwithoutsync) */
        if (enqueue(ssQ, i) == -1) {
            break;
        }
    }
    enqueue(ssQ, -1);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT2: AFTER");
    return NULL;
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
  int formylating_filchers = 7;
  char ***********calorie_myopically = 0;
  char **********secohmmeter_pantomimicry = 0;
  char *********syntonizer_phersephatta = 0;
  char ********frowzled_orangeman = 0;
  char *******ellington_christianizing = 0;
  char ******delectation_pegless = 0;
  char *****ecthymatous_stannid = 0;
  char ****cinemagoers_chrysatropic = 0;
  char ***synop_unteach = 0;
  char **defoliation_hellandite = 0;
  char *sirrah_seamrog = 0;
  int dascylus_zygobranchia = 0;
  char *overreaches_tergiversant = 0;
  char *pneumodynamic_alraun;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&cliffed_spinder,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpkbPlH8_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      pneumodynamic_alraun = getenv("ELUTRIATION_CLUBFELLOW");
      if (pneumodynamic_alraun != 0) {;
        dascylus_zygobranchia = ((int )(strlen(pneumodynamic_alraun)));
        overreaches_tergiversant = ((char *)(malloc(dascylus_zygobranchia + 1)));
        if (overreaches_tergiversant == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(overreaches_tergiversant,0,dascylus_zygobranchia + 1);
        memcpy(overreaches_tergiversant,pneumodynamic_alraun,dascylus_zygobranchia);
        defoliation_hellandite = &overreaches_tergiversant;
        synop_unteach = &defoliation_hellandite;
        cinemagoers_chrysatropic = &synop_unteach;
        ecthymatous_stannid = &cinemagoers_chrysatropic;
        delectation_pegless = &ecthymatous_stannid;
        ellington_christianizing = &delectation_pegless;
        frowzled_orangeman = &ellington_christianizing;
        syntonizer_phersephatta = &frowzled_orangeman;
        secohmmeter_pantomimicry = &syntonizer_phersephatta;
        calorie_myopically = &secohmmeter_pantomimicry;
        cothy_quomodos(formylating_filchers,calorie_myopically);
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

void cothy_quomodos(int wizes_rectorates,char ***********hurtleberry_belvedered)
{
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data* stonesoupData;
  char *gerhardus_geogony = 0;
  ++stonesoup_global_variable;
  wizes_rectorates--;
  if (wizes_rectorates > 0) {
    cothy_quomodos(wizes_rectorates,hurtleberry_belvedered);
    return ;
  }
  gerhardus_geogony = ((char *)( *( *( *( *( *( *( *( *( *( *hurtleberry_belvedered)))))))))));
    tracepoint(stonesoup_trace, weakness_start, "CWE543", "A", "Use of a Singleton Pattern Without Synchronization in a Multithreaded Context");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(gerhardus_geogony) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(gerhardus_geogony) + 1));
        if ((sscanf(gerhardus_geogony, "%d %s %s %d",
                  &(stonesoupData->qsize),
                    stonesoupData->file1,
                    stonesoupData->file2,
                  &(stonesoupData->data)) == 4) &&
                    stonesoupData->qsize >= 0 &&
                    stonesoupData->data >= 0 &&
            (strlen(stonesoupData->file1) != 0) &&
            (strlen(stonesoupData->file2) != 0))
        {
            tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->data", stonesoupData->data, &(stonesoupData->data), "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "INITIAL-STATE");
            tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "INITIAL-STATE");
            tracepoint(stonesoup_trace, trace_point, "Spawning threads.");
            if (pthread_create(&stonesoup_t0, NULL, stonesoup_calc_data, stonesoupData) != 0) {
                stonesoup_printf("Thread 0 failed to spawn.");
            }
            if (pthread_create(&stonesoup_t1, NULL, stonesoup_print_data, stonesoupData) != 0) {
                stonesoup_printf("Thread 1 failed to spawn.");
            }
            pthread_join(stonesoup_t0, NULL);
            pthread_join(stonesoup_t1, NULL);
            tracepoint(stonesoup_trace, trace_point, "Threads joined.");
        } else {
            tracepoint(stonesoup_trace, trace_error, "Error parsng data.");
            stonesoup_printf("Error parsing data\n");
        }
        free(stonesoupData->file1);
        free(stonesoupData->file2);
        free(stonesoupData);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if ( *( *( *( *( *( *( *( *( *( *hurtleberry_belvedered))))))))) != 0) 
    free(((char *)( *( *( *( *( *( *( *( *( *( *hurtleberry_belvedered))))))))))));
stonesoup_close_printf_context();
}
