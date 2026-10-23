/*-------------------------------------------------------------------------
 *
 * hashfn.c
 *		Hash functions for use in dynahash.c hashtables
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/hash/hashfn.c
 *
 * NOTES
 *	  It is expected that every bit of a hash function's 32-bit result is
 *	  as random as every other; failure to ensure this is likely to lead
 *	  to poor performance of hash tables.  In most cases a hash
 *	  function should use hash_any() or its variant hash_uint32().
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/hash.h"
/*
 * string_hash: hash function for keys that are NUL-terminated strings.
 *
 * NOTE: this is the default hash function if none is specified.
 */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
int appaloosas_irrecuperable = 0;
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
void arabin_lite(char **rimosities_jacconet);
void nanaimo_tribulation(char **phyllodination_dors);
void musicalness_curtise(char **buddleias_protoconulid);
void colloquialist_exemplificator(char **loller_eyases);
void unbombarded_billen(char **bought_dragonish);
void directional_tungstic(char **flexography_coupes);
void cervicolingual_dhava(char **megatypy_sophisticate);
void diceratidae_quotum(char **hydrologists_eluviating);
void phosphoroscope_eulogizing(char **brainwater_spongiozoon);
void slackwitted_enchasers(char **berdache_doggone);
void proofreading_hopkinsian(char **tomfooleries_tenaha);
void thallodal_rangeley(char **adiaphoresis_winnowers);
void requisitioning_prisonous(char **limberest_rambutans);
void rebuttably_acadialite(char **nonrioting_semicircularly);
void quindecima_reinhabit(char **camargo_unlaboured);
void overconcerns_ecrevisse(char **philosophising_geonyctinastic);
void fedarie_lehmer(char **beeswings_mooney);
void spodomancy_misbetide(char **hydrogenator_ludian);
void cardipaludism_comoquer(char **precuneus_pyramidic);
void xenoparasitism_guana(char **frankincensed_operatics);
void axillary_savoyed(char **olga_quintons);
void anterior_alleviant(char **excerp_magnetimeter);
void blood_stills(char **lindsley_maidly);
void enouncement_overidolatrous(char **monogamik_penuriously);
void evaporators_discomfort(char **draconitic_gemelled);
void uncrinkling_vichy(char **tincted_sinisian);
void pollyanna_nichol(char **hobnobbed_incarcerating);
void petrpolis_pretests(char **pachuco_soppier);
void airfoil_trotter(char **oime_javas);
void channahon_brachialis(char **bels_boarders);
void dicranaceous_nyctalope(char **stich_peroxiding);
void personeity_bedimpling(char **speculatrices_sattley);
void unflesh_lyres(char **romeos_micrurgy);
void chalybite_pendecagon(char **turus_cassiopeian);
void twining_mislights(char **absoluteness_breveting);
void unsolubleness_anatase(char **nonremedies_nill);
void radication_stabiles(char **menshevist_dais);
void laulau_queerest(char **straked_backlasher);
void outglare_showier(char **gracemont_pedaled);
void heresies_heliographical(char **overfatigued_gorki);
void nude_shellshake(char **aella_saipan);
void unbold_quadrille(char **otorrhoea_calia);
void fictioner_reckoning(char **gunstocker_catamiting);
void circumlitio_appalto(char **concausal_diablery);
void laniary_yirring(char **vasoconstrictor_char);
void underrealized_schene(char **rifton_eyereach);
void kookery_codpieces(char **nemean_mazard);
void spaceful_protogenist(char **homy_unequitableness);
void diacrisis_falconries(char **shtik_cytoplasm);
void squalus_documenters(char **lom_apteria);
int stonesoup_191_global_var = 0;

uint32 string_hash(const void *key,Size keysize)
{
/*
	 * If the string exceeds keysize-1 bytes, we want to hash only that many,
	 * because when it is copied into the hash table it will be truncated at
	 * that length.
	 */
  Size s_len = strlen(((const char *)key));
  s_len = (s_len < keysize - 1?s_len : keysize - 1);
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )s_len)))) & 0xffffffff);
}
/*
 * tag_hash: hash function for fixed-size tag values
 */

uint32 tag_hash(const void *key,Size keysize)
{
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )keysize)))) & 0xffffffff);
}
/*
 * oid_hash: hash function for keys that are OIDs
 *
 * (tag_hash works for this case too, but is slower)
 */

uint32 oid_hash(const void *key,Size keysize)
{
  char **drou_comparate = 0;
  char *danaite_geeky[72] = {0};
  char *adenocellulitis_catastatic;;
  if (__sync_bool_compare_and_swap(&appaloosas_irrecuperable,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmprwsxMq_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&adenocellulitis_catastatic,"ARROGANCE_BEHEMOTHS");
      if (adenocellulitis_catastatic != 0) {;
        danaite_geeky[47] = adenocellulitis_catastatic;
        drou_comparate = danaite_geeky;
        arabin_lite(drou_comparate);
      }
    }
  }
  ;
  ;
  return (uint32 )(((Datum )(hash_uint32(((uint32 )( *((const Oid *)key)))))) & 0xffffffff);
}
/*
 * bitmap_hash: hash function for keys that are (pointers to) Bitmapsets
 *
 * Note: don't forget to specify bitmap_match as the match function!
 */

uint32 bitmap_hash(const void *key,Size keysize)
{
  ;
  return bms_hash_value( *((const Bitmapset *const *)key));
}
/*
 * bitmap_match: match function to use with bitmap_hash
 */

int bitmap_match(const void *key1,const void *key2,Size keysize)
{
  ;
  return !bms_equal( *((const Bitmapset *const *)key1), *((const Bitmapset *const *)key2));
}

void arabin_lite(char **rimosities_jacconet)
{
  ++stonesoup_global_variable;;
  nanaimo_tribulation(rimosities_jacconet);
}

void nanaimo_tribulation(char **phyllodination_dors)
{
  ++stonesoup_global_variable;;
  musicalness_curtise(phyllodination_dors);
}

void musicalness_curtise(char **buddleias_protoconulid)
{
  ++stonesoup_global_variable;;
  colloquialist_exemplificator(buddleias_protoconulid);
}

void colloquialist_exemplificator(char **loller_eyases)
{
  ++stonesoup_global_variable;;
  unbombarded_billen(loller_eyases);
}

void unbombarded_billen(char **bought_dragonish)
{
  ++stonesoup_global_variable;;
  directional_tungstic(bought_dragonish);
}

void directional_tungstic(char **flexography_coupes)
{
  ++stonesoup_global_variable;;
  cervicolingual_dhava(flexography_coupes);
}

void cervicolingual_dhava(char **megatypy_sophisticate)
{
  ++stonesoup_global_variable;;
  diceratidae_quotum(megatypy_sophisticate);
}

void diceratidae_quotum(char **hydrologists_eluviating)
{
  ++stonesoup_global_variable;;
  phosphoroscope_eulogizing(hydrologists_eluviating);
}

void phosphoroscope_eulogizing(char **brainwater_spongiozoon)
{
  ++stonesoup_global_variable;;
  slackwitted_enchasers(brainwater_spongiozoon);
}

void slackwitted_enchasers(char **berdache_doggone)
{
  ++stonesoup_global_variable;;
  proofreading_hopkinsian(berdache_doggone);
}

void proofreading_hopkinsian(char **tomfooleries_tenaha)
{
  ++stonesoup_global_variable;;
  thallodal_rangeley(tomfooleries_tenaha);
}

void thallodal_rangeley(char **adiaphoresis_winnowers)
{
  ++stonesoup_global_variable;;
  requisitioning_prisonous(adiaphoresis_winnowers);
}

void requisitioning_prisonous(char **limberest_rambutans)
{
  ++stonesoup_global_variable;;
  rebuttably_acadialite(limberest_rambutans);
}

void rebuttably_acadialite(char **nonrioting_semicircularly)
{
  ++stonesoup_global_variable;;
  quindecima_reinhabit(nonrioting_semicircularly);
}

void quindecima_reinhabit(char **camargo_unlaboured)
{
  ++stonesoup_global_variable;;
  overconcerns_ecrevisse(camargo_unlaboured);
}

void overconcerns_ecrevisse(char **philosophising_geonyctinastic)
{
  ++stonesoup_global_variable;;
  fedarie_lehmer(philosophising_geonyctinastic);
}

void fedarie_lehmer(char **beeswings_mooney)
{
  ++stonesoup_global_variable;;
  spodomancy_misbetide(beeswings_mooney);
}

void spodomancy_misbetide(char **hydrogenator_ludian)
{
  ++stonesoup_global_variable;;
  cardipaludism_comoquer(hydrogenator_ludian);
}

void cardipaludism_comoquer(char **precuneus_pyramidic)
{
  ++stonesoup_global_variable;;
  xenoparasitism_guana(precuneus_pyramidic);
}

void xenoparasitism_guana(char **frankincensed_operatics)
{
  ++stonesoup_global_variable;;
  axillary_savoyed(frankincensed_operatics);
}

void axillary_savoyed(char **olga_quintons)
{
  ++stonesoup_global_variable;;
  anterior_alleviant(olga_quintons);
}

void anterior_alleviant(char **excerp_magnetimeter)
{
  ++stonesoup_global_variable;;
  blood_stills(excerp_magnetimeter);
}

void blood_stills(char **lindsley_maidly)
{
  ++stonesoup_global_variable;;
  enouncement_overidolatrous(lindsley_maidly);
}

void enouncement_overidolatrous(char **monogamik_penuriously)
{
  ++stonesoup_global_variable;;
  evaporators_discomfort(monogamik_penuriously);
}

void evaporators_discomfort(char **draconitic_gemelled)
{
  ++stonesoup_global_variable;;
  uncrinkling_vichy(draconitic_gemelled);
}

void uncrinkling_vichy(char **tincted_sinisian)
{
  ++stonesoup_global_variable;;
  pollyanna_nichol(tincted_sinisian);
}

void pollyanna_nichol(char **hobnobbed_incarcerating)
{
  ++stonesoup_global_variable;;
  petrpolis_pretests(hobnobbed_incarcerating);
}

void petrpolis_pretests(char **pachuco_soppier)
{
  ++stonesoup_global_variable;;
  airfoil_trotter(pachuco_soppier);
}

void airfoil_trotter(char **oime_javas)
{
  ++stonesoup_global_variable;;
  channahon_brachialis(oime_javas);
}

void channahon_brachialis(char **bels_boarders)
{
  ++stonesoup_global_variable;;
  dicranaceous_nyctalope(bels_boarders);
}

void dicranaceous_nyctalope(char **stich_peroxiding)
{
  ++stonesoup_global_variable;;
  personeity_bedimpling(stich_peroxiding);
}

void personeity_bedimpling(char **speculatrices_sattley)
{
  ++stonesoup_global_variable;;
  unflesh_lyres(speculatrices_sattley);
}

void unflesh_lyres(char **romeos_micrurgy)
{
  ++stonesoup_global_variable;;
  chalybite_pendecagon(romeos_micrurgy);
}

void chalybite_pendecagon(char **turus_cassiopeian)
{
  ++stonesoup_global_variable;;
  twining_mislights(turus_cassiopeian);
}

void twining_mislights(char **absoluteness_breveting)
{
  ++stonesoup_global_variable;;
  unsolubleness_anatase(absoluteness_breveting);
}

void unsolubleness_anatase(char **nonremedies_nill)
{
  ++stonesoup_global_variable;;
  radication_stabiles(nonremedies_nill);
}

void radication_stabiles(char **menshevist_dais)
{
  ++stonesoup_global_variable;;
  laulau_queerest(menshevist_dais);
}

void laulau_queerest(char **straked_backlasher)
{
  ++stonesoup_global_variable;;
  outglare_showier(straked_backlasher);
}

void outglare_showier(char **gracemont_pedaled)
{
  ++stonesoup_global_variable;;
  heresies_heliographical(gracemont_pedaled);
}

void heresies_heliographical(char **overfatigued_gorki)
{
  ++stonesoup_global_variable;;
  nude_shellshake(overfatigued_gorki);
}

void nude_shellshake(char **aella_saipan)
{
  ++stonesoup_global_variable;;
  unbold_quadrille(aella_saipan);
}

void unbold_quadrille(char **otorrhoea_calia)
{
  ++stonesoup_global_variable;;
  fictioner_reckoning(otorrhoea_calia);
}

void fictioner_reckoning(char **gunstocker_catamiting)
{
  ++stonesoup_global_variable;;
  circumlitio_appalto(gunstocker_catamiting);
}

void circumlitio_appalto(char **concausal_diablery)
{
  ++stonesoup_global_variable;;
  laniary_yirring(concausal_diablery);
}

void laniary_yirring(char **vasoconstrictor_char)
{
  ++stonesoup_global_variable;;
  underrealized_schene(vasoconstrictor_char);
}

void underrealized_schene(char **rifton_eyereach)
{
  ++stonesoup_global_variable;;
  kookery_codpieces(rifton_eyereach);
}

void kookery_codpieces(char **nemean_mazard)
{
  ++stonesoup_global_variable;;
  spaceful_protogenist(nemean_mazard);
}

void spaceful_protogenist(char **homy_unequitableness)
{
  ++stonesoup_global_variable;;
  diacrisis_falconries(homy_unequitableness);
}

void diacrisis_falconries(char **shtik_cytoplasm)
{
  ++stonesoup_global_variable;;
  squalus_documenters(shtik_cytoplasm);
}

void squalus_documenters(char **lom_apteria)
{
 FILE * stonesoup_random_data = 0;
 char * stonesoup_buff = 0;
 unsigned long long stonesoup_num = 0;
 unsigned long long stonesoup_i = 0;
  char *semidiameter_paunchily = 0;
  ++stonesoup_global_variable;;
  semidiameter_paunchily = ((char *)lom_apteria[47]);
    tracepoint(stonesoup_trace, weakness_start, "CWE191", "A", "Integer Underflow (Wrap or Wraparound)");
 stonesoup_random_data = fopen("/dev/urandom", "r");
 if(stonesoup_random_data != NULL){
  stonesoup_num = strtoull(semidiameter_paunchily, NULL, 10);
  if(stonesoup_num > (unsigned long long) 0 ){
   if(stonesoup_num > (unsigned long long) 100 ){
    stonesoup_num = (unsigned long long) 100;
   }
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_num", stonesoup_num, &stonesoup_num, "TRIGGER-STATE");
   /* STONESOUP: CROSSOVER-POINT (Integer Underflow) */
   /* STONESOUP: TRIGGER-POINT (Integer Underflow) */
   stonesoup_buff = malloc((stonesoup_num - (unsigned long long) 10) * sizeof(char *));
   for(stonesoup_i = 0; stonesoup_i < stonesoup_num - (unsigned long long) 10; stonesoup_i++){
    stonesoup_buff[stonesoup_i] = fgetc(stonesoup_random_data);
   }
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
  }
  stonesoup_printf("data is:  %s", stonesoup_buff);
  fclose(stonesoup_random_data);
  if(stonesoup_buff != NULL){
   free(stonesoup_buff);
  }
 }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (lom_apteria[47] != 0) 
    free(((char *)lom_apteria[47]));
stonesoup_close_printf_context();
}
