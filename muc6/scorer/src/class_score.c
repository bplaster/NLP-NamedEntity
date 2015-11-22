/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	class_score.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Class_score data structure.
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
static	char	SccsId[] = "@(#)class_score.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"

/************************************************************************
 *									*
 *  class_score_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Class_score *
class_score_create(Class_def *class, char *docnum)
#else
Class_score *
class_score_create(class, docnum)
Class_def *class;
char *docnum;
#endif	/* UsePrototypes */
{
    Class_score *newscore;
    char *cname = class->name;
    
    if((newscore = UALLOC(Class_score, 1)) == (Class_score *)NULL)
    {
	fprintf(stderr, "Error: [class_score_create]: Allocation failed.\n");
	exit(1);
    }
    
    newscore->name = STRALLOC(cname);
    newscore->docnum = STRALLOC(docnum);
    newscore->has_instances = False;
    newscore->tallies = tallies_create();
    
    /* create an empty table for the slot scores for this class
       and build it by calling initialize_slot_scores. */
    newscore->slot_scores = table_list_create(slot_score_compare,
					      slot_score_release,
					      default_destroy);

    if(newscore->slot_scores == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: [class_score_create]: '%s' - ", cname);
	fprintf(stderr, "unable to create slot_scores table.\n");
	exit(1);
    }

    initialize_slot_scores((TABLE *) newscore->slot_scores, class, docnum);

    return(newscore);
}


/************************************************************************
 *									*
 *  class_score_release							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
class_score_release(char *elt)
#else
void
class_score_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Class_score *ptr = (Class_score *)elt;
    
    if(ptr == (Class_score *)NULL)
	return;
    
    UFREE(ptr->name);
    UFREE(ptr->docnum);
    tallies_release((char *) ptr->tallies);
    
    /* free up the table of slot_scores */
    table_destroy(ptr->slot_scores);
    
    UFREE(ptr);
}


/************************************************************************
 *									*
 *  class_score_compare							*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
class_score_compare(char *elt1, char *elt2)
#else
int
class_score_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Class_score *ptr1 = (Class_score *)elt1;
    Class_score *ptr2 = (Class_score *)elt2;
    
    return(strcasecmp(ptr1->name, ptr2->name));
}


/************************************************************************
 *									*
 *  initialize_class_scores						*
 *									*
 *  initialize the class_scores table by creating a Class_score		*
 *  structure for every defined class.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
initialize_class_scores(TABLE *scores, char *docnum)
#else
void
initialize_class_scores(scores, docnum)
TABLE *scores;
char *docnum;
#endif	/* UsePrototypes */
{
    TABLE  *Class_defs = MUC_Class_defs;
    Class_def  *class_def;
    Class_score *newscore;
    
    table_first(Class_defs);
    for(class_def = (Class_def *)table_next(Class_defs);
	class_def;
	class_def = (Class_def *)table_next(Class_defs))
    {
	newscore = class_score_create(class_def, docnum);

	/* insert into table.  don't bother checking for pre-existing
	   structure in the list.  we're basically copying the Class_defs
	   list, which shouldn't have any duplication */
    
	if(table_append((char *) newscore, (TABLE *) scores) == TABLE_ERR)
	{
	    fprintf(stderr, "Error: [initialize_class_scores]: ");
	    fprintf(stderr, " table insertion failed.\n");
	    exit(1);
	}
    }

    return;
}


/************************************************************************
 *									*
 *  find_class_score							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Class_score *
find_class_score(TABLE *scores, char *cname)
#else
Class_score *
find_class_score(scores, cname)
TABLE *scores;
char *cname;
#endif	/* UsePrototypes */
{
    Class_score  *this_score;
    
    /* search the table for a class_score having the same
       name as cname. */
    table_first(scores);
    for(this_score = (Class_score *)table_next(scores);
	this_score && !(STRCASEEQ(cname, this_score->name));
	this_score = (Class_score *)table_next(scores))
	;
    
    return(this_score);
}
