/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	slot_score.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Slot_score data structure.
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
static	char	SccsId[] = "@(#)slot_score.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"


/************************************************************************
 *									*
 *  slot_score_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Slot_score *
slot_score_create(char *name, char *docnum)
#else
Slot_score *
slot_score_create(name, docnum)
char *name;
char *docnum;
#endif	/* UsePrototypes */
{
    Slot_score *newscore;
    
    if((newscore = UALLOC(Slot_score, 1)) == (Slot_score *)NULL)
    {
	fprintf(stderr, "Error: [slot_score_create]: Allocation failed.\n");
	exit(1);
    }
    
    newscore->name = STRALLOC(name);
    newscore->docnum = STRALLOC(docnum);
    newscore->tallies = tallies_create();
    
    return(newscore);
}


/************************************************************************
 *									*
 *  slot_score_release							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
slot_score_release(char *elt)
#else
void
slot_score_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Slot_score *ptr = (Slot_score *)elt;
    
    UFREE(ptr->name);
    UFREE(ptr->docnum);
    tallies_release((char *) ptr->tallies);
    
    UFREE(ptr);
}


/************************************************************************
 *									*
 *  slot_score_compare							*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
slot_score_compare(char *elt1, char *elt2)
#else
int
slot_score_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Slot_score *ptr1 = (Slot_score *)elt1;
    Slot_score *ptr2 = (Slot_score *)elt2;
    
    return(strcasecmp(ptr1->name, ptr2->name));
}


/************************************************************************
 *									*
 *  initialize_slot_scores						*
 *									*
 *  initialize the slot_scores table by creating a Slot_score		*
 *  structure for every defined slot of CLASS.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
initialize_slot_scores(TABLE *scores, Class_def *class, char *docnum)
#else
void
initialize_slot_scores(scores, class, docnum)
TABLE *scores;
Class_def *class;
char *docnum;
#endif	/* UsePrototypes */
{
    TABLE  *slots = class->slot_defs;
    Slot_def  *slot;
    Slot_score *newscore;
    
    table_first(slots);
    for(slot = (Slot_def *)table_next(slots);
	slot;
	slot = (Slot_def *)table_next(slots))
    {
	newscore = slot_score_create(slot->name, docnum);

	/* insert into table.  don't bother checking for pre-existing
	   structure in the list.  we're basically copying the slot_defs
	   list of this class, which shouldn't have any duplication */

	if(table_append((char *) newscore, (TABLE *) scores) == TABLE_ERR)
	{
	    fprintf(stderr, "Error: [initialize_slot_scores]: ");
	    fprintf(stderr, " table insertion failed.\n");
	    exit(1);
	}
    }

    return;
}


/************************************************************************
 *									*
 *  find_slot_score							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Slot_score *
find_slot_score(TABLE *scores, char *sname)
#else
Slot_score *
find_slot_score(scores, sname)
TABLE *scores;
char *sname;
#endif	/* UsePrototypes */
{
    Slot_score  *this_score;
    
    /* search the table for a slot_score having the same
       name as sname. */
    table_first(scores);
    for(this_score = (Slot_score *)table_next(scores);
	this_score && !(STRCASEEQ(sname, this_score->name));
	this_score = (Slot_score *)table_next(scores))
	;
    
    return(this_score);
}
