/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	root.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Root data structure.
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
static	char	SccsId[] = "@(#)root.c	1.2 21 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  root_create								*
 *									*
 ***********************************************************************/

#if UsePrototypes
Root *
root_create(char *docnum)
#else
Root *
root_create(docnum)
char *docnum;
#endif	/* UsePrototypes */
{
    Root *newroot;
    
    if((newroot = UALLOC(Root, 1)) == (Root *)NULL)
    {
	fprintf(stderr, "Error: [root_create]: Allocation failed.\n");
	exit(1);
    }
    
    newroot->docnum = STRALLOC(docnum);
    
    /* create an empty table of class_instance_lists */
    newroot->class_instance_lists = table_list_create(class_instance_list_compare,
						     class_instance_list_release,
						     default_destroy);

    if(newroot->class_instance_lists == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: [root_create]: ");
	fprintf(stderr, "Unable to create class_instance_lists table.\n");
	exit(1);
    }

    return(newroot);
}


/************************************************************************
 *									*
 *  root_release							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
root_release(char *elt)
#else
void
root_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Root *ptr = (Root *)elt;
    
    if(ptr == (Root *)NULL)
	return;
    
    UFREE(ptr->docnum);
    
    /* free up the table of classes (containing class_instances)
       belonging to this root. */
    table_destroy(ptr->class_instance_lists);

    UFREE(ptr);
}


/************************************************************************
 *									*
 *  print_root								*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_root(Root *root, int indent)
#else
void
print_root(root, indent)
Root *root;
int indent;
#endif	/* UsePrototypes */
{
    TABLE  *lists;
    Class_instance_list *list;
    
    if (root == (Root *) NULL)
	return;
    
    printf("%.*sroot %s, %d lists\n", indent, blanks, root->docnum,
	   table_count((TABLE *) root->class_instance_lists));

    lists = (TABLE *) root->class_instance_lists;
    
    table_first(lists);
    for(list = (Class_instance_list *)table_next(lists);
	list;
	list = (Class_instance_list *)table_next(lists))
	print_class_instance_list(list, indent+2);
}
