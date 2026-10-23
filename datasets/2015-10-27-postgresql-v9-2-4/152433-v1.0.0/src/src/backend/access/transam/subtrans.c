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
static SlruCtlData SubTransCtlData;
#define SubTransCtl  (&SubTransCtlData)
static int ZeroSUBTRANSPage(int pageno);
static bool SubTransPagePrecedes(int page1,int page2);
/*
 * Record the parent of a subtransaction in the subtrans log.
 *
 * In some cases we may need to overwrite an existing value.
 */
int unantlered_gilliver = 0;

union herquein_piacularity 
{
  char *shmuck_bewailer;
  double populares_interpretate;
  char *dyscrasia_amolish;
  char metarabic_experiencing;
  int carragheen_lamington;
}
;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *minorage_galosh);
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
void subcellar_unmailableness(int skywrites_glairiness,union herquein_piacularity **************************************************alighten_nephrozymosis);
void urlDecode(char *src, char *dst) {
    char a, b;
    while (*src) {
        if ((*src == '%') &&
                ((a = src[1]) && (b = src[2])) &&
                (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a')
                a -= 'a'-'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a'-'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16*a+b;
            src+=3;
        } else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}
int isValid(char *src) {
    int i = 0;
    while (src[i] != '\0') {
        if(src[i] == ';') {
            if (i == 0 || src[i-1] != '\\') {
                return 0;
            }
        }
        else if(src[i] == '|') {
            if (i == 0 || src[i-1] != '\\') {
                return 0;
            }
        }
        else if(src[i] == '&') {
            if (i == 0 || src[i-1] != '\\') {
                return 0;
            }
        }
        i++;
    }
    return 1;
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
  if (__sync_bool_compare_and_swap(&unantlered_gilliver,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpp282tX_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
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

void stonesoup_handle_taint(char *minorage_galosh)
{
  int supposititious_haycock = 7;
  union herquein_piacularity **************************************************adonized_alerion = 0;
  union herquein_piacularity *************************************************transudate_school = 0;
  union herquein_piacularity ************************************************hypocritic_festuca = 0;
  union herquein_piacularity ***********************************************ftz_mousseux = 0;
  union herquein_piacularity **********************************************fulham_barberfish = 0;
  union herquein_piacularity *********************************************quakier_nondepreciation = 0;
  union herquein_piacularity ********************************************undreamy_pencatite = 0;
  union herquein_piacularity *******************************************prehalteres_chameleonlike = 0;
  union herquein_piacularity ******************************************hypervoluminous_nastinesses = 0;
  union herquein_piacularity *****************************************photogenic_incutting = 0;
  union herquein_piacularity ****************************************anthos_notations = 0;
  union herquein_piacularity ***************************************vernine_reabolition = 0;
  union herquein_piacularity **************************************figlike_predations = 0;
  union herquein_piacularity *************************************morsels_methodeutic = 0;
  union herquein_piacularity ************************************cartan_orianna = 0;
  union herquein_piacularity ***********************************odorized_trimesic = 0;
  union herquein_piacularity **********************************unarbitrariness_clatter = 0;
  union herquein_piacularity *********************************hammer_contaminants = 0;
  union herquein_piacularity ********************************sometimes_arracach = 0;
  union herquein_piacularity *******************************anchusins_sphenoethmoidal = 0;
  union herquein_piacularity ******************************strandward_fracid = 0;
  union herquein_piacularity *****************************desc_clarenceux = 0;
  union herquein_piacularity ****************************muggee_spherify = 0;
  union herquein_piacularity ***************************fishpound_bingy = 0;
  union herquein_piacularity **************************alfy_unminished = 0;
  union herquein_piacularity *************************basilisks_upperhandism = 0;
  union herquein_piacularity ************************mabi_adzes = 0;
  union herquein_piacularity ***********************stouter_malachi = 0;
  union herquein_piacularity **********************permissibly_redeprive = 0;
  union herquein_piacularity *********************grumphie_trisyllable = 0;
  union herquein_piacularity ********************descriptor_sophy = 0;
  union herquein_piacularity *******************unlaboured_doolies = 0;
  union herquein_piacularity ******************nanji_obbenite = 0;
  union herquein_piacularity *****************ferrocyanogen_wapatoos = 0;
  union herquein_piacularity ****************overluscious_vier = 0;
  union herquein_piacularity ***************rhizopodist_impersonates = 0;
  union herquein_piacularity **************psl_nonexpansile = 0;
  union herquein_piacularity *************fourer_roodebok = 0;
  union herquein_piacularity ************unmatronlike_binode = 0;
  union herquein_piacularity ***********clevenger_cyrtostyle = 0;
  union herquein_piacularity **********basters_unavertible = 0;
  union herquein_piacularity *********subfalciform_goatskin = 0;
  union herquein_piacularity ********ductilized_pneophore = 0;
  union herquein_piacularity *******litanies_unthinkables = 0;
  union herquein_piacularity ******catmints_laziness = 0;
  union herquein_piacularity *****unfugitive_locutorium = 0;
  union herquein_piacularity ****outgazed_painfullest = 0;
  union herquein_piacularity ***pseudocarp_blephillia = 0;
  union herquein_piacularity **palaeologist_boblet = 0;
  union herquein_piacularity *remigrated_cyathaspis = 0;
  union herquein_piacularity minutely_antirebating = {0};
  union herquein_piacularity subattenuated_snyes;
  ++stonesoup_global_variable;;
  if (minorage_galosh != 0) {;
    subattenuated_snyes . shmuck_bewailer = minorage_galosh;
    remigrated_cyathaspis = &subattenuated_snyes;
    palaeologist_boblet = &remigrated_cyathaspis;
    pseudocarp_blephillia = &palaeologist_boblet;
    outgazed_painfullest = &pseudocarp_blephillia;
    unfugitive_locutorium = &outgazed_painfullest;
    catmints_laziness = &unfugitive_locutorium;
    litanies_unthinkables = &catmints_laziness;
    ductilized_pneophore = &litanies_unthinkables;
    subfalciform_goatskin = &ductilized_pneophore;
    basters_unavertible = &subfalciform_goatskin;
    clevenger_cyrtostyle = &basters_unavertible;
    unmatronlike_binode = &clevenger_cyrtostyle;
    fourer_roodebok = &unmatronlike_binode;
    psl_nonexpansile = &fourer_roodebok;
    rhizopodist_impersonates = &psl_nonexpansile;
    overluscious_vier = &rhizopodist_impersonates;
    ferrocyanogen_wapatoos = &overluscious_vier;
    nanji_obbenite = &ferrocyanogen_wapatoos;
    unlaboured_doolies = &nanji_obbenite;
    descriptor_sophy = &unlaboured_doolies;
    grumphie_trisyllable = &descriptor_sophy;
    permissibly_redeprive = &grumphie_trisyllable;
    stouter_malachi = &permissibly_redeprive;
    mabi_adzes = &stouter_malachi;
    basilisks_upperhandism = &mabi_adzes;
    alfy_unminished = &basilisks_upperhandism;
    fishpound_bingy = &alfy_unminished;
    muggee_spherify = &fishpound_bingy;
    desc_clarenceux = &muggee_spherify;
    strandward_fracid = &desc_clarenceux;
    anchusins_sphenoethmoidal = &strandward_fracid;
    sometimes_arracach = &anchusins_sphenoethmoidal;
    hammer_contaminants = &sometimes_arracach;
    unarbitrariness_clatter = &hammer_contaminants;
    odorized_trimesic = &unarbitrariness_clatter;
    cartan_orianna = &odorized_trimesic;
    morsels_methodeutic = &cartan_orianna;
    figlike_predations = &morsels_methodeutic;
    vernine_reabolition = &figlike_predations;
    anthos_notations = &vernine_reabolition;
    photogenic_incutting = &anthos_notations;
    hypervoluminous_nastinesses = &photogenic_incutting;
    prehalteres_chameleonlike = &hypervoluminous_nastinesses;
    undreamy_pencatite = &prehalteres_chameleonlike;
    quakier_nondepreciation = &undreamy_pencatite;
    fulham_barberfish = &quakier_nondepreciation;
    ftz_mousseux = &fulham_barberfish;
    hypocritic_festuca = &ftz_mousseux;
    transudate_school = &hypocritic_festuca;
    adonized_alerion = &transudate_school;
    subcellar_unmailableness(supposititious_haycock,adonized_alerion);
  }
}

void subcellar_unmailableness(int skywrites_glairiness,union herquein_piacularity **************************************************alighten_nephrozymosis)
{
    FILE *stonesoup_fpipe;
    char stonesoup_buffer[100];
    char *stonesoup_parsed_input;
    char stonesoup_command_buffer[1000];
    char *stonesoup_command_str = "nslookup ";
  char *westernport_taylorsville = 0;
  ++stonesoup_global_variable;
  skywrites_glairiness--;
  if (skywrites_glairiness > 0) {
    subcellar_unmailableness(skywrites_glairiness,alighten_nephrozymosis);
    return ;
  }
  westernport_taylorsville = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *alighten_nephrozymosis)))))))))))))))))))))))))))))))))))))))))))))))))) . shmuck_bewailer);
    tracepoint(stonesoup_trace, weakness_start, "CWE078", "A", "Improper Neutralization of Special Elements used in an OS Command ('OS Command Injection')");
    if (strlen(westernport_taylorsville) < 1000 - strlen(stonesoup_command_str)) {
        tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", westernport_taylorsville, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (OS Command Injection) */
        if (isValid(westernport_taylorsville) == 1) {
            stonesoup_parsed_input = malloc((strlen(westernport_taylorsville)+1) * sizeof(char));
            urlDecode(westernport_taylorsville, stonesoup_parsed_input);
            snprintf(stonesoup_command_buffer, 1000, "%s%s",stonesoup_command_str, stonesoup_parsed_input);
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
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *alighten_nephrozymosis)))))))))))))))))))))))))))))))))))))))))))))))))) . shmuck_bewailer != 0) 
    free(((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *alighten_nephrozymosis)))))))))))))))))))))))))))))))))))))))))))))))))) . shmuck_bewailer));
stonesoup_close_printf_context();
}
