/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	option.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Option data structure.
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
static	char	SccsId[] = "@(#)option.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include "muc_globals.h"

/* internal function prototypes */

Proto	(static int, reset_muc_option, (Option *opt));
Proto	(static int, print_muc_option, (Option *opt));


/************************************************************************
 *									*
 *  find_muc_option							*
 *									*
 *  search the MUC_Options array for OPTNAME.				*
 *  give warning if it's not a recognized option.			*
 *									*
 ***********************************************************************/

#if UsePrototypes
Option *
find_muc_option(char *optname)
#else
Option *
find_muc_option(optname)
char *optname;
#endif	/* UsePrototypes */
{
    Option *option;
    register int i;
    
    for (i=0; i<Num_Options; i++)
    {
	option = &(MUC_Options[i]);
	if (STRCASEEQ(optname, option->name))
	    return(option);
    }
    
    /* if it gets this far, it means the search failed. */
    printf("Warning: unrecognized configuration option '%s'.\n", optname);
    return((Option *)NULL);
}


/************************************************************************
 *									*
 *  add_muc_option_value						*
 *									*
 *  add a value to a configuration option.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
add_muc_option_value(Option *opt, char *val)
#else
int
add_muc_option_value(opt, val)
Option *opt;
char *val;
#endif	/* UsePrototypes */
{
    int opt_type = opt->type;
    int num = opt->num_vals;

    switch(opt_type)
    {
    case OPTVAL:
    case TOGGLE:
    case SELECT:
	/* if this is an overwrite, then first free the existing value. */
	if(opt->value != (char *) NULL)
	    UFREE(opt->value);
	opt->value = STRALLOC(val);
	opt->num_vals = 1;
	return(0);
    case VAL_LIST:
	opt->value_list = UREALLOC(opt->value_list, char *, (num+1));
	opt->value_list[num] = STRALLOC(val);
	opt->num_vals++;
	return(0);
    case SLOTS:		/* scored subsets of slots and templates */
    case TEMPLATES:	/* are not yet supported except for :all */
	if(opt->value != (char *) NULL)
	    UFREE(opt->value);
	opt->value = STRALLOC(val);
	return(0);
    default:
	return(0);
    }
}


/************************************************************************
 *									*
 *  reset_muc_options							*
 *									*
 *  when re-initializing the scorer, re-set the options by		*
 *  deleting and freeing the option values.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
reset_muc_options(void)
#else
int
reset_muc_options()
#endif	/* UsePrototypes */
{
    register int i;
    
    for (i=0; i<Num_Options; i++)
	reset_muc_option(&(MUC_Options[i]));

    return(0);
}


#if UsePrototypes
static
int
reset_muc_option(Option *opt)
#else
static
int
reset_muc_option(opt)
Option *opt;
#endif	/* UsePrototypes */
{
    register int i;
    int num;

    UFREE(opt->value);
    
    num = opt->num_vals;
    for(i=0; i<num; i++)
	UFREE(opt->value_list[i]);
    UFREE(opt->value_list);
    opt->num_vals = 0;
    
    return(0);
}


/************************************************************************
 *									*
 *  print_muc_options							*
 *									*
 *  print out the contents of the muc configuration options		*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
print_muc_options(void)
#else
int
print_muc_options()
#endif	/* UsePrototypes */
{
    register int i;
    
    for(i=0; i<Num_Options; i++)
	print_muc_option(&(MUC_Options[i]));
    
    return(0);
}

#if UsePrototypes
static
int
print_muc_option(Option *opt)
#else
static
int
print_muc_option(opt)
Option *opt;
#endif	/* UsePrototypes */
{
    register int i;
    int num = opt->num_vals;
    
    printf(" %s:\n", opt->name);

    if(opt->type == VAL_LIST)
    {
	printf("   %d values\n", num);
	for(i=0; i<num; i++)
	    printf("     %s\n", opt->value_list[i]);
    }
    else
    {
	if (opt->value != (char *) NULL)
	    printf("     %s\n", opt->value);
	else
	    printf("   default = %s\n",
		   (opt->default_value ? opt->default_value
				       : "no default"));
    }
    
    return(0);
}
