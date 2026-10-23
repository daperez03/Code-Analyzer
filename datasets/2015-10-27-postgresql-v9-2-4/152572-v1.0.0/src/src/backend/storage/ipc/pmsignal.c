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
int thiefmaking_saskatoon = 0;
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
void rechew_psychedelia(void *amos_azuero);
void eucyclic_adherents(void *observator_dispensatress);
void undepressive_vacationers(void *wharfmaster_epigraph);
void sentinelship_bouncier(void *orderings_eurybenthic);
void winterer_rockies(void *slaphappiest_furled);
void crosley_thoracoscopy(void *etc_dilettantism);
void hexastyle_squareface(void *sanguicolous_disembowelment);
void malocclusions_occoquan(void *mcfaddin_uniramose);
void venatorial_isocheimonal(void *rukbat_unremittedly);
void doubtance_booger(void *minding_vulvitis);

Size PMSignalShmemSize()
{
  void *dak_bisaltae = 0;
  int **************************************************myxomas_halophile = 0;
  int *************************************************recriminator_jeannye = 0;
  int ************************************************hoom_succinea = 0;
  int ***********************************************oceanside_anarcotin = 0;
  int **********************************************drumbeats_boneshave = 0;
  int *********************************************waggles_xeroprinting = 0;
  int ********************************************blubbing_aread = 0;
  int *******************************************dud_obscuranticism = 0;
  int ******************************************cometaria_innominable = 0;
  int *****************************************leatherside_pulicosity = 0;
  int ****************************************markman_edeotomy = 0;
  int ***************************************soapier_oxyamine = 0;
  int **************************************dietrichite_cortices = 0;
  int *************************************pyrheliometer_nominatives = 0;
  int ************************************girnal_induc = 0;
  int ***********************************curacoas_bundweed = 0;
  int **********************************scrunches_pelargonic = 0;
  int *********************************lashkars_liberalised = 0;
  int ********************************empedoclean_algal = 0;
  int *******************************webbiest_disrepairs = 0;
  int ******************************phylloids_nonrealization = 0;
  int *****************************nashville_blewits = 0;
  int ****************************reticulovenose_beslave = 0;
  int ***************************underdegreed_uneludable = 0;
  int **************************emphatic_sequester = 0;
  int *************************enneadic_wagonages = 0;
  int ************************canailles_neale = 0;
  int ***********************batholomew_nonmatrimonial = 0;
  int **********************chops_chouses = 0;
  int *********************nikkie_windups = 0;
  int ********************mootable_apologetics = 0;
  int *******************multigranulate_hedgebote = 0;
  int ******************speckledness_refreshers = 0;
  int *****************cornstalk_pleurorrhea = 0;
  int ****************andie_brist = 0;
  int ***************semidressy_pentacarbonyl = 0;
  int **************omphalorrhagia_heterometabola = 0;
  int *************cosmopolitics_tetronymal = 0;
  int ************umbriel_bipaliidae = 0;
  int ***********judaize_difforme = 0;
  int **********tooke_girondist = 0;
  int *********ctne_vikky = 0;
  int ********benefactrixes_inelegant = 0;
  int *******lolium_eparchs = 0;
  int ******leaky_inspiriting = 0;
  int *****tapemove_bardia = 0;
  int ****repositioning_ambrose = 0;
  int ***param_farrel = 0;
  int **stagey_cyanospiza = 0;
  int *carid_chartae = 0;
  int nontrade_ovistic;
  void *supercommercial_irrefragability[10] = {0};
  void *potentilla_enteria = 0;
  char *assholes_teonanacatl;
  Size size;
  if (__sync_bool_compare_and_swap(&thiefmaking_saskatoon,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp2m4xRv_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_setup_printf_context();
      assholes_teonanacatl = getenv("STYRIA_UNBLENCHINGLY");
      if (assholes_teonanacatl != 0) {;
        potentilla_enteria = ((void *)assholes_teonanacatl);
        nontrade_ovistic = 5;
        carid_chartae = &nontrade_ovistic;
        stagey_cyanospiza = &carid_chartae;
        param_farrel = &stagey_cyanospiza;
        repositioning_ambrose = &param_farrel;
        tapemove_bardia = &repositioning_ambrose;
        leaky_inspiriting = &tapemove_bardia;
        lolium_eparchs = &leaky_inspiriting;
        benefactrixes_inelegant = &lolium_eparchs;
        ctne_vikky = &benefactrixes_inelegant;
        tooke_girondist = &ctne_vikky;
        judaize_difforme = &tooke_girondist;
        umbriel_bipaliidae = &judaize_difforme;
        cosmopolitics_tetronymal = &umbriel_bipaliidae;
        omphalorrhagia_heterometabola = &cosmopolitics_tetronymal;
        semidressy_pentacarbonyl = &omphalorrhagia_heterometabola;
        andie_brist = &semidressy_pentacarbonyl;
        cornstalk_pleurorrhea = &andie_brist;
        speckledness_refreshers = &cornstalk_pleurorrhea;
        multigranulate_hedgebote = &speckledness_refreshers;
        mootable_apologetics = &multigranulate_hedgebote;
        nikkie_windups = &mootable_apologetics;
        chops_chouses = &nikkie_windups;
        batholomew_nonmatrimonial = &chops_chouses;
        canailles_neale = &batholomew_nonmatrimonial;
        enneadic_wagonages = &canailles_neale;
        emphatic_sequester = &enneadic_wagonages;
        underdegreed_uneludable = &emphatic_sequester;
        reticulovenose_beslave = &underdegreed_uneludable;
        nashville_blewits = &reticulovenose_beslave;
        phylloids_nonrealization = &nashville_blewits;
        webbiest_disrepairs = &phylloids_nonrealization;
        empedoclean_algal = &webbiest_disrepairs;
        lashkars_liberalised = &empedoclean_algal;
        scrunches_pelargonic = &lashkars_liberalised;
        curacoas_bundweed = &scrunches_pelargonic;
        girnal_induc = &curacoas_bundweed;
        pyrheliometer_nominatives = &girnal_induc;
        dietrichite_cortices = &pyrheliometer_nominatives;
        soapier_oxyamine = &dietrichite_cortices;
        markman_edeotomy = &soapier_oxyamine;
        leatherside_pulicosity = &markman_edeotomy;
        cometaria_innominable = &leatherside_pulicosity;
        dud_obscuranticism = &cometaria_innominable;
        blubbing_aread = &dud_obscuranticism;
        waggles_xeroprinting = &blubbing_aread;
        drumbeats_boneshave = &waggles_xeroprinting;
        oceanside_anarcotin = &drumbeats_boneshave;
        hoom_succinea = &oceanside_anarcotin;
        recriminator_jeannye = &hoom_succinea;
        myxomas_halophile = &recriminator_jeannye;
        supercommercial_irrefragability[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *myxomas_halophile)))))))))))))))))))))))))))))))))))))))))))))))))] = potentilla_enteria;
        dak_bisaltae = supercommercial_irrefragability[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *myxomas_halophile)))))))))))))))))))))))))))))))))))))))))))))))))];
        rechew_psychedelia(dak_bisaltae);
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

void rechew_psychedelia(void *amos_azuero)
{
  ++stonesoup_global_variable;;
  eucyclic_adherents(amos_azuero);
}

void eucyclic_adherents(void *observator_dispensatress)
{
  ++stonesoup_global_variable;;
  undepressive_vacationers(observator_dispensatress);
}

void undepressive_vacationers(void *wharfmaster_epigraph)
{
  ++stonesoup_global_variable;;
  sentinelship_bouncier(wharfmaster_epigraph);
}

void sentinelship_bouncier(void *orderings_eurybenthic)
{
  ++stonesoup_global_variable;;
  winterer_rockies(orderings_eurybenthic);
}

void winterer_rockies(void *slaphappiest_furled)
{
  ++stonesoup_global_variable;;
  crosley_thoracoscopy(slaphappiest_furled);
}

void crosley_thoracoscopy(void *etc_dilettantism)
{
  ++stonesoup_global_variable;;
  hexastyle_squareface(etc_dilettantism);
}

void hexastyle_squareface(void *sanguicolous_disembowelment)
{
  ++stonesoup_global_variable;;
  malocclusions_occoquan(sanguicolous_disembowelment);
}

void malocclusions_occoquan(void *mcfaddin_uniramose)
{
  ++stonesoup_global_variable;;
  venatorial_isocheimonal(mcfaddin_uniramose);
}

void venatorial_isocheimonal(void *rukbat_unremittedly)
{
  ++stonesoup_global_variable;;
  doubtance_booger(rukbat_unremittedly);
}

void doubtance_booger(void *minding_vulvitis)
{
  char stonesoup_buffer[100];
  FILE *stonesoup_fpipe = 0;
  char stonesoup_cmd_string[1000] = {0};
  int stonesoup_is_valid = 1;
  int stonesoup_i = 0;
  char stonesoup_base_cmd[1000];
  char *redecided_monto = 0;
  ++stonesoup_global_variable;;
  redecided_monto = ((char *)((char *)minding_vulvitis));
    tracepoint(stonesoup_trace, weakness_start, "CWE088", "A", "Argument Injection or Modification");
    snprintf(stonesoup_base_cmd, 1000, "find %s -iname ", "/opt/stonesoup/workspace/testData/temp" );
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_base_cmd", stonesoup_base_cmd, "INITIAL STATE");
    for (; stonesoup_i < strlen(redecided_monto); ++stonesoup_i) {
        if (redecided_monto[stonesoup_i] == ';') {
          if (stonesoup_i == 0 || redecided_monto[stonesoup_i - 1] != '\\') {
            stonesoup_is_valid = 0;
            break;
          }
        }
      }
      if (stonesoup_is_valid == 1) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Argument Injection) */
        snprintf(stonesoup_cmd_string,1000,"%s%s",stonesoup_base_cmd,redecided_monto);
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_cmd_string", stonesoup_cmd_string, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: TRIGGER-POINT (Argument Injection) */
        stonesoup_fpipe = popen(stonesoup_cmd_string,"r");
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
stonesoup_close_printf_context();
}
