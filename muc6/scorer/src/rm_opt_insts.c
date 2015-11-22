/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	rm_opt_insts.c
 *
 *	previous name - remove_optional_instances.c
 *			(exceeds 15-char ar limit)
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	Functions to identify and remove optional instances.
 * After the objects that should be mapped are mapped, as many
 * unmapped key objects and spurious response objects are 
 * "connected" as possible.  But before this happens, the unmapped
 * key objects which are optional are identified, and removed from
 * the catalog.  Hence, this file.
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
static	char	SccsId[] = "@(#)rm_opt_insts.c	1.7 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"


typedef struct optional_instance
{
    struct class_instance     *inst;
    struct optional_instance  *next;
} Optional_instance;


/*  internal function prototypes */

Proto	(extern	void,
	 find_and_remove_catalog_entry_optional_instances,
	 (Catalog_entry *entry));
Proto	(static	void,
	 find_and_collect_optional_instances,
	 (Class_instance *kinst, Class_def *class));
Proto	(static	void,
	 collect_flat_optional_instances,
	 (Class_instance *kinst));
Proto	(static	void, find_optional_instances, (Class_instance *kinst,
						Slot_def *slot));
Proto	(static	void,
	 find_optional_instances_from_alternate_fills,
	 (Class_instance *parent, Slot_def *slot,
	  Slot_fill *kfill, Slot_fill *rfill));

Proto	(static	void, collect_optional_instances, (Class_instance *parent,
						   Slot_fill *sfill));
Proto	(static	void,
	 collect_optional_instances_from_alt_set,
	 (Class_instance *parent, Alt_fill_set *set));
Proto	(static	void, remove_back_pointer, (Class_instance *child,
					    Class_instance *parent));
Proto	(static	void, remove_optional_instances, (TABLE *instlists));
Proto	(static	void, remove_instance, (Class_instance *this_inst,
					TABLE *instlists));
Proto	(static	void, switch_mappings_with_ident, (Class_instance *inst));
Proto	(static	void, remove_from_ident_pointers, (Class_instance *inst));
Proto	(static	void, remove_ident_pointer, (Class_instance *inst,
					     Class_instance *target));

Proto	(static	Optional_instance *,
	 optional_instance_create,
	 (Class_instance *optinst));
Proto	(static	void, optional_instance_release, (char *elt));
Proto	(static	void, add_optional_instance, (Optional_instance *new));


static Optional_instance *MUC_optional_instances;


#if UsePrototypes
static
Optional_instance *
optional_instance_create(Class_instance *optinst)
#else
static
Optional_instance *
optional_instance_create(optinst)
Class_instance *optinst;
#endif	/* UsePrototypes */
{
    Optional_instance *new = (Optional_instance *) NULL;
    
    if ((new = UALLOC(Optional_instance, 1)) == (Optional_instance *) NULL)
    {
	fprintf(stderr,	"Error: [optional_instance_create]: ");
	fprintf(stderr, "Allocation failed.\n");
	exit(1);
    }

    new->inst = optinst;
    new->next = (Optional_instance *) NULL;
    
    return(new);
}

#if UsePrototypes
static
void
optional_instance_release(char *elt)
#else
static
void
optional_instance_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Optional_instance *ptr = (Optional_instance *) elt;
    
    if (ptr == (Optional_instance *) NULL)
	return;
    
    /* there is nothing to free, because nothing was allocated */
    ptr->inst = (Class_instance *) NULL;

    /* recursive call will release the tail of this linked list first */

    optional_instance_release((char *) ptr->next);

    ptr->next = (Optional_instance *) NULL;

    UFREE(ptr);
    
    return;
}


#if UsePrototypes
static
void
add_optional_instance(Optional_instance *new)
#else
static
void
add_optional_instance(new)
Optional_instance *new;
#endif	/* UsePrototypes */
{
    Optional_instance *last;
    
    /* is this the first ? */
    if (MUC_optional_instances == (Optional_instance *) NULL)
	MUC_optional_instances = new;
    else
    {
	/* an instance may already be on the list, since any number
	   of parents may optionally point to it, so don't add new
	   if new->inst is already in the list.
	   search until existing inst is found, or end of list is
	   reached. */

	for (last = MUC_optional_instances;
	     (last->inst != new->inst) && last->next;
	     last = last->next)
	    ;

	if (last->inst == new->inst)
	    optional_instance_release((char *) new);
	else
	    last->next = new;
    }
    
    return;
}


/************************************************************************
 *									*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
find_and_remove_MUC_optional_instances(void)
#else
void
find_and_remove_MUC_optional_instances()
#endif	/* UsePrototypes */
{
    TABLE  *Catalog = (TABLE *)MUC_Catalog;
    Catalog_entry *entry;

    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
    {
	find_and_remove_catalog_entry_optional_instances(entry);
    }
}


#if UsePrototypes
void
find_and_remove_catalog_entry_optional_instances(Catalog_entry *entry)
#else
void
find_and_remove_catalog_entry_optional_instances(entry)
Catalog_entry *entry;
#endif	/* UsePrototypes */
{
    TABLE *classes = (TABLE *) MUC_Class_defs;
    Root *k_root = entry->k_root;
    TABLE *k_class_instlists;
    Class_instance_list *k_class_instlist;
    TABLE *k_instances;
    Class_instance *kinst;
    Class_def *class;

    MUC_optional_instances = (Optional_instance *) NULL;
    
    if (k_root == (Root *) NULL)
	return;
    else
    {
	k_class_instlists = (TABLE *) k_root->class_instance_lists;
	
	table_first(classes);
	for (class = (Class_def *) table_next(classes);
	     class;
	     class = (Class_def *) table_next(classes))
	{
	    k_class_instlist = find_class_instance_list(k_class_instlists,
							class->name);

	    if (k_class_instlist != (Class_instance_list *) NULL)
	    {
		k_instances = (TABLE *) k_class_instlist->instances;

		table_first(k_instances);
		for (kinst = (Class_instance *) table_next(k_instances);
		     kinst;
		     kinst = (Class_instance *) table_next(k_instances))
		{
		    /* optional corefs will be ignored when building
		       the equivalency_classes */
		    if (MUC_scoring_task == SCENARIO_TEMPLATE)
			find_and_collect_optional_instances(kinst, class);
		    else if (MUC_scoring_task == TEMPLATE_ELEMENT)
			collect_flat_optional_instances(kinst);
		    else if (MUC_scoring_task == NAMED_ENTITY)
			collect_flat_optional_instances(kinst);
		}
	    }
	}

	/* clean up any possible optional instances from this entry */

	remove_optional_instances(k_class_instlists);
	optional_instance_release((char *) MUC_optional_instances);
	MUC_optional_instances = (Optional_instance *) NULL;
    }
}


#if UsePrototypes
static
void
find_and_collect_optional_instances(Class_instance *kinst, Class_def *class)
#else
static
void
find_and_collect_optional_instances(kinst, class)
Class_instance *kinst;
Class_def *class;
#endif	/* UsePrototypes */
{
    TABLE *slots = (TABLE *) class->slot_defs;
    Slot_def *slot;
    int i, num_fill_types;

    table_first(slots);
    for (slot = (Slot_def *) table_next(slots);
	 slot;
	 slot = (Slot_def *) table_next(slots))
    {
	num_fill_types = slot->num_fill_types;

	/* this slot may be defined as multi_fill_typed, so check
	   if any of the fill_types is a pointer.  proceed to
	   find_optional_instances if it is a pointer_fill_type. */

	for (i=0; i<num_fill_types; i++)

	    if(find_class_def_by_name(slot->fill_types[i]))
	    {
		find_optional_instances(kinst, slot);		
		break;
	    }
    }
    
    return;
}


#if UsePrototypes
static
void
collect_flat_optional_instances(Class_instance *kinst)
#else
static
void
collect_flat_optional_instances(kinst)
Class_instance *kinst;
#endif	/* UsePrototypes */
{
    Slot_fill *status = find_slot_fill(kinst->slot_fills, "status");
    Optional_instance *optinst;

    /* optional only if kinst is unmapped and is marked as optional */
    if ( ( (MUC_scoring_task == SCENARIO_TEMPLATE)
	  || (MUC_scoring_task == TEMPLATE_ELEMENT) )
	&& (kinst->mapped_to == (Class_instance *) NULL)
	&& (status != (Slot_fill *) NULL)
	&& ( STRCASEEQ("optional", (status->strfills[0])->fill)
	    || STRCASEEQ("opt", (status->strfills[0])->fill) ))
    {
	optinst = optional_instance_create(kinst);
	add_optional_instance(optinst);
    }
    else if ( (MUC_scoring_task == NAMED_ENTITY)
	     && (kinst->mapped_to == (Class_instance *) NULL)
	     && kinst->is_optional )
    {
	optinst = optional_instance_create(kinst);
	add_optional_instance(optinst);
    }

    return;
}


#if UsePrototypes
static
void
find_optional_instances(Class_instance *kinst, Slot_def *slot)
#else
static
void
find_optional_instances(kinst, slot)
Class_instance *kinst;
Slot_def *slot;
#endif	/* UsePrototypes */
{
    Class_instance *rinst = kinst->mapped_to;
    Slot_fill *kfill;
    Slot_fill *rfill = (Slot_fill *) NULL;
    Alt_fill_set *alt_set;
    
    kfill = find_slot_fill(kinst->slot_fills, slot->name);

    if (kfill == (Slot_fill *) NULL)
	return;
    else
    {
	if (rinst != (Class_instance *) NULL)
	    rfill = find_slot_fill(rinst->slot_fills, slot->name);

	/* optional instances can occur either in an optional fill
	   or in a fill containing alternate fills. */

	if (kfill->is_optional)
	{
	    collect_optional_instances(kinst, kfill);

	    /* are there any alternate fills ? */
	    if ((alt_set = kfill->alt_fill_sets) != (Alt_fill_set *) NULL)
	    {
		while(alt_set != (Alt_fill_set *) NULL)
		{
		    collect_optional_instances_from_alt_set(kinst, alt_set);
		    alt_set = alt_set->next;
		}
	    }
	}
	/* no optional fills, how about alternate fills ? */
	else if (kfill->alt_fill_sets != (Alt_fill_set *) NULL)
	    find_optional_instances_from_alternate_fills(kinst, slot,
							 kfill, rfill);
    }
    
    return;
}


#if UsePrototypes
static
void
find_optional_instances_from_alternate_fills(Class_instance *parent,
					     Slot_def *slot,
					     Slot_fill *kfill,
					     Slot_fill *rfill)
#else
static
void
find_optional_instances_from_alternate_fills(parent, slot, kfill, rfill)
Class_instance *parent;
Slot_def *slot;
Slot_fill *kfill;
Slot_fill *rfill;
#endif	/* UsePrototypes */
{
    Tallies *temp = tallies_create();
    Alt_fill_set *this_alt_set, *best_alt_set;
    float temp_f, best_f;
    int num_r, num_best, num_temp, num_fills;
    Bool best_is_from_alt_set = False;
    Bool rfill_is_missing = (rfill == (Slot_fill *) NULL);

    /* score the key slot_fill first, before iterating over
       the alternate sets */
    if (rfill_is_missing)
    {
	num_fills = kfill->num_strs + kfill->num_ptrs;
	tally_possible(temp, num_fills);
	tally_missing(temp, num_fills);
    }
    else
	score_fills(slot, temp, kfill, rfill);
    
    best_f = compute_tallies_f(temp, F_PR);
    num_best = temp->possible;

    /* num_r equals the number of rfill strings or pointers */
    num_r = temp->actual;
    
    /* see if there's a better match from the alternate sets */

    for (this_alt_set = kfill->alt_fill_sets;
	 this_alt_set;
	 this_alt_set = this_alt_set->next)
    {
	tallies_clear(temp);
	if (rfill_is_missing)
	{
	    num_fills = this_alt_set->num_strs + this_alt_set->num_ptrs;
	    tally_possible(temp, num_fills);
	    tally_missing(temp, num_fills);
	}
	else
	    score_alternate_set(slot, temp, this_alt_set, rfill, (String_fill *) NULL);

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
	    /* collect optional instances from the previous best */
	    if (best_is_from_alt_set)
		collect_optional_instances_from_alt_set(parent, best_alt_set);
	    else
	    {
		collect_optional_instances(parent, kfill);
		best_is_from_alt_set = True;
	    }

	    /* set the new best set */
	    best_f = temp_f;
	    num_best = num_temp;
	    best_alt_set = this_alt_set;
	}
	else
	    /* this wasn't better, so collect optional instances from it */
	    collect_optional_instances_from_alt_set(parent, this_alt_set);
    }
    
    tallies_release((char *) temp);

    return;
}


#if UsePrototypes
static
void
collect_optional_instances(Class_instance *parent, Slot_fill *sfill)
#else
static
void
collect_optional_instances(parent, sfill)
Class_instance *parent;
Slot_fill *sfill;
#endif	/* UsePrototypes */
{
    Pointer *thisptr = sfill->instptrs;
    Class_instance *this_inst;
    Optional_instance *optinst;

    while(thisptr != (Pointer *) NULL)
    {
	this_inst = thisptr->inst;
	
	/* if this_inst is a mapped in_and_out instance but it has an
	   unmapped 'twin', then give the 'twin' this_inst's mapping. */
	if STRCASEEQ(this_inst->name, "in_and_out")
         /* if (muc_cmp_special(this_inst->name, "in_and_out") ||
              muc_cmp_special(this_inst->name, "IN_AND_OUT"))*/
	    switch_mappings_with_ident(this_inst);

	/* this_inst may be optional only if it's not mapped */
	if(this_inst->mapped_to == (Class_instance *) NULL)
	{
	    optinst = optional_instance_create(this_inst);
	    add_optional_instance(optinst);
	}

	/* whether or not any of the insts are optional (i.e., unmapped),
	   nevertheless they are not a necessary fill of the parent, so
	   they no longer need to be pointed to by parent */

	remove_back_pointer(this_inst, parent);

	/* continue loop */
	thisptr = thisptr->next;
    }

    /* free all the instance pointers. */
    pointer_release((char *) sfill->instptrs);
    sfill->instptrs = (Pointer *) NULL;
    sfill->num_ptrs = 0;
    
    return;
}


#if UsePrototypes
static
void
collect_optional_instances_from_alt_set(Class_instance *parent,
					Alt_fill_set *set)
#else
static
void
collect_optional_instances_from_alt_set(parent, set)
Class_instance *parent;
Alt_fill_set *set;
#endif	/* UsePrototypes */
{
    Pointer *thisptr = set->instptrs;
    Class_instance *this_inst;
    Optional_instance *optinst;

    while(thisptr != (Pointer *) NULL)
    {
	this_inst = thisptr->inst;
	
	/* if this_inst is a mapped in_and_out instance but it has an
	   unmapped 'twin', then give the 'twin' this_inst's mapping. */
	if STRCASEEQ(this_inst->name, "in_and_out")
/*
          if (muc_cmp_special(this_inst->name, "in_and_out") ||
              muc_cmp_special(this_inst->name, "in_and_out"))
*/
	    switch_mappings_with_ident(this_inst);

	/* this_inst may be optional only if it's not mapped */
	if(this_inst->mapped_to == (Class_instance *) NULL)
	{
	    optinst = optional_instance_create(this_inst);
	    add_optional_instance(optinst);
	}

	/* whether or not any of the insts are optional (i.e., unmapped),
	   nevertheless they are not a necessary fill of the parent, so
	   they no longer need to be pointed to by parent */

	remove_back_pointer(this_inst, parent);

	/* continue loop */
	thisptr = thisptr->next;
    }

    /* free all the instance pointers. */
    pointer_release((char *) set->instptrs);

    set->instptrs = (Pointer *) NULL;
    set->num_ptrs = 0;
    
    return;
}


#if UsePrototypes
static
void
remove_back_pointer(Class_instance *child, Class_instance *parent)
#else
static
void
remove_back_pointer(child, parent)
Class_instance *child;
Class_instance *parent;
#endif	/* UsePrototypes */
{
    Pointer *first = child->back_pointers;
    Pointer *prev, *this;
/*
    printf("removing back_pointer from %s-%s-%s to %s-%s-%s\n",
	   child->name, child->docnum, child->id,
	   parent->name, parent->docnum, parent->id);
    printf("before: %d back_pointers\n", child->num_backptrs);
    print_back_pointers(child);
*/
    if (first != (Pointer *) NULL)
    {
	/* is parent the first back_pointer ? */
	if(parent == first->inst)
	{
	    /* move the rest of the list up, while removing the first */
	    child->back_pointers = first->next;
	    first->next = (Pointer *) NULL;
	    pointer_release((char *) first);
	    child->num_backptrs--;
	}
	else
	{
	    prev = first;
	    while ((this = prev->next) != (Pointer *) NULL)
	    {
		if(parent == this->inst)
		{
		    /* maintain the list while removing this */
		    prev->next = this->next;
		    this->next = (Pointer *) NULL;
		    pointer_release((char *) this);
		    child->num_backptrs--;
		    break;
		}
		else	/* continue loop */
		    prev = this;
	    }
	}
    }
/*
    printf("after: %d back_pointers\n", child->num_backptrs);
    print_back_pointers(child);
*/    
    return;
}


#if UsePrototypes
static
void
remove_optional_instances(TABLE *instlists)
#else
static
void
remove_optional_instances(instlists)
TABLE *instlists;
#endif	/* UsePrototypes */
{
    Optional_instance  *this_opt = MUC_optional_instances;

    while(this_opt)
    {
	/* since remove_instance is recursive, it's possible for an
	   instance in the optional list to have already been removed
	   by the time this list traversal gets to it. */

	if (this_opt->inst != (Class_instance *) NULL)
	    remove_instance(this_opt->inst, instlists);

	this_opt->inst = (Class_instance *) NULL;
	this_opt = this_opt->next;
    }
    
    return;
}


#if UsePrototypes
static
void
remove_instance(Class_instance *this_inst, TABLE *instlists)
#else
static
void
remove_instance(this_inst, instlists)
Class_instance *this_inst;
TABLE *instlists;
#endif	/* UsePrototypes */
{
    Class_instance_list  *instlist;
    TABLE  *instances;
    Class_instance *target;
    Class_def *class;
    TABLE *slots;
    Slot_def *slot;
    Slot_fill  *sfill;
    Alt_fill_set  *alt_set;
    Pointer *first_child_ptr, *child_ptr;
    Optional_instance *opt;
    
    /* if this_inst is mapped or still has back_pointers,
       then it isn't truly optional, so skip it. */

    if ( (this_inst->mapped_to != (Class_instance *) NULL)
	|| (this_inst->back_pointers != (Pointer *) NULL) )
	return;
    else
    {
	class = find_class_def_by_name(this_inst->name);
	slots = (TABLE *) class->slot_defs;
	    
	table_first(slots);
	for (slot = (Slot_def *) table_next(slots);
	     slot;
	     slot = (Slot_def *) table_next(slots))
	{
	    sfill = find_slot_fill(this_inst->slot_fills, slot->name);
	    if (sfill != (Slot_fill *) NULL)
	    {
		/* before removing this_inst, adjust the back_pointers lists
		   of its children. */

		child_ptr = sfill->instptrs;
		while (child_ptr != (Pointer *) NULL)
		{
		    remove_back_pointer(child_ptr->inst, this_inst);

		    /* recursive removal of child instance */
		    remove_instance(child_ptr->inst, instlists);

		    /* set the child_ptr->inst value to NULL */
		    child_ptr->inst = (Class_instance *) NULL;
		    
		    child_ptr = child_ptr->next;
		}

		alt_set = sfill->alt_fill_sets;
		while (alt_set != (Alt_fill_set *) NULL)
		{
		    child_ptr = alt_set->instptrs;
		    while (child_ptr != (Pointer *) NULL)
		    {
			remove_back_pointer(child_ptr->inst, this_inst);

			/* recursive removal of child instance */
			remove_instance(child_ptr->inst, instlists);

			/* set the child_ptr->inst value to NULL */
			child_ptr->inst = (Class_instance *) NULL;

			child_ptr = child_ptr->next;
		    }

		    alt_set = alt_set->next;
		}
	    }
	}

	instlist = find_class_instance_list(instlists, class->name);
	instances = (TABLE *) instlist->instances;

	/* some optional_instance structures may still point to
	   this_inst, so traverse the MUC_optional_instances and set
	   those pointers to NULL. */

	for (opt = MUC_optional_instances;
	     opt;
	     opt = opt->next)
	{
	    if (opt->inst == this_inst)
		opt->inst = (Class_instance *) NULL;
	}

	/* if this is an in_and_out instance, remove it from any
	   identical pointers lists */
        if STRCASEEQ(this_inst->name, "in_and_out")
/*
        if (muc_cmp_special(this_inst->name, "in_and_out") ||
            muc_cmp_special(this_inst->name, "IN_AND_OUT"))
*/
	    remove_from_ident_pointers(this_inst);

	/* this_inst should now be totally disconnected,
	   so create a dummy instance with the same 'key'
	   as this_inst, and delete it from the table. */

	/* create a new class_instance */

	if((target = UALLOC(Class_instance, 1)) == (Class_instance *)NULL)
	{
	    fprintf(stderr, "Error: [remove_optional_instance]: ");
	    fprintf(stderr, "Allocation failed.\n");
	    exit(1);
	}

	/* initialize */
	target->name = (char *) NULL;
	target->docnum = (char *) NULL;
	target->id = STRALLOC(this_inst->id);
	/* create an empty table for the slot_fills */
	target->slot_fills = (TABLE *) NULL;
	target->mapped_to = (Class_instance *) NULL;
	target->connected_to = (Class_instance *) NULL;
	target->ident_in_and_out_ptrs = (Pointer *) NULL;
	target->back_pointers = (Pointer *) NULL;
	
	printf("removing optional %s-%s-%s...",
	       this_inst->name, this_inst->docnum, this_inst->id);
	if(table_delete((char *) target, instances) == TABLE_OK)
	{
	    printf("done\n");
	    class_instance_release((char *) target);
	}
	else
	    printf("failed\n");
    }
    
    return;
}


/************************************************************************
 *									*
 *  switch_mappings_with_ident						*
 *									*
 *  when two in_and_out key instances are identical, the 'wrong' key	*
 *  may have been mapped to the response.  the scorer will give the	*
 *  benefit of the doubt by checking all identicals for a mapping.	*
 *  however, if there is an in_and_out instance that may be optional,	*
 *  then there is no negative effect of passing its mapping on to an	*
 *  identical but unmapped key.  this will ensure that 'optional'	*
 *  in_and_out instances are not kept around and counted as missing.	*
 *									*
 ***********************************************************************/
#if UsePrototypes
static
void
switch_mappings_with_ident(Class_instance *inst)
#else
static
void
switch_mappings_with_ident(inst)
Class_instance *inst;
#endif	/* UsePrototypes */
{
    Pointer *ident_ptr;
    Class_instance *mapped_to, *ident;

    if (mapped_to = inst->mapped_to)
    {
	for (ident_ptr = inst->ident_in_and_out_ptrs;
	     ident_ptr;
	     ident_ptr = ident_ptr->next)
	{
	    if ((ident = ident_ptr->inst)
		&& (ident->mapped_to == (Class_instance *) NULL))
	    {
		printf("passing the mapping of %s-%s-%s",
		       inst->name, inst->docnum, inst->id);
		printf(" to identical %s-%s-%s\n",
		       ident->name, ident->docnum, ident->id);

		/* make the switch */
		ident->mapped_to = mapped_to;
		mapped_to->mapped_to = ident;
		inst->mapped_to = (Class_instance *) NULL;
		break;
	    }
	}
    }
    
    return;
}


#if UsePrototypes
static
void
remove_from_ident_pointers(Class_instance *inst)
#else
static
void
remove_from_ident_pointers(inst)
Class_instance *inst;
#endif	/* UsePrototypes */
{
    Pointer *ident_ptr;
    Class_instance *ident;
    
    for(ident_ptr = inst->ident_in_and_out_ptrs;
	ident_ptr;
	ident_ptr = ident_ptr->next)
    {
	if((ident = ident_ptr->inst) != (Class_instance *) NULL)
	    remove_ident_pointer(ident, inst);
    }
    
    return;
}


#if UsePrototypes
static
void
remove_ident_pointer(Class_instance *inst, Class_instance *target)
#else
static
void
remove_ident_pointer(inst, target)
Class_instance *inst;
Class_instance *target;
#endif	/* UsePrototypes */
{
    Pointer *first = inst->ident_in_and_out_ptrs;
    Pointer *prev, *this;
    
    if (first != (Pointer *) NULL)
    {
	/* is target the first identical instance ? */
	if(first->inst == target)
	{
	    /* move the rest of the list up, while removing the first */
	    inst->ident_in_and_out_ptrs = first->next;
	    first->next = (Pointer *) NULL;
	    pointer_release((char *) first);
	}
	else
	{
	    prev = first;
	    while ((this = prev->next) != (Pointer *) NULL)
	    {
		if(this->inst == target)
		{
		    /* maintain the list while removing this */
		    prev->next = this->next;
		    this->next = (Pointer *) NULL;
		    pointer_release((char *) this);
		    break;
		}
		else	/* continue loop */
		    prev = this;
	    }
	}
    }

    return;
}
