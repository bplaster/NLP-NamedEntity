/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 * FILE 
 *	inst_pair.c
 *
 * SYNOPSIS
 * DESCRIPTION
 *	support routines for Inst_pair data structure.
 *
 * DIAGNOSTICS
 * FILES
 * NOTES
 *      consolidated Mapping_pair and Connecting_pair into Inst_pair
 *             -ad 11/05/96
 *
 * SEE ALSO
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)inst_pair.c	1.4 03 Sep 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"


/************************************************************************
 *									*
 *  inst_pair_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Inst_pair *
inst_pair_create(void)
#else
Inst_pair *
inst_pair_create()
#endif	/* UsePrototypes */
{
    Inst_pair *newpair;
    
    if((newpair = UALLOC(Inst_pair, 1)) == (Inst_pair *) NULL)
    {
	fprintf(stderr, "Error: allocation for inst_pair failed.\n");
	exit(1);
    }

    newpair->score = (float) 0;
    newpair->threshold_score = (float) 0;
    newpair->key = (Class_instance *) NULL;
    newpair->response = (Class_instance *) NULL;
    
    return(newpair);
}


/************************************************************************
 *									*
 *  inst_pair_release						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
inst_pair_release(char *elt)
#else
void
inst_pair_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Inst_pair *ptr = (Inst_pair *) elt;
    
    /* there really is nothing to free, since the inst pointers
       are still valid pointers */

    UFREE(ptr);

    return;
}


/************************************************************************
 *									*
 *  inst_pair_compare						*
 *									*
 *  used to compare inst_pair structures during sorting.		*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
inst_pair_compare(const void *elt1, const void *elt2)
#else
int
inst_pair_compare(elt1, elt2)
char *elt1;
char *elt2;
#endif	/* UsePrototypes */
{
    Inst_pair *ptr1 = *(Inst_pair **)elt1;
    Inst_pair *ptr2 = *(Inst_pair **)elt2;

    int dif;
    
    /* qsort will normally sort in ascending order.  however,
       higher f-scores indicate better mapping results, so an
       a descending order is preferred.  reversing the order
       of comparison is one way, but i will instead apply the
       unary minus to the result of normal comparison. */

    dif = (int) -(ptr1->score - ptr2->score);

    return(dif);
}


/************************************************************************
 *									*
 *  add_inst_pair_to_all						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_inst_pair_to_all(Inst_pairings *pairings, Inst_pair *pair)
#else
void
add_inst_pair_to_all(pairings, pair)
Inst_pairings *pairings;
Inst_pair *pair;
#endif	/* UsePrototypes */
{
    int	num = pairings->num_all;
    
    pairings->all = UREALLOC(pairings->all,
			     Inst_pair *,
			     num+1);
    
    pairings->all[num] = (Inst_pair *) pair;

    pairings->num_all++;
    
    return;
}


/************************************************************************
 *									*
 *  add_inst_pair_to_final						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_inst_pair_to_final(Inst_pairings *pairings, Inst_pair *pair)
#else
void
add_inst_pair_to_final(pairings, pair)
Inst_pairings *pairings;
Inst_pair *pair;
#endif	/* UsePrototypes */
{
    int num = pairings->num_final;
    
    pairings->final = UREALLOC(pairings->final,
			       Inst_pair *,
			       num+1);
    
    pairings->final[num] = (Inst_pair *) pair;
    
    pairings->num_final++;
    
    return;
}



/************************************************************************
 *									*
 *  print_inst_pair							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_inst_pair(Inst_pair *pair)
#else
void
print_inst_pair(pair)
Inst_pair *pair;
#endif	/* UsePrototypes */
{
    printf("%.2f, key %s-%s, response %s-%s\n",
	   pair->score,
	   pair->key->docnum,
	   pair->key->id,
	   pair->response->docnum,
	   pair->response->id);
}
