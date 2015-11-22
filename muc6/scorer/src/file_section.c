/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	file_section.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for File_section data structure.
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
static	char	SccsId[] = "@(#)file_section.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  file_section_create							*
 *									*
 ***********************************************************************/

#if UsePrototypes
File_section *
file_section_create(char *fname)
#else
File_section *
file_section_create(fname)
char *fname;
#endif	/* UsePrototypes */
{
    File_section *newsection;

    if((newsection = UALLOC(File_section, 1)) == (File_section *)NULL)
    {
	fprintf(stderr, "Error: [file_section_create]: Allocation failed.\n");
	exit(1);
    }

    newsection->file_name = STRALLOC(fname);
    newsection->begin = (long) 0;
    newsection->end = (long) 0;
    
    return(newsection);
}


/************************************************************************
 *									*
 *  file_section_release						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
file_section_release(char *elt)
#else
void
file_section_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    File_section *ptr = (File_section *)elt;

    if(ptr == (File_section *)NULL)
	return;
    
    UFREE(ptr->file_name);

    UFREE(ptr);
}


/************************************************************************
 *									*
 *  print_file_section							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_file_section(File_section *section, int indent)
#else
void
print_file_section(section, indent)
File_section *section;
int indent;
#endif	/* UsePrototypes */
{
    if (section == (File_section *) NULL)
	return;
    
    printf("%.*sfile = %s\n", indent, blanks, section->file_name);
    printf("%.*sbegin = %ld\n", indent, blanks, section->begin);
    printf("%.*send = %ld\n", indent, blanks, section->end);
}
