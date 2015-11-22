/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	init_muc.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	initialize the MUC scorer by parsing the configuration files
 *	and the document/template files.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *	Scratch pad - modification history, plans for enhancements.
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)init_muc.c	1.2 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"	   /* nested includes of muc_defs.h and cdefs.h */
#include  "muc_protos.h"
#include  "muc_globals.h"


/*  internal function prototypes */

Proto	(static void, initialize_MUC_globals, (void));
Proto	(static	void, check_MUC_options, (void));


/*
 *  Initialize MUC
 */

#if UsePrototypes
void
initialize_MUC_scorer(void)
#else
void
initialize_MUC_scorer()
#endif	/* UsePrototypes */
{
    /* set MUC_mode to 'INIT' */
    MUC_mode = INIT_MODE;
    
    /* read the configuration file */
    parse_config(MUC_configuration_filename);

    /* check status on a couple of options */
    check_MUC_options();
    
    /* initialize catalog, class defs and scoring structs */
    initialize_MUC_globals();
    
    /* parse */

    parse_slotconfig();

    parse_texts();
    parse_keys();
    parse_responses();

    parse_mapconfig();
    
    /* initialize some global options */
    Scoring_method =   (Option *)find_muc_option(":scoring_method");
    Premodifiers =     (Option *)find_muc_option(":premodifiers");
    Postmodifiers =    (Option *)find_muc_option(":postmodifiers");
    Corp_designators = (Option *)find_muc_option(":corporate_designators");
    Map_order =        (Option *)find_muc_option(":map_order");

    /* reset MUC_mode */
    MUC_mode = NO_MODE;

    return;
}    



/*
 *  Initialize MUC_globals
 */

#if UsePrototypes
static
void
initialize_MUC_globals(void)
#else
static
void
initialize_MUC_globals()
#endif	/* UsePrototypes */
{

    MUC_Catalog = table_list_create(catalog_entry_compare,
				    catalog_entry_release,
				    default_destroy);
    
    if(MUC_Catalog == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: Unable to initialize MUC_Catalog table.\n");
	exit(1);
    }
    

    MUC_Class_defs = table_list_create(class_def_compare,
				       class_def_release,
				       default_destroy);

    if(MUC_Class_defs == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: Unable to initialize MUC_Class_defs table.\n");
	exit(1);
    }

    
    MUC_Summary_Class_scores = table_list_create(class_score_compare,
						 class_score_release,
						 default_destroy);

    if(MUC_Summary_Class_scores == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: Unable to initialize Summary_Class_scores table.\n");
	exit(1);
    }


    /* NE specific */
    HEADLINE_tallies = tallies_create();
    headline_tallies = tallies_create();
    SLUG_tallies = tallies_create();
    PREAMBLE_tallies = tallies_create();
    hl_tallies = tallies_create();
    HL_tallies = tallies_create();
    TXT_tallies = tallies_create();
    TEXT_tallies = tallies_create();
    IDS_tallies = tallies_create();
    TRAILER_tallies = tallies_create();
    date_tallies = tallies_create();


    dd_tallies = tallies_create();
    DATELINE_tallies = tallies_create();
    
    enamex_organization_tallies = tallies_create();
    enamex_person_tallies = tallies_create();
    enamex_location_tallies = tallies_create();
    enamex_other_tallies = tallies_create();
    timex_date_tallies = tallies_create();
    timex_time_tallies = tallies_create();
    timex_other_tallies = tallies_create();
    numex_money_tallies = tallies_create();
    numex_percent_tallies = tallies_create();
    numex_other_tallies = tallies_create();
}


/*
 *  check some options.
 */
#if UsePrototypes
static
void
check_MUC_options(void)
#else
static
void
check_MUC_options()
#endif	/* UsePrototypes */
{
    Option *scoring_task = (Option *)find_muc_option(":scoring_task");
    char *task_name = scoring_task->value;

    if (task_name == (char *) NULL)
    {
	fprintf(stderr, "Scoring task undefined.  Cannot continue.\n");
	fprintf(stderr, "(see configuration file)\n");
	exit(1);
    }
    
    else if ( STRCASEEQ(task_name, "scenario_template")
	     || STRCASEEQ(task_name, "scenario-template"))
	MUC_scoring_task = SCENARIO_TEMPLATE;
    else if ( STRCASEEQ(task_name, "template_element")
	     || STRCASEEQ(task_name, "template-element"))
	MUC_scoring_task = TEMPLATE_ELEMENT;
    else if ( STRCASEEQ(task_name, "named_entity")
	     || STRCASEEQ(task_name, "named-entity"))
	MUC_scoring_task = NAMED_ENTITY;
    else if ( STRCASEEQ(task_name, "coreference") )
	MUC_scoring_task = COREFERENCE;

    else
    {
	fprintf(stderr, "Scoring task '%s' unrecognized.", task_name);
	fprintf(stderr, "  Cannot continue.\n");
	exit(1);
    }
    
    return;
}
