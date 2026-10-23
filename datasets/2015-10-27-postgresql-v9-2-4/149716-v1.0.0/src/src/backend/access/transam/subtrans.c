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
int lockwood_uptight = 0;
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
void semiorbicularis_archprimate(char **transylvanian_palpebral);
void styliform_lighterage(char **advancement_hexose);
void metayer_unpawn(char **fredericton_bedur);
void preliterary_khubber(char **terry_overelegance);
void clangoured_schizophasia(char **dellies_overnormalized);
void readding_salience(char **acroteric_sasebo);
void tantaluses_dodecaphonism(char **corniculate_testingly);
void concertstuck_assholes(char **agacante_smeddum);
void scolopendrium_byssaceous(char **conrey_lobelines);
void klaipeda_panomphic(char **siphonaria_standage);
void precapture_rappee(char **beady_impatiens);
void hasa_afflictionless(char **casklike_positivism);
void fingerbreadth_pungled(char **wsd_sinistrally);
void talcums_losing(char **erl_unreaped);
void mouthful_impartible(char **anallantoidean_characids);
void mimbars_silber(char **matrilateral_sny);
void tobaccomen_brutage(char **megbote_pilies);
void humourize_kirks(char **archbishopry_monophagism);
void sphygmoid_spayed(char **stealthiest_recompetitor);
void undecennial_significancies(char **doctus_saitic);
void kelwen_dewittville(char **incomparably_humanitarian);
void dasypod_superinduced(char **vocate_unrequiter);
void rigged_bradyseismism(char **hierogamy_retinerved);
void dicasteries_idest(char **famiglietti_bullnose);
void metisses_idiocratically(char **ausable_unleashes);
void divestitive_gowon(char **outdwelt_pseudonymity);
void mandrake_hagiolith(char **beggarwoman_spectral);
void matriculating_lipides(char **cents_icarus);
void moonraker_souldie(char **collossians_neverthelater);
void cappiest_crambo(char **unresting_flats);
void impractically_duplicando(char **enchodontid_bushwa);
void scaffolder_defension(char **megalohepatia_premaking);
void ensilist_soothsayer(char **enantioses_bacchanalianism);
void toxiphagus_hybridae(char **barrytown_purpuras);
void tutorship_pyropes(char **cambodia_dupont);
void rhexia_judicia(char **confidingness_hibernize);
void protocatechuic_edom(char **pithecology_coreveller);
void granch_prognosticate(char **embargoing_vulgarians);
void citable_ungorgeous(char **pretrims_ernesto);
void packtong_pararosolic(char **escapes_undiffusiveness);
void cynthius_upheal(char **upheld_precommitting);
void squillian_undivorceable(char **citrines_scrobiculus);
void chuckies_digitalization(char **postarthritic_hydraulis);
void camoodie_porphyropsin(char **inapprehensibly_tipplers);
void preforgave_quinotannic(char **petronilla_vereeniging);
void control_pulicine(char **mullenize_talwood);
void nonbelligerent_stagewise(char **isiac_fracture);
void psid_colourfast(char **rupturing_waymaker);
void psychometrical_twitt(char **hagiolater_hayfield);
void epidia_sacramentum(char **unguenta_homotaxial);
void *my_malloc(unsigned int size)
{
  if (size > 512)
/* STONESOUP: CROSSOVER-POINT */
    return 0;
  return malloc(size);
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
  char **kilk_gink = 0;
  char *planned_semileafless[72] = {0};
  char *saratoga_holbein;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&lockwood_uptight,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp6k6OAk_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&saratoga_holbein,"HELOTES_WUDGE");
      if (saratoga_holbein != 0) {;
        planned_semileafless[13] = saratoga_holbein;
        kilk_gink = planned_semileafless;
        semiorbicularis_archprimate(kilk_gink);
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

void semiorbicularis_archprimate(char **transylvanian_palpebral)
{
  ++stonesoup_global_variable;;
  styliform_lighterage(transylvanian_palpebral);
}

void styliform_lighterage(char **advancement_hexose)
{
  ++stonesoup_global_variable;;
  metayer_unpawn(advancement_hexose);
}

void metayer_unpawn(char **fredericton_bedur)
{
  ++stonesoup_global_variable;;
  preliterary_khubber(fredericton_bedur);
}

void preliterary_khubber(char **terry_overelegance)
{
  ++stonesoup_global_variable;;
  clangoured_schizophasia(terry_overelegance);
}

void clangoured_schizophasia(char **dellies_overnormalized)
{
  ++stonesoup_global_variable;;
  readding_salience(dellies_overnormalized);
}

void readding_salience(char **acroteric_sasebo)
{
  ++stonesoup_global_variable;;
  tantaluses_dodecaphonism(acroteric_sasebo);
}

void tantaluses_dodecaphonism(char **corniculate_testingly)
{
  ++stonesoup_global_variable;;
  concertstuck_assholes(corniculate_testingly);
}

void concertstuck_assholes(char **agacante_smeddum)
{
  ++stonesoup_global_variable;;
  scolopendrium_byssaceous(agacante_smeddum);
}

void scolopendrium_byssaceous(char **conrey_lobelines)
{
  ++stonesoup_global_variable;;
  klaipeda_panomphic(conrey_lobelines);
}

void klaipeda_panomphic(char **siphonaria_standage)
{
  ++stonesoup_global_variable;;
  precapture_rappee(siphonaria_standage);
}

void precapture_rappee(char **beady_impatiens)
{
  ++stonesoup_global_variable;;
  hasa_afflictionless(beady_impatiens);
}

void hasa_afflictionless(char **casklike_positivism)
{
  ++stonesoup_global_variable;;
  fingerbreadth_pungled(casklike_positivism);
}

void fingerbreadth_pungled(char **wsd_sinistrally)
{
  ++stonesoup_global_variable;;
  talcums_losing(wsd_sinistrally);
}

void talcums_losing(char **erl_unreaped)
{
  ++stonesoup_global_variable;;
  mouthful_impartible(erl_unreaped);
}

void mouthful_impartible(char **anallantoidean_characids)
{
  ++stonesoup_global_variable;;
  mimbars_silber(anallantoidean_characids);
}

void mimbars_silber(char **matrilateral_sny)
{
  ++stonesoup_global_variable;;
  tobaccomen_brutage(matrilateral_sny);
}

void tobaccomen_brutage(char **megbote_pilies)
{
  ++stonesoup_global_variable;;
  humourize_kirks(megbote_pilies);
}

void humourize_kirks(char **archbishopry_monophagism)
{
  ++stonesoup_global_variable;;
  sphygmoid_spayed(archbishopry_monophagism);
}

void sphygmoid_spayed(char **stealthiest_recompetitor)
{
  ++stonesoup_global_variable;;
  undecennial_significancies(stealthiest_recompetitor);
}

void undecennial_significancies(char **doctus_saitic)
{
  ++stonesoup_global_variable;;
  kelwen_dewittville(doctus_saitic);
}

void kelwen_dewittville(char **incomparably_humanitarian)
{
  ++stonesoup_global_variable;;
  dasypod_superinduced(incomparably_humanitarian);
}

void dasypod_superinduced(char **vocate_unrequiter)
{
  ++stonesoup_global_variable;;
  rigged_bradyseismism(vocate_unrequiter);
}

void rigged_bradyseismism(char **hierogamy_retinerved)
{
  ++stonesoup_global_variable;;
  dicasteries_idest(hierogamy_retinerved);
}

void dicasteries_idest(char **famiglietti_bullnose)
{
  ++stonesoup_global_variable;;
  metisses_idiocratically(famiglietti_bullnose);
}

void metisses_idiocratically(char **ausable_unleashes)
{
  ++stonesoup_global_variable;;
  divestitive_gowon(ausable_unleashes);
}

void divestitive_gowon(char **outdwelt_pseudonymity)
{
  ++stonesoup_global_variable;;
  mandrake_hagiolith(outdwelt_pseudonymity);
}

void mandrake_hagiolith(char **beggarwoman_spectral)
{
  ++stonesoup_global_variable;;
  matriculating_lipides(beggarwoman_spectral);
}

void matriculating_lipides(char **cents_icarus)
{
  ++stonesoup_global_variable;;
  moonraker_souldie(cents_icarus);
}

void moonraker_souldie(char **collossians_neverthelater)
{
  ++stonesoup_global_variable;;
  cappiest_crambo(collossians_neverthelater);
}

void cappiest_crambo(char **unresting_flats)
{
  ++stonesoup_global_variable;;
  impractically_duplicando(unresting_flats);
}

void impractically_duplicando(char **enchodontid_bushwa)
{
  ++stonesoup_global_variable;;
  scaffolder_defension(enchodontid_bushwa);
}

void scaffolder_defension(char **megalohepatia_premaking)
{
  ++stonesoup_global_variable;;
  ensilist_soothsayer(megalohepatia_premaking);
}

void ensilist_soothsayer(char **enantioses_bacchanalianism)
{
  ++stonesoup_global_variable;;
  toxiphagus_hybridae(enantioses_bacchanalianism);
}

void toxiphagus_hybridae(char **barrytown_purpuras)
{
  ++stonesoup_global_variable;;
  tutorship_pyropes(barrytown_purpuras);
}

void tutorship_pyropes(char **cambodia_dupont)
{
  ++stonesoup_global_variable;;
  rhexia_judicia(cambodia_dupont);
}

void rhexia_judicia(char **confidingness_hibernize)
{
  ++stonesoup_global_variable;;
  protocatechuic_edom(confidingness_hibernize);
}

void protocatechuic_edom(char **pithecology_coreveller)
{
  ++stonesoup_global_variable;;
  granch_prognosticate(pithecology_coreveller);
}

void granch_prognosticate(char **embargoing_vulgarians)
{
  ++stonesoup_global_variable;;
  citable_ungorgeous(embargoing_vulgarians);
}

void citable_ungorgeous(char **pretrims_ernesto)
{
  ++stonesoup_global_variable;;
  packtong_pararosolic(pretrims_ernesto);
}

void packtong_pararosolic(char **escapes_undiffusiveness)
{
  ++stonesoup_global_variable;;
  cynthius_upheal(escapes_undiffusiveness);
}

void cynthius_upheal(char **upheld_precommitting)
{
  ++stonesoup_global_variable;;
  squillian_undivorceable(upheld_precommitting);
}

void squillian_undivorceable(char **citrines_scrobiculus)
{
  ++stonesoup_global_variable;;
  chuckies_digitalization(citrines_scrobiculus);
}

void chuckies_digitalization(char **postarthritic_hydraulis)
{
  ++stonesoup_global_variable;;
  camoodie_porphyropsin(postarthritic_hydraulis);
}

void camoodie_porphyropsin(char **inapprehensibly_tipplers)
{
  ++stonesoup_global_variable;;
  preforgave_quinotannic(inapprehensibly_tipplers);
}

void preforgave_quinotannic(char **petronilla_vereeniging)
{
  ++stonesoup_global_variable;;
  control_pulicine(petronilla_vereeniging);
}

void control_pulicine(char **mullenize_talwood)
{
  ++stonesoup_global_variable;;
  nonbelligerent_stagewise(mullenize_talwood);
}

void nonbelligerent_stagewise(char **isiac_fracture)
{
  ++stonesoup_global_variable;;
  psid_colourfast(isiac_fracture);
}

void psid_colourfast(char **rupturing_waymaker)
{
  ++stonesoup_global_variable;;
  psychometrical_twitt(rupturing_waymaker);
}

void psychometrical_twitt(char **hagiolater_hayfield)
{
  ++stonesoup_global_variable;;
  epidia_sacramentum(hagiolater_hayfield);
}

void epidia_sacramentum(char **unguenta_homotaxial)
{
  unsigned int stonesoup_size_buffer;
  int stonesoup_buffer_value;
  char *stonesoup_malloc_buffer = 0;
  char *untastily_fisheater = 0;
  ++stonesoup_global_variable;;
  untastily_fisheater = ((char *)unguenta_homotaxial[13]);
      tracepoint(stonesoup_trace, weakness_start, "CWE476", "F", "NULL Pointer Dereference");
      stonesoup_buffer_value = atoi(untastily_fisheater);
      tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buffer_value", stonesoup_buffer_value, &stonesoup_buffer_value, "INITIAL-STATE");
      if (stonesoup_buffer_value < 0)
        stonesoup_buffer_value = 0;
      stonesoup_size_buffer = ((unsigned int )stonesoup_buffer_value);
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
      stonesoup_malloc_buffer = my_malloc(stonesoup_size_buffer);
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
      tracepoint(stonesoup_trace, variable_address, "stonesoup_malloc_buffer", stonesoup_malloc_buffer, "TRIGGER-STATE");
/* STONESOUP: TRIGGER-POINT (Null Pointer Dereference: Wrapped malloc) */
      memset(stonesoup_malloc_buffer,0,stonesoup_size_buffer);
      stonesoup_printf("Buffer size is %d\n", stonesoup_size_buffer);
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
      if (stonesoup_malloc_buffer != 0) {
        free(stonesoup_malloc_buffer);
      }
      tracepoint(stonesoup_trace, weakness_end);
;
  if (unguenta_homotaxial[13] != 0) 
    free(((char *)unguenta_homotaxial[13]));
stonesoup_close_printf_context();
}
