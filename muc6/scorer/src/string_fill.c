/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	string_fill.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for String_fill data structure.
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
 *	Gary Dungca	October 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)string_fill.c	1.1 27 Oct 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  string_fill_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
String_fill *
string_fill_create(char *str)
#else
String_fill *
string_fill_create(str)
char *str;
#endif	/* UsePrototypes */
{
    String_fill *newfill;
    
    if((newfill = UALLOC(String_fill, 1)) == (String_fill *) NULL)
    {
	fprintf(stderr, "Error: [string_fill_create]: Allocation failed.\n");
	exit(1);
    }

    newfill->fill = STRALLOC(str);
    newfill->match_status = MISMATCH;
    
    return(newfill);
}


/************************************************************************
 *									*
 *  string_fill_release							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
string_fill_release(char *elt)
#else
void
string_fill_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    String_fill *ptr = (String_fill *) elt;
    register int  i;
    int  num;
    
    if(ptr == (String_fill *)NULL)
	return;

    /* free the pointer to the allocated text string */
    UFREE(ptr->fill);

    UFREE(ptr);
}


/************************************************************************
 *									*
 *  print_string_fill							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_string_fill(String_fill *sfill, int indent)
#else
void
print_string_fill(sfill, indent)
String_fill *sfill;
int indent;
#endif	/* UsePrototypes */
{
    printf("%.*s%s\n", indent+2, blanks, sfill->fill);
}
