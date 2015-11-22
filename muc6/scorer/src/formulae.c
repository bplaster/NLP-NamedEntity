/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	formulae.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	functions to compute f, error, overgeneration, 
 *      undergeneration, precision, recall, substitution
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *	
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)formulae.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"


/************************************************************************
 *									*
 *  compute_tallies_f							*
 *									*
 *  compute an f-measure based on the scoring information in tallies.	*
 *  recall = (#correct + (0.5 * #partial)) / #possible			*
 *  precision = (#actual + (0.5 * #partial) / #actual			*
 *  return -1.0 if either #pos or #act = 0				*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_tallies_f(Tallies *tallies, int which_f)
#else
float
compute_tallies_f(tallies, which_f)
Tallies *tallies;
int which_f;
#endif	/* UsePrototypes */
{
    static float f_pr = 0.5;
    static float f_2pr = 0.8;
    static float f_p2r = 0.2;
    
    float fpos, fact, fcor, fpar, frecall, fprecision, beta, result;
    
    fpos = (float) tallies->possible;
    fact = (float) tallies->actual;
    fcor = (float) tallies->correct;
    fpar = (float) tallies->partial;
    
    switch(which_f)
    {
    case F_PR:
	beta = f_pr;
	break;
    case F_2PR:
	beta = f_2pr;
	break;
    case F_P2R:
	beta = f_p2r;
	break;
    default:
	beta = f_pr;
    }

    /* get recall and precision percentages before calling
       compute_f to get result.  if either fpos or fact = 0.0,
       then this falls out, and the return value is -1.0 */
    if ((fpos > 0.0) && (fact > 0.0))
    {
	frecall = compute_recall(tallies);
	fprecision = compute_precision(tallies);

	result = compute_f(beta, fprecision, frecall);
	return(result);
    }
    else
	return(-1.0);
}


/************************************************************************
 *									*
 *  compute_f								*
 *									*
 *  = (	1.0 / ( (b * (1.0 / p)) +					*
 *		((1.0 - b) * (1.0 / r)) ) )				*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_f(float beta, float precision, float recall)
#else
float
compute_f(beta, precision, recall)
float beta;
float precision;
float recall;
#endif	/* UsePrototypes */
{
    float fval = 0.0;

    if ((precision == 0.0) || (recall == 0.0))
	return(fval);
    
    fval = (1.0 / ((beta * (1.0 / precision)) +
		   ((1.0 - beta) * (1.0 / recall))));
    
    return(fval);
}


/************************************************************************
 *									*
 *  compute_error_per_response_fill					*
 *									*
 *  = ( ((partial / 2) + incorrect + missing + spurious)		*
 *      / (correct + partial + incorrect + missing + spurious))		*
 *    * 100.0								*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_error_per_response_fill(Tallies *tallies)
#else
float
compute_error_per_response_fill(tallies)
Tallies *tallies;
#endif	/* UsePrototypes */
{
    float correct = (float) tallies->correct;
    float partial = (float) tallies->partial;
    float incorrect = (float) tallies->incorrect;
    float missing = (float) tallies->missing;
    float spurious = (float) tallies->spurious;
    float num, denom, errprf;
    
    num = (partial / 2.0) + incorrect + missing + spurious;
    denom = correct + partial + incorrect + missing + spurious;
    
    if (denom == 0.0)
	errprf = -1.0;
    else
	errprf = (num / denom) * 100.0;
    
    return(errprf);
}


/************************************************************************
 *									*
 *  compute_overgeneration						*
 *									*
 *  = (spurious / actual) * 100.0					*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_overgeneration(Tallies *tallies)
#else
float
compute_overgeneration(tallies)
Tallies *tallies;
#endif	/* UsePrototypes */
{
    float actual = (float) tallies->actual;
    float spurious = (float) tallies->spurious;
    float overgeneration;
    
    if (actual == 0.0)
	overgeneration = -1.0;
    else
	overgeneration = (spurious / actual) * 100.0;
    
    return(overgeneration);
}


/************************************************************************
 *									*
 *  compute_precision							*
 *									*
 *  = ((correct + (partial / 2)) / actual) * 100			*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_precision(Tallies *tallies)
#else
float
compute_precision(tallies)
Tallies *tallies;
#endif	/* UsePrototypes */
{
    float actual = (float) tallies->actual;
    float correct = (float) tallies->correct;
    float partial = (float) tallies->partial;
    float precision;
    
    if (actual == 0.0)
	precision = -1.0;
    else
	precision = ((correct + (partial / 2.0)) / actual) * 100.0;
    
    return(precision);
}


/************************************************************************
 *									*
 *  compute_recall							*
 *									*
 *  = ((correct + (partial / 2)) / possible) * 100			*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_recall(Tallies *tallies)
#else
float
compute_recall(tallies)
Tallies *tallies;
#endif	/* UsePrototypes */
{
    float possible = (float) tallies->possible;
    float correct = (float) tallies->correct;
    float partial = (float) tallies->partial;
    float recall;
    
    if (possible == 0.0)
	recall = -1.0;
    else
	recall = ((correct + (float)(partial / 2.0)) / possible) * 100.0;
    
    return(recall);
}


/************************************************************************
 *									*
 *  compute_substitution						*
 *									*
 *  = ( (incorrect + (partial / 2))					*
 *      / (correct + partial + incorrect))				*
 *    * 100.0								*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_substitution(Tallies *tallies)
#else
float
compute_substitution(tallies)
Tallies *tallies;
#endif	/* UsePrototypes */
{
    float correct = (float) tallies->correct;
    float partial = (float) tallies->partial;
    float incorrect = (float) tallies->incorrect;
    float denom, substitution;
    
    denom = correct + partial + incorrect;
    
    if (denom == 0.0)
	substitution = -1.0;
    else
	substitution = ((incorrect + (partial / 2.0)) / denom) * 100.0;
    
    return(substitution);
}


/************************************************************************
 *									*
 *  compute_undergeneration						*
 *									*
 *  = (missing / possible) * 100.0					*
 *									*
 ***********************************************************************/

#if UsePrototypes
float
compute_undergeneration(Tallies *tallies)
#else
float
compute_undergeneration(tallies)
Tallies *tallies;
#endif	/* UsePrototypes */
{
    float possible = (float) tallies->possible;
    float missing = (float) tallies->missing;
    float undergeneration;
    
    if (possible == 0.0)
	undergeneration = -1.0;
    else
	undergeneration = (missing / possible) * 100.0;
    
    return(undergeneration);
}
