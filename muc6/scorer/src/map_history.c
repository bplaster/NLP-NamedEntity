/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	map_history.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	routines to write the map history and the connect history.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES Changed Mapping_pair and supporting funtions to Inst_pair 
 * -ad 12/06/96
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 * */

#ifndef	lint
static	char	SccsId[] = "@(#)map_history.c	1.2  25 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include <stdio.h>
#include "table.h"
#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"


/*  internal function prototypes */

Proto	(static	void, display_entry_final_mappings, (Catalog_entry *entry,
						     int indent));
Proto	(static	void, display_mapping_pair, (Inst_pair *pair,
					     int indent));
Proto	(static	void, open_muc_history_file, (char *option_name));
Proto	(static	void, close_muc_history_file, (void));


/*  local globals */

static FILE  *muc_history_file;


/************************************************************************
 *									*
 *  display_MUC_map_history						*
 *									*
 *  display the mapping history for all documents and templates.	*
 *  calls display_catalog_entry_mappings.				*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
display_MUC_map_history(void)
#else
void
display_MUC_map_history()
#endif	/* UsePrototypes */
{
    TABLE  *Catalog = (TABLE *)MUC_Catalog;
    Catalog_entry *entry;
    int indent = 0;

    printf("Writing Map History.\n");

    open_muc_history_file(":map_history_file");

    fprintf(muc_history_file,
	    "Mapping history for configuration '%s'\n",
	    MUC_configuration_filename);

    fprintf(muc_history_file,
	    "%.*skey%.*sresponse%.*s(f_score)\n",
	    indent+6, blanks,
	    17, blanks,
	    8, blanks);

    /* iterate over the list of entries */
    table_first(Catalog);
    for (entry = (Catalog_entry *)table_next(Catalog);
	 entry;
	 entry = (Catalog_entry *)table_next(Catalog))
	display_entry_final_mappings(entry, indent+2);
    
    close_muc_history_file();

    printf("Map History Completed.\n");
    return;
}


/************************************************************************
 *									*
 *  display_entry_final_mappings					*
 *									*
 *  for every entry, print out the final instance mappings for		*
 *  each class.								*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
display_entry_final_mappings(Catalog_entry *entry, int indent)
#else
static
void
display_entry_final_mappings(entry, indent)
Catalog_entry *entry;
int indent;
#endif	/* UsePrototypes */
{
    register int i;
    TABLE  *class_map_list = (TABLE *) entry->class_mappings_list;
    Inst_pairings  *mappings;
    int num;

    fprintf(muc_history_file, "\n%.*s%s:\n", indent, blanks, entry->docnum);
    
    table_first(class_map_list);
    for (mappings = (Inst_pairings *) table_next(class_map_list);
	 mappings;
	 mappings = (Inst_pairings *) table_next(class_map_list))
    {
	int num = mappings->num_final;

	if (num > 0)
	{
	    fprintf(muc_history_file,
		    "\n%.*s%s:\n",
		    indent+2, blanks,
		    mappings->name);
	    for (i=0; i<num; i++)
		display_mapping_pair(mappings->final[i], indent+4);
	}
    }
    
}


/************************************************************************
 *									*
 *  display_mapping_pair						*
 *									*
 *  for each mapped pair, display the docnum and id for both the	*
 *  key and the response instance, as well as the mapping f-score	*
 *  for the pair.							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
display_mapping_pair(Inst_pair *pair, int indent)
#else
static
void
display_mapping_pair(pair, indent)
Inst_pair *pair;
int indent;
#endif	/* UsePrototypes */
{
    fprintf(muc_history_file,
	    "%.*s%s-%-4s<--> %s-%-5s(%.2f)\n",
	    indent, blanks,
	    pair->key->docnum,
	    pair->key->id,
	    pair->response->docnum,
	    pair->response->id,
	    pair->score);
}


/************************************************************************
 *									*
 *  open_muc_history_file, close_muc_history_file			*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
open_muc_history_file(char *option_name)
#else
static
void
open_muc_history_file(option_name)
char *option_name;
#endif	/* UsePrototypes */
{
    FILE	*fopen();
    Option	*basediropt, *histfileopt, *respfileopt;
    char	*basedir, *histfname, *respfname, *displayfname;
    
    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *) NULL)
	basedir = basediropt->default_value;

    histfileopt = find_muc_option(option_name);
    if ((histfname = histfileopt->value) == (char *) NULL)
    {
	/* the default file name is a concatenation of the name of
	   the response file and the default history_file value */
	histfname = histfileopt->default_value;

	respfileopt = find_muc_option(":response_file");
	if ((respfname = respfileopt->value) == (char *) NULL)
	    respfname = respfileopt->default_value;
	
	displayfname = (char *) malloc (strlen(basedir)
					+ strlen(respfname)
					+ strlen(histfname)
					+ 1);
	displayfname = strcat(strcat(strcpy(displayfname, basedir),
				     respfname),
			      histfname);
    }
    else
    {
	displayfname = (char *) malloc (strlen(basedir)
					+ strlen(histfname)
					+ 1);
	displayfname = strcat(strcpy(displayfname, basedir), histfname);
    }
    
    if ((muc_history_file = fopen(displayfname, "w")) == (FILE *) NULL)
    {
	fprintf(stderr, "Could not open file '%s' to print history. \n",
		displayfname);
	exit(1);
    }

    UFREE(displayfname);
    
    return;
}

    
#if UsePrototypes
static
void
close_muc_history_file(void)
#else
static
void
close_muc_history_file()
#endif	/* UsePrototypes */
{
    fclose(muc_history_file);
}
