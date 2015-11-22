/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 *  muc.h include file.
 *
 *  Provide data structures and function definitions/prototypes.
 *
 *  SccsId:  @(#)muc.h	1.8  18 Jan 1996
 */


#ifndef _MUC_H_
#define _MUC_H_  1

/*
 *  Preliminary Data Structures for MUC6.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cdefs.h"
#include "muc_defs.h"
#include "table.h"


/************************************************************************
 *									*
 *  Scoring structure used for tallying all the scoring categories.	*
 *									*
 ***********************************************************************/

typedef struct tallies
{
    int    possible;
    int    actual;
    int    correct;
    int    partial;
    int    incorrect;
    int    missing;
    int    spurious;
    int    noncommittal;
} Tallies;



/************************************************************************
 *									*
 *  MUC structure declarations						*
 *									*
 ***********************************************************************/

/************************************************************************
 *									*
 *  Structures for general class and slot definitions.  Class and slot	*
 *  definitions will be parsed from the slot-configuration file.	*
 *  Each class_def will be defined with its own set of slot_defs.	*
 *									*
 ***********************************************************************/

typedef struct class_def
{
    char	    *name;
    char	    *pattern;		/* the string pattern as it appears
					   in the document or template file */
    int		    map_threshold;	/* default to 0 */
    Bool	    is_scored_class;	/* are instances of this class type
					   to be scored?  (true or false) */
    struct tallies  *total_tallies;	/* accumulated scores for all
					   instantiations of this class
					   type */
    TABLE	    *slot_defs;		/* a table of slot definitions */
} Class_def;


typedef struct slot_def
{
    char          *name;
    char	  *of_class;		/* which class is this a slot of? */
    char          *pattern;		/* the string pattern as it appears
					   in the document or template file */
    char          **fill_types;		/* if this slot has alternate
					   fill_types, then realloc a (char *)
					   for each additional type.
					   also, set the has_multi_fill_types
					   field */
    int		  num_fill_types;	/* how many fill_types are there? */
    Bool	  has_multi_fill_types;
    int           map_weight;		/* default to 0 */
    Bool	  is_scored_slot;	/* are instances of this slot type
					   to be scored?  (true or false) */
    struct tallies  *total_tallies;	/* accumulated scores for all
					   instantiations of this slot
					   type (for a given class type) */
} Slot_def;


/************************************************************************
 *									*
 *  catalog_entry - a structural representation of a sectionized	*
 *		    document:						*
 *									*
 *	- text_doc, k_doc, r_doc point to the input files, with		*
 *	  beginning and ending file positions for this particular	*
 *	  document.							*
 *	- k_root, r_root point to the root instantiation of the input	*
 *	  data.								*
 *	- class_mappings_list is a table of class_mappings.		*
 *	- class_connections_list is a table of class_connections.	*
 *									*
 ***********************************************************************/

typedef struct catalog_entry
{
    char		 *docnum;
    struct file_section  *text_doc;	   /* text document */
    struct file_section  *k_doc;	   /* key document */
    struct file_section  *r_doc;	   /* response document */
    struct root		 *k_root;	   /* internal key data */
    struct root		 *r_root;	   /* internal response data */
    TABLE		 *class_mappings_list;  /* mapping results */
    TABLE		 *class_connections_list;  /* connection results */
    TABLE		 *class_scores;	   /* overall scoring data for
					      this entry */
    struct tallies	 *relevance;
} Catalog_entry;

/*
 *  file_section
 */

typedef struct file_section
{
    char   *file_name;
    long   begin;		/* beginning file position of document. */
    long   end;			/* ending file position of document. */
} File_section;

/************************************************************************
 *									*
 *  class_score - scoring data for all instances of this class type
 *		  in one document.  points to a table of slot_scores.
 *									*
 ***********************************************************************/

typedef struct class_score
{
    char	     *name;
    char	     *docnum;
    Bool	     has_instances;   /* no need to score this class type
					 if there are no instantiations */
    struct tallies   *tallies;
    TABLE	     *slot_scores;    /* a table containing the scoring
					 data for the slots of this class,
					 for this document */
} Class_score;

/************************************************************************
 *									*
 *  slot_score - scoring data for one slot of a given class, for all	*
 *		 the instances of that class type in one document.	*
 *									*
 ***********************************************************************/

typedef struct slot_score
{
    char	     *name;
    char	     *docnum;
    struct tallies   *tallies;
} Slot_score;

    
/*  structure declarations for instances.  */

typedef struct root
{
    char		*docnum;
    TABLE		*class_instance_lists;
} Root;

/************************************************************************
 *									*
 *  a class_instance_list contains a pointer to a table of instantiated	*
 *  objects of that class type, with each instance being of the same	*
 *  docnum, but having a unique id (1up_number).			*
 *									*
 ***********************************************************************/

typedef struct class_instance_list
{
    char	*name;
    char	*docnum;
    TABLE	*instances;	/* a list of class instantiations
				   for this document */
} Class_instance_list;


/************************************************************************
 *									*
 *  Class_instances are also sometimes known as objects.  Each is a	*
 *  singular instantiation of a defined class type.  Each instance	*
 *  contains a pointer to a table of slot_fill structures.		*
 *									*
 ***********************************************************************/

typedef struct class_instance
{
    char		   *name;
    char		   *docnum;
    char		   *id;
    TABLE		   *slot_fills;	 /* a list of instantiated slots */
    struct class_instance  *mapped_to;	 /* a pointer to the class_instance
					    that this instance is mapped
					    to */
    struct class_instance  *connected_to; /* if not mapped, then this is a
					     pointer to the class_instance
					     that this instance is
					     connected to */
    /* for in_and_out instances -
       mappings between instances may be affected if there are identical
       keys, in which case the 'wrong' key may be mapped to the 'wrong'
       response.  set this pointer to the identical in_and_out instance,
       so that the parent succession_event can determine which of the
       two (or more?) instances yields the better score. */
    struct pointer  *ident_in_and_out_ptrs;

    /* set the following two booleans when this instance is created
       as a pointer or as a 'true' object instantiation. */
    Bool		   is_instantiated;
    Bool		   is_pointed_to;

    struct pointer	   *back_pointers;  /* a list of instances that
					       point to this instance */
    int			   num_backptrs;

    /* for named_entity and coreference tasks */
    Bool		   is_optional;
    int			   start_offset;
    int			   end_offset;
    int			   doc_section;
    int                    save_headline;
    int                    save_text;

} Class_instance;


/************************************************************************
 *									*
 *  Each slot_fill contains pointers to arrays of string fill		*
 *  structures, which are the extracted data from the documents.	*
 *  It could also have a list of instance pointers.			*
 *									*
 ***********************************************************************/

typedef struct slot_fill
{
    char		   *name;           /* name of the slot */
    Bool 		   is_optional;	    /* true or false */
    struct string_fill	   **strfills;	    /* array of string fills */
    struct pointer         *instptrs;	    /* instance pointers */
    int			   num_strs;	    /* how many string fills */
    int			   num_ptrs;	    /* how many pointers */
    struct alt_fill_set	   *alt_fill_sets;  /* linked_list of alternate
					       fill sets */
    struct string_fill	   *min_string;	    /* minimum string fill */
} Slot_fill;

typedef struct alt_fill_set
{
    struct string_fill	   **strfills;	 /* array of string fills */
    struct pointer         *instptrs;	 /* instance pointers */
    int			   num_strs;	 /* how many string fills */
    int			   num_ptrs;	 /* how many pointers */
    struct alt_fill_set	   *next;	 /* the next set */
} Alt_fill_set;

typedef struct string_fill
{
    char	*fill;		/* text */
    int		match_status;	/* matched?, partial?, unmatched? */
} String_fill;

typedef struct pointer
{
    Class_instance	*inst;	/* the instance that this points to */
    struct pointer	*next;	/* the next pointer in the list */
} Pointer;


/************************************************************************
 *									*
 *  data structures for configuration options				*
 *									*
 ***********************************************************************/

/* subset scores not yet supported */
typedef struct muc_option
{
    char    *name;
    int	    type;
    char    *value;
    char    *default_value;
    char    **value_list;
    int	    num_vals;
} Option;


/************************************************************************
 *									*
 *  inst_pairings							*
 *									*
 *  this structure contains listings of pairs of                        *
 *  instances of the same class.  **all is a list of all results	* 
 *  from comparing all the keys to all the responses.  **final is a     *
 *  sublist of **all, corresponding to a bipartite "matching" of        *
 *  keys with responses.                                                *
 *									*
 ***********************************************************************/

typedef struct inst_pairings
{
    char		 *name;          /* name of the class */
    struct inst_pair	 **all;
    struct inst_pair	 **final;
    int			 num_all;
    int			 num_final;
} Inst_pairings;



/************************************************************************
 *									*
 *  inst_pair 
 *
 *  One pair of instances, together with some scoring information
 *									*
 ***********************************************************************/

typedef struct inst_pair
{
    float	    score;	       /* end result of this comparison */
    float	    threshold_score;   /* initial result of comparison */
    Class_instance  *key;
    Class_instance  *response;
} Inst_pair;


#endif   /* _MUC_H_ */
