/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	catalog_entry.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Catalog_entry data structure.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
	Changed  class_connections and class_mappings functions
	     to  inst_pairings functions  -ad 12/06/96
 *
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)catalog_entry.c	1.3 03 Sep 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  catalog_entry_create						*
 *									*
 *  Initialize a new catalog_entry with DOCNUM as key, and search	*
 *  the Catalog table for a matching entry.  If it exists, return	*
 *  the existing entry, else insert the new entry into the table,	*
 *  and return the new entry.						*
 *									*
 ***********************************************************************/

#if UsePrototypes
Catalog_entry *
catalog_entry_create(char *docnum)
#else
Catalog_entry *
catalog_entry_create(docnum)
char *docnum;
#endif	/* UsePrototypes */
{
    TABLE *Catalog = (TABLE *) MUC_Catalog;
    Catalog_entry *newentry, *oldentry;
    
    if((newentry = UALLOC(Catalog_entry, 1)) == (Catalog_entry *)NULL)
    {
	fprintf(stderr, "Error: [catalog_entry_create]: Allocation failed.\n");
	exit(1);
    }
    
    /* allocate a new string and copy the docnum string */
    newentry->docnum = STRALLOC(docnum);

    /* set NULL pointers to the unitialized fields */
    newentry->text_doc = (File_section *) NULL;
    newentry->k_doc = (File_section *) NULL;
    newentry->r_doc = (File_section *) NULL;
    newentry->k_root = (Root *) NULL;
    newentry->r_root = (Root *) NULL;

    /* create an empty table for class mappings, then build it
       by calling initialize_inst_pairings_list. */
    newentry->class_mappings_list =
	table_list_create(inst_pairings_compare,
			  inst_pairings_release,
			  default_destroy);

    if(newentry->class_mappings_list == (TABLE *)NULL) {
    fprintf(stderr, "Error: [catalog_entry_create]: ");
    fprintf(stderr, "Unable to create class_mappings_list table.\n");
    exit(1);
    }

    initialize_inst_pairings_list(newentry->class_mappings_list);

    /* create an empty table for class connections, then build it
       by calling initialize_inst_pairings_list. */
    newentry->class_connections_list =
	table_list_create(inst_pairings_compare,
			  inst_pairings_release,
			  default_destroy);

    if(newentry->class_connections_list == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: [catalog_entry_create]: ");
	fprintf(stderr, "Unable to create class_connections_list table.\n");
	exit(1);
    }

    initialize_inst_pairings_list(newentry->class_connections_list);

    /* create an empty table for class_scores, then build it
       by calling initialize_class_scores_list. */
    newentry->class_scores = table_list_create(class_score_compare,
					       class_score_release,
					       default_destroy);

    if(newentry->class_scores == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: [catalog_entry_create]: ");
	fprintf(stderr, "Unable to create class_scores table.\n");
	exit(1);
    }

    initialize_class_scores(newentry->class_scores, docnum);
    
    newentry->relevance = tallies_create();
	
    /* search for an existing entry.  if it exists, free up the new
       entry and re-set it to the existing one. */
    if (oldentry = (Catalog_entry *)table_find((char *)newentry, Catalog))
    {
	catalog_entry_release((char *) newentry);
	newentry = oldentry;
    }
    else   /* no existing entry */
    {
	/* insert the new entry into the table */
	if(table_append((char *) newentry, (TABLE *) Catalog) == TABLE_ERR)
	{
	    fprintf(stderr, "Error: [catalog_entry_create]: ");
	    fprintf(stderr, "table insertion failed.\n");
	    exit(1);
	}
    }

    return(newentry);
}


/************************************************************************
 *									*
 *  catalog_entry_release						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
catalog_entry_release(char *elt)
#else
void
catalog_entry_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Catalog_entry *ptr = (Catalog_entry *)elt;
    
    if(ptr == (Catalog_entry *)NULL)
	return;

    UFREE(ptr->docnum);
    file_section_release((char *)ptr->text_doc);
    file_section_release((char *)ptr->k_doc);
    file_section_release((char *)ptr->r_doc);
    root_release((char *)ptr->k_root);
    root_release((char *)ptr->r_root);

    /* free up the tables of mappings, connections, and class_scores */
    table_destroy(ptr->class_mappings_list);
    table_destroy(ptr->class_connections_list);
    table_destroy(ptr->class_scores);

    tallies_release((char *)ptr->relevance);
    
    UFREE(ptr);
}


/************************************************************************
 *									*
 *  catalog_entry_compare						*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
catalog_entry_compare(char *elt1, char *elt2)
#else
int
catalog_entry_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Catalog_entry *ptr1 = (Catalog_entry *)elt1;
    Catalog_entry *ptr2 = (Catalog_entry *)elt2;

    return(strcasecmp(ptr1->docnum, ptr2->docnum));
}


/************************************************************************
 *									*
 *  print_muc_catalog							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_muc_catalog(void)
#else
void
print_muc_catalog()
#endif
{
    TABLE *Catalog = (TABLE *) MUC_Catalog;
    Catalog_entry *entry;
    
    printf("\nList of Catalog entries:\n");
    
    table_first(Catalog);
    for(entry = (Catalog_entry *)table_next(Catalog);
	entry;
	entry = (Catalog_entry *)table_next(Catalog))
	print_catalog_entry(entry, 2);
}
/***********************************************************************
 *									*
 *  print_catalog_entry							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_catalog_entry(Catalog_entry *entry, int indent)
#else
void
print_catalog_entry(entry, indent)
Catalog_entry *entry;
int indent;
#endif
{
    Inst_pairings *pairings;
    TABLE *pairings_list;

    printf("%.*sCatalog Entry: %s:\n", indent, blanks, entry->docnum);

    print_file_section(entry->text_doc, indent+2);

    print_file_section(entry->k_doc, indent+2);
    print_file_section(entry->r_doc, indent+2);

    print_root(entry->k_root, indent+2);
    print_root(entry->r_root, indent+2);

    printf("%.*sMapped:\n", indent, blanks );
    pairings_list = entry->class_mappings_list;
    table_first(pairings_list);
    for(pairings = (Inst_pairings *)table_next(pairings_list);
	pairings;
	pairings = (Inst_pairings *)table_next(pairings_list))
      print_inst_pairings(pairings);

    printf("%.*sConnected:\n", indent, blanks );
    pairings_list = entry->class_connections_list;
    table_first(pairings_list);
    for(pairings = (Inst_pairings *)table_next(pairings_list);
	pairings;
	pairings = (Inst_pairings *)table_next(pairings_list))
      print_inst_pairings(pairings);
}

