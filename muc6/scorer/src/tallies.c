/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	tallies.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Tallies data structure.
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
static	char	SccsId[] = "@(#)tallies.c	1.2 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"


/************************************************************************
 *									*
 *  tallies_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Tallies *
tallies_create(void)
#else
Tallies *
tallies_create()
#endif	/* UsePrototypes */
{
    Tallies *newtallies;
    
    if((newtallies = UALLOC(Tallies, 1)) == (Tallies *)NULL)
    {
	fprintf(stderr, "Error: [tallies_create]: Allocation failed.\n");
	exit(1);
    }

    newtallies->possible = 0;
    newtallies->actual = 0;
    newtallies->correct = 0;
    newtallies->partial = 0;
    newtallies->incorrect = 0;
    newtallies->missing = 0;
    newtallies->spurious = 0;
    newtallies->noncommittal = 0;
    
    return(newtallies);
}


/************************************************************************
 *									*
 *  tallies_release							*
 *									*
 *  since there are no dynamically allocated blocks within the current	*
 *  structure definition, we could simply call UFREE on the tallies	*
 *  ptr instead of calling this routine.  however, in the event that	*
 *  the structure definition changes to include allocated memory, then	*
 *  it will be simpler to change the freeing process in this routine	*
 *  than changing all the instances of UFREE calls on this pointer.	*
 *									*
 ***********************************************************************/


#if UsePrototypes
void
tallies_release(char *elt)
#else
void
tallies_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Tallies *ptr = (Tallies *)elt;
    
    if(ptr == (Tallies *)NULL)
	return;

    UFREE(ptr);
}


/************************************************************************
 *									*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
tallies_copy(Tallies *t1, Tallies *t2)
#else
void
tallies_copy(t1, t2)
Tallies *t1;
Tallies *t2;
#endif	/* UsePrototypes */
{
    t1->possible = t2->possible;
    t1->actual = t2->actual;
    t1->correct = t2->correct;
    t1->partial = t2->partial;
    t1->incorrect = t2->incorrect;
    t1->missing = t2->missing;
    t1->spurious = t2->spurious;
    t1->noncommittal = t2->noncommittal;

    return;
}

#if UsePrototypes
void
tallies_combine(Tallies *t1, Tallies *t2)
#else
void
tallies_combine(t1, t2)
Tallies *t1;
Tallies *t2;
#endif	/* UsePrototypes */
{
    t1->possible += t2->possible;
    t1->actual += t2->actual;
    t1->correct += t2->correct;
    t1->partial += t2->partial;
    t1->incorrect += t2->incorrect;
    t1->missing += t2->missing;
    t1->spurious += t2->spurious;
    t1->noncommittal += t2->noncommittal;

    return;
}


#if UsePrototypes
void
tallies_subtract(Tallies *t1, Tallies *t2)
#else
void
tallies_subtract(t1, t2)
Tallies *t1;
Tallies *t2;
#endif	/* UsePrototypes */
{
    t1->possible -= t2->possible;
    t1->actual -= t2->actual;
    t1->correct -= t2->correct;
    t1->partial -= t2->partial;
    t1->incorrect -= t2->incorrect;
    t1->missing -= t2->missing;
    t1->spurious -= t2->spurious;
    t1->noncommittal -= t2->noncommittal;

    return;
}


#if UsePrototypes
void
tallies_clear(Tallies *t)
#else
void
tallies_clear(t)
Tallies *t;
#endif	/* UsePrototypes */
{
    t->possible = 0;
    t->actual = 0;
    t->correct = 0;
    t->partial = 0;
    t->incorrect = 0;
    t->missing = 0;
    t->spurious = 0;
    t->noncommittal = 0;

    return;
}
