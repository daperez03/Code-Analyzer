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
int invariantively_eatton = 0;
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
  char stonesoup_buffer[80];
  FILE *stonesoup_pFile = 0;
  char *phenmiazine_sterilisability = 0;
  char ***************************************************unscioned_enfranch = 0;
  char **************************************************trottles_giss = 0;
  char *************************************************conspissate_gypped = 0;
  char ************************************************knowlesville_keratometer = 0;
  char ***********************************************profitless_oversweet = 0;
  char **********************************************mesoscapula_lymphoedema = 0;
  char *********************************************skew_dictyoceratina = 0;
  char ********************************************unsubjectedness_bradawls = 0;
  char *******************************************izy_cockies = 0;
  char ******************************************jugging_engin = 0;
  char *****************************************uncertainty_supersevere = 0;
  char ****************************************rhizopodous_nonfeasance = 0;
  char ***************************************stilton_poca = 0;
  char **************************************unbecomingly_didi = 0;
  char *************************************legitimisation_interacted = 0;
  char ************************************persius_pergamum = 0;
  char ***********************************medjidieh_itchproof = 0;
  char **********************************askapart_faradizing = 0;
  char *********************************comtian_gelee = 0;
  char ********************************stalk_burses = 0;
  char *******************************oreodontidae_animateness = 0;
  char ******************************uncrowns_centrifuge = 0;
  char *****************************poolrooms_deciduae = 0;
  char ****************************essayist_morin = 0;
  char ***************************grots_vespoidea = 0;
  char **************************equipedal_immatchless = 0;
  char *************************pressure_sympathism = 0;
  char ************************conglution_letterman = 0;
  char ***********************reconditions_djailolo = 0;
  char **********************gesneria_marxianism = 0;
  char *********************oxyanthracene_stabilizator = 0;
  char ********************joanne_tobaccomen = 0;
  char *******************confederative_carbuncle = 0;
  char ******************haplocaulescent_kefti = 0;
  char *****************photomurals_dumpty = 0;
  char ****************golliwog_blautok = 0;
  char ***************dongola_lawmen = 0;
  char **************diacritical_manubrium = 0;
  char *************maunderer_uninnateness = 0;
  char ************viny_dikaryophasic = 0;
  char ***********parmigianino_osmolal = 0;
  char **********bellybuttons_patients = 0;
  char *********placated_iterance = 0;
  char ********thymectomize_dygogram = 0;
  char *******jefes_eupnoea = 0;
  char ******staurolitic_nonevaluation = 0;
  char *****disrepairs_scrimshanker = 0;
  char ****tristfully_diametrally = 0;
  char ***ottillia_manifestoes = 0;
  char **secondhandedly_rayburn = 0;
  char *evocable_tia = 0;
  char *nonsuspensive_professionalist;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&invariantively_eatton,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpdylpa4_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      nonsuspensive_professionalist = getenv("EQUISETUMS_ARCHINFORMER");
      if (nonsuspensive_professionalist != 0) {;
        secondhandedly_rayburn = &nonsuspensive_professionalist;
        ottillia_manifestoes = &secondhandedly_rayburn;
        tristfully_diametrally = &ottillia_manifestoes;
        disrepairs_scrimshanker = &tristfully_diametrally;
        staurolitic_nonevaluation = &disrepairs_scrimshanker;
        jefes_eupnoea = &staurolitic_nonevaluation;
        thymectomize_dygogram = &jefes_eupnoea;
        placated_iterance = &thymectomize_dygogram;
        bellybuttons_patients = &placated_iterance;
        parmigianino_osmolal = &bellybuttons_patients;
        viny_dikaryophasic = &parmigianino_osmolal;
        maunderer_uninnateness = &viny_dikaryophasic;
        diacritical_manubrium = &maunderer_uninnateness;
        dongola_lawmen = &diacritical_manubrium;
        golliwog_blautok = &dongola_lawmen;
        photomurals_dumpty = &golliwog_blautok;
        haplocaulescent_kefti = &photomurals_dumpty;
        confederative_carbuncle = &haplocaulescent_kefti;
        joanne_tobaccomen = &confederative_carbuncle;
        oxyanthracene_stabilizator = &joanne_tobaccomen;
        gesneria_marxianism = &oxyanthracene_stabilizator;
        reconditions_djailolo = &gesneria_marxianism;
        conglution_letterman = &reconditions_djailolo;
        pressure_sympathism = &conglution_letterman;
        equipedal_immatchless = &pressure_sympathism;
        grots_vespoidea = &equipedal_immatchless;
        essayist_morin = &grots_vespoidea;
        poolrooms_deciduae = &essayist_morin;
        uncrowns_centrifuge = &poolrooms_deciduae;
        oreodontidae_animateness = &uncrowns_centrifuge;
        stalk_burses = &oreodontidae_animateness;
        comtian_gelee = &stalk_burses;
        askapart_faradizing = &comtian_gelee;
        medjidieh_itchproof = &askapart_faradizing;
        persius_pergamum = &medjidieh_itchproof;
        legitimisation_interacted = &persius_pergamum;
        unbecomingly_didi = &legitimisation_interacted;
        stilton_poca = &unbecomingly_didi;
        rhizopodous_nonfeasance = &stilton_poca;
        uncertainty_supersevere = &rhizopodous_nonfeasance;
        jugging_engin = &uncertainty_supersevere;
        izy_cockies = &jugging_engin;
        unsubjectedness_bradawls = &izy_cockies;
        skew_dictyoceratina = &unsubjectedness_bradawls;
        mesoscapula_lymphoedema = &skew_dictyoceratina;
        profitless_oversweet = &mesoscapula_lymphoedema;
        knowlesville_keratometer = &profitless_oversweet;
        conspissate_gypped = &knowlesville_keratometer;
        trottles_giss = &conspissate_gypped;
        unscioned_enfranch = &trottles_giss;
        if ( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *unscioned_enfranch))))))))))))))))))))))))))))))))))))))))))))))))) != 0) {
          goto silhouetted_smoldering;
        }
        ++stonesoup_global_variable;
        silhouetted_smoldering:;
        phenmiazine_sterilisability = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *unscioned_enfranch)))))))))))))))))))))))))))))))))))))))))))))))))));
      tracepoint(stonesoup_trace, weakness_start, "CWE476", "E", "NULL Pointer Dereference");
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT */
      stonesoup_pFile = fopen(phenmiazine_sterilisability,"r");
      stonesoup_buffer[0] = 0;
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
      tracepoint(stonesoup_trace, variable_address, "stonesoup_pFile", stonesoup_pFile, "TRIGGER-STATE");
/* STONESOUP: TRIGGER-POINT (Null Pointer Dereference: Unchecked file read) */
      fgets(stonesoup_buffer,79,stonesoup_pFile);
      stonesoup_printf(stonesoup_buffer);
      stonesoup_printf("\n");
      fclose(stonesoup_pFile);
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
      tracepoint(stonesoup_trace, weakness_end);
;
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
