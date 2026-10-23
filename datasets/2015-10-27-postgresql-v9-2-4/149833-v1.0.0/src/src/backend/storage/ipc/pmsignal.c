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
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 

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
int sonants_shealings = 0;

struct antivice_iciest 
{
  char *unitize_vases;
  double hymnody_cargian;
  char *attenuative_mahdi;
  char culdesac_inhuman;
  int cevadine_podophyllum;
}
;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *photographs_lehi);
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
void geast_delegatus(int temporary_gossypium,struct antivice_iciest cynomoriaceae_metayer);
void chromene_mimesises(int sabaean_toughest,struct antivice_iciest biafra_crambes);

Size PMSignalShmemSize()
{
  Size size;
  if (__sync_bool_compare_and_swap(&sonants_shealings,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpje2mAo_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_read_taint();
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

void stonesoup_handle_taint(char *photographs_lehi)
{
  int vinegarer_unmoderately = 7;
  struct antivice_iciest centetes_iconophile = {0};
  int **************************************************emf_phosphoretted = 0;
  int *************************************************unbudgeable_nonejecting = 0;
  int ************************************************undistractingly_acture = 0;
  int ***********************************************murarium_unteach = 0;
  int **********************************************screenwriter_alleman = 0;
  int *********************************************stiritis_carbonimeter = 0;
  int ********************************************streambed_bwts = 0;
  int *******************************************supergratified_documented = 0;
  int ******************************************microsplenic_belays = 0;
  int *****************************************maser_quizzicality = 0;
  int ****************************************unannexedly_qsl = 0;
  int ***************************************doggone_cruors = 0;
  int **************************************libelling_nekrasov = 0;
  int *************************************bloomsdale_lonelihood = 0;
  int ************************************unpiteously_incredibleness = 0;
  int ***********************************hemianacusia_thilanottine = 0;
  int **********************************piepowder_scillain = 0;
  int *********************************doglike_poroporo = 0;
  int ********************************sattley_blissfully = 0;
  int *******************************scutelliform_cued = 0;
  int ******************************pteranodon_chowry = 0;
  int *****************************darwinite_assument = 0;
  int ****************************cadded_muteness = 0;
  int ***************************nuculacea_miradors = 0;
  int **************************argent_subnervian = 0;
  int *************************codbank_tirrlie = 0;
  int ************************kempton_emory = 0;
  int ***********************computerite_arake = 0;
  int **********************preenroll_trocar = 0;
  int *********************faqir_finnmark = 0;
  int ********************ravenousnesses_silicones = 0;
  int *******************hiodontidae_destructionism = 0;
  int ******************besnuff_wifelessness = 0;
  int *****************quantrill_jovialties = 0;
  int ****************unspeculative_zygopleural = 0;
  int ***************interlaudation_burkundauze = 0;
  int **************babe_karynne = 0;
  int *************iodizer_thyrostracan = 0;
  int ************aspiculous_vestments = 0;
  int ***********furil_incongruence = 0;
  int **********tremelliform_subspecialist = 0;
  int *********media_unprettified = 0;
  int ********coramine_correligionist = 0;
  int *******revocatory_factitious = 0;
  int ******sawbucks_anaesthetizer = 0;
  int *****colaxais_grainier = 0;
  int ****ravisher_glunch = 0;
  int ***reitboks_rummy = 0;
  int **disoppilate_sancho = 0;
  int *hasslet_eighteenths = 0;
  int oxcarts_fieldbird;
  struct antivice_iciest heightened_onlay[10] = {0};
  struct antivice_iciest counterspy_noveboracensis;
  ++stonesoup_global_variable;;
  if (photographs_lehi != 0) {;
    counterspy_noveboracensis . unitize_vases = ((char *)photographs_lehi);
    oxcarts_fieldbird = 5;
    hasslet_eighteenths = &oxcarts_fieldbird;
    disoppilate_sancho = &hasslet_eighteenths;
    reitboks_rummy = &disoppilate_sancho;
    ravisher_glunch = &reitboks_rummy;
    colaxais_grainier = &ravisher_glunch;
    sawbucks_anaesthetizer = &colaxais_grainier;
    revocatory_factitious = &sawbucks_anaesthetizer;
    coramine_correligionist = &revocatory_factitious;
    media_unprettified = &coramine_correligionist;
    tremelliform_subspecialist = &media_unprettified;
    furil_incongruence = &tremelliform_subspecialist;
    aspiculous_vestments = &furil_incongruence;
    iodizer_thyrostracan = &aspiculous_vestments;
    babe_karynne = &iodizer_thyrostracan;
    interlaudation_burkundauze = &babe_karynne;
    unspeculative_zygopleural = &interlaudation_burkundauze;
    quantrill_jovialties = &unspeculative_zygopleural;
    besnuff_wifelessness = &quantrill_jovialties;
    hiodontidae_destructionism = &besnuff_wifelessness;
    ravenousnesses_silicones = &hiodontidae_destructionism;
    faqir_finnmark = &ravenousnesses_silicones;
    preenroll_trocar = &faqir_finnmark;
    computerite_arake = &preenroll_trocar;
    kempton_emory = &computerite_arake;
    codbank_tirrlie = &kempton_emory;
    argent_subnervian = &codbank_tirrlie;
    nuculacea_miradors = &argent_subnervian;
    cadded_muteness = &nuculacea_miradors;
    darwinite_assument = &cadded_muteness;
    pteranodon_chowry = &darwinite_assument;
    scutelliform_cued = &pteranodon_chowry;
    sattley_blissfully = &scutelliform_cued;
    doglike_poroporo = &sattley_blissfully;
    piepowder_scillain = &doglike_poroporo;
    hemianacusia_thilanottine = &piepowder_scillain;
    unpiteously_incredibleness = &hemianacusia_thilanottine;
    bloomsdale_lonelihood = &unpiteously_incredibleness;
    libelling_nekrasov = &bloomsdale_lonelihood;
    doggone_cruors = &libelling_nekrasov;
    unannexedly_qsl = &doggone_cruors;
    maser_quizzicality = &unannexedly_qsl;
    microsplenic_belays = &maser_quizzicality;
    supergratified_documented = &microsplenic_belays;
    streambed_bwts = &supergratified_documented;
    stiritis_carbonimeter = &streambed_bwts;
    screenwriter_alleman = &stiritis_carbonimeter;
    murarium_unteach = &screenwriter_alleman;
    undistractingly_acture = &murarium_unteach;
    unbudgeable_nonejecting = &undistractingly_acture;
    emf_phosphoretted = &unbudgeable_nonejecting;
    heightened_onlay[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *emf_phosphoretted)))))))))))))))))))))))))))))))))))))))))))))))))] = counterspy_noveboracensis;
    centetes_iconophile = heightened_onlay[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *emf_phosphoretted)))))))))))))))))))))))))))))))))))))))))))))))))];
    geast_delegatus(vinegarer_unmoderately,centetes_iconophile);
  }
}

void geast_delegatus(int temporary_gossypium,struct antivice_iciest cynomoriaceae_metayer)
{
  size_t stonesoup_j = 0;
  size_t stonesoup_i = 0;
  char *stonesoup_second_buff = 0;
  char *stonesoup_finder = "aba";
  int stonesoup_check = 0;
  char *formalesque_biconjugate = 0;
  ++stonesoup_global_variable;
  temporary_gossypium--;
  if (temporary_gossypium > 0) {
    chromene_mimesises(temporary_gossypium,cynomoriaceae_metayer);
    return ;
  }
  formalesque_biconjugate = ((char *)cynomoriaceae_metayer . unitize_vases);
      tracepoint(stonesoup_trace, weakness_start, "CWE476", "B", "NULL Pointer Dereference");
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
      for (stonesoup_i = 0; ((int )stonesoup_i) <= ((int )(strlen(formalesque_biconjugate) - strlen(stonesoup_finder))); ++stonesoup_i) {
        for (stonesoup_j = 0; stonesoup_j < strlen(stonesoup_finder); ++stonesoup_j) {
          if (formalesque_biconjugate[stonesoup_i + stonesoup_j] != stonesoup_finder[stonesoup_j]) {
            stonesoup_check = 0;
            break;
          }
          stonesoup_check = 1;
        }
/* STONESOUP: CROSSOVER-POINT (Null Pointer Dereference) */
        if (stonesoup_check == 1 && stonesoup_j == strlen(stonesoup_finder)) {
          stonesoup_printf("Found aba string\n");
          stonesoup_second_buff = &formalesque_biconjugate[stonesoup_i];
          break;
        }
      }
      tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
      tracepoint(stonesoup_trace, variable_address, "stonesoup_second_buff", stonesoup_second_buff, "TRIGGER-STATE");
/* STONESOUP: TRIGGER-POINT (Null Pointer Dereference) */
      stonesoup_printf("String length is %i\n", strlen(stonesoup_second_buff));
      tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
      tracepoint(stonesoup_trace, weakness_end);
;
  if (cynomoriaceae_metayer . unitize_vases != 0) 
    free(((char *)cynomoriaceae_metayer . unitize_vases));
stonesoup_close_printf_context();
}

void chromene_mimesises(int sabaean_toughest,struct antivice_iciest biafra_crambes)
{
  ++stonesoup_global_variable;
  geast_delegatus(sabaean_toughest,biafra_crambes);
}
