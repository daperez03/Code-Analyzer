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
#include <pthread.h> 
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
int loving_magnochromite = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *fullers_fustians);
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
void dodecaphonism_liasing(char **const soots_nonmotivation);
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmph7Mgtp_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmph7Mgtp_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "Finished reading sync file.");
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmph7Mgtp_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "get_instance");
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmph7Mgtp_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "stonesoup_print_data");
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmph7Mgtp_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c", "stonesoup_calc_data");
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

Size PMSignalShmemSize()
{
  Size size;
  if (__sync_bool_compare_and_swap(&loving_magnochromite,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmph7Mgtp_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
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

void stonesoup_handle_taint(char *fullers_fustians)
{
  char *minitrack_cornuting[93] = {0};
  ++stonesoup_global_variable;;
  if (fullers_fustians != 0) {;
    minitrack_cornuting[78] = fullers_fustians;
    dodecaphonism_liasing(minitrack_cornuting);
  }
}

void dodecaphonism_liasing(char **const soots_nonmotivation)
{
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data* stonesoupData;
  char *preescape_jebusitic = 0;
  jmp_buf aspergilloses_confustication;
  int manstopping_postiche;
  ++stonesoup_global_variable;;
  manstopping_postiche = setjmp(aspergilloses_confustication);
  if (manstopping_postiche == 0) {
    longjmp(aspergilloses_confustication,1);
  }
  preescape_jebusitic = ((char *)((char **)soots_nonmotivation)[78]);
    tracepoint(stonesoup_trace, weakness_start, "CWE543", "A", "Use of a Singleton Pattern Without Synchronization in a Multithreaded Context");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(preescape_jebusitic) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(preescape_jebusitic) + 1));
        if ((sscanf(preescape_jebusitic, "%d %s %s %d",
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
  if (((char **)soots_nonmotivation)[78] != 0) 
    free(((char *)((char **)soots_nonmotivation)[78]));
stonesoup_close_printf_context();
}
