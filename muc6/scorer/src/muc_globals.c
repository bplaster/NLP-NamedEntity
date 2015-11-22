/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	muc_globals.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	global MUC variables
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *	Added definition of MUC_options to this file -ad 01/02/97
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)muc_globals.c	1.2 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"	/* nested includes of muc_defs.h and cdefs.h */


/* some global variables */

/************************************************************************

 *  MUC_Catalog is a table of catalog_entries.				*
 *  MUC_Class_defs is a table of class_defs					*
 *  MUC_Summary_Class_scores is a table containing the scoring data for	*
 *    the classes (and slots) across the entire document set.		*
 *									*
 ***********************************************************************/

TABLE   *MUC_Catalog;
TABLE   *MUC_Class_defs;
TABLE   *MUC_Summary_Class_scores;

char	*MUC_configuration_filename;

int	MUC_scoring_task;
int	MUC_file_type;
int	MUC_instance_type;

int	MUC_mode;

int	NE_doc_section;
int	NE_headline_type;
int	NE_text_type;
int	NE_category;

Option	*Premodifiers;
Option	*Postmodifiers;
Option	*Corp_designators;
Option	*Scoring_method;

Option	*Map_order;

Option	MUC_Options[] = 
{
    {
	":muc_base_directory",
	OPTVAL,
	(char *) NULL,
	"",
	(char **) NULL,
	0
    },
    {
	":slot_file",
	OPTVAL,
	(char *) NULL,
	"slotconfig",
	(char **) NULL,
	0
    },
    {
	":map_file",
	OPTVAL,
	(char *) NULL,
	"mapconfig",
	(char **) NULL,
	0
    },
    {
	":text_file",
	OPTVAL,
	(char *) NULL,
	(char *) NULL,
	(char **) NULL,
	0
    },
    {
	":key_file",
	OPTVAL,
	(char *) NULL,
	"key-templates",
	(char **) NULL,
	0
    },
    {
	":response_file",
	OPTVAL,
	(char *) NULL,
	"response-templates",
	(char **) NULL,
	0
    },
    {
	":history_file",
	OPTVAL,
	(char *) NULL,
	"_history",
	(char **) NULL,
	0
    },
    {
	":map_history_file",
	OPTVAL,
	(char *) NULL,
	"_map-history",
	(char **) NULL,
	0
    },
    {
	":connect_history_file",
	OPTVAL,
	(char *) NULL,
	"_connect-history",
	(char **) NULL,
	0
    },
    {
	":slot_error_file",
	OPTVAL,
	(char *) NULL,
	"_slot-history",
	(char **) NULL,
	0
    },
    {
	":score_file",
	OPTVAL,
	(char *) NULL,
	"_score-report",
	(char **) NULL,
	0
    },
    {
	":scoring_task",
	OPTVAL,
	(char *) NULL,
	(char *) NULL,
	(char **) NULL,
	0
    },
    {
	":display_type",
	SELECT,
	(char *) NULL,
	"all_objects",
	(char **) NULL,
	0
    },
    {
	":scoring_method",
	SELECT,
	(char *) NULL,
	"key2response",
	(char **) NULL,
	0
    },
    {
	":query_verbose",
	TOGGLE,
	(char *) NULL,
	OFF,
	(char **) NULL,
	0
    },
    {
	":enable_edit",
	TOGGLE,
	(char *) NULL,
	OFF,
	(char **) NULL,
	0
    },
    {
	":map_query",
	TOGGLE,
	(char *) NULL,
	OFF,
	(char **) NULL,
	0
    },
    {
	":save_query_history",
	TOGGLE,
	(char *) NULL,
	OFF,
	(char **) NULL,
	0
    },
    {
	":save_map_history",
	TOGGLE,
	(char *) NULL,
	OFF,
	(char **) NULL,
	0
    },
    {
	":save_connect_history",
	TOGGLE,
	(char *) NULL,
	OFF,
	(char **) NULL,
	0
    },
    {
	":premodifiers",
	VAL_LIST,
	(char *) NULL,
	(char *) NULL,
	(char **) NULL,
	0
    },
    {
	":postmodifiers",
	VAL_LIST,
	(char *) NULL,
	(char *) NULL,
	(char **) NULL,
	0
    },
    {
	":corporate_designators",
	VAL_LIST,
	(char *) NULL,
	(char *) NULL,
	(char **) NULL,
	0
    },

    {
	":map_order",
	VAL_LIST,
	(char *) NULL,
	(char *) NULL,
	(char **) NULL,
	0
    },
    {
	":report_file",
	OPTVAL,
	(char *) NULL,
	"_report-summary",
	(char **) NULL,
	0
    },
};

int  Num_Options = (sizeof(MUC_Options) / sizeof(MUC_Options[0]));


char	*blanks = "                                                                                ";

/* NE specific */

/*** kao tally ***/
Tallies *HEADLINE_tallies; 
Tallies *headline_tallies;
Tallies *SLUG_tallies;
Tallies *PREAMBLE_tallies;
Tallies *hl_tallies;
Tallies *HL_tallies;
Tallies *TXT_tallies;
Tallies *TEXT_tallies;
Tallies *IDS_tallies;
Tallies *TRAILER_tallies;
Tallies *date_tallies;


Tallies *dd_tallies;
Tallies *DATELINE_tallies;

Tallies *enamex_organization_tallies;
Tallies *enamex_person_tallies;
Tallies *enamex_location_tallies;
Tallies *enamex_other_tallies;
Tallies *timex_date_tallies;
Tallies *timex_time_tallies;
Tallies *timex_other_tallies;
Tallies *numex_money_tallies;
Tallies *numex_percent_tallies;
Tallies *numex_other_tallies;
