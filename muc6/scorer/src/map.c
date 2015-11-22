/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	map.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	mapping functions for MUC scorer.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
	Changed  Class_mappings to Inst_pairings 
        Changed  Mapping_pair   to Inst_pair      -ad 12/06/96
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)map.c	1.6 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include <stdio.h>
#include "table.h"
#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"


/*  internal function declarations */

Proto	(static void, map_class_instances, (Class_def *class,
					    Inst_pairings *mappings,
					    Class_instance_list *klist,
					    Class_instance_list *rlist));
Proto	(static void, update_mappings, (Inst_pairings *mappings));
Proto	(static void, map_instance_pair, (Class_def *class,
					  Inst_pair *pair));
Proto	(static float, map_slot, (Slot_def *slot,
				  Class_instance *kinst,
				  Class_instance *rinst));
Proto	(static	int, check_offset_overlap, (Class_instance *kinst,
					    Class_instance *rinst));

    
/************************************************************************
 *									*
 *  map_MUC_catalog()							*
 *									*
 *  map all documents and templates.					*
 *  calls map_catalog_entry.						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
map_MUC_catalog(void)
#else
void
map_MUC_catalog()
#endif	/* UsePrototypes */
{
    TABLE  *Catalog = (TABLE *)MUC_Catalog;
    Catalog_entry *entry;

    printf("Begin Mapping...\n");
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
    {
	printf("mapping %s ...", entry->docnum);
	map_catalog_entry(entry);
	printf("done.\n");
    }

    printf("Mapping Completed.\n");
    return;
}


/************************************************************************
 *									*
 *  map_catalog_entry							*
 *									*
 *  if catalog_entry points to both a key and a response root, then	*
 *  each root will have a table of classes, with each class pointing	*
 *  to a table of class instances.  for each class, call the function	*
 *  map_class_instances to compare the instances between the key and	*
 *  response.								*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
map_catalog_entry(Catalog_entry *entry)
#else
void
map_catalog_entry(entry)
Catalog_entry *entry;
#endif	/* UsePrototypes */
{
    TABLE  *class_map_list = (TABLE *) entry->class_mappings_list;
    Root  *k_root, *r_root;
    TABLE  *k_class_inst_lists, *r_class_inst_lists;
    Class_instance_list  *k_class_inst_list, *r_class_inst_list;
    Inst_pairings  *thismappings;
    Class_def  *thisclass;
    char **map_order = Map_order->value_list;
    int  num_order = Map_order->num_vals;
    int i;
    char *classname;

    /* set MUC_mode to 'MAP', so that certain shared functions can be
       disabled if necessary (e.g., add_to_ne_subcategorization_tallies */
    MUC_mode = MAP_MODE;
    
    /* must have both k_root and r_root, otherwise there's nothing
       to map if either is null. */
    if(((k_root = entry->k_root) == (Root *) NULL)
       || ((r_root = entry->r_root) == (Root *) NULL))
	return;
    
    k_class_inst_lists = (TABLE *) k_root->class_instance_lists;
    r_class_inst_lists = (TABLE *) r_root->class_instance_lists;

    /* map the classes for which there are key and response instances */

    for(i=0; i<num_order; i++)
    {
	classname = map_order[i];
	thisclass = find_class_def_by_name(classname);
	thismappings = find_inst_pairings(class_map_list, classname);
	k_class_inst_list = find_class_instance_list(k_class_inst_lists,
						     classname);
	r_class_inst_list = find_class_instance_list(r_class_inst_lists,
						     classname);
	/* proceed with map only if there are both key and
	   response instances. */
	if((k_class_inst_list != (Class_instance_list *) NULL)
	   && (r_class_inst_list != (Class_instance_list *) NULL))
	{
	    map_class_instances(thisclass,
				thismappings,
				k_class_inst_list,
				r_class_inst_list);
	    /* special case with in_and_out instances. */
	   if STRCASEEQ(classname, "in_and_out")
             /* if (!(muc_cmp_special(classname,"in_and_out")) ||
                  !(muc_cmp_special(classname,"IN_AND_OUT")))*/
		find_identical_in_and_out_instances(k_class_inst_list);
	}
    }

    /* reset MUC_mode */
    MUC_mode = NO_MODE;
    
    return;
}
    

/************************************************************************
 *									*
 *  map_class_instances							*
 *									*
 *  create a inst_pair structure for every pair of key and		*
 *  response instances.  call the function map_instance to compare	*
 *  each pair.  qsort all the inst_pairs, then set the final		*
 *  mappings by setting the mapped_to pointers for each member of a	*
 *  mapped pair, and ignoring subsequent mappings that contain an	*
 *  already mapped member.						*
 *
 ***********************************************************************/

#if UsePrototypes
static
void
map_class_instances(Class_def *class, Inst_pairings *mappings,
		    Class_instance_list *klist, Class_instance_list *rlist)
#else
static
void
map_class_instances(class, mappings, klist, rlist)
Class_def *class;
Inst_pairings *mappings;
Class_instance_list *klist;
Class_instance_list *rlist;
#endif	/* UsePrototypes */
{
    TABLE  *k_instances = (TABLE *) klist->instances;
    TABLE  *r_instances = (TABLE *) rlist->instances;
    Class_instance *kinst;
    Class_instance *rinst;
    Inst_pair *thispair;
    
    table_first(k_instances);
    for(kinst = (Class_instance *)table_next(k_instances);
	kinst;
	kinst = (Class_instance *)table_next(k_instances))
    {
	table_first(r_instances);
	for(rinst = (Class_instance *)table_next(r_instances);
	    rinst;
	    rinst = (Class_instance *)table_next(r_instances))
	{
	    thispair = inst_pair_create();
	    thispair->key = kinst;
	    thispair->response = rinst;
	    
	    /* template instances will be automatically mapped
	       if both key and response exist, so don't bother
	       with this step. */
	    if (!STRCASEEQ(class->name, "template"))
	      map_instance_pair(class, thispair);    /* bang 'em together    */
	                                             /* and see what happens */

	    add_inst_pair_to_all(mappings, thispair);
	}
    }

    /* qsort all the pairs */
    qsort((char *)mappings->all,
	  mappings->num_all,
	  sizeof (Inst_pair *),
	  inst_pair_compare);
    
    update_mappings(mappings);

    return;
}


/************************************************************************
 *									*
 *  update_mappings							*
 *									*
 *  select the final mappings from the list of all mappings.		*
 *  different criteria apply, based on scoring_task:			*
 *									*
 *  ST or TE:								*
 *      1. both kinst and rinst are not yet mapped.			*
 *    and								*
 *        2. the class type is either 'template'			*
 *           (i.e., templates are automatically mapped),		*
 *      or								*
 *        3. if class type is not 'template',				*
 *	   the f_score for the pair > 0.0				*
 *									*
 *  NE:									*
 *      1. both kinst and rinst are not yet mapped.			*
 *    and								*
 *      2. the f_score for the pair >= 0.0				*
 *         (this is because mapping is essentially based on overlap	*
 *          of text offsets, the assumption being that they attempted	*
 *          to annotate the same piece of text, even if the		*
 *          annotations are of wrong subcategory (i.e., TYPE).		*
 *          non-overlapping pairs are given a score of -1.0.)		*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
update_mappings(Inst_pairings *mappings)
#else
static
void
update_mappings(mappings)
Inst_pairings *mappings;
#endif	/* UsePrototypes */
{
    register int i;
    Inst_pair *thispair;
    Class_instance *kinst;
    Class_instance *rinst;
    int num_all = mappings->num_all;

    for (i=0; i<num_all; i++)
    {
	thispair = mappings->all[i];
	kinst = thispair->key;
	rinst = thispair->response;

	if ( (((MUC_scoring_task == SCENARIO_TEMPLATE)
	       || (MUC_scoring_task == TEMPLATE_ELEMENT))
	      && ( !kinst->mapped_to && !rinst->mapped_to )
	      && (STRCASEEQ(mappings->name, "template")

		  || (thispair->score > 0.0) ))

	    || ( (MUC_scoring_task == NAMED_ENTITY)
		&& ( !kinst->mapped_to && !rinst->mapped_to )
		&& (thispair->score >= 0.0) ))
	{
	    kinst->mapped_to = rinst;
	    rinst->mapped_to = kinst;
	    add_inst_pair_to_final(mappings, thispair);
	}
	else
	    ;
    }

    return;
}


/************************************************************************
 *									*
 *  map_instance_pair							*
 *									*
 *  for every slot in this class type, call the function map_slot to	*
 *  compare the slot fills in the key to the slot fills in the		*
 *  response.  compute an f_score for the comparison, then multiply	*
 *  the f_score with the slot map weight.  return the total score for	*
 *  all the slot comparisons.						*
 *									*
 *  for every comparison that is >= to the mapping threshold, score	*
 *  the pair, then compute an f_score.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
map_instance_pair(Class_def *class, Inst_pair *pair)
#else
static
void
map_instance_pair(class, pair)
Class_def *class;
Inst_pair *pair;
#endif	/* UsePrototypes */
{
    TABLE *slotlist = (TABLE *) class->slot_defs;
    Slot_def *slot;
    Class_score *c_score = class_score_create(class, pair->key->docnum);
    Tallies *slot_tallies = tallies_create();
    float threshold = (float) class->map_threshold;
    float total_slot_score = 0;

    /* skip this if we're mapping NE and the text offsets don't overlap.
       set the f_score to -1.0, so we don't add it to final mappings. */
    if( (MUC_scoring_task == NAMED_ENTITY)
       && !check_offset_overlap(pair->key, pair->response) )
	pair->score = -1.0;
    else
    {
	table_first(slotlist);
	for(slot = (Slot_def *)table_next(slotlist);
	    slot;
	    slot = (Slot_def *)table_next(slotlist))
	    total_slot_score += map_slot(slot, pair->key, pair->response);
    
	/* does the total slot score exceed threshold?
	   or if this is NE, go ahead and score it anyway, regardless
	   of slot score, since we assume that if they overlap, they
	   were probably intended to be the same */
	if(((pair->threshold_score = total_slot_score) >= threshold)
	   || (MUC_scoring_task == NAMED_ENTITY))
	{
	    score_instance_pair(slotlist, c_score,
				pair->key, pair->response);
	    tallies_clear(slot_tallies);
	    combine_slot_tallies(c_score, slot_tallies);
	    pair->score = compute_tallies_f(slot_tallies, F_PR);
	}
    }
    
    tallies_release((char *) slot_tallies);
    class_score_release((char *) c_score);
    
    return;
}


/************************************************************************
 *									*
 *  map_slot								*
 *									*
 *  compare the slot_fills in the key to those in the response.		*
 *  based on the fill-type, call the appropriate slot comparison	*
 *  (or scoring) function.   if there are alternate sets, then call	*
 *  the function with every combination of fills between the key and	*
 *  response and determine the best combination.			*
 *
 *  This function doesn't "map," it only returns the weighted slot
 *  score for the two instances
 *									*
 ***********************************************************************/

#if UsePrototypes
static
float
map_slot(Slot_def *slot, Class_instance *kinst, Class_instance *rinst)
#else
static
float
map_slot(slot, kinst, rinst)
Slot_def *slot;
Class_instance *kinst;
Class_instance *rinst;
#endif	/* Use Prototypes */
{
    Slot_fill *kfill, *rfill;
    Slot_score *s_score;
    float this_score = (float) 0;
    float map_weight = (float) slot->map_weight;
    
    kfill = find_slot_fill(kinst->slot_fills, slot->name);
    rfill = find_slot_fill(rinst->slot_fills, slot->name);

    /* return 0 if either key or response is empty */
    if ((kfill == (Slot_fill *) NULL)
	|| (rfill == (Slot_fill *) NULL))
	return(this_score);
    else
    {
	s_score = slot_score_create(slot->name, kinst->docnum);
	score_slot(slot, s_score, kfill, rfill);
	this_score = map_weight * compute_tallies_f(s_score->tallies, F_PR);
	slot_score_release((char *) s_score);
    }
    
    return(this_score);
}


/************************************************************************
 *									*
 *  check_offset_overlap						*
 *									*
 *  for NE instances, mapping can occur only between two instances	*
 *  whose text offset ranges have some overlap.				*
 *									*
 *  this is a pretty generous overlap check -- the only fail		*
 *  condition is if there is no overlap at all.  this occurs if		*
 *  there is a gap between the two ranges				*
 *  i.e., (k_end < r_start) or (r_end < k_start)			*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
int
check_offset_overlap(Class_instance *kinst, Class_instance *rinst)
#else
static
int
check_offset_overlap(kinst, rinst)
Class_instance *kinst;
Class_instance *rinst;
#endif	/* Use Prototypes */
{
    int	 status;
    
    status = (((kinst->end_offset < rinst->start_offset)
	       || (rinst->end_offset < kinst->start_offset))
	      ? 0 : 1);
    
    return(status);
}


/************************************************************************
 *									*
 *  combine_slot_tallies						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
combine_slot_tallies(Class_score *c_score, Tallies *total)
#else
void
combine_slot_tallies(c_score, total)
Class_score *c_score;
Tallies *total;
#endif	/* UsePrototypes */
{
    TABLE *slot_scores = (TABLE *) c_score->slot_scores;
    Slot_score *s_score;
    
    table_first(slot_scores);
    for (s_score = (Slot_score *) table_next(slot_scores);
	 s_score;
	 s_score = (Slot_score *) table_next(slot_scores))
	tallies_combine(total, s_score->tallies);
    
    return;
}
