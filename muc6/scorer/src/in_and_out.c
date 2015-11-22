/*
 * NAME
 *
 * 
 * FILE 
 *	in_and_out.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	special routines to create links between identical
 *	in_and_out instances.  identical instances will yield
 *	identical map scores, and may lead to the 'wrong' key
 *	being mapped to the 'wrong' response, the effect being
 *	that the succession_events that point to these may be
 *	affected, both in mapping and in scoring.
 *
 *	links between identical instances allows the mapper and
 *	the scorer to find the one that yields the most favorable
 *	score.
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
 *	Gary Dungca	September 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)in_and_out.c	1.2  27 Oct 1995  04 Sep 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include <stdio.h>
#include "table.h"
#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"

/* internal function prototypes */

Proto	(static	Bool, identical_instances, (Class_instance *inst1,
					    Class_instance *inst2));
Proto	(static	Bool, compare_in_and_out_instances, (Class_instance *inst1,
						     Class_instance *inst2));
Proto	(static	Bool, compare_in_and_out_alt_fills, (Slot_fill *xfill,
						     Slot_fill *yfill));
Proto	(static	Bool, compare_in_and_out_fills, (Slot_fill *xfill,
						 Slot_fill *yfill));
Proto	(static	Bool, compare_in_and_out_alt_sets, (Alt_fill_set *xset,
						    Alt_fill_set *yset));
Proto	(static	void, link_identical_instances, (Class_instance *inst1,
						 Class_instance *inst2));


/************************************************************************
 *									*
 *  find_identical_in_and_out_instances					*
 *									*
 *  special situation involving in_and_out instances:			*
 *  identical keys may be defined, but pointed to by different		*
 *  succession_events.  since identical instances will yield		*
 *  identical mapping scores, the 'wrong' key may be mapped to		*
 *  the 'wrong' response, affecting the scores of the succession	*
 *  events.								*
 *									*
 *  a solution is to create links from one instance to all other	*
 *  identical instances.  when scoring the in_and_out pointer slots	*
 *  of succession event instances, give 'correct' credit if at		*
 *  least one of the identical in_and_out instances is mapped to	*
 *  the response instance.						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
find_identical_in_and_out_instances(Class_instance_list *io_list)
#else
void
find_identical_in_and_out_instances(io_list)
Class_instance_list *io_list;
#endif	/* UsePrototypes */
{
    register int inner, outer, io_index;
    TABLE *io_instances = (TABLE *) io_list->instances;
    Class_instance *io_inst, *ident_inst;
    int io_count = table_count(io_instances);

    for (io_index=0; io_index<io_count; io_index++)
    {
	/* progressively work through the table by getting the
	   io_inst at io_index. */
	table_first(io_instances);
	for(io_inst = (Class_instance *)table_next(io_instances), outer=0;
	    outer<io_index;
	    io_inst = (Class_instance *)table_next(io_instances), outer++)
	    ;

	if(io_inst != (Class_instance *) NULL)
	{
	    /* for every other inst (not at the io_index position),
	       check if it is identical to io_inst. */
	    table_first(io_instances);
	    for(ident_inst = (Class_instance *)table_next(io_instances),
		inner=0;
		ident_inst;
		ident_inst = (Class_instance *)table_next(io_instances),
		inner++)
	    {
		if((ident_inst != (Class_instance *) NULL)
		   && (inner != io_index)
		   && identical_instances(io_inst, ident_inst))
		    link_identical_instances(io_inst, ident_inst);
	    }
	}
    }
}


#if UsePrototypes
static
Bool
identical_instances(Class_instance *inst1, Class_instance *inst2)
#else
static
Bool
identical_instances(inst1, inst2)
Class_instance *inst1;
Class_instance *inst2;
#endif	/* UsePrototypes */
{
    Pointer *lastptr;
    Bool identical = False;

    /* before comparing the two instances, find out if they've
       been linked previously */
    for(lastptr = inst1->ident_in_and_out_ptrs;
	lastptr && !(identical = (lastptr->inst == inst2));
	lastptr = lastptr->next)
	;

    /* if they're not already linked, then compare the two instances */
    if (!identical)
	identical = compare_in_and_out_instances(inst1, inst2);

    return(identical);
}


#if UsePrototypes
static
Bool
compare_in_and_out_instances(Class_instance *inst1, Class_instance *inst2)
#else
static
Bool
compare_in_and_out_instances(inst1, inst2)
Class_instance *inst1;
Class_instance *inst2;
#endif	/* UsePrototypes */
{
    Class_def  *io_def = find_class_def_by_name("in_and_out");
    TABLE  *io_slots = (TABLE *) io_def->slot_defs;
    Slot_def  *io_slot;
    Slot_fill  *inst1_fill, *inst2_fill;
    char  *sname;
    Bool  identical = True;
    
    /* iterate over the list of slots */
    table_first(io_slots);
    for (io_slot = (Slot_def *) table_next(io_slots);
	 io_slot;
	 io_slot = (Slot_def *) table_next(io_slots))
    {
	/* compare slot fills until either all comparisons are done
	   or until unequal slot fills are found. */
	if (io_slot->is_scored_slot)
	{
	    sname = io_slot->name;
	    inst1_fill = find_slot_fill(inst1->slot_fills, sname);
	    inst2_fill = find_slot_fill(inst2->slot_fills, sname);
	    if (inst1_fill
		     && (inst1_fill->alt_fill_sets != (Alt_fill_set *) NULL))
		identical = compare_in_and_out_alt_fills(inst1_fill,
							 inst2_fill);
	    else
		identical = compare_in_and_out_fills(inst1_fill, inst2_fill);
	}
	/* if any slots are not identical, then end the comparison. */
	if (!identical)
	    break;
    }
    
    return(identical);
}


#if UsePrototypes
static
Bool
compare_in_and_out_alt_fills(Slot_fill *xfill, Slot_fill *yfill)
#else
static
Bool
compare_in_and_out_alt_fills(xfill, yfill)
Slot_fill *xfill;
Slot_fill *yfill;
#endif	/* UsePrototypes */
{
    Alt_fill_set *xset, *yset;
    Bool identical;
    
    /* compare the main fills first.  if they are identical,
       then compare the alternate sets. */
    if (identical = compare_in_and_out_fills(xfill, yfill))
    {
	/* iterate over both lists of alternate sets.  stop when both lists
	   are finished, or when any pair of sets are not identical. */
	for (xset = xfill->alt_fill_sets, yset = yfill->alt_fill_sets;
	     identical && (xset || yset);
	     xset = xset->next, yset = yset->next)
	{
	    /* if only one of them is NULL, then they aren't identical */
	    if ((xset == (Alt_fill_set *) NULL)
		|| (yset == (Alt_fill_set *) NULL))
		identical = False;
	    else
		identical = compare_in_and_out_alt_sets(xset, yset);
	}
    }
    
    return(identical);
}


#if UsePrototypes
static
Bool
compare_in_and_out_fills(Slot_fill *xfill, Slot_fill *yfill)
#else
static
Bool
compare_in_and_out_fills(xfill, yfill)
Slot_fill *xfill;
Slot_fill *yfill;
#endif	/* UsePrototypes */
{
    register int i, j;
    char *nullstr = (char *) NULL;
    Bool identical = True;
    Bool match_found;

    String_fill **xstrs, **ystrs;
    int num_xstrs, num_ystrs;
    Pointer *xptrs, *yptrs, *xptr, *yptr;
    Class_instance *xinst, *yinst;

    if((xfill == (Slot_fill *) NULL)
       && (yfill == (Slot_fill *) NULL))
	identical = True;
    else if ((xfill == (Slot_fill *) NULL)
	     || (yfill == (Slot_fill *) NULL))
	identical = False;
    else
    {
	xstrs = xfill->strfills;
	num_xstrs = xfill->num_strs;
	xptrs = xfill->instptrs;

	ystrs = yfill->strfills;
	num_ystrs = yfill->num_strs;
	yptrs = yfill->instptrs;

	/* if number of fills are different, then they aren't identical */
	if ((num_xstrs != num_ystrs)
	    || (xfill->num_ptrs != yfill->num_ptrs))
	    identical = False;
	else
	{
	    /* reset the match_status for all keys and responses */
	    for(i=0; i<num_xstrs; i++)
		xstrs[i]->match_status = MISMATCH;
	    for(j=0; j<num_ystrs; j++)
		ystrs[j]->match_status = MISMATCH;

	    /* compare only unmatched fills */
	    for (i=0;
		 i<num_xstrs && (xstrs[i]->match_status != MATCH);
		 i++)
	    {
		for (j=0;
		     j<num_ystrs && (ystrs[j]->match_status != MATCH);
		     j++)
		{
		    if(muc_streq(xstrs[i]->fill, ystrs[j]->fill, nullstr))
		    {
			xstrs[i]->match_status = MATCH;
			ystrs[j]->match_status = MATCH;
			break;
		    }
		}
		/* if no match was found for xstrs[i], then these fills
		   are not completely identical. */
		if (xstrs[i]->match_status != MATCH)
		{
		    identical = False;
		    break;
		}
	    }
	    /* continue with pointer comparison only if the fills are
	       still identical.
	       pointers must be the same, otherwise these two instances
	       are not identical. */
	    if (identical)
	    {
		for (xptr = xptrs; xptr; xptr = xptr->next)
		{
		    match_found = False;

		    if (xinst = xptr->inst)
		    {
			for (yptr = yptrs; yptr; yptr = yptr->next)
			{
			    if ((yinst = yptr->inst)
				&& ((Class_instance *) xinst
				    == (Class_instance *) yinst))
			    {
				match_found = True;
				break;
			    }
			}
			/* if no match was found for xptr, then these fills
			   are not completely identical. */
			if (!match_found)
			{
			    identical = False;
			    break;
			}
		    }
		    /* either xinst does not exist or it's not mapped */
		    else
			identical = False;
		}
	    }
	}
    }
    return(identical);
}


#if UsePrototypes
static
Bool
compare_in_and_out_alt_sets(Alt_fill_set *xset, Alt_fill_set *yset)
#else
static
Bool
compare_in_and_out_alt_sets(xset, yset)
Alt_fill_set *xset;
Alt_fill_set *yset;
#endif	/* UsePrototypes */
{
    register int i, j;
    char *nullstr = (char *) NULL;
    Bool identical = True;
    Bool match_found;

    String_fill **xstrs = xset->strfills;
    int num_xstrs = xset->num_strs;
    Pointer *xptrs = xset->instptrs;

    String_fill **ystrs = yset->strfills;
    int num_ystrs = yset->num_strs;
    Pointer *yptrs = yset->instptrs;

    Pointer *xptr, *yptr;
    Class_instance *xinst, *yinst;

    /* if number of fills are different, then they aren't identical */
    if ((num_xstrs != num_ystrs)
	|| (xset->num_ptrs != yset->num_ptrs))
	identical = False;
    else
    {
	/* reset the match_status for all keys and responses */
	for(i=0; i<num_xstrs; i++)
	    xstrs[i]->match_status = MISMATCH;
	for(j=0; j<num_ystrs; j++)
	    ystrs[j]->match_status = MISMATCH;

	/* compare only unmatched fills */
	for (i=0;
	     i<num_xstrs && (xstrs[i]->match_status != MATCH);
	     i++)
	{
	    for (j=0;
		 j<num_ystrs && (ystrs[j]->match_status != MATCH);
		 j++)
	    {
		if(muc_streq(xstrs[i]->fill, ystrs[j]->fill, nullstr))
		{
		    xstrs[i]->match_status = MATCH;
		    ystrs[j]->match_status = MATCH;
		    break;
		}
	    }
	    /* if no match was found for xstrs[i], then these fills
	       are not completely identical. */
	    if (xstrs[i]->match_status != MATCH)
	    {
		identical = False;
		break;
	    }
	}
	/* continue with pointer comparison only if the fills are
	   still identical.
	   pointers must be the same, otherwise these two instances
	   are not identical. */
	if (identical)
	{
	    for (xptr = xptrs; xptr; xptr = xptr->next)
	    {
		match_found = False;

		if (xinst = xptr->inst)
		{
		    for (yptr = yptrs; yptr; yptr = yptr->next)
		    {
			if ((yinst = yptr->inst)
			    && ((Class_instance *) xinst
				== (Class_instance *) yinst))
			{
			    match_found = True;
			    break;
			}
		    }
		    /* if no match was found for xptr, then these fills
		       are not completely identical. */
		    if (!match_found)
		    {
			identical = False;
			break;
		    }
		}
		/* either xinst does not exist or it's not mapped */
		else
		    identical = False;
	    }
	}
    }

    return(identical);
}


#if UsePrototypes
static
void
link_identical_instances(Class_instance *inst1, Class_instance *inst2)
#else
static
void
link_identical_instances(inst1, inst2)
Class_instance *inst1;
Class_instance *inst2;
#endif	/* UsePrototypes */
{
    Pointer *inst1_ptr, *inst2_ptr, *lastptr;
    
    if((inst1_ptr = UALLOC(Pointer, 1)) == (Pointer *) NULL)
    {
	fprintf(stderr, "Error: [link_identical_instances]: ");
	fprintf(stderr, "Allocation of first Pointer failed.\n");
	exit(1);
    }
    
    inst1_ptr->inst = inst1;
    inst1_ptr->next = (Pointer *) NULL;

    if((inst2_ptr = UALLOC(Pointer, 1)) == (Pointer *) NULL)
    {
	fprintf(stderr, "Error: [link_identical_instances]: ");
	fprintf(stderr, "Allocation of second Pointer failed.\n");
	exit(1);
    }
    
    inst2_ptr->inst = inst2;
    inst2_ptr->next = (Pointer *) NULL;

    /* is this the first of inst1's ident_in_and_out pointers? */
    if (inst1->ident_in_and_out_ptrs == (Pointer *)NULL)
	inst1->ident_in_and_out_ptrs = inst2_ptr;
    else
    {
	for(lastptr = inst1->ident_in_and_out_ptrs;
	    lastptr->next;
	    lastptr = lastptr->next)
	    ;
	lastptr->next = inst2_ptr;
    }
    
    /* is this the first of inst2's ident_in_and_out pointers? */
    if (inst2->ident_in_and_out_ptrs == (Pointer *)NULL)
	inst2->ident_in_and_out_ptrs = inst1_ptr;
    else
    {
	for(lastptr = inst2->ident_in_and_out_ptrs;
	    lastptr->next;
	    lastptr = lastptr->next)
	    ;
	lastptr->next = inst1_ptr;
    }

    return;
}
