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
int metensarcosis_arboreous = 0;
int stonesoup_global_variable;

struct anemochorous_husbandries 
{
  char *mogerly_internetted;
  double sunspots_oversteadiness;
  char *violle_mushmelon;
  char filipinization_retied;
  int metusia_prelatry;
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
void surlier_squatinidae(struct anemochorous_husbandries **pennatulidae_veray);
void zambezian_swashbucklerdom(struct anemochorous_husbandries **uncollectibly_citternhead);
void pyjamas_patrols(struct anemochorous_husbandries **anginous_servers);
void phersephatta_korfonta(struct anemochorous_husbandries **bylander_choremen);
void sybille_binnacle(struct anemochorous_husbandries **antoine_talocalcanean);
void luged_byres(struct anemochorous_husbandries **jackeroos_kwantung);
void mediumship_taplin(struct anemochorous_husbandries **capablest_mol);
void scavenge_jetsom(struct anemochorous_husbandries **kampsville_gorz);
void nudged_dishellenize(struct anemochorous_husbandries **outguessing_oversoak);
void fennici_pantascope(struct anemochorous_husbandries **underturnkey_redelegating);

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
  struct anemochorous_husbandries **hypocrinism_peever = 0;
  struct anemochorous_husbandries *tunisian_decrepitude = 0;
  struct anemochorous_husbandries tonicobalsamic_darr = {0};
  struct anemochorous_husbandries deseret_spoilsports;
  int occlusor_pebrine = 45;
  char *sailable_refiling;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&metensarcosis_arboreous,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpeymOGg_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&sailable_refiling,"8325",occlusor_pebrine);
      if (sailable_refiling != 0) {;
        deseret_spoilsports . mogerly_internetted = ((char *)sailable_refiling);
        tunisian_decrepitude = &deseret_spoilsports;
        hypocrinism_peever = &tunisian_decrepitude;
        surlier_squatinidae(hypocrinism_peever);
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

void surlier_squatinidae(struct anemochorous_husbandries **pennatulidae_veray)
{
  ++stonesoup_global_variable;;
  zambezian_swashbucklerdom(pennatulidae_veray);
}

void zambezian_swashbucklerdom(struct anemochorous_husbandries **uncollectibly_citternhead)
{
  ++stonesoup_global_variable;;
  pyjamas_patrols(uncollectibly_citternhead);
}

void pyjamas_patrols(struct anemochorous_husbandries **anginous_servers)
{
  ++stonesoup_global_variable;;
  phersephatta_korfonta(anginous_servers);
}

void phersephatta_korfonta(struct anemochorous_husbandries **bylander_choremen)
{
  ++stonesoup_global_variable;;
  sybille_binnacle(bylander_choremen);
}

void sybille_binnacle(struct anemochorous_husbandries **antoine_talocalcanean)
{
  ++stonesoup_global_variable;;
  luged_byres(antoine_talocalcanean);
}

void luged_byres(struct anemochorous_husbandries **jackeroos_kwantung)
{
  ++stonesoup_global_variable;;
  mediumship_taplin(jackeroos_kwantung);
}

void mediumship_taplin(struct anemochorous_husbandries **capablest_mol)
{
  ++stonesoup_global_variable;;
  scavenge_jetsom(capablest_mol);
}

void scavenge_jetsom(struct anemochorous_husbandries **kampsville_gorz)
{
  ++stonesoup_global_variable;;
  nudged_dishellenize(kampsville_gorz);
}

void nudged_dishellenize(struct anemochorous_husbandries **outguessing_oversoak)
{
  ++stonesoup_global_variable;;
  fennici_pantascope(outguessing_oversoak);
}

void fennici_pantascope(struct anemochorous_husbandries **underturnkey_redelegating)
{
  FILE *stonesoup_csv = 0;
  FILE *stonesoup_temp = 0;
  char stonesoup_col1[80] = {0};
  char stonesoup_col2[80] = {0};
  char stonesoup_col3[80] = {0};
  char *stonesoup_cols[3] = {0};
  char *sweetens_espinette = 0;
  ++stonesoup_global_variable;;
  sweetens_espinette = ((char *)( *( *underturnkey_redelegating)) . mogerly_internetted);
  tracepoint(stonesoup_trace, weakness_start, "CWE476", "A", "NULL Pointer Dereference");
  tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", sweetens_espinette, "INITIAL-STATE");
  stonesoup_csv = fopen(sweetens_espinette,"r");
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
  if (( *( *underturnkey_redelegating)) . mogerly_internetted != 0) 
    free(((char *)( *( *underturnkey_redelegating)) . mogerly_internetted));
stonesoup_close_printf_context();
}
