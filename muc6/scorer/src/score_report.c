/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	score_report.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	routines to write the score report.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *	added "static" keyword to some internal function definitions
 *        (to bring them in line with prototypes), and added some 
 *        missing ";"  - ad 11/13/96
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)score_report.c	1.6 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include <stdio.h>
#include "table.h"
#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"


/*  internal function prototypes */

Proto	(static	void, display_class_and_slot_scores, (Class_def *class,
						      Class_score *c_score));
Proto	(static	void, display_catalog_total_scores, (void));
Proto	(static	void, initialize_display_tallies, (void));
Proto	(static	void, free_display_tallies, (void));
Proto	(static	void, clear_class_and_slot_total_scores, (void));
Proto	(static	void, display_ne_subcategorization_scores, (void));
Proto	(static	void, display_ne_doc_section_scores, (void));
Proto   (static void, display_ne_report_scores, (void));

Proto	(static	void, format_tallies, (char *id, Tallies *tallies));
Proto	(static	void, format_f_measures, (Tallies *tallies));
Proto	(static	void, format_template_ids, (Catalog_entry *entry));
Proto	(static	void, format_score_table_header, (void));
Proto	(static	void, format_score_table_line, (void));
Proto	(static	void, open_muc_score_file, (void));
Proto	(static	void, open_muc_report_file, (void));
Proto	(static	void, close_muc_score_file, (void));
Proto	(static	void, close_muc_report_file, (void));
Proto   (static void, format_report_header, (void));
Proto   (static void, format_correct_items, (void));
Proto   (static void, report_class_instance, (Class_instance *inst, 
                                              int kind));
Proto   (static void, report_correct_class_instance, (Class_instance *inst, Class_instance *inst2,
						int kind));
Proto   (static void, print_report_string_fill, (String_fill *sfill));
Proto   (static void, print_correct_report, (int kind));
Proto   (static void, print_missing_report, (int kind));
Proto   (static void, print_spurious_report, (int kind));
Proto   (static void, print_incorrect_report, (int kind));


/*  local globals */

static FILE  *muc_score_file;
static FILE  *muc_report_file;

static char *score_table_header =
"SLOT           POS   ACT| COR PAR INC | MIS  SPU  NON| REC PRE UND OVG SUB ERR";

static char *score_table_line =
"------------------------+-------------+--------------+------------------------";
static char *report_header_line = 
"--------------------------------------------------------------------------------------------------------------------";
static char *report_header1 =
"map/score map/score  key                       key                      Response                 Response";
static char *report_header2 =
"category  category   str                       str                      str                      str";
static char *report_header3 =
"type      text       type                      text                     type                     text";
static Tallies *All_objects_slot_tallies;
static Tallies *Document_total_slot_tallies;
static Tallies *Subtotal_tallies;
static Tallies *Single_text_filtering;
static Tallies *Total_text_filtering;


/************************************************************************
 *									*
 *  display_MUC_catalog_scores()					*
 *									*
 *  display the scores for all documents and templates.			*
 *  calls display_catalog_entry_scores.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
display_MUC_catalog_scores(void)
#else
void
display_MUC_catalog_scores()
#endif	/* UsePrototypes */
{
    TABLE  *Catalog = (TABLE *)MUC_Catalog;
    Catalog_entry *entry;

    initialize_display_tallies();

    printf("Begin Score Report.\n");

    open_muc_score_file();
    clear_class_and_slot_total_scores();

    fprintf(muc_score_file, "Configuration: %s\n\n", MUC_configuration_filename);

    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
    {
	printf("reporting scores for %s...", entry->docnum);
	display_catalog_entry_scores(entry);
	printf("done.\n");
    }
    
    display_catalog_total_scores();

    if (MUC_scoring_task == NAMED_ENTITY)
    {
	display_ne_subcategorization_scores();
	display_ne_doc_section_scores();
        display_ne_report_scores();
    }

    free_display_tallies();
    
    close_muc_score_file();

    printf("Score Report Completed.\n");
    return;
}


#if UsePrototypes
static
void
initialize_display_tallies(void)
#else
static
void
initialize_display_tallies()
#endif	/* UsePrototypes */
{
    All_objects_slot_tallies = tallies_create();
    Document_total_slot_tallies = tallies_create();
    Subtotal_tallies = tallies_create();
    Single_text_filtering = tallies_create();
    Total_text_filtering = tallies_create();
}


#if UsePrototypes
static
void
free_display_tallies(void)
#else
static
void
free_display_tallies()
#endif	/* UsePrototypes */
{
    tallies_release((char *) All_objects_slot_tallies);
    tallies_release((char *) Document_total_slot_tallies);
    tallies_release((char *) Subtotal_tallies);
    tallies_release((char *) Single_text_filtering);
    tallies_release((char *) Total_text_filtering);
}


/************************************************************************
 *									*
 *  display_catalog_entry_scores					*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
display_catalog_entry_scores(Catalog_entry *entry)
#else
void
display_catalog_entry_scores(entry)
Catalog_entry *entry;
#endif	/* UsePrototypes */
{
    TABLE  *Class_defs = MUC_Class_defs;
    TABLE  *class_scores;
    Class_def  *this_class;
    Class_score *c_score;

    format_template_ids(entry);

    format_score_table_line();
    format_score_table_header();
    format_score_table_line();

    /* relevancy judgments are made on scenario_template task only */
    if (MUC_scoring_task == SCENARIO_TEMPLATE)
    {
	tallies_clear(Single_text_filtering);
	score_relevancy(entry, Single_text_filtering);
	tallies_combine(Total_text_filtering, Single_text_filtering);
    }
    
    tallies_clear(Document_total_slot_tallies);
    
    class_scores = (TABLE *) entry->class_scores;

    /* follow the Class_defs list to gather all the class_level and
       slot_level scores for all instances of this entry. */

    table_first(Class_defs);
    for(this_class = (Class_def *)table_next(Class_defs);
	this_class;
	this_class = (Class_def *)table_next(Class_defs))
    {
	if (this_class->is_scored_class)
	{
	    c_score = find_class_score(class_scores, this_class->name);
	    
	    /* display the class and slot scores */
	    display_class_and_slot_scores(this_class, c_score);
	}
    }

    format_score_table_line();
    format_tallies("TOTAL", Document_total_slot_tallies);
    format_score_table_line();

    if (MUC_scoring_task == SCENARIO_TEMPLATE)
    {
	format_tallies("TEXT FILTER", Single_text_filtering);
	format_score_table_line();
    }
    
    fprintf(muc_score_file, "\f\n");

    return;
}


/************************************************************************
 *									*
 *  display_catalog_total_scores					*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
display_catalog_total_scores(void)
#else
static
void
display_catalog_total_scores()
#endif	/* UsePrototypes */
{
    TABLE  *Class_defs = MUC_Class_defs;
    TABLE  *slots;
    Class_def  *class;
    Slot_def  *slot;
    char *slotid;
    
    fprintf(muc_score_file, " * * * TOTAL SLOT SCORES * * *\n");
    format_score_table_line();
    format_score_table_header();
    format_score_table_line();

    tallies_clear(Document_total_slot_tallies);

    /* follow the Class_defs list to print the total class and slot scores */

    table_first(Class_defs);
    for(class = (Class_def *)table_next(Class_defs);
	class;
	class = (Class_def *)table_next(Class_defs))
    {
	if (class->is_scored_class)
	{
	    format_tallies(class->name, class->total_tallies);

	    slots = (TABLE *) class->slot_defs;

	    /* follow the slot_defs list to print the total slot scores */

	    table_first(slots);
	    for (slot = (Slot_def *)table_next(slots);
		 slot;
		 slot = (Slot_def *)table_next(slots))
	    {
		if (slot->is_scored_slot)
		{
		    /* combine the slot score with the total tallies */
		    tallies_combine(All_objects_slot_tallies,
				    slot->total_tallies);

		    slotid = (char *) malloc (strlen(slot->name) + 3);
		    slotid = strcat(strcpy(slotid, "  "), slot->name);
	
		    /* display the slot_level total scores */
		    format_tallies(slotid, slot->total_tallies);
		    UFREE(slotid);
		}
	    }
	}
    }

    format_score_table_line();
    format_tallies("ALL OBJECTS", All_objects_slot_tallies);
    format_score_table_line();

    if (MUC_scoring_task == SCENARIO_TEMPLATE)
    {
	format_tallies("TEXT FILTER", Total_text_filtering);
	format_score_table_line();
    }

    format_f_measures(All_objects_slot_tallies);

    return;
}


/************************************************************************
 *									*
 *  clear_class_and_slot_total_scores					*
 *									*
 *  clear all the total_tallies for class_defs and slot_defs		*
 *  before collecting the scoring information from the class_scores	*
 *  and slot_scores.							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
clear_class_and_slot_total_scores(void)
#else
static
void
clear_class_and_slot_total_scores()
#endif	/* UsePrototypes */
{
    TABLE  *classes = MUC_Class_defs;
    Class_def  *this_class;
    TABLE  *slots;
    Slot_def  *this_slot;

    /* follow the Class_defs list to clear all the class_level and
       slot_level total tallies. */

    table_first(classes);
    for(this_class = (Class_def *)table_next(classes);
	this_class;
	this_class = (Class_def *)table_next(classes))
    {
	tallies_clear(this_class->total_tallies);
	slots = (TABLE *) this_class->slot_defs;

	/* follow the slot_defs list to clear all the slot_level
	   total tallies. */

	table_first(slots);
	for (this_slot = (Slot_def *)table_next(slots);
	     this_slot;
	     this_slot = (Slot_def *)table_next(slots))
	    tallies_clear(this_slot->total_tallies);
    }
    
    return;
}
	    

/************************************************************************
 *									*
 *  display_class_and_slot_total_scores					*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
display_class_and_slot_scores(Class_def *class, Class_score *c_score)
#else
static
void
display_class_and_slot_scores(class, c_score)
Class_def *class;
Class_score *c_score;
#endif	/* UsePrototypes */
{
    TABLE *slots = (TABLE *) class->slot_defs;
    TABLE *slot_scores = (TABLE *) c_score->slot_scores;
    Slot_def *slot;
    Slot_score *s_score;
    char *slotid;

    /* combine the scores of this class with the total tallies */
    tallies_combine(class->total_tallies, c_score->tallies);

    /* display the class_level scores for this catalog entry */
    format_tallies(c_score->name, c_score->tallies);
    
    /* follow the slot_defs list to collect all the slot scores for
       this class */
/*
    tallies_clear(Subtotal_tallies);
*/
    table_first(slots);
    for (slot = (Slot_def *)table_next(slots);
	 slot;
	 slot = (Slot_def *)table_next(slots))
    {
	if (slot->is_scored_slot)
	{
	    s_score = find_slot_score(slot_scores, slot->name);

	    /* print only if possible or actual > 0 */
	    if (s_score->tallies->possible || s_score->tallies->actual)
	    {
		/* combine the slot score with the total tallies */
		tallies_combine(slot->total_tallies, s_score->tallies);
/*
		tallies_combine(Subtotal_tallies, s_score->tallies);
*/
		tallies_combine(Document_total_slot_tallies, s_score->tallies);

		slotid = (char *) malloc (strlen(slot->name) + 3);
		slotid = strcat(strcpy(slotid, "  "), slot->name);
	
		/* display the slot_level scores */
		format_tallies(slotid, s_score->tallies);
		UFREE(slotid);
	    }
	}
    }
/*
    format_tallies("  subtotal", Subtotal_tallies);
*/

    return;
}


/************************************************************************
 *									*
 *  NE specific display routines					*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_to_ne_subcategorization_tallies(Slot_fill *slot, Tallies *s_tallies)
#else
void
add_to_ne_subcategorization_tallies(slot, s_tallies)
Slot_fill *slot;
Tallies *s_tallies;
#endif	/* UsePrototypes */
{
    String_fill *strfill;
    
    if( slot == NULL ) return;
    
    strfill = slot->strfills[0];
    if( strfill == NULL ) return;

    /* do this only while in scoring mode,
       and only if it's a 'type' slot */
    if ( (MUC_mode == SCORE_MODE)
	&& STRCASEEQ(slot->name, "TYPE") )
    {
	switch (NE_category)
	{
	case ENAMEX:
	    if STRCASEEQ(strfill->fill, "ORGANIZATION")
		tallies_combine(enamex_organization_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "PERSON")
		tallies_combine(enamex_person_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "LOCATION")
		tallies_combine(enamex_location_tallies, s_tallies);
	    else
		tallies_combine(enamex_other_tallies, s_tallies);
	    break;
	case TIMEX:
	    if STRCASEEQ(strfill->fill, "DATE")
		tallies_combine(timex_date_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "TIME")
		tallies_combine(timex_time_tallies, s_tallies);
	    else
		tallies_combine(timex_other_tallies, s_tallies);
	    break;
	case NUMEX:
	    if STRCASEEQ(strfill->fill, "MONEY")
		tallies_combine(numex_money_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "PERCENT")
		tallies_combine(numex_percent_tallies, s_tallies);
	    else
		tallies_combine(numex_other_tallies, s_tallies);
	    break;
	}
    }

    return;
}


#if UsePrototypes
void
subtract_from_ne_subcategorization_tallies(Slot_fill *slot, Tallies *s_tallies)
#else
void
subtract_from_ne_subcategorization_tallies(slot, s_tallies)
Slot_fill *slot;
Tallies *s_tallies;
#endif	/* UsePrototypes */
{
    String_fill *strfill;
    
    if( slot == NULL ) return;
    
    strfill = slot->strfills[0];
    if( strfill == NULL ) return;


    /* do this only while in scoring mode,
       and only if it's a 'type' slot */
    if ( (MUC_mode == SCORE_MODE)
	&& STRCASEEQ(slot->name, "TYPE") )
    {
	switch (NE_category)
	{
	case ENAMEX:
	    if STRCASEEQ(strfill->fill, "ORGANIZATION")
		tallies_subtract(enamex_organization_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "PERSON")
		tallies_subtract(enamex_person_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "LOCATION")
		tallies_subtract(enamex_location_tallies, s_tallies);
	    else
		tallies_subtract(enamex_other_tallies, s_tallies);
	    break;
	case TIMEX:
	    if STRCASEEQ(strfill->fill, "DATE")
		tallies_subtract(timex_date_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "TIME")
		tallies_subtract(timex_time_tallies, s_tallies);
	    else
		tallies_subtract(timex_other_tallies, s_tallies);
	    break;
	case NUMEX:
	    if STRCASEEQ(strfill->fill, "MONEY")
		tallies_subtract(numex_money_tallies, s_tallies);
	    else if STRCASEEQ(strfill->fill, "PERCENT")
		tallies_subtract(numex_percent_tallies, s_tallies);
	    else
		tallies_subtract(numex_other_tallies, s_tallies);
	    break;
	}
    }

    return;
}


#if UsePrototypes
void
add_to_ne_doc_section_tallies(Tallies *s_tallies)
#else
     void
     add_to_ne_doc_section_tallies(s_tallies)
     Tallies *s_tallies;
#endif	/* UsePrototypes */
{
  /* do this only while in scoring mode */
  if (MUC_mode == SCORE_MODE)
    {
      switch(NE_doc_section)
	{
        case HL:
	  tallies_combine (HL_tallies, s_tallies);
	  break;
        case hl:
	  tallies_combine (hl_tallies, s_tallies);
	  break;
	case HEADLINE:
	  tallies_combine(HEADLINE_tallies, s_tallies);
	  break;
	case headline:
	  tallies_combine(headline_tallies, s_tallies);
	  break;
	case SLUG:
	  tallies_combine(SLUG_tallies, s_tallies);
	  break;
	case PREAMBLE:
	  tallies_combine(PREAMBLE_tallies, s_tallies);
	  break;
        case DD:
	case dd:
	  tallies_combine(dd_tallies, s_tallies);
	  break;
	case DATELINE:
	case dateline:
	  tallies_combine(DATELINE_tallies, s_tallies);
	  break;
        case DATE:
	case date:
	  tallies_combine(date_tallies, s_tallies);
	  break;
        case TEXT:
	case text:
	  tallies_combine(TEXT_tallies, s_tallies);
	  break;
        case TXT:
	case txt:
	  tallies_combine(TXT_tallies, s_tallies);
	  break;
        case IDS:
	case ids:
	  tallies_combine(IDS_tallies, s_tallies);
	  break;
        case TRAILER:
	case trailer:
	  tallies_combine(TRAILER_tallies, s_tallies);
	  break;

	}
    }

  return;
}


#if UsePrototypes
void
subtract_from_ne_doc_section_tallies(Tallies *s_tallies)
#else
void
subtract_from_ne_doc_section_tallies(s_tallies)
Tallies *s_tallies;
#endif	/* UsePrototypes */
{
    /* do this only while in scoring mode */
    if (MUC_mode == SCORE_MODE)
    {
	switch(NE_doc_section)
	{
	case HL:
	    tallies_subtract(HL_tallies, s_tallies);
	    break;
	case hl:
	    tallies_subtract(hl_tallies, s_tallies);
            break;
	case HEADLINE:
	    tallies_subtract(HEADLINE_tallies, s_tallies);
            break;
	case headline:
	    tallies_subtract(headline_tallies, s_tallies);
            break;
	case SLUG:
	    tallies_subtract(SLUG_tallies, s_tallies);
	    break;
	case PREAMBLE:
	    tallies_subtract(PREAMBLE_tallies, s_tallies);
	    break;
	case IDS:
	case ids:
	    tallies_subtract(IDS_tallies, s_tallies);
	    break;
	case DD:
	case dd:
	    tallies_subtract(dd_tallies, s_tallies);
	    break;
	case DATELINE:
	case dateline:
	    tallies_subtract(DATELINE_tallies, s_tallies);
	    break;
        case DATE:
        case date:
            tallies_subtract(date_tallies, s_tallies);
            break;
	case TXT:
	case txt:
	    tallies_subtract(TXT_tallies, s_tallies);
	    break;
	case TEXT:
	case text:
	    tallies_subtract(TEXT_tallies, s_tallies);
	    break;
	case TRAILER:
	case trailer:
	    tallies_subtract(TRAILER_tallies, s_tallies);
	    break;
	}
    }

    return;
}


#if UsePrototypes
static
void
display_ne_subcategorization_scores(void)
#else
static
void
display_ne_subcategorization_scores()
#endif	/* UsePrototypes */
{
    fprintf(muc_score_file, "\n * * * TASK SUBCATEGORIZATION SCORES * * *\n");
    format_score_table_line();
    format_score_table_header();
    format_score_table_line();

    fprintf(muc_score_file, "Enamex:\n");
    format_tallies("  organization", enamex_organization_tallies);
    format_tallies("  person", enamex_person_tallies);
    format_tallies("  location", enamex_location_tallies);
    format_tallies("  other", enamex_other_tallies);
    format_score_table_line();

    fprintf(muc_score_file, "Timex:\n");
    format_tallies("  date", timex_date_tallies);
    format_tallies("  time", timex_time_tallies);
    format_tallies("  other", timex_other_tallies);
    format_score_table_line();

    fprintf(muc_score_file, "Numex:\n");
    format_tallies("  money", numex_money_tallies);
    format_tallies("  percent", numex_percent_tallies);
    format_tallies("  other", numex_other_tallies);
    format_score_table_line();

    return;
}

#if UsePrototypes
static
void
display_ne_doc_section_scores(void)
#else
static
void
display_ne_doc_section_scores()
#endif	/* UsePrototypes */
{
    fprintf(muc_score_file, "\n * * * DOCUMENT SECTION SCORES * * *\n");
    format_score_table_line();
    format_score_table_header();
    format_score_table_line();

    format_tallies("HEADLINE", HEADLINE_tallies);
    format_tallies("headline", headline_tallies);
    format_tallies("HL", HL_tallies); 
    format_tallies("hl", hl_tallies); 
    format_tallies("SLUG", SLUG_tallies);
    format_tallies("PREAMBLE", PREAMBLE_tallies);
    format_score_table_line();
    format_tallies("DD", dd_tallies);
    format_tallies("DATE", date_tallies);
    format_score_table_line();
    format_tallies("DATELINE", DATELINE_tallies);
    format_score_table_line();
/***
    if(NE_headline_type == HL)
       format_tallies("Tot headlines", HL_tallies);
    else if (NE_headline_type == HEADLINE)
       format_tallies("Tot headlines", HEADLINE_tallies);
    else if (NE_headline_type == headline)
       format_tallies("Tot headlines", headline_tallies);
    else if (NE_headline_type == hl)
       format_tallies("Tot headlines", hl_tallies);
    else if (NE_headline_type == SLUG)
       format_tallies("Tot headlines", SLUG_tallies);
***/
    format_tallies("TXT", TXT_tallies);
    format_tallies("TEXT", TEXT_tallies);
    format_score_table_line();
/***
    if (NE_text_type== TXT)
       format_tallies("Tot text", TXT_tallies);
    else if (NE_text_type== TEXT)
       format_tallies("Tot text", TEXT_tallies);
***/
    format_tallies("IDS", IDS_tallies); 
    format_score_table_line();
    format_tallies("TRAILER", TRAILER_tallies); 
    format_score_table_line();
    format_score_table_line();
    
    return;
}

#if UsePrototypes
static
void
display_ne_report_scores (void)
#else
static
void
display_ne_report_scores ()
#endif	/* UsePrototypes */
{
    open_muc_report_file();
    fprintf(muc_report_file, "\n * * * REPORT SUMMARY * * *\n");
    fprintf(muc_report_file, "%s\n", report_header_line);
    format_report_header ();
    fprintf(muc_report_file, "%s\n", report_header_line);
    format_correct_items();
    close_muc_report_file();

return;
}

/************************************************************************
 *									*
 *  format_tallies							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
format_tallies(char *id, Tallies *tallies)
#else
static
void
format_tallies(id, tallies)
char *id;
Tallies *tallies;
#endif	/* UsePrototypes */
{
    int  recall, precision;
    int  undergeneration, overgeneration;
    int  error_prf, substitution;

    /* the following computed measures are cast to int to truncate the
       decimal value.  however, in order to round 'up' if the decimal
       is > .5, we add 0.5 before truncating. */

    recall = (int) (0.5 + compute_recall(tallies));
    precision = (int) (0.5 + compute_precision(tallies));
    undergeneration = (int) (0.5 + compute_undergeneration(tallies));
    overgeneration = (int) (0.5 + compute_overgeneration(tallies));
    substitution = (int) (0.5 + compute_substitution(tallies));
    error_prf = (int) (0.5 + compute_error_per_response_fill(tallies));
    
    fprintf(muc_score_file,
	    "%-13.12s%5d %5d|%4d %3d %4d|%4d %4d %4d|",
	    id,
	    tallies->possible,
	    tallies->actual,
	    tallies->correct,
	    tallies->partial,
	    tallies->incorrect,
	    tallies->missing,
	    tallies->spurious,
	    tallies->noncommittal);

    if (recall == -1.0)
	fprintf(muc_score_file, "   *");
    else
	fprintf(muc_score_file, "%4d", recall);
    
    if (precision == -1.0)
	fprintf(muc_score_file, "   *");
    else
	fprintf(muc_score_file, "%4d", precision);
    
    if (undergeneration == -1.0)
	fprintf(muc_score_file, "   *");
    else
	fprintf(muc_score_file, "%4d", undergeneration);
    
    if (overgeneration == -1.0)
	fprintf(muc_score_file, "   *");
    else
	fprintf(muc_score_file, "%4d", overgeneration);
    
    if (substitution == -1.0)
	fprintf(muc_score_file, "   *");
    else
	fprintf(muc_score_file, "%4d", substitution);
    
    if (error_prf == -1.0)
	fprintf(muc_score_file, "   *");
    else
	fprintf(muc_score_file, "%4d", error_prf);

    fprintf(muc_score_file, "\n");
    
    return;
}


/************************************************************************
 *									*
 *  format_f_measures							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
format_f_measures(Tallies *tallies)
#else
static
void
format_f_measures(tallies)
Tallies *tallies;
#endif	/* UsePrototypes */
{
    float fpr, f2pr, fp2r;
    
    fpr = compute_tallies_f(tallies, F_PR);
    f2pr = compute_tallies_f(tallies, F_2PR);
    fp2r = compute_tallies_f(tallies, F_P2R);
    
    fprintf(muc_score_file, "%55s%10s%10s\n", "P&R", "2P&R", "P&2R");
    
    if( (fpr < 0.0) || (f2pr < 0.0) || (fp2r < 0.0))
	fprintf(muc_score_file,
		"F-MEASURES not available - Recall or Precision invalid.\n");
    else
	fprintf(muc_score_file,
		"F-MEASURES%45.2f%10.2f%10.2f\n", fpr, f2pr, fp2r);

    return;
}


/************************************************************************
 *									*
 *  miscellaneous format functions					*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
format_template_ids(Catalog_entry *entry)
#else
static
void
format_template_ids(entry)
Catalog_entry *entry;
#endif	/* UsePrototypes */
{
    Root *k_root = entry->k_root;
    Root *r_root = entry->r_root;
    
    fprintf(muc_score_file,
	    "%-15skey: %-15sresponse:%15s\n",
	    entry->docnum,
	    (k_root ? k_root->docnum : "<NONE>"),
	    (r_root ? r_root->docnum : "<NONE>"));

    return;
}


#if UsePrototypes
static
void
format_score_table_header(void)
#else
static
void
format_score_table_header()
#endif	/* UsePrototypes */
{
    fprintf(muc_score_file, "%s\n", score_table_header);
    return;
}


#if UsePrototypes
static
void
format_score_table_line(void)
#else
static
void
format_score_table_line()
#endif	/* UsePrototypes */
{
    fprintf(muc_score_file, "%s\n", score_table_line);
    return;
}



/************************************************************************
 *									*
 *  open_muc_score_file, close_muc_score_file				*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
open_muc_score_file(void)
#else
static
void
open_muc_score_file()
#endif	/* UsePrototypes */
{
    FILE	*fopen();
    Option	*basediropt, *scorefileopt, *respfileopt;
    char	*basedir, *scorefname, *respfname, *displayfname;
    
    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *) NULL)
	basedir = basediropt->default_value;

    scorefileopt = find_muc_option(":score_file");
    if ((scorefname = scorefileopt->value) == (char *) NULL)
    {
	/* the default file name is a concatenation of the name of
	   the response file and the default score_file value */
	scorefname = scorefileopt->default_value;

	respfileopt = find_muc_option(":response_file");
	if ((respfname = respfileopt->value) == (char *) NULL)
	    respfname = respfileopt->default_value;
	
	displayfname = (char *) malloc (strlen(basedir)
					+ strlen(respfname)
					+ strlen(scorefname)
					+ 1);
	displayfname = strcat(strcat(strcpy(displayfname, basedir),
				     respfname),
			      scorefname);
    }
    else
    {
	displayfname = (char *) malloc (strlen(basedir)
					+ strlen(scorefname)
					+ 1);
	displayfname = strcat(strcpy(displayfname, basedir), scorefname);
    }
    
    if ((muc_score_file = fopen(displayfname, "w")) == (FILE *) NULL)
    {
	fprintf(stderr, "Could not open file '%s' for score report. \n",
		displayfname);
	exit(1);
    }

    UFREE(displayfname);
    
    return;
}

    
#if UsePrototypes
static
void
close_muc_score_file(void)
#else
static
void
close_muc_score_file()
#endif	/* UsePrototypes */
{
    fclose(muc_score_file);
}

#if UsePrototypes
static
void
format_report_header (void)
#else
static
void
format_report_header ()
#endif	/* UsePrototypes */
{
    fprintf(muc_report_file, "%s\n", report_header1);
    fprintf(muc_report_file, "%s\n", report_header2);
    fprintf(muc_report_file, "%s\n", report_header3);
    return;
}
#if UsePrototypes
static
void
format_correct_items (void)
#else
static
void
format_correct_items()
#endif
{
    print_correct_report(1);
    print_missing_report(3);
    print_spurious_report(4);
}

#if UsePrototypes
static
void
report_class_instance(Class_instance *kinst, int kind)
#else
static
void
report_class_instance(kinst,kind)
Class_instance *kinst;
int kind;
#endif	/* UsePrototypes */
{
    TABLE      *slot_fills;
    Slot_fill  *sfill;
    register int i;
    
    if (kinst == (Class_instance *) NULL)
	return;

    
    if (kind == 1)
    fprintf(muc_report_file,"\nCOR");
    else if (kind == 2)
      fprintf(muc_report_file,"\nINC");
    else if (kind == 3)
      fprintf(muc_report_file,"\nMIS                  ");
    else if (kind == 4)
      fprintf(muc_report_file, "\nSPU                                                                   ");
    slot_fills = (TABLE *) kinst->slot_fills;
    
    table_first(slot_fills);
    for(sfill = (Slot_fill *)table_next(slot_fills);
	sfill;
	sfill = (Slot_fill *)table_next(slot_fills))
     {
        int num = sfill->num_strs;
        for (i=0;i<num;i++)
              print_report_string_fill(sfill->strfills[i]);
      }
     fprintf(muc_report_file, "\n");

}
#if UsePrototypes
static
void
report_correct_class_instance(Class_instance *kinst, Class_instance *rinst, int kind)
#else
static
void
report_correct_class_instance(kinst,rinst, kind)
Class_instance *kinst, *rinst;
int kind;
#endif	/* UsePrototypes */
{
  TABLE      *kslot_fills, *rslot_fills;
  Slot_fill  *sfill,*rfill;
  register int i;
  char *k_fill, *r_fill;
  
  if (kinst == (Class_instance *) NULL)
    return;
  if (rinst == (Class_instance *) NULL)
    return;
  
  /* compare type slots */

  kslot_fills = (TABLE *) kinst->slot_fills;
  rslot_fills = (TABLE *) rinst->slot_fills;

  k_fill = r_fill = NULL;

  sfill = find_slot_fill( kslot_fills, "type" );
  if( sfill && sfill->strfills[0] )
    k_fill = sfill->strfills[0]->fill;

  rfill = find_slot_fill( rslot_fills, "type" );
  if( rfill && rfill->strfills[0] )
    r_fill = rfill->strfills[0]->fill;

  if ( k_fill == NULL && r_fill == NULL )
    fprintf(muc_report_file, "NON");    
  else if ( k_fill == NULL )
    fprintf(muc_report_file, "SPU");
  else if ( r_fill == NULL )
    fprintf(muc_report_file, "MIS");
  else if (STRCASEEQ(k_fill, r_fill))
    fprintf(muc_report_file, "COR");
  else
    fprintf(muc_report_file, "INC");

  /* compare text slots */

  k_fill = r_fill = NULL;

  sfill = find_slot_fill( kslot_fills, "text" );
  if( sfill && sfill->strfills[0] )
    k_fill = sfill->strfills[0]->fill;

  rfill = find_slot_fill( rslot_fills, "text" );
  if( rfill && rfill->strfills[0] )
    r_fill = rfill->strfills[0]->fill;

  if ( k_fill == NULL && r_fill == NULL )
    fprintf(muc_report_file, "       NON        ");
  else if ( k_fill == NULL )
    fprintf(muc_report_file, "       SPU        ");
  else if ( r_fill == NULL )
    fprintf(muc_report_file, "       MIS        ");
  else if (!(strcmp (k_fill, r_fill)))
    fprintf(muc_report_file, "       COR        ");
  else
    fprintf(muc_report_file, "       INC        ");
  
  /* print key slots */
  
  sfill = find_slot_fill( kslot_fills, "type" );
  if( sfill )
    print_report_string_fill(sfill->strfills[0]);
  else 
    print_report_string_fill(NULL);

  sfill = find_slot_fill( kslot_fills, "text" );
  if( sfill )
    print_report_string_fill(sfill->strfills[0]);
  else 
    print_report_string_fill(NULL);
  
  /* print response slots */

  rfill = find_slot_fill( rslot_fills, "type" );
  if( rfill )
    print_report_string_fill(rfill->strfills[0]);
  else 
    print_report_string_fill(NULL);

  rfill = find_slot_fill( rslot_fills, "text" );
  if( rfill )
    print_report_string_fill(rfill->strfills[0]);
  else 
    print_report_string_fill(NULL);

  fprintf(muc_report_file, "\n");
}

#if UsePrototypes
static
void
print_report_string_fill(String_fill *sfill)
#else
static
void
print_report_string_fill(sfill)
String_fill *sfill;
#endif
{
  char *temp_str,*new;
  char *str;
  int len, i;

  if( sfill == NULL )
      new = "";
  else
    {
      str = sfill->fill;
      len = strlen(str);
      new = UALLOC (char, (len + 1));
      for (i = 0; i < len; i++)
	{
	  new[i] = str[i];
	  if (str[i] == '\n'){
	    new[i] = ' ';
	  }
	}
      new[len] = '\0';
    }
  fprintf(muc_report_file, "%-25s", new);
  if( sfill )
    UFREE( new );
}
 
#if UsePrototypes
static
void
print_correct_report(kind)
#else
static
void
print_correct_report(kind)
int kind;
#endif
{
    TABLE *Catalog = (TABLE *) MUC_Catalog;
    TABLE  *klists, *rlists;
    TABLE	*kinstances, *rinstances;
    Root   *rroot, *kroot;
    Catalog_entry *entry;
    Class_instance_list *klist, *rlist;
    Class_instance  *kinst, *rinst;
    Class_instance  *kmapped_to, *rmapped_to;
    
    
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
       { 
/*        rroot = entry->r_root;*/
        kroot = entry->k_root;
        klists = (TABLE *) kroot->class_instance_lists;
        if (kroot == (Root *) NULL)
        	return;
        table_first(klists);
        for(klist = (Class_instance_list *)table_next(klists);
            klist;
	    klist = (Class_instance_list *)table_next(klists))
        {
    
         if (klist == (Class_instance_list *) NULL)
	    return;
    
         kinstances = (TABLE *) klist->instances;
    
         table_first(kinstances);
         for(kinst = (Class_instance *)table_next(kinstances);
	    kinst;
	    kinst = (Class_instance *)table_next(kinstances))
         {
           if (kinst == (Class_instance *) NULL)
	     return;
            if((kmapped_to = kinst->mapped_to) != (Class_instance *) NULL)
                  
	          report_correct_class_instance(kinst,kinst->mapped_to,kind);
         }
        }
    }
}


#if UsePrototypes
static
void
print_missing_report(kind)
#else
static
void
print_missing_report(kind)
int kind;
#endif
{
    TABLE *Catalog = (TABLE *) MUC_Catalog;
    TABLE  *klists;
    TABLE	*kinstances;
    Root   *rroot, *kroot;
    Catalog_entry *entry;
    Class_instance_list *klist;
    Class_instance  *kinst;
    Class_instance  *kmapped_to;
    
    
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
       { 
/*        rroot = entry->r_root;*/
        kroot = entry->k_root;
        klists = (TABLE *) kroot->class_instance_lists;
        if (kroot == (Root *) NULL)
        	return;
        table_first(klists);
        for(klist = (Class_instance_list *)table_next(klists);
            klist;
	    klist = (Class_instance_list *)table_next(klists))
        {
    
         if (klist == (Class_instance_list *) NULL)
	    return;
    
         kinstances = (TABLE *) klist->instances;
    
         table_first(kinstances);
         for(kinst = (Class_instance *)table_next(kinstances);
	    kinst;
	    kinst = (Class_instance *)table_next(kinstances))
         {
           if (kinst == (Class_instance *) NULL)
	     return;
            if((kmapped_to = kinst->mapped_to) == (Class_instance *) NULL) 
            {
	          report_class_instance(kinst,kind);
             }
         }
        }
    }
}
#if UsePrototypes
static
void
print_spurious_report(kind)
#else
static
void
print_spurious_report(kind)
int kind;
#endif
{
    TABLE *Catalog = (TABLE *) MUC_Catalog;
    TABLE  *rlists;
    TABLE	*rinstances;
    Root   *rroot, *kroot;
    Catalog_entry *entry;
    Class_instance_list *rlist;
    Class_instance  *rinst;
    Class_instance  *rmapped_to;
    
    
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
       { 
       rroot = entry->r_root;
        rlists = (TABLE *) rroot->class_instance_lists;
        if (rroot == (Root *) NULL)
        	return;
        table_first(rlists);
        for(rlist = (Class_instance_list *)table_next(rlists);
            rlist;
	    rlist = (Class_instance_list *)table_next(rlists))
        {
    
         if (rlist == (Class_instance_list *) NULL)
	    return;
    
         rinstances = (TABLE *) rlist->instances;
    
         table_first(rinstances);
         for(rinst = (Class_instance *)table_next(rinstances);
	    rinst;
	    rinst = (Class_instance *)table_next(rinstances))
         {
           if (rinst == (Class_instance *) NULL)
	     return;
            if((rmapped_to = rinst->mapped_to) == (Class_instance *) NULL) 
            {
	          report_class_instance(rinst,kind);
             }
         }
        }
    }
}
#if UsePrototypes
static
void
print_incorrect_report(kind)
#else
static
void
print_incorrect_report(kind)
int kind;
#endif
{
    TABLE *Catalog = (TABLE *) MUC_Catalog;
    TABLE  *klists;
    TABLE	*kinstances;
    Root   *rroot, *kroot;
    Catalog_entry *entry;
    Class_instance_list *klist;
    Class_instance  *kinst;
    Class_instance  *kconnected_to;
    
    
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
       { 
        rroot = entry->r_root;
        kroot = entry->k_root;
        klists = (TABLE *) kroot->class_instance_lists;
        if (kroot == (Root *) NULL)
        	return;
        table_first(klists);
        for(klist = (Class_instance_list *)table_next(klists);
            klist;
	    klist = (Class_instance_list *)table_next(klists))
        {
    
         if (klist == (Class_instance_list *) NULL)
	    return;
    
         kinstances = (TABLE *) klist->instances;
    
         table_first(kinstances);
         for(kinst = (Class_instance *)table_next(kinstances);
	    kinst;
	    kinst = (Class_instance *)table_next(kinstances))
         {
           if (kinst == (Class_instance *) NULL)
	     return;
            if((kconnected_to= kinst->connected_to) == (Class_instance *) NULL) {
             return;
            }
            else  {
	          report_class_instance(kinst,kind);
             }
         }
        }
    }
}

#if UsePrototypes
static
void
open_muc_report_file(void)
#else
static
void
open_muc_report_file()
#endif	/* UsePrototypes */
{
    FILE	*fopen();
    Option	*basediropt, *scorefileopt, *respfileopt;
    char	*basedir, *scorefname, *respfname, *displayfname;
    
    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *) NULL)
	basedir = basediropt->default_value;

    scorefileopt = find_muc_option(":report_file");
    if ((scorefname = scorefileopt->value) == (char *) NULL)
    {
	/* the default file name is a concatenation of the name of
	   the response file and the default score_file value */
	scorefname = scorefileopt->default_value;

	respfileopt = find_muc_option(":response_file");
	if ((respfname = respfileopt->value) == (char *) NULL)
	    respfname = respfileopt->default_value;
	
	displayfname = (char *) malloc (strlen(basedir)
					+ strlen(respfname)
					+ strlen(scorefname)
					+ 1);
	displayfname = strcat(strcat(strcpy(displayfname, basedir),
				     respfname),
			      scorefname);
    }
    else
    {
	displayfname = (char *) malloc (strlen(basedir)
					+ strlen(scorefname)
					+ 1);
	displayfname = strcat(strcpy(displayfname, basedir), scorefname);
    }
    
    if ((muc_report_file = fopen(displayfname, "w")) == (FILE *) NULL)
    {
	fprintf(stderr, "Could not open file '%s' for report summary file. \n",
		displayfname);
	exit(1);
    }

    UFREE(displayfname);
    
    return;
}

#if UsePrototypes
static
void
close_muc_report_file(void)
#else
static
void
close_muc_report_file()
#endif	/* UsePrototypes */
{
    fclose(muc_report_file);
}


