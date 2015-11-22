/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	slot_def.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Slot_def data structure.
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
static	char	SccsId[] = "@(#)slot_def.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  slot_def_create							*
 *									*
 *  Initialize a new slot_def with NAME as key, and search the table	*
 *  for a matching class_def.  If it exists, give warning about a	*
 *  re-definition.  On the assumption that a re-definition is legal,	*
 *  free the existing slot_def, then insert the new entry into the	*
 *  table.								*
 *  									*
 *  returns a pointer to a slot_def.					*
 *  									*
 ***********************************************************************/

#if UsePrototypes
Slot_def *
slot_def_create(char *name, TABLE *table, char *classname)
#else
Slot_def *
slot_def_create(name, table, classname)
char *name;
TABLE *table;
char *classname;
#endif	/* UsePrototypes */
{
    Slot_def *newslot, *oldslot;
    
    if((newslot = UALLOC(Slot_def, 1)) == (Slot_def *)NULL)
    {
	fprintf(stderr, "Error: [slot_def_create]: Allocation failed.\n");
	exit(1);
    }

    newslot->name = STRALLOC(name);
    newslot->of_class = STRALLOC(classname);
    newslot->pattern = (char *) NULL;
    newslot->fill_types = (char **) NULL;
    newslot->num_fill_types = 0;
    newslot->has_multi_fill_types = False;
    newslot->map_weight = 0;
    newslot->is_scored_slot = False;
    newslot->total_tallies = tallies_create();

    if(oldslot = (Slot_def *)table_find((char *)newslot, table))
    {
	fprintf(stderr, "Warning: [slot_def_create]: ");
	fprintf(stderr, "Redefinition of slot '%s' in class '%s'.\n",
		name, classname);
	table_delete((char *)oldslot, table);
    }

    /* insert into table */
    if(table_append((char *) newslot, (TABLE *) table) == TABLE_ERR)
    {
	fprintf(stderr, "Error: [slot_def_create]: table insertion failed.\n");
	exit(1);
    }

    return(newslot);
}


/************************************************************************
 *  									*
 *  slot_def_release							*
 *  									*
 ***********************************************************************/

#if UsePrototypes
void
slot_def_release(char *elt)
#else
void
slot_def_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Slot_def *ptr = (Slot_def *)elt;
    register int i;
    int num;

    if(ptr == (Slot_def *)NULL)
	return;

    UFREE(ptr->name);
    UFREE(ptr->of_class);
    UFREE(ptr->pattern);

    /* fill_types is a (char **), but before we can free up fill_types,
       we must first find out how many (char *)'s were allocated and
       free those first. */

    num = ptr->num_fill_types;
    for(i=0; i<num; i++)
	UFREE(ptr->fill_types[i]);
    UFREE(ptr->fill_types);

    tallies_release((char *)ptr->total_tallies);

    /* we don't need to free up the function pointers, since space
       was never allocated for them. */

    UFREE(ptr);
}


/************************************************************************
 *  									*
 *  slot_def_compare							*
 *  									*
 ***********************************************************************/

#if UsePrototypes
int
slot_def_compare(char *elt1, char *elt2)
#else
int
slot_def_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Slot_def *ptr1 = (Slot_def *)elt1;
    Slot_def *ptr2 = (Slot_def *)elt2;
    
    return(strcasecmp(ptr1->name, ptr2->name));
}


/************************************************************************
 *  									*
 *  add_slot_def_fill_type						*
 *									*
 *  usually, each slot is of only one fill type.  however, there	*
 *  are occasions when a slot may have multiple fill types		*
 *  (e.g., either a pointer or a string may go in the slot).            *
 *  this function reallocates                             		*
 *  the size of the (char **) fill_types field by 1, and allocates	*
 *  a copy of the string FT.  if the number of fill_types is greater	*
 *  than 1, then it sets the has_multi_fill_types flag to True.		*
 *  									*
 ***********************************************************************/


#if UsePrototypes
int
add_slot_def_fill_type(Slot_def *sd, char *ft)
#else
int
add_slot_def_fill_type(sd, ft)
Slot_def *sd;
char *ft;
#endif
{
    int num = sd->num_fill_types;

/*   printf("slot %s, num = %d\n", sd->name, num); */
    sd->fill_types = UREALLOC(sd->fill_types, char *, (num+1));
    sd->fill_types[num] = STRALLOC(ft);

    /* increment num_fill_types */
    if((sd->num_fill_types = num+1) > 1)
	/* is there now more than one fill_type? */
	sd->has_multi_fill_types = True;

/*   printf("  ft%d = %s\n", num, sd->fill_types[num]); */

    return(0);
}


/************************************************************************
 *									*
 *  find_slot_def_by_name						*
 *									*
 *  checks to see if there exists a Slot_def by name of SNAME.		*
 *									*
 ***********************************************************************/

#if UsePrototypes
Slot_def *
find_slot_def_by_name(Class_def *class, char *sname)
#else
Slot_def *
find_slot_def_by_name(class, sname)
Class_def *class;
char *sname;
#endif	/* UsePrototypes */
{
    TABLE  *slot_defs = (TABLE *) class->slot_defs;
    Slot_def  *thisdef;
    
    /* search the table for a slot_def having the same
       name as sname */

    table_first(slot_defs);
    for(thisdef = (Slot_def *)table_next(slot_defs);
	thisdef && !(STRCASEEQ(sname, thisdef->name));
	thisdef = (Slot_def *)table_next(slot_defs))
	;
    
    return(thisdef);
}


/************************************************************************
 *									*
 *  find_slot_def_by_pattern						*
 *									*
 *  checks to see if there exists a Slot_def with a string pattern	*
 *  equal to SPATTERN.							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Slot_def *
find_slot_def_by_pattern(Class_def *class, char *spattern)
#else
Slot_def *
find_slot_def_by_pattern(class, spattern)
Class_def *class;
char *spattern;
#endif	/* UsePrototypes */
{
    TABLE  *slot_defs = (TABLE *) class->slot_defs;
    Slot_def  *thisdef;
    
    /* search the table for a slot_def having the same
       string pattern as spattern */

    table_first(slot_defs);
    for(thisdef = (Slot_def *)table_next(slot_defs);
	thisdef && !(STRCASEEQ(spattern, thisdef->pattern));
	thisdef = (Slot_def *)table_next(slot_defs))
	;
    
    return(thisdef);
}


/************************************************************************
 *									*
 *  print_slot_def							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_slot_def(Slot_def *sd, int indent)
#else
void
print_slot_def(sd, indent)
Slot_def *sd;
int indent;
#endif
{
    register int i;
    int num;
    
    printf("%.*s%s =\n", indent, blanks, sd->name);
    printf("%.*s%s\n", indent, blanks, sd->pattern);
    num = sd->num_fill_types;
    for(i=0; i<num; i++)
	printf("%.*sft%d: %s\n", indent+2, blanks, i, sd->fill_types[i]);
}
