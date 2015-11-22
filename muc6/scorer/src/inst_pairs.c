/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	inst_pairs.c
 *
 *	should be inst_pairings.c, but that reaches 
 *             15 char ar limit
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Inst_pairings data structure
 *      Inst_pairings is the head of a list of Inst_pair
 *      structures.  A list of Inst_pairings has one
 *      Inst_pairings structure for each class in the 
 *      MUC task.  Inst_pairings structures contain
 *      two lists of Inst_pair structures.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *      Consolidated Class_mappings and Class_connections DS's
 *      into Inst_pairings -ad 12/05/96
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)inst_pairings.c	1.4 03 Sep 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/*  internal function prototypes */

Proto	(static void, print_all_pairings, (Inst_pairings *pairings));
Proto	(static void, print_final_pairings, (Inst_pairings *pairings));
Proto   (static void, print_inst_pair_list, (Inst_pair **list, 
					     int list_size ));


/************************************************************************
 *									*
 *  inst_pairings_create						*
 *									*
 ***********************************************************************/

#if UsePrototypes
Inst_pairings *
inst_pairings_create(char *cname)
#else
Inst_pairings *
inst_pairings_create(cname)
char *cname;
#endif	/* UsePrototypes */
{
    Inst_pairings *new;
    
    if ((new = UALLOC(Inst_pairings, 1)) == (Inst_pairings *) NULL)
    {
	fprintf(stderr, "Error: Allocation for Inst_pairings failed.\n");
	exit(1);
    }
    
    new->name = STRALLOC(cname);
    new->all = (Inst_pair **) NULL;
    new->final = (Inst_pair **) NULL;
    new->num_all = 0;
    new->num_final = 0;
    
    return(new);
}


/************************************************************************
 *									*
 *  inst_pairings_release						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
inst_pairings_release(char *elt)
#else
void
inst_pairings_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Inst_pairings *ptr = (Inst_pairings *) elt;
    register int i;
    int num = ptr->num_all;
    
    UFREE(ptr->name);

    /* free all the inst_pair structures in ptr->all only.
       since ptr->final points to a subset of inst_pairs in
       ptr->all, there won't be a need to free them from ptr->final. */
    for (i=0; i<num; i++)
	inst_pair_release((char *) ptr->all[i]);
    UFREE(ptr->all);
    UFREE(ptr->final);
    
    UFREE(ptr);

    return;
}


/************************************************************************
 *									*
 *  inst_pairings_compare						*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
inst_pairings_compare(char *elt1, char *elt2)
#else
int
inst_pairings_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Inst_pairings *ptr1 = (Inst_pairings *)elt1;
    Inst_pairings *ptr2 = (Inst_pairings *)elt2;

    return(strcasecmp(ptr1->name, ptr2->name));
}


/************************************************************************
 *									*
 *  initialize_inst_pairings_list					*
 *									*
 *  initialize the pairings lists by creating a Inst_pairings		*
 *  structure for every defined class.					*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
initialize_inst_pairings_list(TABLE *pairings_list)
#else
void
initialize_inst_pairings_list(pairings_list)
TABLE *pairings_list;
#endif	/* UsePrototypes */
{
    TABLE  *Class_defs = MUC_Class_defs;
    Class_def  *class_def;
    Inst_pairings *newpairings;
    
    table_first(Class_defs);
    for(class_def = (Class_def *)table_next(Class_defs);
	class_def;
	class_def = (Class_def *)table_next(Class_defs))
    {
	newpairings = inst_pairings_create(class_def->name);

	/* insert into table.  don't bother checking for pre-existing
	   structure in the list.  we're basically copying the Class_defs
	   list, which shouldn't have any duplication */

	if(table_append((char *) newpairings, (TABLE *) pairings_list)
	   == TABLE_ERR)
	{
	    fprintf(stderr, "Error: [initialize_inst_pairings_list]: ");
	    fprintf(stderr, " table insertion failed.\n");
	    exit(1);
	}
    }

    return;
}


/************************************************************************
 *									*
 *  find_inst_pairings							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Inst_pairings *
find_inst_pairings(TABLE *pairings_list, char *cname)
#else
Inst_pairings *
find_inst_pairings(pairings_list, cname)
TABLE *pairings_list;
char *cname;
#endif	/* UsePrototypes */
{
    Inst_pairings  *this_pairings;
    
    /* search the table for an inst_pairings having the same
       name as cname. */
    table_first(pairings_list);
    for(this_pairings = (Inst_pairings *)table_next(pairings_list);
	this_pairings && !(STRCASEEQ(cname, this_pairings->name));
	this_pairings = (Inst_pairings *)table_next(pairings_list))
	;
    
    return(this_pairings);
}


/************************************************************************
 *									*
 *  print_inst_pairings
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_inst_pairings(Inst_pairings *pairings)
#else
void
print_inst_pairings(pairings)
Inst_pairings *pairings;
#endif	/* UsePrototypes */
{
    register int i;
    int num = pairings->num_all;
    
    printf("\n%s instance pairings (%d):\n", pairings->name );

    printf("\n all pairings (%d):\n", pairings->num_all);
    print_inst_pair_list( pairings->all, pairings->num_all );

    printf("\n final pairings (%d):\n", pairings->num_final );
    print_inst_pair_list( pairings->final, pairings->num_final );
}


/************************************************************************
 *
 *  print_inst_pair_list 
 *
 *  In inst_pairings, there are two lists of inst_pair structures.  
 *  This function prints either list.
 *
 ***********************************************************************/

#if UsePrototypes
static
void
print_inst_pair_list( Inst_pair **list, int list_size )
#else
static
void
print_inst_pair_list( list, list_size )
Inst_pair **list;
int list_size;
#endif	/* UsePrototypes */
{
    int i;
    
    for (i=0; i<list_size; i++)
	print_inst_pair( list[i] );
}




