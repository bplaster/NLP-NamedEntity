/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	class_inst.c
 *
 *	previous name - class_instance.c (exceeds 15-char ar limit)
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	support routines for Class_instance data structure.
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
static	char	SccsId[] = "%W% %G%  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


/************************************************************************
 *									*
 *  class_instance_create						*
 *									*
 ***********************************************************************/

#if UsePrototypes
Class_instance *
class_instance_create(TABLE *instlist, char *name, char *docnum, char *id)
#else
Class_instance *
class_instance_create(instlist, name, docnum, id)
TABLE  *instlist;
char  *name;
char  *docnum;
char  *id;
#endif	/* UsePrototypes */
{
    Class_instance *newinst, *oldinst;
    
    /* create a new class_instance */

    if((newinst = UALLOC(Class_instance, 1)) == (Class_instance *)NULL)
    {
	fprintf(stderr, "Error: [class_instance_create]: Allocation failed.\n");
	exit(1);
    }

    /* initialize */
    newinst->name = STRALLOC(name);
    newinst->docnum = STRALLOC(docnum);
    newinst->id = STRALLOC(id);
    /* create an empty table for the slot_fills */
    newinst->slot_fills = table_list_create(slot_fill_compare,
					    slot_fill_release,
					    default_destroy);
    
    if(newinst->slot_fills == (TABLE *)NULL)
    {
	fprintf(stderr, "Error: [class_instance_create]: ");
	fprintf(stderr, "Unable to create slot_fills table.\n");
	exit(1);
    }

    newinst->mapped_to = (Class_instance *) NULL;
    newinst->connected_to = (Class_instance *) NULL;
    newinst->ident_in_and_out_ptrs = (Pointer *) NULL;
    newinst->is_instantiated = False;
    newinst->is_pointed_to = False;
    newinst->back_pointers = (Pointer *) NULL;
    newinst->num_backptrs = 0;

    /* for named_entity and coreference tasks */
    newinst->is_optional = False;
    newinst->start_offset = 0;
    newinst->end_offset = 0;
    newinst->doc_section = 0;

    /* search for an existing instance.  if it exists, free up the new
       instance and re-set it to the existing one. */
    if (oldinst = (Class_instance *)table_find((char *)newinst, instlist))
    {
	class_instance_release((char *) newinst);
	newinst = oldinst;
    }
    else	/* no existing instance */
    {
	/* insert the new instance into the table */
	if(table_append((char *) newinst, (TABLE *) instlist) == TABLE_ERR)
	{
	    fprintf(stderr, "Error: [class_instance_create]: ");
	    fprintf(stderr, "table insertion failed.\n");
	    exit(1);
	}
    }

    return(newinst);
}


/************************************************************************
 *									*
 *  class_instance_release						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
class_instance_release(char *elt)
#else
void
class_instance_release(elt)
char *elt;
#endif	/* UsePrototypes */
{
    Class_instance *ptr = (Class_instance *)elt;
    
    if(ptr == (Class_instance *)NULL)
	return;
    
    UFREE(ptr->name);
    UFREE(ptr->docnum);
    UFREE(ptr->id);

    /* free up the table of slot_fills */
    table_destroy(ptr->slot_fills);
    
    /* clean up the mapped_to pointers between mapped instances */
    if(ptr->mapped_to != (Class_instance *)NULL)
    {
	ptr->mapped_to->mapped_to = (Class_instance *)NULL;
	ptr->mapped_to = (Class_instance *)NULL;
    }

    /* clean up the connected_to pointers between connected instances */
    if(ptr->connected_to != (Class_instance *)NULL)
    {
	ptr->connected_to->connected_to = (Class_instance *)NULL;
	ptr->connected_to = (Class_instance *)NULL;
    }

    /* clean up the ident_in_and_out pointers */
    pointer_release((char *) ptr->ident_in_and_out_ptrs);

    /* clean up the list of back_pointers */
    pointer_release((char *) ptr->back_pointers);
    ptr->back_pointers = (Pointer *) NULL;

    UFREE(ptr);
}


/************************************************************************
 *									*
 *  class_instance_compare						*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
class_instance_compare(char *elt1, char *elt2)
#else
int
class_instance_compare(elt1, elt2)
char *elt1, *elt2;
#endif	/* UsePrototypes */
{
    Class_instance *ptr1 = (Class_instance *)elt1;
    Class_instance *ptr2 = (Class_instance *)elt2;
    
    return(strcasecmp(ptr1->id, ptr2->id));
}


/************************************************************************
 *									*
 *  print_class_instance						*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_class_instance(Class_instance *inst, int indent)
#else
void
print_class_instance(inst, indent)
Class_instance *inst;
int indent;
#endif	/* UsePrototypes */
{
    TABLE      *slot_fills;
    Slot_fill  *sfill;
    Class_instance  *mapped_to;
    Class_instance  *connected_to;
    
    if (inst == (Class_instance *) NULL)
	return;
    
    printf("%.*s%s-%s:\n", indent, blanks, inst->docnum, inst->id);

    if((mapped_to = inst->mapped_to) != (Class_instance *) NULL)
	printf("%.*smapped to %s-%s:\n", indent+4, blanks,
	       mapped_to->docnum, mapped_to->id);
    
    if((connected_to = inst->connected_to) != (Class_instance *) NULL)
	printf("%.*sconnected to %s-%s:\n", indent+4, blanks,
	       connected_to->docnum, connected_to->id);
    
    slot_fills = (TABLE *) inst->slot_fills;
    
    table_first(slot_fills);
    for(sfill = (Slot_fill *)table_next(slot_fills);
	sfill;
	sfill = (Slot_fill *)table_next(slot_fills))
	print_slot_fill(sfill, indent+2);
}


/************************************************************************
 *									*
 *  add_back_pointer -- called when one instance gets a pointer fill to
 *                      another instance.  Was in back_pointer.c, but
 *                      Back_pointers and Pointers were really the same,
 *                      so back_pointer.c was eliminated.
 *									*
 ***********************************************************************/

#if UsePrototypes
void
add_back_pointer(Class_instance *this, Class_instance *back)
#else
void
add_back_pointer(this, back)
Class_instance *this;
Class_instance *back;
#endif	/* UsePrototypes */
{
    Pointer *bptr, *last;

    /* create a new back_pointer */
    bptr = pointer_create(back);
    
    /* is back the first pointer? */
    if (this->back_pointers == (Pointer *) NULL)
    {
	this->back_pointers = bptr;
	this->num_backptrs++;
    }
    else
    {
	for(last = this->back_pointers;
	    last->next;
	    last = last->next)
	    ;
	last->next = bptr;
	this->num_backptrs++;
    }

    return;
}


/************************************************************************
 *									*
 *  print_back_pointers							*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
print_back_pointers(Class_instance *child)
#else
void
print_back_pointers(child)
Class_instance *child;
#endif	/* UsePrototypes */
{
    Pointer *ptr = child->back_pointers;

    while(ptr)
    {
	print_back_pointer(ptr);
	ptr = ptr->next;
    }

    return;
}


#if UsePrototypes
void
print_back_pointer(Pointer *bptr)
#else
void
print_back_pointer(bptr)
Pointer *bptr;
#endif	/* UsePrototypes */
{
    Class_instance *parent = bptr->inst;

    if (parent != (Class_instance *) NULL)
	printf("bp: %s-%s-%s\n", parent->name, parent->docnum, parent->id);

    return;
}
