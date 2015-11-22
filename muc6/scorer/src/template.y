%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	template.y
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the yacc specifications for the MUC
 *	template file parser.
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
static	char	SccsId[] = "@(#)template.y	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc_protos.h"	/* nested include of muc.h */
#include  "muc_globals.h"


Proto	(extern void, yyerror, (const char *msg));
Proto	(extern int, yywrap, (void));
Proto	(extern void, finish_template, (void));

Proto	(static void, start_instance, (char *instheader));
Proto	(static void, find_current_entry, ());
Proto	(static void, set_cur_entry, ());
Proto	(static void, set_cur_class_def, (char *pattern));

Proto	(static void, start_slot, (char *slotstr));
Proto	(static void, start_alt_set, (void));
Proto	(static void, start_pointer, (char *instheader));

extern	int	yyleng;
extern	long	templlineno;
extern	long	templcharno;
extern	long	templbegin;
extern	char	*templfname;


Class_def		*cur_class_def;
Slot_def		*cur_slot_def;

Catalog_entry		*cur_entry;
File_section    	*cur_section;

Root			*cur_root;
TABLE			*cur_classinstlists;

Class_instance_list	*cur_classinstlist;
TABLE			*cur_instances;
Class_instance		*cur_inst, *this_inst;

Slot_fill		*cur_slot;
Alt_fill_set		*cur_altfillset;

char			*this_cname, *cur_cname;
char			*this_docnum, *cur_docnum;
char			*this_id, *cur_id;
char			*this_sname, *cur_sname;
char			*this_filler;
Pointer			*this_pointer;


%}


%union 
{
    int   ival;
    char  *sval;
}

%token <sval> INST_START
%token <sval> SLOTNAME
%token <sval> POINTER
%token <sval> SLOT_FILL
%token <sval> QSTRING
%token OPTMARK ALTMARK

%%

/*  parsing a template file */

inst_specs:	inst_spec
	|	inst_specs inst_spec
	;

inst_spec:	inst_start slot_specs
	;

inst_start:	INST_START
	{
	    start_instance(yylval.sval);
	    UFREE(yylval.sval);
	}
	;

slot_specs:	slot_spec
	|	slot_specs slot_spec
	;

slot_spec:	slotname
	|	slotname slot_fills
	;


slotname:	SLOTNAME
	{
	    start_slot(yylval.sval);
	    UFREE(yylval.sval);
	}
	;


slot_fills:	slot_fill
	|	slot_fills slot_fill
	;

slot_fill:	optmark
	|	altmark
	|	text
	|	pointer
	;

optmark:	OPTMARK
	{
	    cur_slot->is_optional = True;
	}
	;

altmark:	ALTMARK
	{
	    start_alt_set();
	}
	;

text:		string
	|	qstring
	;

string:		SLOT_FILL
	{
	    this_filler = trim_quotes(yylval.sval);

	    if (cur_altfillset)
		add_alt_string_fill(cur_altfillset, this_filler);
	    else
		add_string_fill(cur_slot, this_filler);

	    UFREE(this_filler);
	    UFREE(yylval.sval);
	}
	;

qstring:	QSTRING
	{
	    this_filler = trim_quotes(yylval.sval);
	    trim_whitespace(this_filler);

	    if (cur_altfillset)
		add_alt_string_fill(cur_altfillset, this_filler);
	    else
		add_string_fill(cur_slot, this_filler);

	    UFREE(this_filler);
	    UFREE(yylval.sval);
	}
	;

pointer:	POINTER
	{
	    start_pointer(yylval.sval);
	    if (cur_altfillset)
		add_alt_pointer_fill(cur_altfillset, this_pointer);
	    else
		add_pointer_fill(cur_slot, this_pointer);
	    UFREE(yylval.sval);
	}
	;


%%


#if UsePrototypes
static
void
start_instance(char *instheader)
#else
static
void
start_instance(instheader)
char *instheader;
#endif	/* UsePrototypes */
{
    char  *cpattern;
    
    /* allocate memory for strings */
    cpattern = STRALLOC(instheader);
    this_docnum = STRALLOC(instheader);
    this_id = STRALLOC(instheader);

    /* extract the classpattern, this_docnum, and id. */
    get_class_docnum_id(instheader, cpattern, this_docnum, this_id);

    set_cur_class_def(cpattern);

    this_cname = STRALLOC(cur_class_def->name);
    
    find_current_entry();

    cur_inst = class_instance_create(cur_instances, this_cname,
				     this_docnum, this_id);

    /* this is an instantiation instead of a pointer. */
    cur_inst->is_instantiated = True;

    UFREE(cur_cname);
    cur_cname = STRALLOC(this_cname);

    UFREE(cur_docnum);
    cur_docnum = STRALLOC(this_docnum);

    UFREE(cur_id);
    cur_id = STRALLOC(this_id);

    UFREE(cpattern);
    UFREE(this_cname);
    UFREE(this_docnum);
    UFREE(this_id);
}


#if UsePrototypes
static
void
find_current_entry(void)
#else
static
void
find_current_entry()
#endif	/* UsePrototypes */
{
    extern Bool first_entry;

    /* if this is the first instance, then this will also be the first
       catalog_entry */

    if (first_entry)
    {
	printf("parsing %s %s...",
	       (MUC_instance_type == KEY) ? "key" : "response",
	       this_docnum);
	set_cur_entry();
	first_entry = False;
    }
    
    /* compare this docnum with the current - if it's the same, then
       it's just another instance to add to the current set, so do
       nothing.  if it's different, then this is the start of a
       catalog entry for a new set of instances. */

    else if (!STRCASEEQ(cur_docnum, this_docnum))
    {
	/* set the end position of the current File_section before
	   creating a new one. */
	cur_section->end = templbegin;
	printf("done.\n");
	printf("parsing %s %s...",
	       (MUC_instance_type == KEY) ? "key" : "response",
	       this_docnum);
	set_cur_entry();
    }

    /* if this is a new class of instances */
    cur_classinstlist = class_instance_list_create(cur_classinstlists,
						   this_cname,
						   this_docnum);

    cur_instances = (TABLE *) cur_classinstlist->instances;
}


#if UsePrototypes
static
void
set_cur_entry(void)
#else
static
void
set_cur_entry()
#endif	/* UsePrototypes */
{
    cur_entry = catalog_entry_create(this_docnum);
    cur_section = file_section_create(templfname);
    cur_section->begin = templbegin;
    
    cur_root = root_create(this_docnum);
    cur_classinstlists = cur_root->class_instance_lists;
    
    if(MUC_instance_type == KEY)
    {
	cur_entry->k_doc = cur_section;
	cur_entry->k_root = cur_root;
    }
    else
    {
	cur_entry->r_doc = cur_section;
	cur_entry->r_root = cur_root;
    }

    return;
}


#if UsePrototypes
static
void
set_cur_class_def(char *pattern)
#else
static
void
set_cur_class_def(pattern)
char *pattern;
#endif	/* UsePrototypes */
{
    /* search the table for a class_def having the same
       string pattern as classname */

    if ((cur_class_def = (find_class_def_by_pattern(pattern)))
	== (Class_def *) NULL)
    {
	/* not found */
	fprintf(stderr, "Error: [parse template]: ");
	fprintf(stderr, "class '%s' has not been defined.\n", pattern);
	exit(1);
    }

    return;
}


#if UsePrototypes
static
void
start_slot(char *slotstr)
#else
static
void
start_slot(slotstr)
char *slotstr;
#endif	/* UsePrototypes */
{
    char *spattern, *sptr;
    TABLE  *slots = cur_inst->slot_fills;
    
    spattern = STRALLOC(slotstr);
    sptr = spattern;
    
    /* copy slotstr onto spattern, but first skip leading whitespace */
    while(!isalpha(*slotstr))
	slotstr++;
    while((*sptr++ = *slotstr++) != '\0')
	;
    
    /* is this one of the slots of the current class_def? */
    if ((cur_slot_def = find_slot_def_by_pattern (cur_class_def, spattern))
	== (Slot_def *) NULL)
    {
	fprintf(stderr, "Error: [parse template]: slot '%s'", spattern);
	fprintf(stderr, " is not defined for class '%s'.\n", cur_cname);
	exit(1);
    }
    
    UFREE(cur_sname);
    cur_sname = STRALLOC(cur_slot_def->name);
    
    cur_slot = slot_fill_create(slots, cur_sname);

    /* this is a new slot, so reset this */
    cur_altfillset = (Alt_fill_set *) NULL;

    UFREE(spattern);
}


#if UsePrototypes
static
void
start_alt_set(void)
#else
static
void
start_alt_set()
#endif	/* UsePrototypes */
{
    Alt_fill_set  *this_altfillset;
    
    this_altfillset = alt_fill_set_create();

    /* is this the first alt_fill_set? */
    if (cur_altfillset == (Alt_fill_set *) NULL)
	cur_slot->alt_fill_sets = this_altfillset;
    else
	cur_altfillset->next = this_altfillset;
    
    cur_altfillset = this_altfillset;
    
    return;
}


#if UsePrototypes
static
void
start_pointer(char *instheader)
#else
static
void
start_pointer(instheader)
char *instheader;
#endif	/* UsePrototypes */
{
    Class_instance_list  *instlist;
    TABLE  *instances;
    Class_def  *cdef;
    char  *cpattern;
    
    /* allocate memory for strings */
    cpattern = STRALLOC(instheader);
    this_docnum = STRALLOC(instheader);
    this_id = STRALLOC(instheader);

    /* extract the classpattern, this_docnum, and id. */
    get_class_docnum_id(instheader, cpattern, this_docnum, this_id);

    cdef = find_class_def_by_pattern(cpattern);

    this_cname = STRALLOC(cdef->name);

    instlist = class_instance_list_create(cur_classinstlists,
					  this_cname,
					  this_docnum);

    instances = (TABLE *) instlist->instances;

    this_inst = class_instance_create(instances, this_cname,
				      this_docnum, this_id);

    this_pointer = pointer_create(this_inst);
    this_inst->is_pointed_to = True;

    /* since this_inst is being pointed to by cur_inst, create a
       back_pointer from this_inst to cur_inst. */

    add_back_pointer(this_inst, cur_inst);
    
    UFREE(cpattern);
    UFREE(this_cname);
    UFREE(this_docnum);
    UFREE(this_id);
}


#if UsePrototypes
void
finish_template(void)
#else
void
finish_template()
#endif	/* UsePrototypes */
{
    cur_section->end = templcharno;
    printf("done.\n");
    UFREE(cur_cname);
    UFREE(cur_docnum);
    UFREE(cur_id);
}

