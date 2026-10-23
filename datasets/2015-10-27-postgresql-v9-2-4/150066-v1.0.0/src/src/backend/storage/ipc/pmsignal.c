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
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <fcntl.h> 
#include <math.h> 

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
int antikenotoxin_fourteenfold = 0;
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
struct stonesoup_data {
    char *data;
    char *file1;
    char *file2;
};
struct stonesoup_data *stonesoupData;
int stonesoup_loop;
int *stonesoup_global1;
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpTbVmmG_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Finished reading sync file.");
}
void waitForSig() {
    int fd;
    char outStr[25] = {0};
    char filename[500] = {0};
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpTbVmmG_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "waitForSig");
    stonesoup_printf("In waitForSig\n");
    sprintf(outStr, "%d.pid", getpid());
    strcat(filename, "/opt/stonesoup/workspace/testData/");
    strcat(filename, outStr);
    if ((fd = open(filename, O_CREAT|O_WRONLY, 0666)) == -1) {
        tracepoint(stonesoup_trace, trace_error, "Error opening file.");
        stonesoup_printf("Error opening file.");
    }
    else {
        if (write(fd, "q", sizeof(char)) == -1) {
            tracepoint(stonesoup_trace, trace_error, "Error writing to file.");
            stonesoup_printf("Error writing to file.");
        }
        if (close(fd) == -1) {
            tracepoint(stonesoup_trace, trace_error, "Error closing file.");
            stonesoup_printf("Error closing file.");
        }
        tracepoint(stonesoup_trace, trace_point, "Finished writing .pid file.");
        stonesoup_printf("Reading file1\n");
        stonesoup_readFile(stonesoupData->file1);
        stonesoup_readFile(stonesoupData->file2);
    }
}
void stonesoup_sig_handler (int sig) {
    stonesoup_printf("In stonesoup_sig_handler\n");
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpTbVmmG_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "stonesoup_sig_handler");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (signal handler for multiple signals) */
    /* STONESOUP: TRIGGER-POINT (signal handler for multiple signals) */
    stonesoup_global1[0] = -1;
    free(stonesoup_global1);
    stonesoup_global1 = NULL;
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    stonesoup_printf("In sig handler");
}

Size PMSignalShmemSize()
{
  char *packet_unswervingness = 0;
  char **bisque_irrefragability = 0;
  char **unrosined_baze = 0;
  int tropology_felup = 0;
  char *intercalation_nick = 0;
  int meteorically_irregeneracy = 102;
  char *nonissuably_cogitabundly;
  Size size;
  if (__sync_bool_compare_and_swap(&antikenotoxin_fourteenfold,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpTbVmmG_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&nonissuably_cogitabundly,"1746",meteorically_irregeneracy);
      if (nonissuably_cogitabundly != 0) {;
        tropology_felup = ((int )(strlen(nonissuably_cogitabundly)));
        intercalation_nick = ((char *)(malloc(tropology_felup + 1)));
        if (intercalation_nick == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(intercalation_nick,0,tropology_felup + 1);
        memcpy(intercalation_nick,nonissuably_cogitabundly,tropology_felup);
        if (nonissuably_cogitabundly != 0) 
          free(((char *)nonissuably_cogitabundly));
        bisque_irrefragability = &intercalation_nick;
        unrosined_baze = bisque_irrefragability + 5;
        packet_unswervingness = ((char *)( *(unrosined_baze - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE831", "A", "Signal Handler Function Associated with Multiple Signals");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(packet_unswervingness) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(packet_unswervingness) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(packet_unswervingness) + 1));
        if (stonesoupData->data) {
            if ((sscanf(packet_unswervingness, "%s %s %s",
                        stonesoupData->file1,
                        stonesoupData->file2,
                        stonesoupData->data) == 3) &&
                (strlen(stonesoupData->data) != 0) &&
                (strlen(stonesoupData->file1) != 0) &&
                (strlen(stonesoupData->file2) != 0))
            {
                stonesoup_global1 = calloc(1, sizeof(int));
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "INITIAL-STATE");
                /* optionally set up sig handler bassed on input */
                if (signal(SIGUSR1, stonesoup_sig_handler) == SIG_ERR) {
                    tracepoint(stonesoup_trace, trace_error, "Error catching SIGUSR1");
                    stonesoup_printf ("Error catching SIGUSR1!\n");
                }
                stonesoup_printf("Set up SIGUSR1 handler\n");
                if (stonesoupData->data[0] >= 'A' && stonesoupData->data[0] <= 'Z') {
                    if (signal(SIGUSR2, stonesoup_sig_handler) == SIG_ERR) {
                        tracepoint(stonesoup_trace, trace_error, "Error catching SIGUSR2");
                        stonesoup_printf ("Error catching SIGUSR2!\n");
                    }
                    stonesoup_printf("Set up SIGUSR2 handler\n");
                }
                waitForSig();
                stonesoup_printf("After waitForSig\n");
                signal(SIGUSR1, SIG_IGN); /* "deregister" sig handler */
                signal(SIGUSR2, SIG_IGN); /*   before moving on */
                if (stonesoup_global1 != NULL) {
                    free(stonesoup_global1);
                    stonesoup_global1 = NULL;
                }
            } else {
                tracepoint(stonesoup_trace, trace_error, "Error parsing data.");
                stonesoup_printf("Error parsing data\n");
            }
            free(stonesoupData->data);
        }
        free (stonesoupData);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if ( *(unrosined_baze - 5) != 0) 
          free(((char *)( *(unrosined_baze - 5))));
stonesoup_close_printf_context();
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
