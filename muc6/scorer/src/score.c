/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	score.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	scoring functions for the MUC scorer.
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
static	char	SccsId[] = "@(#)score.c	1.11 25 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include <stdio.h>
#include "table.h"
#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"


/*  internal function prototypes */

Proto	(static void,
	 score_template_instances,
	 (Class_def *class,
	  Class_score *c_score,
	  Class_instance_list *instlist));
Proto	(static void, score_class_instances, (Class_def *class,
					      Class_score *c_score,
					      Class_instance_list *instlist));
Proto	(static void, score_unmapped_instance, (TABLE *slot_defs,
						Class_score *c_score,
						Class_instance *inst));
Proto	(static void, score_unmapped_key_slot, (Class_instance *inst,
						Slot_def *slot,
						Slot_score *s_score,
						Slot_fill *fill));
Proto	(static void, score_unmapped_response_slot, (Class_instance *inst,
						     Slot_def *slot,
						     Slot_score *s_score,
						     Slot_fill *fill));
Proto	(static	void, score_connected_pair, (TABLE *slot_defs,
					     Class_score *c_score,
					     Class_instance *kinst,
					     Class_instance *rinst));
Proto	(static	void, score_connected_slot, (Slot_def *slot,
					     Slot_score *s_score,
					     Slot_fill *kfill,
					     Slot_fill *rfill));
Proto	(static void, score_noncommittal, (Tallies *tallies,
					   Slot_fill *kfill,
					   Slot_fill *rfill));
Proto	(static void, score_missing, (Tallies *tallies,
				      Slot_fill *kfill,
				      Slot_fill *rfill));
Proto	(static void, score_spurious, (Tallies *tallies,
				       Slot_fill *kfill,
				       Slot_fill *rfill));
Proto	(static void, score_alternate_fills, (Slot_def *slot,
					      Tallies *tallies,
					      Slot_fill *kfill,
					      Slot_fill *rfill));
Proto	(static void, score_pointer_fills, (Slot_def *slot,
					    Tallies *tallies,
					    Pointer *kptr,
					    int numk,
					    Pointer *rptr,
					    int numr));
Proto	(static void, score_set_fills, (Slot_def *slot,
					Tallies *tallies,
					String_fill **keys,
					int numk,
					String_fill **responses,
					int numr));
Proto	(static void, score_string_fills, (Slot_def *slot,
					   Tallies *tallies,
					   String_fill **keys,
					   int numk,
					   String_fill **responses,
					   int numr,
					   String_fill *minstr));
Proto	(static int, count_min_alt_fills, (Alt_fill_set *set));
Proto	(static int, determine_fill_type, (Slot_def *slot));
Proto	(static Bool, is_a_multi_fill_type, (Slot_def *slot));
Proto	(static Bool, is_a_string_fill_type, (Slot_def *slot));
Proto	(static Bool, is_a_set_fill_type, (Slot_def *slot));
Proto	(static Bool, is_a_location_fill_type, (Slot_def *slot));
Proto	(static Bool, is_noncommittal, (Slot_fill *kfill, Slot_fill *rfill));
Proto	(static Bool, is_missing, (Slot_fill *kfill, Slot_fill *rfill));
Proto	(static Bool, is_spurious, (Slot_fill *kfill, Slot_fill *rfill));
Proto	(static Bool, is_with_alternates, (Slot_fill *kfill, Slot_fill
					   *rfill));

    
/************************************************************************
 *									*
 *  score_MUC_catalog()							*
 *									*
 *  score all documents and templates.					*
 *  calls score_catalog_entry.						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
score_MUC_catalog(void)
#else
void
score_MUC_catalog()
#endif	/* UsePrototypes */
{
    TABLE  *Catalog = (TABLE *)MUC_Catalog;
    Catalog_entry *entry;

    printf("Begin Scoring.\n");
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
    {
	printf("scoring %s...", entry->docnum);
	score_catalog_entry(entry);
	printf("done.\n");
    }

    printf("Scoring Completed.\n");
    return;
}


/************************************************************************
 *									*
 *  score_catalog_entry							*
 *									*
 *  if catalog_entry points to both a key and a response root, then	*
 *  each root will have a table of classes, with each class pointing	*
 *  to a table of class instances.  for each class, call the function	*
 *  score_class_instances to score the instances in the key against	*
 *  the instances in the response.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
score_catalog_entry(Catalog_entry *entry)
#else
void
score_catalog_entry(entry)
Catalog_entry *entry;
#endif	/* UsePrototypes */
{
    TABLE  *Class_defs = (TABLE *) MUC_Class_defs;
    TABLE  *k_class_instlists, *r_class_instlists;
    TABLE  *class_scores;
    Root  *k_root, *r_root;
    Class_instance_list  *k_class_instlist, *r_class_instlist;
    Class_def  *class;
    Class_score *c_score;
    char *classname;

    /* set MUC_mode to 'SCORE', so that certain shared functions can be
       disabled or enabled if necessary
       (e.g., add_to_ne_subcategorization_tallies */
    MUC_mode = SCORE_MODE;

    k_root = entry->k_root;
    r_root = entry->r_root;
    
    /* must have k_root or r_root, otherwise there's nothing
       to score if they're both null. */
    if((k_root == (Root *) NULL) && (r_root == (Root *) NULL))
	return;

    if (k_root)
	k_class_instlists = (TABLE *) k_root->class_instance_lists;
    if (r_root)
	r_class_instlists = (TABLE *) r_root->class_instance_lists;

    class_scores = (TABLE *) entry->class_scores;

    /* follow the Class_defs list to score the classes for which there
       are key and/or response instances */

    table_first(Class_defs);
    for(class = (Class_def *)table_next(Class_defs);
	class;
	class = (Class_def *)table_next(Class_defs))
    {
	if (class->is_scored_class)
	{
	    classname = class->name;
	    if (k_root)
		k_class_instlist = find_class_instance_list(k_class_instlists,
							    classname);
	    else
		k_class_instlist = (Class_instance_list *) NULL;
	    
	    if (r_root)
		r_class_instlist = find_class_instance_list(r_class_instlists,
							    classname);
	    else
		r_class_instlist = (Class_instance_list *) NULL;

	    c_score = find_class_score(class_scores, classname);
	
	    if (k_class_instlist != (Class_instance_list *) NULL)
	    {
		MUC_instance_type = KEY;
	if STRCASEEQ(class->name, "template")
		    score_template_instances(class,
					     c_score,
					     k_class_instlist);
		else
		    score_class_instances(class,
					  c_score,
					  k_class_instlist);
	    }
	
	    if (r_class_instlist != (Class_instance_list *) NULL)
	    {
		MUC_instance_type = RESPONSE;
	if STRCASEEQ(class->name, "template")
                /* if (!(muc_cmp_special(class->name,"template")) ||
                     !(muc_cmp_special(class->name,"TEMPLATE")))*/
		    score_template_instances(class,
					     c_score,
					     r_class_instlist);
		else
		    score_class_instances(class,
					  c_score,
					  r_class_instlist);
	    }
	}
    }

    /* reset MUC_mode */
    MUC_mode = NO_MODE;
    
    return;
}


/************************************************************************
 *									*
 *  score_relevancy							*
 *									*
 *  check the content slot of the first (and only) template instances	*
 *  in the given entry, and record result in the given tallies struct	*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
score_relevancy(Catalog_entry *entry, Tallies *relevancy)
#else
void
score_relevancy(entry, relevancy)
Catalog_entry *entry;
Tallies *relevancy;
#endif	/* UsePrototypes */
{
    Root *k_root = entry->k_root;
    Root *r_root = entry->r_root;
    TABLE *k_class_inst_lists;
    TABLE *r_class_inst_lists;
    Class_instance_list *k_tmpl_inst_list;
    Class_instance_list *r_tmpl_inst_list;
    TABLE *k_instances, *r_instances;
    Class_instance *k_tmpl, *r_tmpl;
    Slot_fill *kfill = (Slot_fill *) NULL;
    Slot_fill *rfill = (Slot_fill *) NULL;
    
    if (k_root)
    {
	k_class_inst_lists = (TABLE *) k_root->class_instance_lists;
	
	k_tmpl_inst_list = find_class_instance_list(k_class_inst_lists,
						    "template");

	if (k_tmpl_inst_list != (Class_instance_list *) NULL)
	{
	    k_instances = (TABLE *) k_tmpl_inst_list->instances;
	    table_first(k_instances);
	    k_tmpl = (Class_instance *) table_next(k_instances);
	    kfill = find_slot_fill(k_tmpl->slot_fills, "content");
	}
    }
    
    if (r_root)
    {
	r_class_inst_lists = (TABLE *) r_root->class_instance_lists;
	
	r_tmpl_inst_list = find_class_instance_list(r_class_inst_lists,
						    "template");

	if (r_tmpl_inst_list != (Class_instance_list *) NULL)
	{
	    r_instances = (TABLE *) r_tmpl_inst_list->instances;
	    table_first(r_instances);
	    r_tmpl = (Class_instance *) table_next(r_instances);
	    rfill = find_slot_fill(r_tmpl->slot_fills, "content");
	}
    }
    
    if (is_noncommittal(kfill, rfill))
	tally1_noncommittal(relevancy);
    else if (is_missing(kfill, rfill))
    {
	tally1_possible(relevancy);
	tally1_missing(relevancy);
    }
    else if (is_spurious(kfill, rfill))
    {
	tally1_actual(relevancy);
	tally1_spurious(relevancy);
    }
    else
    {
	tally1_possible(relevancy);
	tally1_actual(relevancy);
	tally1_correct(relevancy);
    }

    return;
}


/************************************************************************
 *									*
 *  score_template_instances						*
 *									*
 *  for each of the instances, and based on whether they are		*
 *  relevant, call one of the functions:				*
 *	score_unmapped_instance						*
 *	score_instance_pair						*
 *									*
 *  for class level scoring, a key and response template instance are	*
 *  'correct' only if they are both relevant (i.e., they have content).	*
 *  they are 'noncommittal' if both lack content, and 'missing' or	*
 *  'spurious' depending on relevance of key or response.		*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
score_template_instances(Class_def *class,
			 Class_score *c_score,
			 Class_instance_list *instlist)
#else
static
void
score_template_instances(class, c_score, instlist)
Class_def *class;
Class_score *c_score;
Class_instance_list *instlist;
#endif	/* UsePrototypes */
{
    TABLE  *slots = (TABLE *) class->slot_defs;
    TABLE  *instances = (TABLE *) instlist->instances;
    Class_instance *templ, *mapped_to;
    Tallies *c_tallies = c_score->tallies;
    Slot_fill *t_fill = (Slot_fill *) NULL;
    Slot_fill *m_fill = (Slot_fill *) NULL;

    /* there's at most one template instance */
    table_first(instances);
    if (templ = (Class_instance *)table_next(instances))
    {
	t_fill = find_slot_fill(templ->slot_fills, "content");

	if (mapped_to = templ->mapped_to)
	    m_fill = find_slot_fill(mapped_to->slot_fills, "content");

	if(MUC_instance_type == KEY)
	{
	    /* a template may be mapped only because the key and/or
	       the response was instantiated, but not necessarily
	       relevant.  therefore, the check is on the relevance
	       of t_fill and m_fill, and not simply on mapping. */

	    if (is_noncommittal(t_fill, m_fill))
		tally1_noncommittal(c_tallies);
	    else if (is_missing(t_fill, m_fill))
	    {
		tally1_possible(c_tallies);
		tally1_missing(c_tallies);
		score_unmapped_instance(slots, c_score, templ);
	    }
	    else if (is_spurious(t_fill, m_fill))
	    {
		tally1_actual(c_tallies);
		tally1_spurious(c_tallies);
	    }
	    else
	    {
		tally1_possible(c_tallies);
		tally1_actual(c_tallies);
		tally1_correct(c_tallies);
		score_instance_pair(slots, c_score, templ, mapped_to);
	    }
	}
	else	/* RESPONSE */
	{
	    /* mapped response templates have been scored when scoring
	       the mapped keys, so ignore them and score only the
	       unmapped responses, in which case they are either
	       spurious or noncommittal, depending on the relevance
	       of t_fill (m_fill should be NULL). */
	    if (!mapped_to && is_spurious(m_fill, t_fill))
	    {
		tally1_actual(c_tallies);
		tally1_spurious(c_tallies);
		score_unmapped_instance(slots, c_score, templ);
	    }
	    else if (!mapped_to)
		tally1_noncommittal(c_tallies);
	}
    }
    return;
}


/************************************************************************
 *									*
 *  score_class_instances						*
 *									*
 *  for each of the instances, and based on whether they are		*
 *  mapped, connected, or neither, call one of the functions:		*
 *	score_unmapped_instance						*
 *	score_instance_pair						*
 *									*
 *  for class level scoring, a key and response class instance are	*
 *  'correct' only if they are mapped.  they are 'incorrect' if they	*
 *  are connected.  unmapped keys indicate a 'missing' response, and	*
 *  unmapped responses are 'spurious'.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
score_class_instances(Class_def *class,
		      Class_score *c_score,
		      Class_instance_list *instlist)
#else
static
void
score_class_instances(class, c_score, instlist)
Class_def *class;
Class_score *c_score;
Class_instance_list *instlist;
#endif	/* UsePrototypes */
{
    TABLE  *slots = (TABLE *) class->slot_defs;
    TABLE  *instances = (TABLE *) instlist->instances;
    Class_instance *inst, *mapped_to, *connected_to;
    Tallies *c_tallies = c_score->tallies;
    
    table_first(instances);
    for(inst = (Class_instance *)table_next(instances);
	inst;
	inst = (Class_instance *)table_next(instances))
    {
	mapped_to = inst->mapped_to;
	connected_to = inst->connected_to;

	/* for keeping track of which NE category and doc_section.
	   inst->doc_section = 0 if not NE. */
	if (MUC_scoring_task == NAMED_ENTITY)
	{
	    if STRCASEEQ(inst->name, "ENAMEX")
		NE_category = ENAMEX;
	    else if STRCASEEQ(inst->name, "TIMEX")
		NE_category = TIMEX;
	    else if STRCASEEQ(inst->name, "NUMEX")
		NE_category = NUMEX;

	    NE_doc_section = inst->doc_section;
	}

	if(MUC_instance_type == KEY)
	{
	    if(mapped_to)
	    {
		tally1_possible(c_tallies);
		tally1_actual(c_tallies);
		tally1_correct(c_tallies);
		score_instance_pair(slots, c_score, inst, mapped_to);
	    }
	    else if(connected_to)
	    {
		tally1_possible(c_tallies);
		tally1_actual(c_tallies);
		tally1_incorrect(c_tallies);
		score_connected_pair(slots, c_score, inst, connected_to);
	    }
	    else
	    {
		tally1_possible(c_tallies);
		tally1_missing(c_tallies);
		score_unmapped_instance(slots, c_score, inst);
	    }
	}
	else
	{
	    /* mapped responses have been scored when scoring
	       mapped keys, and connected responses have been
	       scored when scoring connected keys, so this leaves
	       only unmapped, unconnected responses to score. */
	    if(!mapped_to && !connected_to)
	    {
		tally1_actual(c_tallies);
		tally1_spurious(c_tallies);
		score_unmapped_instance(slots, c_score, inst);
	    }
	}
    }

    return;
}


/************************************************************************
 *									*
 *  score_unmapped_instance						*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
score_unmapped_instance(TABLE *slot_defs,
			Class_score *c_score,
			Class_instance *inst)
#else
static
void
score_unmapped_instance(slot_defs, c_score, inst)
TABLE *slot_defs;
Class_score *c_score;
Class_instance *inst;
#endif	/* UsePrototypes */
{
    Slot_def *slot;
    Slot_score *s_score;
    Slot_fill *sfill;
    char *sname;

    /* iterate over the list of slots */
    table_first(slot_defs);
    for(slot = (Slot_def *)table_next(slot_defs);
	slot;
	slot = (Slot_def *)table_next(slot_defs))
    {
	if (slot->is_scored_slot)
	{
	    sname = slot->name;
	    s_score = find_slot_score(c_score->slot_scores, sname);

	    sfill = find_slot_fill(inst->slot_fills, sname);

	    /* subtract the current s_score->tallies from the appropriate
	       NE tallies, then add the new s_score->tallies after scoring.
	       the effect is to add the tallies for this pass only */
	    if (MUC_scoring_task == NAMED_ENTITY)
	    {
		subtract_from_ne_subcategorization_tallies(sfill,
							   s_score->tallies);
		subtract_from_ne_doc_section_tallies(s_score->tallies);
	    }
	    
	    if (MUC_instance_type == KEY)
		score_unmapped_key_slot(inst, slot, s_score, sfill);
	    else
		score_unmapped_response_slot(inst, slot, s_score, sfill);

	    /* add the new s_score->tallies to NE scores */
	    if (MUC_scoring_task == NAMED_ENTITY)
	    {
		add_to_ne_subcategorization_tallies(sfill, s_score->tallies);
		add_to_ne_doc_section_tallies(s_score->tallies);
	    }
	}
    }
    
    return;
}

    
/************************************************************************
 *									*
 *  score_unmapped_key_slot						*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
score_unmapped_key_slot(Class_instance *inst,
			Slot_def *slot,
			Slot_score *s_score,
			Slot_fill *fill)
#else
static
void
score_unmapped_key_slot(inst, slot, s_score, fill)
Class_instance *inst;
Slot_def *slot;
Slot_score *s_score;
Slot_fill *fill;
#endif	/* UsePrototypes */
{
    int  num, min, min_alt_set;
    Tallies *s_tallies = s_score->tallies;
    
    /* skip this altogether if fill doesn't exist or if it's an optional fill */
    if ((fill == (Slot_fill *) NULL)
	|| fill->is_optional)
	return;

    /* the type of fill doesn't matter, since either num_ptrs or num_strs
       will be 0, unless it is a multi_fill, in which case we still need
       to combine them for the total. */

    num = min = fill->num_strs + fill->num_ptrs;

    /* check for alternate fills */
    if (fill->alt_fill_sets != (Alt_fill_set *) NULL)
    {
	/* a situation has arisen where the number of fillers in a slot_fill
	   or in an alt_fill_set may be 0, a result of optional instances
	   being removed.  in this event, we need to find a min > 0.
	   note: if the slot fill is_optional, then it's not a problem.
	   it can only occur when there are alternate fills. */
	
	min_alt_set = count_min_alt_fills(fill->alt_fill_sets);

	/* num = 0 only if all recursive nums are 0 */
	if ( (num = muc_min(min, min_alt_set)) == 0 )
	    num = muc_max(min, min_alt_set);
    }
    
    tally_possible(s_tallies, num);
    tally_missing(s_tallies, num);

    return;
}


/************************************************************************
 *									*
 *  score_unmapped_response_slot					*
 *									*
 *  (no support for key2key scoring yet)				*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
score_unmapped_response_slot(Class_instance *inst,
			     Slot_def *slot,
			     Slot_score *s_score,
			     Slot_fill *fill)
#else
static
void
score_unmapped_response_slot(inst, slot, s_score, fill)
Class_instance *inst;
Slot_def *slot;
Slot_score *s_score;
Slot_fill *fill;
#endif	/* UsePrototypes */
{
    int  num;
    Tallies *s_tallies = s_score->tallies;
    
    /* skip this altogether if fill doesn't exist */
    if (fill == (Slot_fill *) NULL)
	return;

    /* the type of fill doesn't matter, since either num_ptrs or num_strs
       will be 0, unless it is a multi_fill, in which case we still need
       to combine them for the total. */

    num = fill->num_strs + fill->num_ptrs;
    
    tally_actual(s_tallies, num);
    tally_spurious(s_tallies, num);

    return;
}


/************************************************************************
 *									*
 *  score_connected_pair						*
 *									*
 *  for every slot in this class type, call the function		*
 *  score_connected_slot to score the slot fills in the key against	*
 *  the slot fills in the response.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
score_connected_pair(TABLE *slot_defs,
		     Class_score *c_score,
		     Class_instance *kinst,
		     Class_instance *rinst)
#else
static
void
score_connected_pair(slot_defs, c_score, kinst, rinst)
TABLE *slot_defs;
Class_score *c_score;
Class_instance *kinst;
Class_instance *rinst;
#endif	/* UsePrototypes */
{
    Slot_def *slot;
    Slot_score *s_score;
    Slot_fill *kfill, *rfill;
    char *sname;

    /* iterate over the list of slots */
    table_first(slot_defs);
    for(slot = (Slot_def *)table_next(slot_defs);
	slot;
	slot = (Slot_def *)table_next(slot_defs))
    {
	if (slot->is_scored_slot)
	{
	    sname = slot->name;
	    s_score = find_slot_score(c_score->slot_scores, sname);
	    kfill = find_slot_fill(kinst->slot_fills, sname);
	    rfill = find_slot_fill(rinst->slot_fills, sname);
	    score_connected_slot(slot, s_score, kfill, rfill);
	}
    }

    return;
}


/************************************************************************
 *									*
 *  score_connected_slot						*
 *									*
 *  do not compare the slot_fills in the key to those in the response.	*
 *  simply count the (minimum) number of fills in the key and the	*
 *  number of fills in the response.  the amount of overlap indicates	*
 *  the number of 'incorrect', and the rest are either 'missing' or	*
 *  'spurious', depending on whether there are more key fills or	*
 *  more response fills.						*
 *									*
 *  (key2key scoring not yet supported)					*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
score_connected_slot(Slot_def *slot, Slot_score *s_score,
		     Slot_fill *kfill, Slot_fill *rfill)
#else
static
void
score_connected_slot(slot, s_score, kfill, rfill)
Slot_def *slot;
Slot_score *s_score;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* Use Prototypes */
{
    int  knum, rnum, kmin, min_alt_set;
    Tallies *s_tallies = s_score->tallies;
    
    /* kfill is either empty or optional, and rfill is empty */
    if (is_noncommittal(kfill, rfill))
	score_noncommittal(s_tallies, kfill, rfill);

    /* kfill but no rfill */
    else if (is_missing(kfill, rfill))
	score_missing(s_tallies, kfill, rfill);

    /* rfill but no kfill */
    else if (is_spurious(kfill, rfill))
	score_spurious(s_tallies, kfill, rfill);

    /* kfill and rfill */
    else
    {
	/* the type of fill doesn't matter, since either num_ptrs or num_strs
	   will be 0, unless it is a multi_fill, in which case we still need
	   to combine them for the total. */

	knum = kmin = kfill->num_strs + kfill->num_ptrs;
	rnum = rfill->num_strs + rfill->num_ptrs;

	/* check for alternate fills in kfill */
	if (kfill->alt_fill_sets != (Alt_fill_set *) NULL)
	{
	    /* a situation has arisen where the number of fillers in a
	       slot_fill or in an alt_fill_set may be 0, a result of
	       optional instances being removed.  in this event, we need
	       to find a kmin > 0.
	       note: if the slot fill is_optional, then there's no worry,
	       since it can only occur when there are alternate fills. */
	
	    min_alt_set = count_min_alt_fills(kfill->alt_fill_sets);

	    /* knum = 0 only if all recursive knums are 0 */
	    if ( (knum = muc_min(kmin, min_alt_set)) == 0 )
		knum = muc_max(kmin, min_alt_set);
	}
    
	tally_possible(s_tallies, knum);
	tally_actual(s_tallies, rnum);
	tally_incorrect(s_tallies, muc_min(knum, rnum));
	if (knum > rnum)
	    tally_missing(s_tallies, (knum - rnum));
	else
	    tally_spurious(s_tallies, (rnum - knum));
    }

    return;
}


/************************************************************************
 *									*
 *  score_instance_pair							*
 *									*
 *  for every slot in this class type, call the function score_slot to	*
 *  score the slot fills in the key against the slot fills in the	*
 *  response.								*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
score_instance_pair(TABLE *slot_defs,
		    Class_score *c_score,
		    Class_instance *kinst,
		    Class_instance *rinst)
#else
void
score_instance_pair(slot_defs, c_score, kinst, rinst)
TABLE *slot_defs;
Class_score *c_score;
Class_instance *kinst;
Class_instance *rinst;
#endif	/* UsePrototypes */
{
    Slot_def *slot;
    Slot_score *s_score;
    Slot_fill *kfill, *rfill;
    char *sname;

    /* iterate over the list of slots */
    table_first(slot_defs);
    for(slot = (Slot_def *)table_next(slot_defs);
	slot;
	slot = (Slot_def *)table_next(slot_defs))
    {
	if (slot->is_scored_slot)
	{
	    sname = slot->name;
	    s_score = find_slot_score(c_score->slot_scores, sname);

	    kfill = find_slot_fill(kinst->slot_fills, sname);
	    rfill = find_slot_fill(rinst->slot_fills, sname);

	    /* subtract the current s_score->tallies from the appropriate
	       NE tallies, then add the new s_score->tallies after scoring.
	       the effect is to add the tallies for this pass only */
	    if (MUC_scoring_task == NAMED_ENTITY)
	    {
		subtract_from_ne_subcategorization_tallies(kfill,
							   s_score->tallies);
		subtract_from_ne_doc_section_tallies(s_score->tallies);
	    }

	    score_slot(slot, s_score, kfill, rfill);
	    
	    /* add the new s_score->tallies to NE scores */
	    if (MUC_scoring_task == NAMED_ENTITY)
	    {
		add_to_ne_subcategorization_tallies(kfill, s_score->tallies);
		add_to_ne_doc_section_tallies(s_score->tallies);
	    }
	}
    }

    return;
}


/************************************************************************
 *									*
 *  score_slot								*
 *									*
 *  compare the slot_fills in the key to those in the response.		*
 *  based on the fill-type, call the appropriate slot comparison	*
 *  (or scoring) function.   if there are alternate sets, then call	*
 *  the function with every combination of fills between the key and	*
 *  response and determine the best combination.			*
 *									*
 *  (key2key scoring not yet supported)					*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
score_slot(Slot_def *slot, Slot_score *s_score,
	   Slot_fill *kfill, Slot_fill *rfill)
#else
void
score_slot(slot, s_score, kfill, rfill)
Slot_def *slot;
Slot_score *s_score;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* Use Prototypes */
{
    Tallies *tallies = s_score->tallies;
    
    /* kfill is either empty or optional, and rfill is empty */
    if (is_noncommittal(kfill, rfill))
	score_noncommittal(tallies, kfill, rfill);

    /* kfill but no rfill */
    else if (is_missing(kfill, rfill))
	score_missing(tallies, kfill, rfill);

    /* rfill but no kfill */
    else if (is_spurious(kfill, rfill))
	score_spurious(tallies, kfill, rfill);

    /* kfill with alternates, and rfill */
    else if (is_with_alternates(kfill, rfill))
	score_alternate_fills(slot, tallies, kfill, rfill);

    /* kfill and rfill */
    else
	score_fills(slot, tallies, kfill, rfill);

    return;
}


#if UsePrototypes
static
void
score_noncommittal(Tallies *tallies, Slot_fill *kfill, Slot_fill *rfill)
#else
static
void
score_noncommittal(tallies, kfill, rfill)
Tallies *tallies;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    tally1_noncommittal(tallies);
    
    return;
}


#if UsePrototypes
static
void
score_missing(Tallies *tallies, Slot_fill *kfill, Slot_fill *rfill)
#else
static
void
score_missing(tallies, kfill, rfill)
Tallies *tallies;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    int num, min, min_alt_set;
    
    /* the type of fill doesn't matter, since either num_ptrs or num_strs
       will be 0, unless it is a multi_fill, in which case we still need
       to combine them for the total. */

    num = min = kfill->num_strs + kfill->num_ptrs;

    /* check for alternate fills */
    if (kfill->alt_fill_sets != (Alt_fill_set *) NULL)
    {
	/* a situation has arisen where the number of fillers in a slot_fill
	   or in an alt_fill_set may be 0, a result of optional instances
	   being removed.  in this event, we need to find a min > 0.
	   note: if the slot fill is_optional, then it's not a problem.
	   it can only occur when there are alternate fills. */
	
	min_alt_set = count_min_alt_fills(kfill->alt_fill_sets);

	/* num = 0 only if all recursive nums are 0 */
	if ( (num = muc_min(min, min_alt_set)) == 0 )
	    num = muc_max(min, min_alt_set);
    }
    
    tally_possible(tallies, num);
    tally_missing(tallies, num);

    return;
}


#if UsePrototypes
static
void
score_spurious(Tallies *tallies, Slot_fill *kfill, Slot_fill *rfill)
#else
static
void
score_spurious(tallies, kfill, rfill)
Tallies *tallies;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    int num;
    
    /* the type of fill doesn't matter, since either num_ptrs or num_strs
       will be 0, unless it is a multi_fill, in which case we still need
       to combine them for the total. */

    num = rfill->num_strs + rfill->num_ptrs;
    
    tally_actual(tallies, num);
    tally_spurious(tallies, num);

    return;
}


#if UsePrototypes
void
score_fills(Slot_def *slot, Tallies *tallies,
	    Slot_fill *kfill, Slot_fill *rfill)
#else
void
score_fills(slot, tallies, kfill, rfill)
Slot_def *slot;
Tallies *tallies;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    String_fill **kstrs = kfill->strfills;
    int num_kstrs = kfill->num_strs;
    Pointer *kptrs = kfill->instptrs;
    int num_kptrs = kfill->num_ptrs;

    String_fill **rstrs = rfill->strfills;
    int num_rstrs = rfill->num_strs;
    Pointer *rptrs = rfill->instptrs;
    int num_rptrs = rfill->num_ptrs;

    String_fill *minstr = kfill->min_string;
    int fill_type = determine_fill_type(slot);

    switch (fill_type)
    {
    case MULTI_FILL:
	/* some assumptions are made with a multi_typed fill:
	   at worst, it is a combination of a pointer_fill and a string_fill.
	   otherwise, there's way too much work to figure out how to
	   score various fill_types in combination. */
	score_string_fills(slot, tallies,
			   kstrs, num_kstrs,
			   rstrs, num_rstrs,
			   minstr);
	score_pointer_fills(slot, tallies,
			    kptrs, num_kptrs,
			    rptrs, num_rptrs);
	break;
    case POINTER_FILL:
	score_pointer_fills(slot, tallies,
			    kptrs, num_kptrs,
			    rptrs, num_rptrs);
	break;
    case SET_FILL:
	score_set_fills(slot, tallies,
			kstrs, num_kstrs,
			rstrs, num_rstrs);
	break;
    case LOCATION_FILL:
    case REGULARIZED_FILL:
    case STRING_FILL:
	score_string_fills(slot, tallies,
			   kstrs, num_kstrs,
			   rstrs, num_rstrs,
			   minstr);
	break;
    default:
	score_string_fills(slot, tallies,
			   kstrs, num_kstrs,
			   rstrs, num_rstrs,
			   minstr);
    }

    return;
}


#if UsePrototypes
static
void
score_alternate_fills(Slot_def *slot, Tallies *tallies,
		      Slot_fill *kfill, Slot_fill *rfill)
#else
static
void
score_alternate_fills(slot, tallies, kfill, rfill)
Slot_def *slot;
Tallies *tallies;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    Tallies *temp = tallies_create();
    Tallies *best = tallies_create();
    Alt_fill_set *alt_set;
    String_fill *minstr = kfill->min_string;
    float temp_f, best_f;
    int num_r, num_best, num_temp;
    
    /* score the key slot_fill first, before iterating over
       the alternate sets */
    score_fills(slot, best, kfill, rfill);
    
    best_f = compute_tallies_f(best, F_PR);
    num_best = best->possible;

    /* num_r equals the number of rfill strings or pointers */
    num_r = best->actual;
    
    if (best_f == 100.0)
	tallies_combine(tallies, best);
    else
    {
	/* see if there's a better match from the alternate sets */

	for (alt_set = kfill->alt_fill_sets;
	     alt_set;
	     alt_set = alt_set->next)
	{
	    tallies_clear(temp);
	    score_alternate_set(slot, temp, alt_set, rfill, minstr);
	    temp_f = compute_tallies_f(temp, F_PR);
	    num_temp = temp->possible;
	    
	    /* either temp_f > best_f,
	       or temp_f = best_f, but temp has less fillers
	       than best and at least as many as rfill */
	    if ( (temp_f > best_f)
		|| ( (temp_f == best_f)
		    && (num_temp < num_best)
		    && (num_r <= num_temp) ) )
	    {
		best_f = temp_f;
		tallies_copy(best, temp);
		num_best = num_temp;

		if (best_f == 100.0)
		    break;	/* stop iteration */
	    }
	}
	tallies_combine(tallies, best);
    }
    
    tallies_release((char *) temp);
    tallies_release((char *) best);
    
    return;
}


#if UsePrototypes
void
score_alternate_set(Slot_def *slot, Tallies *tallies,
		    Alt_fill_set *kset, Slot_fill *rfill,
		    String_fill *minstr)
#else
void
score_alternate_set(slot, tallies, kset, rfill, minstr)
Slot_def *slot;
Tallies *tallies;
Alt_fill_set *kset;
Slot_fill *rfill;
String_fill *minstr;
#endif	/* UsePrototypes */
{
    String_fill **kstrs = kset->strfills;
    int num_kstrs = kset->num_strs;
    Pointer *kptrs = kset->instptrs;
    int num_kptrs = kset->num_ptrs;

    String_fill **rstrs = rfill->strfills;
    int num_rstrs = rfill->num_strs;
    Pointer *rptrs = rfill->instptrs;
    int num_rptrs = rfill->num_ptrs;

    int fill_type = determine_fill_type(slot);

    switch (fill_type)
    {
    case MULTI_FILL:
	/* some assumptions are made with a multi_typed fill:
	   at worst, it is a combination of a pointer_fill and a string_fill.
	   otherwise, there's way too much work to figure out how to
	   score various fill_types in combination. */
	score_string_fills(slot, tallies,
			   kstrs, num_kstrs,
			   rstrs, num_rstrs,
			   minstr);
	score_pointer_fills(slot, tallies,
			    kptrs, num_kptrs,
			    rptrs, num_rptrs);
	break;
    case POINTER_FILL:
	score_pointer_fills(slot, tallies,
			    kptrs, num_kptrs,
			    rptrs, num_rptrs);
	break;
    case SET_FILL:
	score_set_fills(slot, tallies,
			kstrs, num_kstrs,
			rstrs, num_rstrs);
	break;
    case LOCATION_FILL:
    case REGULARIZED_FILL:
    case STRING_FILL:
	score_string_fills(slot, tallies,
			   kstrs, num_kstrs,
			   rstrs, num_rstrs,
			   minstr);
	break;
    default:
	score_string_fills(slot, tallies,
			   kstrs, num_kstrs,
			   rstrs, num_rstrs,
			   minstr);
    }

    return;
}


#if UsePrototypes
static
void
score_pointer_fills(Slot_def *slot, Tallies *tallies,
		    Pointer *kptrs, int numk,
		    Pointer *rptrs, int numr)
#else
static
void
score_pointer_fills(slot, tallies, kptrs, numk, rptrs, numr)
Slot_def *slot;
Tallies *tallies;
Pointer *kptrs;
int numk;
Pointer *rptrs;
int numr;
#endif	/* UsePrototypes */
{
    Pointer *kptr, *rptr, *ident_ptr;
    Class_instance *kinst, *rinst, *mapped_to;
    Class_instance *ident_inst, *ident_mapped_to;
    int pos, act, cor, inc, mis, spu;
    Bool match_found = False;

    pos = act = cor = inc = mis = spu = 0;
    
    pos = numk;
    act = numr;

    /* traverse the key pointers and compare them against the
       response pointers.  if a key and a response are mapped
       to each other, then that is a 'correct' score. */

    for (kptr = kptrs;
	 kptr;
	 kptr = kptr->next)
    {
	kinst = kptr->inst;

        if (kinst && STRCASEEQ(kinst->name, "in_and_out"))
	{
	    for (rptr = rptrs;
		 rptr;
		 rptr = rptr->next)
	    {
		rinst = rptr->inst;

		if ((mapped_to = kinst->mapped_to)
		    && ((Class_instance *) mapped_to
			== (Class_instance *) rinst))
		{
		    cor++;
		    match_found = True;
		}
		else
		{
		    for (ident_ptr = kinst->ident_in_and_out_ptrs;
			 ident_ptr;
			 ident_ptr = ident_ptr->next)
		    {
			if ((ident_inst = ident_ptr->inst)
			    && (ident_mapped_to = ident_inst->mapped_to)
			    && ((Class_instance *) ident_mapped_to
				== (Class_instance *) rinst))
			{
			    cor++;
			    match_found = True;
			    break;
			}
		    }
		}
		if (match_found)
		{
		    match_found = False;
		    break;
		}
	    }
	}
	/* not an in_and_out pointer */
	else if (kinst && (mapped_to = kinst->mapped_to))
	{
	    for (rptr = rptrs;
		 rptr;
		 rptr = rptr->next)
	    {
		rinst = rptr->inst;
		if ((Class_instance *) mapped_to
		    == (Class_instance *) rinst)
		{
		    cor++;
		    break;
		}
	    }
	}
    }

    /* the max number incorrect is equal to the lesser of possible
       and actual.  the total number incorrect is reduced by the
       number of correct.  the difference between possible and actual
       is counted either as missing or spurious.  */
    inc = muc_min(pos, act) - cor;
    mis = pos - (cor + inc);
    spu = act - (cor + inc);
    
    tally_possible(tallies, pos);
    tally_actual(tallies, act);
    tally_correct(tallies, cor);
    tally_incorrect(tallies, inc);
    tally_missing(tallies, mis);
    tally_spurious(tallies, spu);
    
    return;
}


#if UsePrototypes
static
void
score_set_fills(Slot_def *slot, Tallies *tallies,
		String_fill **keys, int numk,
		String_fill **responses, int numr)
#else
static
void
score_set_fills(slot, tallies, keys, numk, responses, numr)
Slot_def *slot;
Tallies *tallies;
String_fill **keys;
int numk;
String_fill **responses;
int numr;
#endif	/* UsePrototypes */
{
    register int j;
    register int i;
    int pos, act, cor, par, inc, mis, spu;
    
    pos = act = cor = par = inc = mis = spu = 0;

    pos = numk;
    act = numr;

    /* reset the match_status for all keys and responses */
    for(i=0; i<pos; i++)
	keys[i]->match_status = MISMATCH;
    for(j=0; j<act; j++)
	responses[j]->match_status = MISMATCH;

    /* compare only unmatched keys and responses */
    for(i=0; i<pos && (keys[i]->match_status != MATCH); i++)
    {
	for(j=0; j<act; j++)
	{
	    if( (responses[j]->match_status != MATCH)
                /* && STRCASEEQ(keys[i]->fill, responses[j]->fill) )*/
                   &&((muc_cmp_special(keys[i]->fill,responses[j]->fill))==0))
	    {
		cor++;
		keys[i]->match_status = MATCH;
		responses[j]->match_status = MATCH;
		break;	/* break out of j loop */
	    }
	}
    }
    
    /* the max number incorrect is equal to the lesser of possible
       and actual.  the total number incorrect is reduced by the number
       of correct and partial.  the difference between possible and actual
       is counted either as missing or spurious.  */
    inc = muc_min(pos, act) - (cor + par);
    mis = pos - (cor + par + inc);
    spu = act - (cor + par + inc);
    
    tally_possible(tallies, pos);
    tally_actual(tallies, act);
    tally_correct(tallies, cor);
    tally_partial(tallies, par);
    tally_incorrect(tallies, inc);
    tally_missing(tallies, mis);
    tally_spurious(tallies, spu);
    
    return;
}
    

#if UsePrototypes
static
void
score_string_fills(Slot_def *slot, Tallies *tallies,
		   String_fill **keys, int numk,
		   String_fill **responses, int numr,
		   String_fill *minstr)
#else
static
void
score_string_fills(slot, tallies, keys, numk, responses, numr, minstr)
Slot_def *slot;
Tallies *tallies;
String_fill **keys;
int numk;
String_fill **responses;
int numr;
String_fill *minstr;
#endif	/* UsePrototypes */
{
    register int j;
    register int i;
    int pos, act, cor, par, inc, mis, spu;
    char *minstrfill = (char *) NULL;
    
    pos = act = cor = par = inc = mis = spu = 0;

    pos = numk;
    act = numr;

    /* if minstr is NULL, then minstrfill remains a (char *) NULL */
    if (minstr != (String_fill *) NULL)
	minstrfill = minstr->fill;
    
    /* reset the match_status for all keys and responses */
    for(i=0; i<pos; i++)
	keys[i]->match_status = MISMATCH;
    for(j=0; j<act; j++)
	responses[j]->match_status = MISMATCH;

    /* compare only unmatched (or partial?) keys and responses */
    for(i=0; i<pos && (keys[i]->match_status != MATCH); i++)
    {
	for(j=0; j<act; j++)
	{
           /* printf("keys[i]->fill :: %s  responses[j]->fill :: %s \n",keys[i]->fill, responses[j]->fill);*/
	    if( (responses[j]->match_status != MATCH)
	       && muc_streq(keys[i]->fill, responses[j]->fill, minstrfill) )
	    {
               /*printf("Match\n");*/
		cor++;
		keys[i]->match_status = MATCH;
		responses[j]->match_status = MATCH;
		break;	/* break out of j loop */
	    }
	}
    }
    
    /* the max number incorrect is equal to the lesser of possible
       and actual.  the total number incorrect is reduced by the number
       of correct and partial.  the difference between possible and actual
       is counted either as missing or spurious.  */
    inc = muc_min(pos, act) - (cor + par);
    mis = pos - (cor + par + inc);
    spu = act - (cor + par + inc);
    
    tally_possible(tallies, pos);
    tally_actual(tallies, act);
    tally_correct(tallies, cor);
    tally_partial(tallies, par);
    tally_incorrect(tallies, inc);
    tally_missing(tallies, mis);
    tally_spurious(tallies, spu);
    
    return;
}


/************************************************************************
 *									*
 *  scoring utilities							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
int
count_min_alt_fills(Alt_fill_set *set)
#else
static
int
count_min_alt_fills(set)
Alt_fill_set *set;
#endif	/* UsePrototypes */
{
    Alt_fill_set *next;
    int this_num, this_min, min_from_rest;

    /* is this a NULL set? */
    if (set == (Alt_fill_set *) NULL)
	return(0);
    else
    {
	/* what is the number of fills for this set? */
	this_num = this_min = set->num_strs + set->num_ptrs;

	/* recursive call returns the minimum of the rest of the list */
	if ((next = set->next) != (Alt_fill_set *) NULL)
	{
	    /* a situation has arisen where the number of fillers in a slot_fill
	       or in an alt_fill_set may be 0, a result of optional instances
	       being removed.  in this event, we need to find a min > 0. */
	
	    min_from_rest = count_min_alt_fills(next);

	    if ( (this_num = muc_min(this_min, min_from_rest)) == 0)
		this_num = muc_max(this_min, min_from_rest);
	}
    }
    
    return(this_num);
}


#if UsePrototypes
static
int
determine_fill_type(Slot_def *slot)
#else
static
int
determine_fill_type(slot)
Slot_def *slot;
#endif	/* UsePrototypes */
{
    if (is_a_multi_fill_type(slot))
	return(MULTI_FILL);
    else if (is_a_string_fill_type(slot))
	return(STRING_FILL);
    else if (is_a_pointer_fill_type(slot))
	return(POINTER_FILL);
    else if (is_a_set_fill_type(slot))
	return(SET_FILL);
    else if (is_a_location_fill_type(slot))
	return(LOCATION_FILL);
    else
	return(REGULARIZED_FILL);
}


#if UsePrototypes
static
Bool
is_a_multi_fill_type(Slot_def *slot)
#else
static
Bool
is_a_multi_fill_type(slot)
Slot_def *slot;
#endif	/* UsePrototypes */
{
    return(slot->has_multi_fill_types);
}

#if UsePrototypes
static
Bool
is_a_string_fill_type(Slot_def *slot)
#else
static
Bool
is_a_string_fill_type(slot)
Slot_def *slot;
#endif	/* UsePrototypes */
{
    char *fill_type = slot->fill_types[0];
    
    if(STRCASEEQ(fill_type, "string"))
	return(True);
    else
	return(False);
}

#if UsePrototypes
Bool
is_a_pointer_fill_type(Slot_def *slot)
#else
Bool
is_a_pointer_fill_type(slot)
Slot_def *slot;
#endif	/* UsePrototypes */
{
    char *fill_type = slot->fill_types[0];
    
    /* is fill_type the name of one of the defined classes? */
    if(find_class_def_by_name(fill_type) == (Class_def *) NULL)
	return(False);
    else
	return(True);
}

#if UsePrototypes
static
Bool
is_a_set_fill_type(Slot_def *slot)
#else
static
Bool
is_a_set_fill_type(slot)
Slot_def *slot;
#endif	/* UsePrototypes */
{
    char *fill_type = slot->fill_types[0];
    
    if(STRCASEEQ(fill_type, "set"))
	return(True);
    else
	return(False);
}

#if UsePrototypes
static
Bool
is_a_location_fill_type(Slot_def *slot)
#else
static
Bool
is_a_location_fill_type(slot)
Slot_def *slot;
#endif	/* UsePrototypes */
{
    char *fill_type = slot->fill_types[0];
    
    if(STRCASEEQ(fill_type, "location")
       || STRCASEEQ(fill_type, "location_country_only"))
	return(True);
    else
	return(False);
}


/************************************************************************
 *									*
 *  comparisons of slot_fill contents					*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
Bool
is_noncommittal(Slot_fill *kfill, Slot_fill *rfill)
#else
static
Bool
is_noncommittal(kfill, rfill)
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    if (((kfill == (Slot_fill *) NULL)
	 || kfill->is_optional)
	&& (rfill == (Slot_fill *) NULL))
	return(True);
    else
	return(False);
}


#if UsePrototypes
Bool
is_optional(Slot_fill *kfill, Slot_fill *rfill)
#else
Bool
is_optional(kfill, rfill)
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    if (kfill
	&& kfill->is_optional
	&& (rfill == (Slot_fill *) NULL))
	return(True);
    else
	return(False);
}


#if UsePrototypes
static
Bool
is_missing(Slot_fill *kfill, Slot_fill *rfill)
#else
static
Bool
is_missing(kfill, rfill)
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    if (kfill
	&& !kfill->is_optional
	&& (rfill == (Slot_fill *) NULL))
	return(True);
    else
	return(False);
}


#if UsePrototypes
static
Bool
is_spurious(Slot_fill *kfill, Slot_fill *rfill)
#else
static
Bool
is_spurious(kfill, rfill)
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    if ((kfill == (Slot_fill *) NULL)
	&& rfill)
	return(True);
    else
	return(False);
}


#if UsePrototypes
static
Bool
is_with_alternates(Slot_fill *kfill, Slot_fill *rfill)
#else
static
Bool
is_with_alternates(kfill, rfill)
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    if (kfill
	&& (kfill->alt_fill_sets != (Alt_fill_set *) NULL)
	&& rfill)
	return(True);
    else
	return(False);
}
