/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	alt_fill_set.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Alt_fill_set data structure.
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
static	char	SccsId[] = "@(#)alt_fill_set.c	1.4 27 Oct 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  alt_fill_set_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Alt_fill_set *
alt_fill_set_create(void)
#else
Alt_fill_set *
alt_fill_set_create()
#endif	/* UsePrototypes */
{
    Alt_fill_set *newalt;
    
    if((newalt = UALLOC(Alt_fill_set, 1)) == (Alt_fill_set *)NULL)
    {
	fprintf(stderr, "Error: [alt_fill_set_create]: Allocation failed.\n");
	exit(1);
    }
    
    newalt->strfills = (String_fill **) NULL;
    newalt->instptrs = (Pointer *) NULL;
    newalt->num_strs = 0;
    newalt->num_ptrs = 0;
    newalt->next = (Alt_fill_set *)NULL;
    
    return(newalt);
}


/************************************************************************
 *									*
 *  alt_fill_set_release						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
alt_fill_set_release(char *elt)
#else
void
alt_fill_set_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Alt_fill_set *ptr = (Alt_fill_set *)elt;
    register int  i;
    int  num;

    if (ptr == (Alt_fill_set *) NULL)
	return;
    
    /* strfills is a (String_fill **), so we need to find out how many
       (String_fill *)'s have been allocated and free those first. */
    num = ptr->num_strs;
    for(i=0; i<num; i++)
	string_fill_release((char *)ptr->strfills[i]);
    UFREE(ptr->strfills);

    pointer_release((char *)ptr->instptrs);
    
    /* recursive call will release the tail of this linked list first */
    alt_fill_set_release((char *)ptr->next);

    UFREE(ptr);
}


/************************************************************************
 *									*
 *  add_alt_string_fill							*
 *									*
 *  add a string fill to a list of alternate fills.			*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_alt_string_fill(Alt_fill_set *set, char *string)
#else
void
add_alt_string_fill(set, string)
Alt_fill_set *set;
char *string;
#endif	/* UsePrototypes */
{
    int num = set->num_strs;
    
    set->strfills = UREALLOC(set->strfills, String_fill *, num+1);
    set->strfills[num] = (String_fill *) string_fill_create(string);

    set->num_strs++;
}


/************************************************************************
 *									*
 *  add_alt_pointer_fill						*
 *									*
 *  add a pointer fill to a list of alternate fills.			*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_alt_pointer_fill(Alt_fill_set *set, Pointer *ptr)
#else
void
add_alt_pointer_fill(set, ptr)
Alt_fill_set *set;
Pointer *ptr;
#endif	/* UsePrototypes */
{
    Pointer  *lastptr;

    /* is this the first pointer? */
    if (set->instptrs == (Pointer *)NULL)
	set->instptrs = ptr;
    else
    {
	for(lastptr = set->instptrs;
	    lastptr->next;
	    lastptr = lastptr->next)
	    ;
	lastptr->next = ptr;
    }

    set->num_ptrs++;
}


/************************************************************************
 *									*
 *  print_alt_fill_set							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_alt_fill_set(Alt_fill_set *set, int setnum, int indent)
#else
void
print_alt_fill_set(set, setnum, indent)
Alt_fill_set *set;
int setnum;
int indent;
#endif	/* UsePrototypes */
{
    register int i;
    int num_strs;
    
    if (set == (Alt_fill_set *)NULL)
	return;
    
    printf("%.*salternate set %d:\n", indent, blanks, setnum);

    num_strs = set->num_strs;
    for (i=0; i<num_strs; i++)
	print_string_fill(set->strfills[i], indent+2);
    
    print_pointer(set->instptrs, indent+2);
    
    /* print the next set on the list */
    print_alt_fill_set(set->next, setnum+1, indent);
}
