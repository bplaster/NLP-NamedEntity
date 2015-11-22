/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	connect.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	connecting functions for MUC scorer.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
	Changed  Class_connections to Inst_pairings 
        Changed  Connecting_pair   to Inst_pair      -ad 12/06/96
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	September 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)connect.c	1.1  04 Sep 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include <stdio.h>
#include "table.h"
#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"


/*  internal function declarations */

Proto	(static void, connect_class_instances, (Class_def *class,
						Inst_pairings *connections,
						Class_instance_list *klist,
						Class_instance_list *rlist));
Proto	(static void, update_connections, (Inst_pairings *connections));
Proto	(static void, connect_instance_pair, (Class_def *class,
					      Inst_pair *pair));

    
/************************************************************************
 *									*
 *  connect_MUC_catalog()						*
 *									*
 *  connect unmapped instances in all documents and templates.		*
 *  calls connect_catalog_entry.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
connect_MUC_catalog(void)
#else
void
connect_MUC_catalog()
#endif	/* UsePrototypes */
{
    TABLE  *Catalog = (TABLE *)MUC_Catalog;
    Catalog_entry *entry;

    printf("Begin Connecting...\n");
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
    {
	printf("connecting %s ...", entry->docnum);
	connect_catalog_entry(entry);
	printf("done.\n");
    }

    printf("Connecting Completed.\n");
    return;
}


/************************************************************************
 *									*
 *  connect_catalog_entry						*
 *									*
 *  if catalog_entry points to both a key and a response root, then	*
 *  each root will have a table of classes, with each class pointing	*
 *  to a table of class instances.  for each class, call the function	*
 *  connect_class_instances to force "best of the rest" pairings	*
 *  among the remaining unmapped instances.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
connect_catalog_entry(Catalog_entry *entry)
#else
void
connect_catalog_entry(entry)
Catalog_entry *entry;
#endif	/* UsePrototypes */
{
    TABLE  *class_connect_list = (TABLE *) entry->class_connections_list;
    Root  *k_root, *r_root;
    TABLE  *k_class_inst_lists, *r_class_inst_lists;
    Class_instance_list  *k_class_inst_list, *r_class_inst_list;
    Inst_pairings *thisconnections;
    Class_def  *thisclass;
    char **connect_order = Map_order->value_list;
    int  num_order = Map_order->num_vals;
    int i;
    char *classname;

    /* must have both k_root and r_root, otherwise there's nothing
       to connect if either is null. */
    if(((k_root = entry->k_root) == (Root *) NULL)
       || ((r_root = entry->r_root) == (Root *) NULL))
	return;
    
    k_class_inst_lists = (TABLE *) k_root->class_instance_lists;
    r_class_inst_lists = (TABLE *) r_root->class_instance_lists;

    /* connect the classes for which there are key and response instances */

    for(i=0; i<num_order; i++)
    {
	classname = connect_order[i];
	thisclass = find_class_def_by_name(classname);
	thisconnections = find_inst_pairings(class_connect_list, classname);
	k_class_inst_list = find_class_instance_list(k_class_inst_lists,
						     classname);
	r_class_inst_list = find_class_instance_list(r_class_inst_lists,
						     classname);
	/* proceed with connect only if there are both key and
	   response instances. */
	if((k_class_inst_list != (Class_instance_list *) NULL)
	   && (r_class_inst_list != (Class_instance_list *) NULL))
	    connect_class_instances(thisclass,
				    thisconnections,
				    k_class_inst_list,
				    r_class_inst_list);
    }

    return;
}
    

/************************************************************************
 *									*
 *  connect_class_instances						*
 *									*
 *  create a inst_pair structure for every pair of unmapped	*
 *  key and response instances.  call the function connect_instance	*
 *  to compare each pair.  qsort all the inst_pairs, then set	*
 *  the final connections by setting the connected_to pointers for	*
 *  each member of a connected pair, and ignoring subsequent		*
 *  connections that contain an already connected member.		*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
connect_class_instances(Class_def *class, Inst_pairings *connections,
			Class_instance_list *klist, Class_instance_list *rlist)
#else
static
void
connect_class_instances(class, connections, klist, rlist)
Class_def *class;
Inst_pairings *connections;
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
	/* if kinst is already mapped, then skip this. */
	if (kinst->mapped_to)
	    ;
	else
	{
	    table_first(r_instances);
	    for(rinst = (Class_instance *)table_next(r_instances);
		rinst;
		rinst = (Class_instance *)table_next(r_instances))
	    {
		/* if rinst is already mapped, then skip this. */
		if (rinst->mapped_to)
		    ;
		else
		{
		    thispair = inst_pair_create();
		    thispair->key = kinst;
		    thispair->response = rinst;
	    
		    connect_instance_pair(class, thispair);
		    add_inst_pair_to_all(connections, thispair);
		}
	    }
	}
    }

    /* qsort all the pairs */
    qsort((char *)connections->all,
	  connections->num_all,
	  sizeof (Inst_pair *),
	  inst_pair_compare);
    
    update_connections(connections);

    return;
}

#if UsePrototypes
static
void
update_connections(Inst_pairings *connections)
#else
static
void
update_connections(connections)
Inst_pairings *connections;
#endif	/* UsePrototypes */
{
    register int i;
    Inst_pair *thispair;
    Class_instance *kinst;
    Class_instance *rinst;
    int num_all = connections->num_all;

    for (i=0; i<num_all; i++)
    {
	thispair = connections->all[i];
	kinst = thispair->key;
	rinst = thispair->response;
	
	/* no minimum score is required for connection.  however, if
	   either kinst or rinst is already connected, then skip this. */
	if (kinst->connected_to
	    || rinst->connected_to)
	    ;
	else
	{
	    kinst->connected_to = rinst;
	    rinst->connected_to = kinst;
	    add_inst_pair_to_final(connections, thispair);
	}
    }
    
    return;
}
	    

/************************************************************************
 *									*
 *  connect_instance_pair						*
 *									*
 *  score each pair, then compute an f_score.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
connect_instance_pair(Class_def *class, Inst_pair *pair)
#else
static
void
connect_instance_pair(class, pair)
Class_def *class;
Inst_pair *pair;
#endif	/* UsePrototypes */
{
    TABLE *slotlist = (TABLE *) class->slot_defs;
    Class_score *c_score = class_score_create(class, pair->key->docnum);
    Tallies *slot_tallies = tallies_create();

    score_instance_pair(slotlist, c_score,
			pair->key, pair->response);
    tallies_clear(slot_tallies);
    combine_slot_tallies(c_score, slot_tallies);
    pair->score = compute_tallies_f(slot_tallies, F_PR);
    
    tallies_release((char *) slot_tallies);
    class_score_release((char *) c_score);
    
    return;
}
