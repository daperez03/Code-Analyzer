/*-------------------------------------------------------------------------
 *
 * pmsignal.c
 *	  routines for signaling the postmaster from its child processes
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/storage/ipc/pmsignal.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include <signal.h>
#include <unistd.h>
#include "miscadmin.h"
#include "postmaster/postmaster.h"
#include "replication/walsender.h"
#include "storage/pmsignal.h"
#include "storage/shmem.h"
/*
 * The postmaster is signaled by its children by sending SIGUSR1.  The
 * specific reason is communicated via flags in shared memory.	We keep
 * a boolean flag for each possible "reason", so that different reasons
 * can be signaled by different backends at the same time.	(However,
 * if the same reason is signaled more than once simultaneously, the
 * postmaster will observe it only once.)
 *
 * The flags are actually declared as "volatile sig_atomic_t" for maximum
 * portability.  This should ensure that loads and stores of the flag
 * values are atomic, allowing us to dispense with any explicit locking.
 *
 * In addition to the per-reason flags, we store a set of per-child-process
 * flags that are currently used only for detecting whether a backend has
 * exited without performing proper shutdown.  The per-child-process flags
 * have three possible states: UNUSED, ASSIGNED, ACTIVE.  An UNUSED slot is
 * available for assignment.  An ASSIGNED slot is associated with a postmaster
 * child process, but either the process has not touched shared memory yet,
 * or it has successfully cleaned up after itself.	A ACTIVE slot means the
 * process is actively using shared memory.  The slots are assigned to
 * child processes at random, and postmaster.c is responsible for tracking
 * which one goes with which PID.
 *
 * Actually there is a fourth state, WALSENDER.  This is just like ACTIVE,
 * but carries the extra information that the child is a WAL sender.
 * WAL senders too start in ACTIVE state, but switch to WALSENDER once they
 * start streaming the WAL (and they never go back to ACTIVE after that).
 */
#define PM_CHILD_UNUSED		0	/* these values must fit in sig_atomic_t */
#define PM_CHILD_ASSIGNED	1
#define PM_CHILD_ACTIVE		2
#define PM_CHILD_WALSENDER	3
/* "typedef struct PMSignalData PMSignalData" appears in pmsignal.h */
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 

struct PMSignalData 
{
/* per-reason flags */
  sig_atomic_t PMSignalFlags[NUM_PMSIGNALS];
/* per-child-process flags */
/* # of entries in PMChildFlags[] */
  int num_child_flags;
/* next slot to try to assign */
  int next_child_flag;
/* VARIABLE LENGTH ARRAY */
  sig_atomic_t PMChildFlags[1];
}
;
static volatile PMSignalData *PMSignalState = ((void *)0);
/*
 * PMSignalShmemSize
 *		Compute space needed for pmsignal.c's shared memory
 */
int colinson_heartwoods = 0;
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
void biorhythm_perirectitis(void **casemates_kyak);
void earwort_simson(void **environmental_chavel);
void dreeda_marinna(void **concurrent_oppositions);
void industrialized_mokena(void **rationalization_alismales);
void pentode_lowermost(void **ungruff_covite);
void idenitifiers_presagement(void **tailband_emmets);
void battlement_corbeling(void **sectoral_multigranulated);
void newscasts_clinicist(void **powerstat_prededicated);
void paters_plasmagel(void **clerus_interregnal);
void ican_bagios(void **qintar_instructedly);
void pokeweeds_sebacic(void **hallsy_indophilism);
void dermatoplasm_obbligatos(void **blaire_saviour);
void ballades_puttees(void **crossett_hieromonach);
void octoploid_crank(void **paloverde_thermotaxis);
void hardstand_ancell(void **precentress_unmerciably);
void unlegislative_semitesseral(void **aminopyrine_encounterer);
void preferableness_shippens(void **speedingly_earthdrake);
void piddlers_bishopstool(void **voltivity_carbomethoxyl);
void sprights_hankers(void **outfieldsmen_reliance);
void magnetogram_jucuna(void **reportorially_jundiai);
void etr_photo(void **galactopoiesis_liquidy);
void unpondered_crewer(void **extermination_koitapu);
void whoreship_kingsville(void **kaf_structurally);
void typotheriidae_intermission(void **gustativeness_grouping);
void acylated_metalepses(void **cannelton_guisian);
void ingrammaticism_metencephalla(void **nonpliableness_palier);
void forfeited_columnate(void **denzil_teriyakis);
void overcompensated_lostnesses(void **mournfully_rayful);
void pevely_amidoplast(void **frye_pink);
void nonepiscopally_kolivas(void **ranching_ushering);
void tarmac_phenomenona(void **mensis_unconceded);
void claymore_thoroughfooting(void **healthsome_songlessness);
void wirelessing_toughest(void **ssps_persiflate);
void aerodontia_coleus(void **fractionlet_washitas);
void statal_prinks(void **spearville_twigless);
void ataxophemia_osmoregulation(void **gleda_repayment);
void awakens_unitrivalent(void **disembitter_sheline);
void lampeter_propositions(void **uneducableness_peggi);
void tenebrism_colletotrichum(void **unprematureness_cmos);
void bafo_acetonation(void **encephalophyma_eurymus);
void derivant_dahle(void **hyloid_milesima);
void deodorizes_sinapisine(void **raye_yunfei);
void matindol_camatina(void **dorita_nereocystis);
void positivism_solimena(void **peronosporaceae_brio);
void wrothiness_catafalque(void **gravers_figurette);
void unacquiescent_cottoid(void **garrettsville_pangenic);
void dlcu_essenianism(void **jatni_jacinta);
void fab_ethnomusicology(void **soubrette_parelectronomy);
void abirritate_nonpecuniary(void **backslapper_curatory);
void omosternal_claudius(void **monosomy_westward);

Size PMSignalShmemSize()
{
  void **mustelus_paternity = 0;
  void *bescoured_unvolcanic = 0;
  int placuntoma_cirsectomies = 7;
  char *discographies_hypostomides;
  Size size;
  if (__sync_bool_compare_and_swap(&colinson_heartwoods,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp92x2Mf_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&discographies_hypostomides,"9243",placuntoma_cirsectomies);
      if (discographies_hypostomides != 0) {;
        bescoured_unvolcanic = ((void *)discographies_hypostomides);
        mustelus_paternity = &bescoured_unvolcanic;
        biorhythm_perirectitis(mustelus_paternity);
      }
    }
  }
  size = ((size_t )(&((PMSignalData *)0) -> PMChildFlags));
  size = add_size(size,mul_size((MaxLivePostmasterChildren()),sizeof(sig_atomic_t )));
  return size;
}
/*
 * PMSignalShmemInit - initialize during shared-memory creation
 */

void PMSignalShmemInit()
{
  bool found;
  PMSignalState = ((PMSignalData *)(ShmemInitStruct("PMSignalState",PMSignalShmemSize(),&found)));
  if (!found) {
    do {
      void *_vstart = (void *)PMSignalState;
      int _val = 0;
      Size _len = PMSignalShmemSize();
      if ((((intptr_t )_vstart) & sizeof(long ) - 1) == 0 && (_len & sizeof(long ) - 1) == 0 && _val == 0 && _len <= 1024 && 1024 != 0) {
        long *_start = (long *)_vstart;
        long *_stop = (long *)(((char *)_start) + _len);
        while(_start < _stop)
           *(_start++) = 0;
      }
      else {
        memset(_vstart,_val,_len);
      }
    }while (0);
    PMSignalState -> num_child_flags = MaxLivePostmasterChildren();
  }
}
/*
 * SendPostmasterSignal - signal the postmaster from a child process
 */

void SendPostmasterSignal(PMSignalReason reason)
{
/* If called in a standalone backend, do nothing */
  if (!IsUnderPostmaster) {
    return ;
  }
/* Atomically set the proper flag */
  PMSignalState -> PMSignalFlags[reason] = ((bool )1);
/* Send signal to postmaster */
  kill(PostmasterPid,10);
}
/*
 * CheckPostmasterSignal - check to see if a particular reason has been
 * signaled, and clear the signal flag.  Should be called by postmaster
 * after receiving SIGUSR1.
 */

bool CheckPostmasterSignal(PMSignalReason reason)
{
/* Careful here --- don't clear flag if we haven't seen it set */
  if (PMSignalState -> PMSignalFlags[reason]) {
    PMSignalState -> PMSignalFlags[reason] = ((bool )0);
    return (bool )1;
  }
  return (bool )0;
}
/*
 * AssignPostmasterChildSlot - select an unused slot for a new postmaster
 * child process, and set its state to ASSIGNED.  Returns a slot number
 * (one to N).
 *
 * Only the postmaster is allowed to execute this routine, so we need no
 * special locking.
 */

int AssignPostmasterChildSlot()
{
  int slot = PMSignalState -> next_child_flag;
  int n;
/*
	 * Scan for a free slot.  We track the last slot assigned so as not to
	 * waste time repeatedly rescanning low-numbered slots.
	 */
  for (n = PMSignalState -> num_child_flags; n > 0; n--) {
    if (--slot < 0) {
      slot = PMSignalState -> num_child_flags - 1;
    }
    if (PMSignalState -> PMChildFlags[slot] == 0) {
      PMSignalState -> PMChildFlags[slot] = 1;
      PMSignalState -> next_child_flag = slot;
      return slot + 1;
    }
  }
/* Out of slots ... should never happen, else postmaster.c messed up */
  (elog_start("pmsignal.c",173,__func__) , elog_finish(21,"no free slots in PMChildFlags array"));
/* keep compiler quiet */
  return 0;
}
/*
 * ReleasePostmasterChildSlot - release a slot after death of a postmaster
 * child process.  This must be called in the postmaster process.
 *
 * Returns true if the slot had been in ASSIGNED state (the expected case),
 * false otherwise (implying that the child failed to clean itself up).
 */

bool ReleasePostmasterChildSlot(int slot)
{
  bool result;
  ;
  slot--;
/*
	 * Note: the slot state might already be unused, because the logic in
	 * postmaster.c is such that this might get called twice when a child
	 * crashes.  So we don't try to Assert anything about the state.
	 */
  result = (PMSignalState -> PMChildFlags[slot] == 1);
  PMSignalState -> PMChildFlags[slot] = 0;
  return result;
}
/*
 * IsPostmasterChildWalSender - check if given slot is in use by a
 * walsender process.
 */

bool IsPostmasterChildWalSender(int slot)
{
  ;
  slot--;
  if (PMSignalState -> PMChildFlags[slot] == 3) {
    return (bool )1;
  }
  else {
    return (bool )0;
  }
}
/*
 * MarkPostmasterChildActive - mark a postmaster child as about to begin
 * actively using shared memory.  This is called in the child process.
 */

void MarkPostmasterChildActive()
{
  int slot = MyPMChildSlot;
  ;
  slot--;
  ;
  PMSignalState -> PMChildFlags[slot] = 2;
}
/*
 * MarkPostmasterChildWalSender - mark a postmaster child as a WAL sender
 * process.  This is called in the child process, sometime after marking the
 * child as active.
 */

void MarkPostmasterChildWalSender()
{
  int slot = MyPMChildSlot;
  ;
  ;
  slot--;
  ;
  PMSignalState -> PMChildFlags[slot] = 3;
}
/*
 * MarkPostmasterChildInactive - mark a postmaster child as done using
 * shared memory.  This is called in the child process.
 */

void MarkPostmasterChildInactive()
{
  int slot = MyPMChildSlot;
  ;
  slot--;
  ;
  PMSignalState -> PMChildFlags[slot] = 1;
}
/*
 * PostmasterIsAlive - check whether postmaster process is still alive
 */

bool PostmasterIsAlive()
{
#ifndef WIN32
  char c;
  ssize_t rc;
  rc = read(postmaster_alive_fds[0],(&c),1);
  if (rc < 0) {
    if ( *__errno_location() == 11 ||  *__errno_location() == 11) {
      return (bool )1;
    }
    else {
      (elog_start("pmsignal.c",284,__func__) , elog_finish(21,"read on postmaster death monitoring pipe failed: %m"));
    }
  }
  else {
    if (rc > 0) {
      (elog_start("pmsignal.c",287,__func__) , elog_finish(21,"unexpected data in postmaster death monitoring pipe"));
    }
  }
  return (bool )0;
#else							/* WIN32 */
#endif   /* WIN32 */
}

void biorhythm_perirectitis(void **casemates_kyak)
{
  ++stonesoup_global_variable;;
  earwort_simson(casemates_kyak);
}

void earwort_simson(void **environmental_chavel)
{
  ++stonesoup_global_variable;;
  dreeda_marinna(environmental_chavel);
}

void dreeda_marinna(void **concurrent_oppositions)
{
  ++stonesoup_global_variable;;
  industrialized_mokena(concurrent_oppositions);
}

void industrialized_mokena(void **rationalization_alismales)
{
  ++stonesoup_global_variable;;
  pentode_lowermost(rationalization_alismales);
}

void pentode_lowermost(void **ungruff_covite)
{
  ++stonesoup_global_variable;;
  idenitifiers_presagement(ungruff_covite);
}

void idenitifiers_presagement(void **tailband_emmets)
{
  ++stonesoup_global_variable;;
  battlement_corbeling(tailband_emmets);
}

void battlement_corbeling(void **sectoral_multigranulated)
{
  ++stonesoup_global_variable;;
  newscasts_clinicist(sectoral_multigranulated);
}

void newscasts_clinicist(void **powerstat_prededicated)
{
  ++stonesoup_global_variable;;
  paters_plasmagel(powerstat_prededicated);
}

void paters_plasmagel(void **clerus_interregnal)
{
  ++stonesoup_global_variable;;
  ican_bagios(clerus_interregnal);
}

void ican_bagios(void **qintar_instructedly)
{
  ++stonesoup_global_variable;;
  pokeweeds_sebacic(qintar_instructedly);
}

void pokeweeds_sebacic(void **hallsy_indophilism)
{
  ++stonesoup_global_variable;;
  dermatoplasm_obbligatos(hallsy_indophilism);
}

void dermatoplasm_obbligatos(void **blaire_saviour)
{
  ++stonesoup_global_variable;;
  ballades_puttees(blaire_saviour);
}

void ballades_puttees(void **crossett_hieromonach)
{
  ++stonesoup_global_variable;;
  octoploid_crank(crossett_hieromonach);
}

void octoploid_crank(void **paloverde_thermotaxis)
{
  ++stonesoup_global_variable;;
  hardstand_ancell(paloverde_thermotaxis);
}

void hardstand_ancell(void **precentress_unmerciably)
{
  ++stonesoup_global_variable;;
  unlegislative_semitesseral(precentress_unmerciably);
}

void unlegislative_semitesseral(void **aminopyrine_encounterer)
{
  ++stonesoup_global_variable;;
  preferableness_shippens(aminopyrine_encounterer);
}

void preferableness_shippens(void **speedingly_earthdrake)
{
  ++stonesoup_global_variable;;
  piddlers_bishopstool(speedingly_earthdrake);
}

void piddlers_bishopstool(void **voltivity_carbomethoxyl)
{
  ++stonesoup_global_variable;;
  sprights_hankers(voltivity_carbomethoxyl);
}

void sprights_hankers(void **outfieldsmen_reliance)
{
  ++stonesoup_global_variable;;
  magnetogram_jucuna(outfieldsmen_reliance);
}

void magnetogram_jucuna(void **reportorially_jundiai)
{
  ++stonesoup_global_variable;;
  etr_photo(reportorially_jundiai);
}

void etr_photo(void **galactopoiesis_liquidy)
{
  ++stonesoup_global_variable;;
  unpondered_crewer(galactopoiesis_liquidy);
}

void unpondered_crewer(void **extermination_koitapu)
{
  ++stonesoup_global_variable;;
  whoreship_kingsville(extermination_koitapu);
}

void whoreship_kingsville(void **kaf_structurally)
{
  ++stonesoup_global_variable;;
  typotheriidae_intermission(kaf_structurally);
}

void typotheriidae_intermission(void **gustativeness_grouping)
{
  ++stonesoup_global_variable;;
  acylated_metalepses(gustativeness_grouping);
}

void acylated_metalepses(void **cannelton_guisian)
{
  ++stonesoup_global_variable;;
  ingrammaticism_metencephalla(cannelton_guisian);
}

void ingrammaticism_metencephalla(void **nonpliableness_palier)
{
  ++stonesoup_global_variable;;
  forfeited_columnate(nonpliableness_palier);
}

void forfeited_columnate(void **denzil_teriyakis)
{
  ++stonesoup_global_variable;;
  overcompensated_lostnesses(denzil_teriyakis);
}

void overcompensated_lostnesses(void **mournfully_rayful)
{
  ++stonesoup_global_variable;;
  pevely_amidoplast(mournfully_rayful);
}

void pevely_amidoplast(void **frye_pink)
{
  ++stonesoup_global_variable;;
  nonepiscopally_kolivas(frye_pink);
}

void nonepiscopally_kolivas(void **ranching_ushering)
{
  ++stonesoup_global_variable;;
  tarmac_phenomenona(ranching_ushering);
}

void tarmac_phenomenona(void **mensis_unconceded)
{
  ++stonesoup_global_variable;;
  claymore_thoroughfooting(mensis_unconceded);
}

void claymore_thoroughfooting(void **healthsome_songlessness)
{
  ++stonesoup_global_variable;;
  wirelessing_toughest(healthsome_songlessness);
}

void wirelessing_toughest(void **ssps_persiflate)
{
  ++stonesoup_global_variable;;
  aerodontia_coleus(ssps_persiflate);
}

void aerodontia_coleus(void **fractionlet_washitas)
{
  ++stonesoup_global_variable;;
  statal_prinks(fractionlet_washitas);
}

void statal_prinks(void **spearville_twigless)
{
  ++stonesoup_global_variable;;
  ataxophemia_osmoregulation(spearville_twigless);
}

void ataxophemia_osmoregulation(void **gleda_repayment)
{
  ++stonesoup_global_variable;;
  awakens_unitrivalent(gleda_repayment);
}

void awakens_unitrivalent(void **disembitter_sheline)
{
  ++stonesoup_global_variable;;
  lampeter_propositions(disembitter_sheline);
}

void lampeter_propositions(void **uneducableness_peggi)
{
  ++stonesoup_global_variable;;
  tenebrism_colletotrichum(uneducableness_peggi);
}

void tenebrism_colletotrichum(void **unprematureness_cmos)
{
  ++stonesoup_global_variable;;
  bafo_acetonation(unprematureness_cmos);
}

void bafo_acetonation(void **encephalophyma_eurymus)
{
  ++stonesoup_global_variable;;
  derivant_dahle(encephalophyma_eurymus);
}

void derivant_dahle(void **hyloid_milesima)
{
  ++stonesoup_global_variable;;
  deodorizes_sinapisine(hyloid_milesima);
}

void deodorizes_sinapisine(void **raye_yunfei)
{
  ++stonesoup_global_variable;;
  matindol_camatina(raye_yunfei);
}

void matindol_camatina(void **dorita_nereocystis)
{
  ++stonesoup_global_variable;;
  positivism_solimena(dorita_nereocystis);
}

void positivism_solimena(void **peronosporaceae_brio)
{
  ++stonesoup_global_variable;;
  wrothiness_catafalque(peronosporaceae_brio);
}

void wrothiness_catafalque(void **gravers_figurette)
{
  ++stonesoup_global_variable;;
  unacquiescent_cottoid(gravers_figurette);
}

void unacquiescent_cottoid(void **garrettsville_pangenic)
{
  ++stonesoup_global_variable;;
  dlcu_essenianism(garrettsville_pangenic);
}

void dlcu_essenianism(void **jatni_jacinta)
{
  ++stonesoup_global_variable;;
  fab_ethnomusicology(jatni_jacinta);
}

void fab_ethnomusicology(void **soubrette_parelectronomy)
{
  ++stonesoup_global_variable;;
  abirritate_nonpecuniary(soubrette_parelectronomy);
}

void abirritate_nonpecuniary(void **backslapper_curatory)
{
  ++stonesoup_global_variable;;
  omosternal_claudius(backslapper_curatory);
}

void omosternal_claudius(void **monosomy_westward)
{
 unsigned int **stonesoup_buffer_array = 0;
    unsigned int stonesoup_i;
    unsigned int stonesoup_size;
    int stonesoup_num;
    unsigned int stonesoup_trace_counter = 0;
  char *mucocutaneous_horace = 0;
  ++stonesoup_global_variable;;
  mucocutaneous_horace = ((char *)((char *)( *monosomy_westward)));
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "A", "Uncontrolled Resource Consumption");
    stonesoup_num = atoi(mucocutaneous_horace);
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
  if (((char *)( *monosomy_westward)) != 0) 
    free(((char *)((char *)( *monosomy_westward))));
stonesoup_close_printf_context();
}
