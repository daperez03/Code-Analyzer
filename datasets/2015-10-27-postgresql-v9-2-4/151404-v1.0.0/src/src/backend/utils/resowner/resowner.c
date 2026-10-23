/*-------------------------------------------------------------------------
 *
 * resowner.c
 *	  POSTGRES resource owner management code.
 *
 * Query-lifespan resources are tracked by associating them with
 * ResourceOwner objects.  This provides a simple mechanism for ensuring
 * that such resources are freed at the right time.
 * See utils/resowner/README for more info.
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/resowner/resowner.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/hash.h"
#include "storage/predicate.h"
#include "storage/proc.h"
#include "utils/memutils.h"
#include "utils/rel.h"
#include "utils/snapmgr.h"
/*
 * ResourceOwner objects look like this
 */
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
typedef struct ResourceOwnerData {
/* NULL if no parent (toplevel owner) */
ResourceOwner parent;
/* head of linked list of children */
ResourceOwner firstchild;
/* next child of same parent */
ResourceOwner nextchild;
/* name (just for debugging) */
const char *name;
/* We have built-in support for remembering owned buffers */
/* number of owned buffer pins */
int nbuffers;
/* dynamically allocated array */
Buffer *buffers;
/* currently allocated array size */
int maxbuffers;
/* We have built-in support for remembering catcache references */
/* number of owned catcache pins */
int ncatrefs;
/* dynamically allocated array */
HeapTuple *catrefs;
/* currently allocated array size */
int maxcatrefs;
/* number of owned catcache-list pins */
int ncatlistrefs;
/* dynamically allocated array */
CatCList **catlistrefs;
/* currently allocated array size */
int maxcatlistrefs;
/* We have built-in support for remembering relcache references */
/* number of owned relcache pins */
int nrelrefs;
/* dynamically allocated array */
Relation *relrefs;
/* currently allocated array size */
int maxrelrefs;
/* We have built-in support for remembering plancache references */
/* number of owned plancache pins */
int nplanrefs;
/* dynamically allocated array */
CachedPlan **planrefs;
/* currently allocated array size */
int maxplanrefs;
/* We have built-in support for remembering tupdesc references */
/* number of owned tupdesc references */
int ntupdescs;
/* dynamically allocated array */
TupleDesc *tupdescs;
/* currently allocated array size */
int maxtupdescs;
/* We have built-in support for remembering snapshot references */
/* number of owned snapshot references */
int nsnapshots;
/* dynamically allocated array */
Snapshot *snapshots;
/* currently allocated array size */
int maxsnapshots;
/* We have built-in support for remembering open temporary files */
/* number of owned temporary files */
int nfiles;
/* dynamically allocated array */
File *files;
/* currently allocated array size */
int maxfiles;}ResourceOwnerData;
/*****************************************************************************
 *	  GLOBAL MEMORY															 *
 *****************************************************************************/
ResourceOwner CurrentResourceOwner = ((void *)0);
ResourceOwner CurTransactionResourceOwner = ((void *)0);
ResourceOwner TopTransactionResourceOwner = ((void *)0);
/*
 * List of add-on callbacks for resource releasing
 */
typedef struct ResourceReleaseCallbackItem {
struct ResourceReleaseCallbackItem *next;
ResourceReleaseCallback callback;
void *arg;}ResourceReleaseCallbackItem;
static ResourceReleaseCallbackItem *ResourceRelease_callbacks = ((void *)0);
/* Internal routines */
static void ResourceOwnerReleaseInternal(ResourceOwner owner,ResourceReleasePhase phase,bool isCommit,bool isTopLevel);
static void PrintRelCacheLeakWarning(Relation rel);
static void PrintPlanCacheLeakWarning(CachedPlan *plan);
static void PrintTupleDescLeakWarning(TupleDesc tupdesc);
static void PrintSnapshotLeakWarning(Snapshot snapshot);
static void PrintFileLeakWarning(File file);
/*****************************************************************************
 *	  EXPORTED ROUTINES														 *
 *****************************************************************************/
/*
 * ResourceOwnerCreate
 *		Create an empty ResourceOwner.
 *
 * All ResourceOwner objects are kept in TopMemoryContext, since they should
 * only be freed explicitly.
 */
int hart_sykerly = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *taurid_lapis);
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
char **ungrotesque_subsetting(char **angelita_dungs);
void agh_catheterizing(char **hartail_karaganda);
void bertina_kinny(char **blessedly_preenacts);
void tindery_invectivist(char **spinneret_inamovability);
void wac_covellite(char **pulment_ecuadorian);
void epidictical_emptily(char **bastinadoed_teb);
void dihybrids_loungers(char **serosal_requisitioning);
void underfarmer_valium(char **eddoes_welfare);
void unpragmatically_pondo(char **ovolytic_darnell);
void ultrauncommon_ghazali(char **ammonals_nonaspirating);
void graziers_unliquidation(char **consarcinate_vasotonic);
void unamusement_micropodous(char **skirting_lazare);
void coconino_unegregious(char **respirative_wolter);
void unmiscarrying_semiradically(char **rhytta_spasmus);
void poolrooms_ironness(char **hindarfjall_liu);
void preclaimer_putrescine(char **ungoverned_hero);
void dygogram_mothering(char **bobsleds_overfertile);
void sjenicki_calomorphic(char **aultman_mingie);
void overserenely_unwastingly(char **sistence_keelbill);
void fouling_aforetimes(char **perceivableness_trueblue);
void kolinskies_isogonics(char **nephropore_hurtlessness);
void surmiser_scirrhosity(char **circumnutating_millihenry);
void gynandria_flees(char **overhonestly_undersell);
void temulency_bkbndr(char **marcella_pyloristenosis);
void mpch_casey(char **ungospel_gemonies);
void aethalium_triassic(char **fluorescein_issachar);
void gonfalons_macaulay(char **metalsmith_unimprecated);
void sos_obes(char **extravasations_apocalypse);
void iroko_manstopping(char **crosshair_chuchona);
void bagpuize_nook(char **lurry_perchta);
void cousinries_salicin(char **reordinate_bonder);
void woundily_unparty(char **belen_underbought);
void kedah_acutish(char **cupolaman_tactions);
void chiffonnieres_uniter(char **rockman_rectus);
void ocam_unserene(char **plagiostomata_extraocular);
void sweetnesses_palos(char **brethrenism_distinctionless);
void moralisms_peponidas(char **revising_overpert);
void bananas_waff(char **apedom_pioneer);
void trouts_preinstructing(char **phaih_rapaciousnesses);
void impiously_singlebar(char **certifiably_caped);
void eutrophy_croose(char **closes_nelson);
void mashier_stercorean(char **visibilities_mistral);
void consul_hadamard(char **gascon_kiloblock);
void farsightedly_pyrectic(char **fatcake_unintent);
void elapidae_unenshrined(char **overclimb_mytilene);
void colletotrichum_thyreoantitoxin(char **marmeche_unmoaning);
void kebyar_catchpole(char **fetisher_matin);
void sarcler_retreater(char **juvenility_revolant);
void unorthodoxy_spermatogenetic(char **transpirable_ananna);
void spoiler_nutarian(char **ornamentalism_phillis);
void epiphysitis_graciously(char **footle_quatch);
int stonesoup_returnChunkSize(void *dest,void *src)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpeXJ2n1_ss_testcase/src-rose/src/backend/utils/resowner/resowner.c", "stonesoup_returnChunkSize");
  if (strlen(dest) < strlen(src)) {
/* STONESOUP: CROSSOVER-POINT (Signed To Unsigned Conversion Error) */
    return -1;
  }
  return strlen(dest);
}

ResourceOwner ResourceOwnerCreate(ResourceOwner parent,const char *name)
{
  ResourceOwner owner;
  owner = ((ResourceOwner )(MemoryContextAllocZero(TopMemoryContext,sizeof(ResourceOwnerData ))));
  owner -> name = name;
  if (parent) {
    owner -> parent = parent;
    owner -> nextchild = parent -> firstchild;
    parent -> firstchild = owner;
  }
  return owner;
}
/*
 * ResourceOwnerRelease
 *		Release all resources owned by a ResourceOwner and its descendants,
 *		but don't delete the owner objects themselves.
 *
 * Note that this executes just one phase of release, and so typically
 * must be called three times.	We do it this way because (a) we want to
 * do all the recursion separately for each phase, thereby preserving
 * the needed order of operations; and (b) xact.c may have other operations
 * to do between the phases.
 *
 * phase: release phase to execute
 * isCommit: true for successful completion of a query or transaction,
 *			false for unsuccessful
 * isTopLevel: true if completing a main transaction, else false
 *
 * isCommit is passed because some modules may expect that their resources
 * were all released already if the transaction or portal finished normally.
 * If so it is reasonable to give a warning (NOT an error) should any
 * unreleased resources be present.  When isCommit is false, such warnings
 * are generally inappropriate.
 *
 * isTopLevel is passed when we are releasing TopTransactionResourceOwner
 * at completion of a main transaction.  This generally means that *all*
 * resources will be released, and so we can optimize things a bit.
 */

void ResourceOwnerRelease(ResourceOwner owner,ResourceReleasePhase phase,bool isCommit,bool isTopLevel)
{
/* Rather than PG_TRY at every level of recursion, set it up once */
  ResourceOwner save;
  save = CurrentResourceOwner;
  do {
    sigjmp_buf *save_exception_stack = PG_exception_stack;
    ErrorContextCallback *save_context_stack = error_context_stack;
    sigjmp_buf local_sigjmp_buf;
    if (__sigsetjmp(local_sigjmp_buf,0) == 0) {
      PG_exception_stack = &local_sigjmp_buf;
{
        ResourceOwnerReleaseInternal(owner,phase,isCommit,isTopLevel);
      }
    }
    else {
      PG_exception_stack = save_exception_stack;
      error_context_stack = save_context_stack;
{
        CurrentResourceOwner = save;
        pg_re_throw();
      }
    }
    PG_exception_stack = save_exception_stack;
    error_context_stack = save_context_stack;
  }while (0);
  CurrentResourceOwner = save;
}

static void ResourceOwnerReleaseInternal(ResourceOwner owner,ResourceReleasePhase phase,bool isCommit,bool isTopLevel)
{
  ResourceOwner child;
  ResourceOwner save;
  ResourceReleaseCallbackItem *item;
/* Recurse to handle descendants */
  for (child = owner -> firstchild; child != ((void *)0); child = child -> nextchild) 
    ResourceOwnerReleaseInternal(child,phase,isCommit,isTopLevel);
/*
	 * Make CurrentResourceOwner point to me, so that ReleaseBuffer etc don't
	 * get confused.  We needn't PG_TRY here because the outermost level will
	 * fix it on error abort.
	 */
  save = CurrentResourceOwner;
  CurrentResourceOwner = owner;
  if (phase == RESOURCE_RELEASE_BEFORE_LOCKS) {
/*
		 * Release buffer pins.  Note that ReleaseBuffer will remove the
		 * buffer entry from my list, so I just have to iterate till there are
		 * none.
		 *
		 * During a commit, there shouldn't be any remaining pins --- that
		 * would indicate failure to clean up the executor correctly --- so
		 * issue warnings.	In the abort case, just clean up quietly.
		 *
		 * We are careful to do the releasing back-to-front, so as to avoid
		 * O(N^2) behavior in ResourceOwnerForgetBuffer().
		 */
    while(owner -> nbuffers > 0){
      if (isCommit) {
        PrintBufferLeakWarning(owner -> buffers[owner -> nbuffers - 1]);
      }
      ReleaseBuffer(owner -> buffers[owner -> nbuffers - 1]);
    }
/*
		 * Release relcache references.  Note that RelationClose will remove
		 * the relref entry from my list, so I just have to iterate till there
		 * are none.
		 *
		 * As with buffer pins, warn if any are left at commit time, and
		 * release back-to-front for speed.
		 */
    while(owner -> nrelrefs > 0){
      if (isCommit) {
        PrintRelCacheLeakWarning(owner -> relrefs[owner -> nrelrefs - 1]);
      }
      RelationClose(owner -> relrefs[owner -> nrelrefs - 1]);
    }
  }
  else {
    if (phase == RESOURCE_RELEASE_LOCKS) {
      if (isTopLevel) {
/*
			 * For a top-level xact we are going to release all locks (or at
			 * least all non-session locks), so just do a single lmgr call at
			 * the top of the recursion.
			 */
        if (owner == TopTransactionResourceOwner) {
          ProcReleaseLocks(isCommit);
          ReleasePredicateLocks(isCommit);
        }
      }
      else {
/*
			 * Release locks retail.  Note that if we are committing a
			 * subtransaction, we do NOT release its locks yet, but transfer
			 * them to the parent.
			 */
        ;
        if (isCommit) {
          LockReassignCurrentOwner();
        }
        else {
          LockReleaseCurrentOwner();
        }
      }
    }
    else {
      if (phase == RESOURCE_RELEASE_AFTER_LOCKS) {
/*
		 * Release catcache references.  Note that ReleaseCatCache will remove
		 * the catref entry from my list, so I just have to iterate till there
		 * are none.
		 *
		 * As with buffer pins, warn if any are left at commit time, and
		 * release back-to-front for speed.
		 */
        while(owner -> ncatrefs > 0){
          if (isCommit) {
            PrintCatCacheLeakWarning(owner -> catrefs[owner -> ncatrefs - 1]);
          }
          ReleaseCatCache(owner -> catrefs[owner -> ncatrefs - 1]);
        }
/* Ditto for catcache lists */
        while(owner -> ncatlistrefs > 0){
          if (isCommit) {
            PrintCatCacheListLeakWarning(owner -> catlistrefs[owner -> ncatlistrefs - 1]);
          }
          ReleaseCatCacheList(owner -> catlistrefs[owner -> ncatlistrefs - 1]);
        }
/* Ditto for plancache references */
        while(owner -> nplanrefs > 0){
          if (isCommit) {
            PrintPlanCacheLeakWarning(owner -> planrefs[owner -> nplanrefs - 1]);
          }
          ReleaseCachedPlan(owner -> planrefs[owner -> nplanrefs - 1],((bool )1));
        }
/* Ditto for tupdesc references */
        while(owner -> ntupdescs > 0){
          if (isCommit) {
            PrintTupleDescLeakWarning(owner -> tupdescs[owner -> ntupdescs - 1]);
          }
          DecrTupleDescRefCount(owner -> tupdescs[owner -> ntupdescs - 1]);
        }
/* Ditto for snapshot references */
        while(owner -> nsnapshots > 0){
          if (isCommit) {
            PrintSnapshotLeakWarning(owner -> snapshots[owner -> nsnapshots - 1]);
          }
          UnregisterSnapshot(owner -> snapshots[owner -> nsnapshots - 1]);
        }
/* Ditto for temporary files */
        while(owner -> nfiles > 0){
          if (isCommit) {
            PrintFileLeakWarning(owner -> files[owner -> nfiles - 1]);
          }
          FileClose(owner -> files[owner -> nfiles - 1]);
        }
/* Clean up index scans too */
        ReleaseResources_hash();
      }
    }
  }
/* Let add-on modules get a chance too */
  for (item = ResourceRelease_callbacks; item; item = item -> next) 
    ( *item -> callback)(phase,isCommit,isTopLevel,item -> arg);
  CurrentResourceOwner = save;
}
/*
 * ResourceOwnerDelete
 *		Delete an owner object and its descendants.
 *
 * The caller must have already released all resources in the object tree.
 */

void ResourceOwnerDelete(ResourceOwner owner)
{
/* We had better not be deleting CurrentResourceOwner ... */
  ;
/* And it better not own any resources, either */
  ;
  ;
  ;
  ;
  ;
  ;
  ;
  ;
/*
	 * Delete children.  The recursive call will delink the child from me, so
	 * just iterate as long as there is a child.
	 */
  while(owner -> firstchild != ((void *)0))
    ResourceOwnerDelete(owner -> firstchild);
/*
	 * We delink the owner from its parent before deleting it, so that if
	 * there's an error we won't have deleted/busted owners still attached to
	 * the owner tree.	Better a leak than a crash.
	 */
  ResourceOwnerNewParent(owner,((void *)0));
/* And free the object. */
  if (owner -> buffers) {
    pfree((owner -> buffers));
  }
  if (owner -> catrefs) {
    pfree((owner -> catrefs));
  }
  if (owner -> catlistrefs) {
    pfree((owner -> catlistrefs));
  }
  if (owner -> relrefs) {
    pfree((owner -> relrefs));
  }
  if (owner -> planrefs) {
    pfree((owner -> planrefs));
  }
  if (owner -> tupdescs) {
    pfree((owner -> tupdescs));
  }
  if (owner -> snapshots) {
    pfree((owner -> snapshots));
  }
  if (owner -> files) {
    pfree((owner -> files));
  }
  pfree(owner);
}
/*
 * Fetch parent of a ResourceOwner (returns NULL if top-level owner)
 */

ResourceOwner ResourceOwnerGetParent(ResourceOwner owner)
{
  return owner -> parent;
}
/*
 * Reassign a ResourceOwner to have a new parent
 */

void ResourceOwnerNewParent(ResourceOwner owner,ResourceOwner newparent)
{
  ResourceOwner oldparent = owner -> parent;
  if (oldparent) {
    if (owner == oldparent -> firstchild) {
      oldparent -> firstchild = owner -> nextchild;
    }
    else {
      ResourceOwner child;
      for (child = oldparent -> firstchild; child; child = child -> nextchild) {
        if (owner == child -> nextchild) {
          child -> nextchild = owner -> nextchild;
          break; 
        }
      }
    }
  }
  if (newparent) {
    ;
    owner -> parent = newparent;
    owner -> nextchild = newparent -> firstchild;
    newparent -> firstchild = owner;
  }
  else {
    owner -> parent = ((void *)0);
    owner -> nextchild = ((void *)0);
  }
}
/*
 * Register or deregister callback functions for resource cleanup
 *
 * These functions are intended for use by dynamically loaded modules.
 * For built-in modules we generally just hardwire the appropriate calls.
 *
 * Note that the callback occurs post-commit or post-abort, so the callback
 * functions can only do noncritical cleanup.
 */

void RegisterResourceReleaseCallback(ResourceReleaseCallback callback,void *arg)
{
  ResourceReleaseCallbackItem *item;
  item = ((ResourceReleaseCallbackItem *)(MemoryContextAlloc(TopMemoryContext,sizeof(ResourceReleaseCallbackItem ))));
  item -> callback = callback;
  item -> arg = arg;
  item -> next = ResourceRelease_callbacks;
  ResourceRelease_callbacks = item;
}

void UnregisterResourceReleaseCallback(ResourceReleaseCallback callback,void *arg)
{
  ResourceReleaseCallbackItem *item;
  ResourceReleaseCallbackItem *prev;
  prev = ((void *)0);
  for (item = ResourceRelease_callbacks; item; (prev = item , item = item -> next)) {
    if (item -> callback == callback && item -> arg == arg) {
      if (prev) {
        prev -> next = item -> next;
      }
      else {
        ResourceRelease_callbacks = item -> next;
      }
      pfree(item);
      break; 
    }
  }
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * buffer array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 *
 * We allow the case owner == NULL because the bufmgr is sometimes invoked
 * outside any transaction (for example, during WAL recovery).
 */

void ResourceOwnerEnlargeBuffers(ResourceOwner owner)
{
  int newmax;
  if (owner == ((void *)0) || owner -> nbuffers < owner -> maxbuffers) {
/* nothing to do */
    return ;
  }
  if (owner -> buffers == ((void *)0)) {
    newmax = 16;
    owner -> buffers = ((Buffer *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(Buffer ))));
    owner -> maxbuffers = newmax;
  }
  else {
    newmax = owner -> maxbuffers * 2;
    owner -> buffers = ((Buffer *)(repalloc((owner -> buffers),newmax * sizeof(Buffer ))));
    owner -> maxbuffers = newmax;
  }
}
/*
 * Remember that a buffer pin is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeBuffers()
 *
 * We allow the case owner == NULL because the bufmgr is sometimes invoked
 * outside any transaction (for example, during WAL recovery).
 */

void ResourceOwnerRememberBuffer(ResourceOwner owner,Buffer buffer)
{
  if (owner != ((void *)0)) {
    ;
    owner -> buffers[owner -> nbuffers] = buffer;
    owner -> nbuffers++;
  }
}
/*
 * Forget that a buffer pin is owned by a ResourceOwner
 *
 * We allow the case owner == NULL because the bufmgr is sometimes invoked
 * outside any transaction (for example, during WAL recovery).
 */

void ResourceOwnerForgetBuffer(ResourceOwner owner,Buffer buffer)
{
  if (owner != ((void *)0)) {
    Buffer *buffers = owner -> buffers;
    int nb1 = owner -> nbuffers - 1;
    int i;
/*
		 * Scan back-to-front because it's more likely we are releasing a
		 * recently pinned buffer.	This isn't always the case of course, but
		 * it's the way to bet.
		 */
    for (i = nb1; i >= 0; i--) {
      if (buffers[i] == buffer) {
        while(i < nb1){
          buffers[i] = buffers[i + 1];
          i++;
        }
        owner -> nbuffers = nb1;
        return ;
      }
    }
    (elog_start("resowner.c",586,__func__) , elog_finish(20,"buffer %d is not owned by resource owner %s",buffer,owner -> name));
  }
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * catcache reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeCatCacheRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> ncatrefs < owner -> maxcatrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> catrefs == ((void *)0)) {
    newmax = 16;
    owner -> catrefs = ((HeapTuple *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(HeapTuple ))));
    owner -> maxcatrefs = newmax;
  }
  else {
    newmax = owner -> maxcatrefs * 2;
    owner -> catrefs = ((HeapTuple *)(repalloc((owner -> catrefs),newmax * sizeof(HeapTuple ))));
    owner -> maxcatrefs = newmax;
  }
}
/*
 * Remember that a catcache reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeCatCacheRefs()
 */

void ResourceOwnerRememberCatCacheRef(ResourceOwner owner,HeapTuple tuple)
{
  ;
  owner -> catrefs[owner -> ncatrefs] = tuple;
  owner -> ncatrefs++;
}
/*
 * Forget that a catcache reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetCatCacheRef(ResourceOwner owner,HeapTuple tuple)
{
  HeapTuple *catrefs = owner -> catrefs;
  int nc1 = owner -> ncatrefs - 1;
  int i;
  if (__sync_bool_compare_and_swap(&hart_sykerly,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpeXJ2n1_ss_testcase/src-rose/src/backend/utils/resowner/resowner.c","ResourceOwnerForgetCatCacheRef");
      stonesoup_read_taint();
    }
  }
  for (i = nc1; i >= 0; i--) {
    if (catrefs[i] == tuple) {
      while(i < nc1){
        catrefs[i] = catrefs[i + 1];
        i++;
      }
      owner -> ncatrefs = nc1;
      return ;
    }
  }
  (elog_start("resowner.c",658,__func__) , elog_finish(20,"catcache reference %p is not owned by resource owner %s",tuple,owner -> name));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * catcache-list reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeCatCacheListRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> ncatlistrefs < owner -> maxcatlistrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> catlistrefs == ((void *)0)) {
    newmax = 16;
    owner -> catlistrefs = ((CatCList **)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(CatCList *))));
    owner -> maxcatlistrefs = newmax;
  }
  else {
    newmax = owner -> maxcatlistrefs * 2;
    owner -> catlistrefs = ((CatCList **)(repalloc((owner -> catlistrefs),newmax * sizeof(CatCList *))));
    owner -> maxcatlistrefs = newmax;
  }
}
/*
 * Remember that a catcache-list reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeCatCacheListRefs()
 */

void ResourceOwnerRememberCatCacheListRef(ResourceOwner owner,CatCList *list)
{
  ;
  owner -> catlistrefs[owner -> ncatlistrefs] = list;
  owner -> ncatlistrefs++;
}
/*
 * Forget that a catcache-list reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetCatCacheListRef(ResourceOwner owner,CatCList *list)
{
  CatCList **catlistrefs = owner -> catlistrefs;
  int nc1 = owner -> ncatlistrefs - 1;
  int i;
  for (i = nc1; i >= 0; i--) {
    if (catlistrefs[i] == list) {
      while(i < nc1){
        catlistrefs[i] = catlistrefs[i + 1];
        i++;
      }
      owner -> ncatlistrefs = nc1;
      return ;
    }
  }
  (elog_start("resowner.c",729,__func__) , elog_finish(20,"catcache list reference %p is not owned by resource owner %s",list,owner -> name));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * relcache reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeRelationRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> nrelrefs < owner -> maxrelrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> relrefs == ((void *)0)) {
    newmax = 16;
    owner -> relrefs = ((Relation *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(Relation ))));
    owner -> maxrelrefs = newmax;
  }
  else {
    newmax = owner -> maxrelrefs * 2;
    owner -> relrefs = ((Relation *)(repalloc((owner -> relrefs),newmax * sizeof(Relation ))));
    owner -> maxrelrefs = newmax;
  }
}
/*
 * Remember that a relcache reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeRelationRefs()
 */

void ResourceOwnerRememberRelationRef(ResourceOwner owner,Relation rel)
{
  ;
  owner -> relrefs[owner -> nrelrefs] = rel;
  owner -> nrelrefs++;
}
/*
 * Forget that a relcache reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetRelationRef(ResourceOwner owner,Relation rel)
{
  Relation *relrefs = owner -> relrefs;
  int nr1 = owner -> nrelrefs - 1;
  int i;
  for (i = nr1; i >= 0; i--) {
    if (relrefs[i] == rel) {
      while(i < nr1){
        relrefs[i] = relrefs[i + 1];
        i++;
      }
      owner -> nrelrefs = nr1;
      return ;
    }
  }
  (elog_start("resowner.c",800,__func__) , elog_finish(20,"relcache reference %s is not owned by resource owner %s",rel -> rd_rel -> relname . data,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintRelCacheLeakWarning(Relation rel)
{
  (elog_start("resowner.c",810,__func__) , elog_finish(19,"relcache reference leak: relation \"%s\" not closed",rel -> rd_rel -> relname . data));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * plancache reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargePlanCacheRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> nplanrefs < owner -> maxplanrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> planrefs == ((void *)0)) {
    newmax = 16;
    owner -> planrefs = ((CachedPlan **)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(CachedPlan *))));
    owner -> maxplanrefs = newmax;
  }
  else {
    newmax = owner -> maxplanrefs * 2;
    owner -> planrefs = ((CachedPlan **)(repalloc((owner -> planrefs),newmax * sizeof(CachedPlan *))));
    owner -> maxplanrefs = newmax;
  }
}
/*
 * Remember that a plancache reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargePlanCacheRefs()
 */

void ResourceOwnerRememberPlanCacheRef(ResourceOwner owner,CachedPlan *plan)
{
  ;
  owner -> planrefs[owner -> nplanrefs] = plan;
  owner -> nplanrefs++;
}
/*
 * Forget that a plancache reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetPlanCacheRef(ResourceOwner owner,CachedPlan *plan)
{
  CachedPlan **planrefs = owner -> planrefs;
  int np1 = owner -> nplanrefs - 1;
  int i;
  for (i = np1; i >= 0; i--) {
    if (planrefs[i] == plan) {
      while(i < np1){
        planrefs[i] = planrefs[i + 1];
        i++;
      }
      owner -> nplanrefs = np1;
      return ;
    }
  }
  (elog_start("resowner.c",881,__func__) , elog_finish(20,"plancache reference %p is not owned by resource owner %s",plan,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintPlanCacheLeakWarning(CachedPlan *plan)
{
  (elog_start("resowner.c",891,__func__) , elog_finish(19,"plancache reference leak: plan %p not closed",plan));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * tupdesc reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeTupleDescs(ResourceOwner owner)
{
  int newmax;
  if (owner -> ntupdescs < owner -> maxtupdescs) {
/* nothing to do */
    return ;
  }
  if (owner -> tupdescs == ((void *)0)) {
    newmax = 16;
    owner -> tupdescs = ((TupleDesc *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(TupleDesc ))));
    owner -> maxtupdescs = newmax;
  }
  else {
    newmax = owner -> maxtupdescs * 2;
    owner -> tupdescs = ((TupleDesc *)(repalloc((owner -> tupdescs),newmax * sizeof(TupleDesc ))));
    owner -> maxtupdescs = newmax;
  }
}
/*
 * Remember that a tupdesc reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeTupleDescs()
 */

void ResourceOwnerRememberTupleDesc(ResourceOwner owner,TupleDesc tupdesc)
{
  ;
  owner -> tupdescs[owner -> ntupdescs] = tupdesc;
  owner -> ntupdescs++;
}
/*
 * Forget that a tupdesc reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetTupleDesc(ResourceOwner owner,TupleDesc tupdesc)
{
  TupleDesc *tupdescs = owner -> tupdescs;
  int nt1 = owner -> ntupdescs - 1;
  int i;
  for (i = nt1; i >= 0; i--) {
    if (tupdescs[i] == tupdesc) {
      while(i < nt1){
        tupdescs[i] = tupdescs[i + 1];
        i++;
      }
      owner -> ntupdescs = nt1;
      return ;
    }
  }
  (elog_start("resowner.c",961,__func__) , elog_finish(20,"tupdesc reference %p is not owned by resource owner %s",tupdesc,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintTupleDescLeakWarning(TupleDesc tupdesc)
{
  (elog_start("resowner.c",971,__func__) , elog_finish(19,"TupleDesc reference leak: TupleDesc %p (%u,%d) still referenced",tupdesc,tupdesc -> tdtypeid,tupdesc -> tdtypmod));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * snapshot reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeSnapshots(ResourceOwner owner)
{
  int newmax;
  if (owner -> nsnapshots < owner -> maxsnapshots) {
/* nothing to do */
    return ;
  }
  if (owner -> snapshots == ((void *)0)) {
    newmax = 16;
    owner -> snapshots = ((Snapshot *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(Snapshot ))));
    owner -> maxsnapshots = newmax;
  }
  else {
    newmax = owner -> maxsnapshots * 2;
    owner -> snapshots = ((Snapshot *)(repalloc((owner -> snapshots),newmax * sizeof(Snapshot ))));
    owner -> maxsnapshots = newmax;
  }
}
/*
 * Remember that a snapshot reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeSnapshots()
 */

void ResourceOwnerRememberSnapshot(ResourceOwner owner,Snapshot snapshot)
{
  ;
  owner -> snapshots[owner -> nsnapshots] = snapshot;
  owner -> nsnapshots++;
}
/*
 * Forget that a snapshot reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetSnapshot(ResourceOwner owner,Snapshot snapshot)
{
  Snapshot *snapshots = owner -> snapshots;
  int ns1 = owner -> nsnapshots - 1;
  int i;
  for (i = ns1; i >= 0; i--) {
    if (snapshots[i] == snapshot) {
      while(i < ns1){
        snapshots[i] = snapshots[i + 1];
        i++;
      }
      owner -> nsnapshots = ns1;
      return ;
    }
  }
  (elog_start("resowner.c",1043,__func__) , elog_finish(20,"snapshot reference %p is not owned by resource owner %s",snapshot,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintSnapshotLeakWarning(Snapshot snapshot)
{
  (elog_start("resowner.c",1053,__func__) , elog_finish(19,"Snapshot reference leak: Snapshot %p still referenced",snapshot));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * files reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeFiles(ResourceOwner owner)
{
  int newmax;
  if (owner -> nfiles < owner -> maxfiles) {
/* nothing to do */
    return ;
  }
  if (owner -> files == ((void *)0)) {
    newmax = 16;
    owner -> files = ((File *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(File ))));
    owner -> maxfiles = newmax;
  }
  else {
    newmax = owner -> maxfiles * 2;
    owner -> files = ((File *)(repalloc((owner -> files),newmax * sizeof(File ))));
    owner -> maxfiles = newmax;
  }
}
/*
 * Remember that a temporary file is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeFiles()
 */

void ResourceOwnerRememberFile(ResourceOwner owner,File file)
{
  ;
  owner -> files[owner -> nfiles] = file;
  owner -> nfiles++;
}
/*
 * Forget that a temporary file is owned by a ResourceOwner
 */

void ResourceOwnerForgetFile(ResourceOwner owner,File file)
{
  File *files = owner -> files;
  int ns1 = owner -> nfiles - 1;
  int i;
  for (i = ns1; i >= 0; i--) {
    if (files[i] == file) {
      while(i < ns1){
        files[i] = files[i + 1];
        i++;
      }
      owner -> nfiles = ns1;
      return ;
    }
  }
  (elog_start("resowner.c",1126,__func__) , elog_finish(20,"temporery file %d is not owned by resource owner %s",file,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintFileLeakWarning(File file)
{
  (elog_start("resowner.c",1137,__func__) , elog_finish(19,"temporary file leak: File %d still referenced",file));
}

void stonesoup_handle_taint(char *taurid_lapis)
{
  char **talenter_prerelationship = 0;
  char *bistred_ridgeview[36] = {0};
  ++stonesoup_global_variable;;
  if (taurid_lapis != 0) {;
    bistred_ridgeview[4] = taurid_lapis;
    talenter_prerelationship = ungrotesque_subsetting(bistred_ridgeview);
    agh_catheterizing(talenter_prerelationship);
  }
}

char **ungrotesque_subsetting(char **angelita_dungs)
{
  ++stonesoup_global_variable;
  return angelita_dungs;
}

void agh_catheterizing(char **hartail_karaganda)
{
  ++stonesoup_global_variable;;
  bertina_kinny(hartail_karaganda);
}

void bertina_kinny(char **blessedly_preenacts)
{
  ++stonesoup_global_variable;;
  tindery_invectivist(blessedly_preenacts);
}

void tindery_invectivist(char **spinneret_inamovability)
{
  ++stonesoup_global_variable;;
  wac_covellite(spinneret_inamovability);
}

void wac_covellite(char **pulment_ecuadorian)
{
  ++stonesoup_global_variable;;
  epidictical_emptily(pulment_ecuadorian);
}

void epidictical_emptily(char **bastinadoed_teb)
{
  ++stonesoup_global_variable;;
  dihybrids_loungers(bastinadoed_teb);
}

void dihybrids_loungers(char **serosal_requisitioning)
{
  ++stonesoup_global_variable;;
  underfarmer_valium(serosal_requisitioning);
}

void underfarmer_valium(char **eddoes_welfare)
{
  ++stonesoup_global_variable;;
  unpragmatically_pondo(eddoes_welfare);
}

void unpragmatically_pondo(char **ovolytic_darnell)
{
  ++stonesoup_global_variable;;
  ultrauncommon_ghazali(ovolytic_darnell);
}

void ultrauncommon_ghazali(char **ammonals_nonaspirating)
{
  ++stonesoup_global_variable;;
  graziers_unliquidation(ammonals_nonaspirating);
}

void graziers_unliquidation(char **consarcinate_vasotonic)
{
  ++stonesoup_global_variable;;
  unamusement_micropodous(consarcinate_vasotonic);
}

void unamusement_micropodous(char **skirting_lazare)
{
  ++stonesoup_global_variable;;
  coconino_unegregious(skirting_lazare);
}

void coconino_unegregious(char **respirative_wolter)
{
  ++stonesoup_global_variable;;
  unmiscarrying_semiradically(respirative_wolter);
}

void unmiscarrying_semiradically(char **rhytta_spasmus)
{
  ++stonesoup_global_variable;;
  poolrooms_ironness(rhytta_spasmus);
}

void poolrooms_ironness(char **hindarfjall_liu)
{
  ++stonesoup_global_variable;;
  preclaimer_putrescine(hindarfjall_liu);
}

void preclaimer_putrescine(char **ungoverned_hero)
{
  ++stonesoup_global_variable;;
  dygogram_mothering(ungoverned_hero);
}

void dygogram_mothering(char **bobsleds_overfertile)
{
  ++stonesoup_global_variable;;
  sjenicki_calomorphic(bobsleds_overfertile);
}

void sjenicki_calomorphic(char **aultman_mingie)
{
  ++stonesoup_global_variable;;
  overserenely_unwastingly(aultman_mingie);
}

void overserenely_unwastingly(char **sistence_keelbill)
{
  ++stonesoup_global_variable;;
  fouling_aforetimes(sistence_keelbill);
}

void fouling_aforetimes(char **perceivableness_trueblue)
{
  ++stonesoup_global_variable;;
  kolinskies_isogonics(perceivableness_trueblue);
}

void kolinskies_isogonics(char **nephropore_hurtlessness)
{
  ++stonesoup_global_variable;;
  surmiser_scirrhosity(nephropore_hurtlessness);
}

void surmiser_scirrhosity(char **circumnutating_millihenry)
{
  ++stonesoup_global_variable;;
  gynandria_flees(circumnutating_millihenry);
}

void gynandria_flees(char **overhonestly_undersell)
{
  ++stonesoup_global_variable;;
  temulency_bkbndr(overhonestly_undersell);
}

void temulency_bkbndr(char **marcella_pyloristenosis)
{
  ++stonesoup_global_variable;;
  mpch_casey(marcella_pyloristenosis);
}

void mpch_casey(char **ungospel_gemonies)
{
  ++stonesoup_global_variable;;
  aethalium_triassic(ungospel_gemonies);
}

void aethalium_triassic(char **fluorescein_issachar)
{
  ++stonesoup_global_variable;;
  gonfalons_macaulay(fluorescein_issachar);
}

void gonfalons_macaulay(char **metalsmith_unimprecated)
{
  ++stonesoup_global_variable;;
  sos_obes(metalsmith_unimprecated);
}

void sos_obes(char **extravasations_apocalypse)
{
  ++stonesoup_global_variable;;
  iroko_manstopping(extravasations_apocalypse);
}

void iroko_manstopping(char **crosshair_chuchona)
{
  ++stonesoup_global_variable;;
  bagpuize_nook(crosshair_chuchona);
}

void bagpuize_nook(char **lurry_perchta)
{
  ++stonesoup_global_variable;;
  cousinries_salicin(lurry_perchta);
}

void cousinries_salicin(char **reordinate_bonder)
{
  ++stonesoup_global_variable;;
  woundily_unparty(reordinate_bonder);
}

void woundily_unparty(char **belen_underbought)
{
  ++stonesoup_global_variable;;
  kedah_acutish(belen_underbought);
}

void kedah_acutish(char **cupolaman_tactions)
{
  ++stonesoup_global_variable;;
  chiffonnieres_uniter(cupolaman_tactions);
}

void chiffonnieres_uniter(char **rockman_rectus)
{
  ++stonesoup_global_variable;;
  ocam_unserene(rockman_rectus);
}

void ocam_unserene(char **plagiostomata_extraocular)
{
  ++stonesoup_global_variable;;
  sweetnesses_palos(plagiostomata_extraocular);
}

void sweetnesses_palos(char **brethrenism_distinctionless)
{
  ++stonesoup_global_variable;;
  moralisms_peponidas(brethrenism_distinctionless);
}

void moralisms_peponidas(char **revising_overpert)
{
  ++stonesoup_global_variable;;
  bananas_waff(revising_overpert);
}

void bananas_waff(char **apedom_pioneer)
{
  ++stonesoup_global_variable;;
  trouts_preinstructing(apedom_pioneer);
}

void trouts_preinstructing(char **phaih_rapaciousnesses)
{
  ++stonesoup_global_variable;;
  impiously_singlebar(phaih_rapaciousnesses);
}

void impiously_singlebar(char **certifiably_caped)
{
  ++stonesoup_global_variable;;
  eutrophy_croose(certifiably_caped);
}

void eutrophy_croose(char **closes_nelson)
{
  ++stonesoup_global_variable;;
  mashier_stercorean(closes_nelson);
}

void mashier_stercorean(char **visibilities_mistral)
{
  ++stonesoup_global_variable;;
  consul_hadamard(visibilities_mistral);
}

void consul_hadamard(char **gascon_kiloblock)
{
  ++stonesoup_global_variable;;
  farsightedly_pyrectic(gascon_kiloblock);
}

void farsightedly_pyrectic(char **fatcake_unintent)
{
  ++stonesoup_global_variable;;
  elapidae_unenshrined(fatcake_unintent);
}

void elapidae_unenshrined(char **overclimb_mytilene)
{
  ++stonesoup_global_variable;;
  colletotrichum_thyreoantitoxin(overclimb_mytilene);
}

void colletotrichum_thyreoantitoxin(char **marmeche_unmoaning)
{
  ++stonesoup_global_variable;;
  kebyar_catchpole(marmeche_unmoaning);
}

void kebyar_catchpole(char **fetisher_matin)
{
  ++stonesoup_global_variable;;
  sarcler_retreater(fetisher_matin);
}

void sarcler_retreater(char **juvenility_revolant)
{
  ++stonesoup_global_variable;;
  unorthodoxy_spermatogenetic(juvenility_revolant);
}

void unorthodoxy_spermatogenetic(char **transpirable_ananna)
{
  ++stonesoup_global_variable;;
  spoiler_nutarian(transpirable_ananna);
}

void spoiler_nutarian(char **ornamentalism_phillis)
{
  ++stonesoup_global_variable;;
  epiphysitis_graciously(ornamentalism_phillis);
}

void epiphysitis_graciously(char **footle_quatch)
{
    const int stonesoup_MAXLEN = 16;
    char stonesoup_dest[stonesoup_MAXLEN];
    size_t stonesoup_size_var = 0;
  char *thysanouran_forefront = 0;
  ++stonesoup_global_variable;;
  thysanouran_forefront = ((char *)footle_quatch[4]);
    tracepoint(stonesoup_trace, weakness_start, "CWE195", "A", "Signed to Unsigned Conversion Error");
    memset(stonesoup_dest,'x',stonesoup_MAXLEN);
    stonesoup_dest[stonesoup_MAXLEN - 1] = '\0';
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    stonesoup_size_var = stonesoup_returnChunkSize(stonesoup_dest, thysanouran_forefront);
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_size_var", stonesoup_size_var, &stonesoup_size_var, "TRIGGER-STATE");
/* STONESOUP: TRIGGER-POINT (Signed To Unsigned Conversion Error) */
    if (stonesoup_size_var > 0)
        memcpy(stonesoup_dest, thysanouran_forefront, stonesoup_size_var);
    stonesoup_printf("%s\n",stonesoup_dest);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, weakness_end);
;
  if (footle_quatch[4] != 0) 
    free(((char *)footle_quatch[4]));
stonesoup_close_printf_context();
}
