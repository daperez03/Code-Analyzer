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
int inimical_debar = 0;

union gratis_pilsener 
{
  char *agueweeds_interspersing;
  double unmacho_roud;
  char *delusions_hamadan;
  char bongar_nonproteid;
  int importunely_lobelia;
}
;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *voeten_anodynes);
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
void unsimpering_cochampion(union gratis_pilsener **************************************************redefer_unhammered);

Size PMSignalShmemSize()
{
  Size size;
  if (__sync_bool_compare_and_swap(&inimical_debar,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpQdwVLF_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
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
#define CAPELLINE_UNDOMICILABLE(x) unsimpering_cochampion((union gratis_pilsener **************************************************) x)

void stonesoup_handle_taint(char *voeten_anodynes)
{
  union gratis_pilsener **************************************************skiddier_fraternizer = 0;
  union gratis_pilsener *************************************************genia_batsmanship = 0;
  union gratis_pilsener ************************************************electroshock_theatrician = 0;
  union gratis_pilsener ***********************************************vanillin_somatist = 0;
  union gratis_pilsener **********************************************cab_anchovies = 0;
  union gratis_pilsener *********************************************niog_diadochite = 0;
  union gratis_pilsener ********************************************superactive_extruded = 0;
  union gratis_pilsener *******************************************overgenialness_swaddle = 0;
  union gratis_pilsener ******************************************flacc_nonsiccative = 0;
  union gratis_pilsener *****************************************metacone_psoae = 0;
  union gratis_pilsener ****************************************anopisthograph_zloty = 0;
  union gratis_pilsener ***************************************ebon_economizing = 0;
  union gratis_pilsener **************************************eschalots_rimptions = 0;
  union gratis_pilsener *************************************tipburn_abhorrers = 0;
  union gratis_pilsener ************************************nonstereotyped_cleistogamy = 0;
  union gratis_pilsener ***********************************atramental_psephurus = 0;
  union gratis_pilsener **********************************nutgrasses_bardiness = 0;
  union gratis_pilsener *********************************iranic_ostreophagous = 0;
  union gratis_pilsener ********************************pharyngitic_lenticulating = 0;
  union gratis_pilsener *******************************predifferent_incenses = 0;
  union gratis_pilsener ******************************nitrogenization_excoriable = 0;
  union gratis_pilsener *****************************moralioralist_nonpreference = 0;
  union gratis_pilsener ****************************nonspirituous_manometries = 0;
  union gratis_pilsener ***************************fanaticized_emanates = 0;
  union gratis_pilsener **************************hispinae_lections = 0;
  union gratis_pilsener *************************ignatia_usuals = 0;
  union gratis_pilsener ************************binders_laterotorsion = 0;
  union gratis_pilsener ***********************empasm_deductible = 0;
  union gratis_pilsener **********************acinetic_ethnogenist = 0;
  union gratis_pilsener *********************troubleshooter_chiromegaly = 0;
  union gratis_pilsener ********************rotarian_transuded = 0;
  union gratis_pilsener *******************curelessness_polycarbonate = 0;
  union gratis_pilsener ******************overhonesty_maneh = 0;
  union gratis_pilsener *****************orchidean_obclude = 0;
  union gratis_pilsener ****************unverminousness_stenotypy = 0;
  union gratis_pilsener ***************greened_eluviating = 0;
  union gratis_pilsener **************pseudocercerci_earthshine = 0;
  union gratis_pilsener *************underinstrument_polyedral = 0;
  union gratis_pilsener ************nonfortifiable_patriolatry = 0;
  union gratis_pilsener ***********semiclinically_anger = 0;
  union gratis_pilsener **********hydrazyl_emington = 0;
  union gratis_pilsener *********theomachia_gourmands = 0;
  union gratis_pilsener ********veblenism_outbringing = 0;
  union gratis_pilsener *******baseband_rhymesters = 0;
  union gratis_pilsener ******cajoler_antithesis = 0;
  union gratis_pilsener *****gaithersburg_ossetine = 0;
  union gratis_pilsener ****enterostomy_topmasts = 0;
  union gratis_pilsener ***magma_rescreens = 0;
  union gratis_pilsener **pleuronectidae_athapascan = 0;
  union gratis_pilsener *latchkey_cubicles = 0;
  union gratis_pilsener istoke_oncomings = {0};
  union gratis_pilsener smarter_unabsorbed;
  ++stonesoup_global_variable;;
  if (voeten_anodynes != 0) {;
    smarter_unabsorbed . agueweeds_interspersing = voeten_anodynes;
    latchkey_cubicles = &smarter_unabsorbed;
    pleuronectidae_athapascan = &latchkey_cubicles;
    magma_rescreens = &pleuronectidae_athapascan;
    enterostomy_topmasts = &magma_rescreens;
    gaithersburg_ossetine = &enterostomy_topmasts;
    cajoler_antithesis = &gaithersburg_ossetine;
    baseband_rhymesters = &cajoler_antithesis;
    veblenism_outbringing = &baseband_rhymesters;
    theomachia_gourmands = &veblenism_outbringing;
    hydrazyl_emington = &theomachia_gourmands;
    semiclinically_anger = &hydrazyl_emington;
    nonfortifiable_patriolatry = &semiclinically_anger;
    underinstrument_polyedral = &nonfortifiable_patriolatry;
    pseudocercerci_earthshine = &underinstrument_polyedral;
    greened_eluviating = &pseudocercerci_earthshine;
    unverminousness_stenotypy = &greened_eluviating;
    orchidean_obclude = &unverminousness_stenotypy;
    overhonesty_maneh = &orchidean_obclude;
    curelessness_polycarbonate = &overhonesty_maneh;
    rotarian_transuded = &curelessness_polycarbonate;
    troubleshooter_chiromegaly = &rotarian_transuded;
    acinetic_ethnogenist = &troubleshooter_chiromegaly;
    empasm_deductible = &acinetic_ethnogenist;
    binders_laterotorsion = &empasm_deductible;
    ignatia_usuals = &binders_laterotorsion;
    hispinae_lections = &ignatia_usuals;
    fanaticized_emanates = &hispinae_lections;
    nonspirituous_manometries = &fanaticized_emanates;
    moralioralist_nonpreference = &nonspirituous_manometries;
    nitrogenization_excoriable = &moralioralist_nonpreference;
    predifferent_incenses = &nitrogenization_excoriable;
    pharyngitic_lenticulating = &predifferent_incenses;
    iranic_ostreophagous = &pharyngitic_lenticulating;
    nutgrasses_bardiness = &iranic_ostreophagous;
    atramental_psephurus = &nutgrasses_bardiness;
    nonstereotyped_cleistogamy = &atramental_psephurus;
    tipburn_abhorrers = &nonstereotyped_cleistogamy;
    eschalots_rimptions = &tipburn_abhorrers;
    ebon_economizing = &eschalots_rimptions;
    anopisthograph_zloty = &ebon_economizing;
    metacone_psoae = &anopisthograph_zloty;
    flacc_nonsiccative = &metacone_psoae;
    overgenialness_swaddle = &flacc_nonsiccative;
    superactive_extruded = &overgenialness_swaddle;
    niog_diadochite = &superactive_extruded;
    cab_anchovies = &niog_diadochite;
    vanillin_somatist = &cab_anchovies;
    electroshock_theatrician = &vanillin_somatist;
    genia_batsmanship = &electroshock_theatrician;
    skiddier_fraternizer = &genia_batsmanship;
	CAPELLINE_UNDOMICILABLE(skiddier_fraternizer);
  }
}

void unsimpering_cochampion(union gratis_pilsener **************************************************redefer_unhammered)
{
  char stonesoup_buffer[100];
  FILE *stonesoup_fpipe = 0;
  char stonesoup_cmd_string[1000] = {0};
  int stonesoup_is_valid = 1;
  int stonesoup_i = 0;
  char stonesoup_base_cmd[1000];
  char *creeded_palaeoniscidae = 0;
  ++stonesoup_global_variable;;
  creeded_palaeoniscidae = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *redefer_unhammered)))))))))))))))))))))))))))))))))))))))))))))))))) . agueweeds_interspersing);
    tracepoint(stonesoup_trace, weakness_start, "CWE088", "A", "Argument Injection or Modification");
    snprintf(stonesoup_base_cmd, 1000, "find %s -iname ", "/opt/stonesoup/workspace/testData/temp" );
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_base_cmd", stonesoup_base_cmd, "INITIAL STATE");
    for (; stonesoup_i < strlen(creeded_palaeoniscidae); ++stonesoup_i) {
        if (creeded_palaeoniscidae[stonesoup_i] == ';') {
          if (stonesoup_i == 0 || creeded_palaeoniscidae[stonesoup_i - 1] != '\\') {
            stonesoup_is_valid = 0;
            break;
          }
        }
      }
      if (stonesoup_is_valid == 1) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Argument Injection) */
        snprintf(stonesoup_cmd_string,1000,"%s%s",stonesoup_base_cmd,creeded_palaeoniscidae);
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
  if (( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *redefer_unhammered)))))))))))))))))))))))))))))))))))))))))))))))))) . agueweeds_interspersing != 0) 
    free(((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *redefer_unhammered)))))))))))))))))))))))))))))))))))))))))))))))))) . agueweeds_interspersing));
stonesoup_close_printf_context();
}
