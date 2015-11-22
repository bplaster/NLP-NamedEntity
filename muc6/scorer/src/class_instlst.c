/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	class_instlst.c
 *
 *	previous name - class_instance_list.c (exceeds 15-char ar limit)
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Class_instance_list data structure.
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
static	char	SccsId[] = "@(#)class_instlst.c	1.2 03 Sep 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  class_instance_list_create						*
 *									*
 ***********************************************************************/

#if UsePrototypes
Class_instance_list *
class_instance_list_create(TABLE *lists, char *name, char *docnum)
#else
Class_instance_list *
class_instance_list_create(lists, name, docnum)
TABLE  *lists;
char  *name;
char  *docnum;
#endif	/* UsePrototypes */
{
    Class_instance_list *newlist, *oldlist;
    
    if((newlist = UALLOC(Class_instance_list, 1)) == (Class_instance_list *)NULL)
    {
	fprintf(stderr, "Error: [class_instance_list_create]: ");
	fprintf(stderr, "Allocation failed.\n");
	exit(1);
    }
    
    newlist->name = STRALLOC(name);
    newlist->docnum = STRALLOC(docnum);

    /* create an empty table of class_instances */
    newlist->instances = table_list_create(class_instance_compare,
					   class_instance_release,
					   default_destroy);
    
    if(newlist->instances == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: [class_instance_list_create]: ");
	fprintf(stderr, "Unable to create instances table.\n");
	exit(1);
    }

    /* search for an existing list.  if it exists, free up the new
       list and re-set it to the existing one. */
    if (oldlist =
	(Class_instance_list *)table_find((char *)newlist, lists))
    {
	class_instance_list_release((char *) newlist);
	newlist = oldlist;
    }
    else	/* no existing list */
    {
	/* insert the new list into the table */
	if(table_append((char *) newlist, (TABLE *) lists) == TABLE_ERR)
	{
	    fprintf(stderr, "Error: [class_instance_list_create]: ");
	    fprintf(stderr, "table insertion failed.\n");
	    exit(1);
	}
    }
    
    return(newlist);
}


/************************************************************************
 *									*
 *  class_instance_list_release						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
class_instance_list_release(char *elt)
#else
void
class_instance_list_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Class_instance_list *ptr = (Class_instance_list *)elt;
    
    if(ptr == (Class_instance_list *)NULL)
	return;
    
    UFREE(ptr->name);
    UFREE(ptr->docnum);
    
    /* free up the table of class instantiations */
    table_destroy(ptr->instances);
    
    UFREE(ptr);
}


/************************************************************************
 *									*
 *  class_instance_list_compare						*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
class_instance_list_compare(char *elt1, char *elt2)
#else
int
class_instance_list_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Class_instance_list *ptr1 = (Class_instance_list *)elt1;
    Class_instance_list *ptr2 = (Class_instance_list *)elt2;
    
    return(strcasecmp(ptr1->name, ptr2->name));
}


/************************************************************************
 *									*
 *  find_class_instance_list						*
 *									*
 *  checks to see if there exists a class_instance_list by name		*
 *  of CNAME in lists.							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Class_instance_list *
find_class_instance_list(TABLE *lists, char *cname)
#else
Class_instance_list *
find_class_instance_list(lists, cname)
TABLE *lists;
char *cname;
#endif	/* UsePrototypes */
{
    Class_instance_list  *thislist;
    
    /* search the table for a class_instance_list having the same
       name as cname */

    table_first(lists);
    for(thislist = (Class_instance_list *)table_next(lists);
	thislist && !(STRCASEEQ(cname, thislist->name));
	thislist = (Class_instance_list *)table_next(lists))
	;

    return(thislist);
}


/************************************************************************
 *									*
 *  print_class_instance_list						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_class_instance_list(Class_instance_list *list, int indent)
#else
void
print_class_instance_list(list, indent)
Class_instance_list *list;
int indent;
#endif	/* UsePrototypes */
{
    TABLE	*instances;
    Class_instance  *inst;
    
    if (list == (Class_instance_list *) NULL)
	return;
    
    printf("%.*sclass %s instances (%s): (%d instances)\n",
	   indent, blanks, list->name, list->docnum,
	   table_count((TABLE *) list->instances));
    
    instances = (TABLE *) list->instances;
    
    table_first(instances);
    for(inst = (Class_instance *)table_next(instances);
	inst;
	inst = (Class_instance *)table_next(instances))
	print_class_instance(inst, indent+2);
}
