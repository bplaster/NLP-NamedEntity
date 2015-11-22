/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	slot_fill.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Slot_fill data structure.
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
static	char	SccsId[] = "@(#)slot_fill.c	1.2 27 Oct 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  slot_fill_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Slot_fill *
slot_fill_create(TABLE *slots, char *name)
#else
Slot_fill *
slot_fill_create(slots, name)
TABLE *slots;
char *name;
#endif	/* UsePrototypes */
{
    Slot_fill *newfill, *oldfill;
    
    if((newfill = UALLOC(Slot_fill, 1)) == (Slot_fill *)NULL)
    {
	fprintf(stderr, "Error: [slot_fill_create]: Allocation failed.\n");
	exit(1);
    }

    newfill->name = STRALLOC(name);
    newfill->is_optional = False;
    newfill->strfills = (String_fill **) NULL;
    newfill->instptrs = (Pointer *) NULL;
    newfill->num_strs = 0;
    newfill->num_ptrs = 0;
    newfill->alt_fill_sets = (Alt_fill_set *) NULL;
    newfill->min_string = (String_fill *) NULL;
    
    /* search for an existing slot_fill.  if it exists, free up the new
       slot_fill and re-set it to the existing one. */
    if (oldfill = (Slot_fill *)table_find((char *)newfill, slots))
    {
	slot_fill_release((char *) newfill);
	newfill = oldfill;
    }
    else	/* no existing slot_fill */
    {
	/* insert the new slot_fill into the table */
	if(table_append((char *) newfill, (TABLE *) slots) == TABLE_ERR)
	{
	    fprintf(stderr, "Error: [slot_fill_create]: ");
	    fprintf(stderr, "table insertion failed.\n");
	    exit(1);
	}
    }

    return(newfill);
}


/************************************************************************
 *									*
 *  slot_fill_release							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
slot_fill_release(char *elt)
#else
void
slot_fill_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Slot_fill *ptr = (Slot_fill *)elt;
    register int  i;
    int  num;
    
    if(ptr == (Slot_fill *)NULL)
	return;
    
    UFREE(ptr->name);
    
    /* strfills is a (String_fill **), so we need to find out how many
       (String_fill *)'s have been allocated and free those first. */
    num = ptr->num_strs;
    for(i=0; i<num; i++)
	string_fill_release((char *)ptr->strfills[i]);
    UFREE(ptr->strfills);

    pointer_release((char *)ptr->instptrs);

    alt_fill_set_release((char *)ptr->alt_fill_sets);
    
    string_fill_release((char *)ptr->min_string);

    UFREE(ptr);
}


/************************************************************************
 *									*
 *  slot_fill_compare							*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
slot_fill_compare(char *elt1, char *elt2)
#else
int
slot_fill_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Slot_fill *ptr1 = (Slot_fill *)elt1;
    Slot_fill *ptr2 = (Slot_fill *)elt2;
    
    return(strcasecmp(ptr1->name, ptr2->name));
}


/************************************************************************
 *									*
 *  add_string_fill							*
 *									*
 *  add a string fill to a list of slot fills.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_string_fill(Slot_fill *slot, char *string)
#else
void
add_string_fill(slot, string)
Slot_fill *slot;
char *string;
#endif	/* UsePrototypes */
{
    int num = slot->num_strs;
    
    slot->strfills = UREALLOC(slot->strfills, String_fill *, num+1);
    slot->strfills[num] = (String_fill *) string_fill_create(string);

    slot->num_strs++;
}


/************************************************************************
 *									*
 *  add_pointer_fill							*
 *									*
 *  add a pointer fill to a list of slot fills.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_pointer_fill(Slot_fill *slot, Pointer *ptr)
#else
void
add_pointer_fill(slot, ptr)
Slot_fill *slot;
Pointer *ptr;
#endif	/* UsePrototypes */
{
    Pointer  *lastptr;

    /* is this the first pointer? */
    if (slot->instptrs == (Pointer *)NULL)
	slot->instptrs = ptr;
    else
    {
	for(lastptr = slot->instptrs;
	    lastptr->next;
	    lastptr = lastptr->next)
	    ;
	lastptr->next = ptr;
    }

    slot->num_ptrs++;
}


/************************************************************************
 *									*
 *  find_slot_fill							*
 *									*
 *  checks to see if there exists a Slot_fill by name of SNAME		*
 *  in lists.								*
 *									*
 ***********************************************************************/

#if UsePrototypes
Slot_fill *
find_slot_fill(TABLE *lists, char *sname)
#else
Slot_fill *
find_slot_fill(lists, sname)
TABLE *lists;
char *sname;
#endif	/* UsePrototypes */
{
    Slot_fill  *thisfill;
    
    /* search the table for a slot_fill having the same name as sname */

    table_first(lists);
    for(thisfill = (Slot_fill *)table_next(lists);
	thisfill && !(STRCASEEQ(sname, thisfill->name));
	thisfill = (Slot_fill *)table_next(lists))
	;

    return(thisfill);
}


/************************************************************************
 *									*
 *  print_slot_fill							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_slot_fill(Slot_fill *sfill, int indent)
#else
void
print_slot_fill(sfill, indent)
Slot_fill *sfill;
int indent;
#endif	/* UsePrototypes */
{
    register int i;
    int   num = sfill->num_strs;
    
    printf("%.*s%s%s:\n", indent, blanks, sfill->name,
	   (sfill->is_optional ? " (optional)" : ""));
    
    if(sfill->min_string != (String_fill *) NULL)
	printf("%.*smin = %s\n", indent+2, blanks, sfill->min_string->fill);
    
    for(i=0; i<num; i++)
	print_string_fill(sfill->strfills[i], indent+2);
    
    print_pointer(sfill->instptrs, indent+2);
    print_alt_fill_set(sfill->alt_fill_sets, 1, indent+2);
}
