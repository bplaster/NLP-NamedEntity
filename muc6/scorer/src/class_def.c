/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	class_def.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Class_def data structure.
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
static	char	SccsId[] = "@(#)class_def.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"

/************************************************************************
 *									*
 *  class_def_create							*
 *									*
 *  Initialize a new class_def with NAME as key, and search the		*
 *  Class_defs table for a matching class_def.  If it exists, give	*
 *  warning about a re-definition.  On the assumption that a		*
 *  re-definition is legal, free the existing class_def, then insert	*
 *  the new entry into the table.					*
 *  									*
 *  returns a pointer to a Class_def.					*
 *  									*
 ************************************************************************/

#if UsePrototypes
Class_def *
class_def_create(char *name)
#else
Class_def *
class_def_create(name)
char *name;
#endif	/* UsePrototypes */
{
    TABLE *Class_defs = (TABLE *) MUC_Class_defs;
    Class_def *newclass, *oldclass;
    
    if((newclass = UALLOC(Class_def, 1)) == (Class_def *)NULL)
    {
	fprintf(stderr,	"Error: [class_def_create]: ");
	fprintf(stderr, "Allocation failed for '%s'.\n", name);
	exit(1);
    }

    newclass->name = STRALLOC(name);
    newclass->pattern = (char *) NULL;
    newclass->map_threshold = 0;
    newclass->is_scored_class = False;
    newclass->total_tallies = tallies_create();

    /* create an empty table for slot_defs */
    newclass->slot_defs = table_list_create(slot_def_compare,
					    slot_def_release,
					    default_destroy);

    if(newclass->slot_defs == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: [class_def_create]: '%s' - ", name);
	fprintf(stderr, "Unable to create slot_defs table.\n");
	exit(1);
    }

    /* is this a redefinition? */
    if(oldclass = (Class_def *)table_find((char *)newclass, Class_defs))
    {
	fprintf(stderr, "Warning: [class_def_create]: ");
	fprintf(stderr, "Redefinition of '%s'.\n", name);
	table_delete((char *)oldclass, Class_defs);
    }

    /* insert into table */
    if(table_append((char *) newclass, (TABLE *) Class_defs) == TABLE_ERR)
    {
	fprintf(stderr, "Error: [class_def_create]: table insertion failed.\n");
	exit(1);
    }

    return(newclass);
}


/************************************************************************
 *									*
 *  class_def_release							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
class_def_release(char *elt)
#else
void
class_def_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Class_def *ptr = (Class_def *)elt;
    
    if(ptr == (Class_def *)NULL)
	return;

    UFREE(ptr->name);
    if(ptr->pattern != (char *) NULL)
	UFREE(ptr->pattern);
    if(ptr->total_tallies != (Tallies *) NULL)
	tallies_release((char *)ptr->total_tallies);
    if(ptr->slot_defs != (TABLE *) NULL)
	table_destroy(ptr->slot_defs);
    
    UFREE(ptr);
}


/************************************************************************
 *									*
 *  class_def_compare							*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
class_def_compare(char *elt1, char *elt2)
#else
int
class_def_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Class_def *ptr1 = (Class_def *)elt1;
    Class_def *ptr2 = (Class_def *)elt2;
    
    return(strcasecmp(ptr1->name, ptr2->name));
}


/************************************************************************
 *									*
 *  find_class_def_by_name						*
 *									*
 *  checks to see if there exists a Class_def by name of CNAME.		*
 *									*
 ***********************************************************************/

#if UsePrototypes
Class_def *
find_class_def_by_name(char *cname)
#else
Class_def *
find_class_def_by_name(cname)
char *cname;
#endif	/* UsePrototypes */
{
    TABLE  *Class_defs = (TABLE *) MUC_Class_defs;
    Class_def  *target = UALLOC(Class_def, 1);
    Class_def  *thisdef = (Class_def *) NULL;

    if(target == (Class_def *) NULL)
    {
	fprintf(stderr,	"Error: [find_class_def_by_name]: ");
	fprintf(stderr, "Allocation failed for '%s'.\n", cname);
	exit(1);
    }

    target->name = STRALLOC(cname);
    target->pattern = (char *) NULL;
    target->total_tallies = (Tallies *) NULL;
    target->slot_defs = (TABLE *) NULL;
    
    /* search the table for a class_def having the same name as cname.
       set thisdef to existing class_def, otherwise NULL */

    thisdef = (Class_def *)table_find((char *)target, Class_defs);

    class_def_release((char *) target);
    
    return(thisdef);
}


/************************************************************************
 *									*
 *  find_class_def_by_pattern						*
 *									*
 *  checks to see if there exists a Class_def with a string pattern	*
 *  equal to CPATTERN.							*
 *									*
 ***********************************************************************/

#if UsePrototypes
Class_def *
find_class_def_by_pattern(char *cpattern)
#else
Class_def *
find_class_def_by_pattern(cpattern)
char *cpattern;
#endif	/* UsePrototypes */
{
    TABLE  *Class_defs = (TABLE *) MUC_Class_defs;
    Class_def  *thisdef;
    
    /* search the table for a class_def having the same
       string pattern as cpattern */

    table_first(Class_defs);
    for(thisdef = (Class_def *)table_next(Class_defs);
	thisdef && !(STRCASEEQ(cpattern, thisdef->pattern));
	thisdef = (Class_def *)table_next(Class_defs))
	;
    
    return(thisdef);
}


/************************************************************************
 *									*
 *  print_muc_class_defs						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_muc_class_defs(void)
#else
void
print_muc_class_defs()
#endif
{
    TABLE *Class_defs = (TABLE *) MUC_Class_defs;
    Class_def *cd;
    
    printf("\nList of Class Definitions:\n");
    
    table_first(Class_defs);
    for(cd = (Class_def *)table_next(Class_defs);
	cd;
	cd = (Class_def *)table_next(Class_defs))
	print_class_def(cd, 2);
}


/************************************************************************
 *									*
 *  print_class_def							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_class_def(Class_def *cd, int indent)
#else
void
print_class_def(cd, indent)
Class_def *cd;
int indent;
#endif
{
    TABLE *slots;
    Slot_def *sd;
    register int i;
    int num;
    
    printf("Class Definition for %s:\n", cd->name);
    printf("%.*s%s\n", indent, blanks, cd->pattern);
    printf("%.*sslot_defs:\n", indent, blanks);
    slots = (TABLE *)cd->slot_defs;
    table_first(slots);
    for(sd = (Slot_def *)table_next(slots);
	sd;
	sd = (Slot_def *)table_next(slots))
	print_slot_def(sd, indent+2);
}
