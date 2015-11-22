/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	pointer.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Pointer data structure.
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
static	char	SccsId[] = "@(#)pointer.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  pointer_create							*
 *									*
 ***********************************************************************/

/*  pointer_create */
#if UsePrototypes
Pointer *
pointer_create(Class_instance *inst)
#else
Pointer *
pointer_create(inst)
Class_instance *inst;
#endif	/* UsePrototypes */
{
    Pointer *newptr;
    
    if((newptr = UALLOC(Pointer, 1)) == (Pointer *) NULL)
    {
	fprintf(stderr, "Error: [pointer_create]: Allocation failed.\n");
	exit(1);
    }
    
    newptr->inst = inst;
    newptr->next = (Pointer *) NULL;

    return(newptr);
}


/************************************************************************
 *									*
 *  pointer_release							*
 *									*
 *  don't free the inst pointer, since it's still a valid 		*
 *  (Class_instance *)							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
pointer_release(char *elt)
#else
void
pointer_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Pointer *ptr = (Pointer *)elt;

    if (ptr == (Pointer *) NULL)
	return;

    ptr->inst = (Class_instance *) NULL;
    
    /* recursive call will release the tail of this linked list first */
    pointer_release((char *)ptr->next);

    ptr->next = (Pointer *) NULL;
    
    UFREE(ptr);

    return;
}


/************************************************************************
 *									*
 *  print_pointer							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_pointer(Pointer *ptr, int indent)
#else
void
print_pointer(ptr, indent)
Pointer *ptr;
int indent;
#endif	/* UsePrototypes */
{
    if (ptr == (Pointer *) NULL)
	return;
    
    printf("%.*s%s-%s-%s\n", indent, blanks, ptr->inst->name,
	   ptr->inst->docnum, ptr->inst->id);
    
    print_pointer(ptr->next, indent);

    return;
}
