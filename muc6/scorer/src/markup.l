%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	markup.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *     The lex scanner for MUC Named Entity and Coreference 
 *     key and response files
 *     
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *	Consolidated ne.l and ne.y -- there is no markup.y 
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	January 1996
 *      Aaron Douthat   December 1996
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)markup.l Dec 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"
#include  "cdefs.h"
#include  "muc_defs.h"

#ifndef FLEX_SCANNER
#undef yywrap
#endif

/* max depth of tag nesting in document */

#define NEST_DEPTH 10


/*
 *  lex definitions to parse MUC annotated named-entity file.
 */

extern	char	        *markup_fname;  /* defined in parsers.c */

long	                total_lineno,  
                        total_charno, 
                        total_ignore;

long	                doc_begin;     
int	                doc_lineno, 
                        doc_charno, 
                        doc_ignore;    

int	                section_stack[NEST_DEPTH],
                        section_stack_ptr,
                        this_tag;

Bool	                first_dd_exists = False;
Bool                    parsing_header;

Class_instance          *tag_stack[NEST_DEPTH];  
int                     tag_stack_ptr;

int	                enamex_count, 
                        numex_count, 
                        timex_count;

Class_def		*cur_class_def;
Slot_def		*cur_slot_def;

Catalog_entry		*cur_entry;
File_section    	*cur_section;

Root			*cur_root;
TABLE			*cur_classinstlists;

Class_instance_list	*cur_classinstlist;
TABLE			*cur_instances;
Class_instance		*cur_inst;

Slot_fill		*cur_slot;
Alt_fill_set		*cur_altfillset;

char			*cur_cname, 
                        *cur_docnum, 
                        *cur_id, 
                        *cur_sname;

/* note: in muc_defs.h, there is a list of #define's, one for each
 * string in the following array.  If this array is changed, be sure
 * to change the #defines too.
 */

char *non_muc_tags[] = {   
                       "DOC", 
		       "DOCNO",
		       "DOCID",
		       "HL",
		       "hl",
		       "HEADLINE",
		       "headline",
		       "SLUG",
                       "PREAMBLE",
		       "IDS",
		       "ids",
		       "TRAILER",
		       "trailer",
		       "DD",
		       "dd",
		       "DATE",
		       "date",
		       "DATELINE",
		       "dateline",
		       "TXT",
		       "txt",
		       "TEXT",
		       "text" };


/* internal function prototypes */

Proto	(static void, start_current_ne_entry, (void));
Proto   (static void, finish_current_ne_entry, (void));

Proto	(static void, start_ne_instance, (char *open_tag));
Proto   (static void, finish_ne_instance, (char *close_tag));

Proto	(static	void, combine_text_fills, (Class_instance *prev_inst,
					Class_instance *this_inst));

Proto   (static int, which_tag_string, (char *non_muc_tag));
Proto   (static char *, get_GI, (char *tag_string));

Proto	(static void, set_cur_ne_class_def, (char *cname));

Proto	(static	void, parse_ne_tag, (char *ne_tag));

Proto	(static void, start_ne_slot, (char *slotstr));
Proto	(static void, start_ne_alt_set, (void));

Proto	(static	void, add_to_text_fill, (char *new_fill));
Proto	(static	void, remove_first_dd_instance, (void));

Proto	(extern void, yyerror, (const char *msg));
Proto	(extern int, yywrap, (void));


%}

%s DOCNO_STATE

%%
%{
   /* non-muc-task open tag */
%}
"<"[a-zA-Z0-9]+                 {
                                  this_tag = which_tag_string( yytext + 1 );
				  switch( this_tag ){

				  case IGNORE:
				    total_charno += yyleng;
				    doc_charno += yyleng;
				    break;

				  case DOC:
				    doc_begin = total_charno;
				    doc_lineno = 1;
				    doc_ignore = 0;

				    total_charno += yyleng;
				    doc_charno = yyleng;

				    enamex_count = numex_count = timex_count = 0;
				    UFREE( cur_docnum );

				    /* reset doc section stack and push IGNORE */
				    section_stack_ptr = 0;
				    section_stack[section_stack_ptr++] = IGNORE;

				    /* reset muc tag stack */
				    tag_stack_ptr = 0;

				    first_dd_exists = False;
				    parsing_header =  True;
				    break;

				  case DOCNO:
				    total_charno += yyleng;
				    doc_charno += yyleng;
				    BEGIN DOCNO_STATE;
				    break;
				    
				  case DOCID:
				    total_charno += yyleng;
				    doc_charno += yyleng;
				    if( cur_docnum == NULL )
				      BEGIN DOCNO_STATE;
				    break;

				  default:
				    total_charno += yyleng;
				    doc_charno += yyleng;
				    /* push doc_section -- no error checking! */
				    section_stack[section_stack_ptr++] = this_tag;
				    if( parsing_header ){
				      /* create a catalog entry for this
					 document - it may be a new entry, 
					 or perhaps a previous document 
					 (from text or key or response file)
					 has already created one. */	    
				      start_current_ne_entry();
				      parsing_header = False;
				    }
				    break;
				  }
                                }
%{
   /* non-muc-task close tag */
%}
"</"[a-zA-Z0-9]+                  {
                                  this_tag = which_tag_string( yytext + 2 );

				  total_charno += yyleng;
				  doc_charno += yyleng;

				  switch( this_tag ){

				  case IGNORE:
				  case DOCNO:
				  case DOCID:
				    break;

				  case DOC:
				    finish_current_ne_entry();
				    break;

				  default:
				    /* pop doc_section -- should */
				    /* have error checking here! */
				    section_stack_ptr--; 
				    break;
				  }
                                }
%{
   /* document number text */
%}
<DOCNO_STATE>[^ \t\n>]+		{
				    total_charno += yyleng;
				    doc_charno += yyleng;
                                    /* extract the docnum, and 
				       save it in cur_docnum */
				    UFREE(cur_docnum);
				    cur_docnum = nice_docnum(yytext);
				    BEGIN INITIAL;
				}

<DOCNO_STATE>[ \t>]		{
				    total_charno++;
				    doc_charno++;
				}
<DOCNO_STATE>\n                 {
				    total_charno++;
				    total_lineno++;
				    doc_charno++;
				    doc_lineno++;
				}

%{
   /* any non-task char (except eol) */
%}
.				{
				    total_charno++;
				    doc_charno++;
				    if (tag_stack_ptr > 0)
				      add_to_text_fill(yytext);
				}

%{
   /* eol */
%}
\n				{
				    total_lineno++;
				    total_charno++;
				    doc_lineno++;
				    doc_charno++;
				    if (tag_stack_ptr > 0)
				      add_to_text_fill(" ");
				}
%{
   /* NE open tags (including attributes) */
%}
"<"[Ee][Nn][Aa][Mm][Ee][Xx][^>]*">"   |
"<"[Nn][Uu][Mm][Ee][Xx][^>]*">"	      |
"<"[Tt][Ii][Mm][Ee][Xx][^>]*">"       {
				    total_ignore += yyleng;
				    doc_ignore += yyleng;
				    if ( section_stack[section_stack_ptr-1] == IGNORE)
					;
				    else
				      start_ne_instance(yytext);
				}

%{
   /* NE close tags */
%}
"</"[Ee][Nn][Aa][Mm][Ee][Xx]">"	 |
"</"[Nn][Uu][Mm][Ee][Xx]">"      |
"</"[Tt][Ii][Mm][Ee][Xx]">"      {
				    total_ignore += yyleng;
				    doc_ignore += yyleng;
				    if ( section_stack[section_stack_ptr-1] == IGNORE)
					;
				    else
				      finish_ne_instance(yytext);
				}



%%

/*
 * which_tag_string -- look up an SGML GI (generic identifier)
 *     in non_muc_tags (the list of non-muc GI's which we 
 *     are interested in).
 */

#if UsePrototypes
static
int
which_tag_string(char *non_muc_tag)
#else
static
int
which_tag_string(non_muc_tag)
char *non_muc_tag;
#endif	/* UsePrototypes */
{
  int i;
  for( i=0; i<IGNORE; i++ )
    if( STREQ(non_muc_tag, non_muc_tags[i]) )
      break;
  return i;
}

/*
 * start_ne_instance -- the parser has found an NE
 *           (TIMEX,NUMEX,ENAMEX) tag in the text.
 *           Insert the tag's info into the scorer's
 *           catalog and prepare to read the content 
 *           (the stuff between the tags) into the 
 *           "text" slot.
 */

#if UsePrototypes
static
void
start_ne_instance(char *open_tag)
#else
static
void
start_ne_instance(open_tag)
char *open_tag;
#endif	/* UsePrototypes */
{
  int ne_id;
  char id_str[10];

  /* generic identifier */
  char *GI_string = get_GI(open_tag);
  
  UFREE(cur_cname);
  cur_cname = string_lower(GI_string);
  UFREE(GI_string);
  
  if( STREQ( cur_cname, "enamex") )
    ne_id = ++enamex_count;
  else if( STREQ( cur_cname, "numex") )
    ne_id = ++numex_count;
  else if( STREQ( cur_cname, "timex") )
    ne_id = ++timex_count;
    
  sprintf(id_str, "%d", ne_id);
  UFREE(cur_id);
  cur_id = STRALLOC(id_str);
  
  set_cur_ne_class_def(cur_cname);
  
  /* if this is a new class of instances,      */
  /* then a new list is created -- otherwise   */
  /* the existing class instance list is found */
  cur_classinstlist = class_instance_list_create(cur_classinstlists,
						 cur_cname,
						 cur_docnum);
  
  cur_instances = (TABLE *) cur_classinstlist->instances;
  cur_inst = class_instance_create(cur_instances, cur_cname,
				   cur_docnum, cur_id);
  
  /* this is an instantiation instead of a pointer. */
  cur_inst->is_instantiated = True;
  
  /* fill in some NE specific info */
  cur_inst->start_offset = doc_charno;
  
  cur_inst->doc_section = section_stack[section_stack_ptr-1];
  
  /* is this a DD section?  if it is, is it the second
     occurrence of DD in the article */
  if (section_stack[section_stack_ptr-1] == DD)
    {
      if (first_dd_exists)
	{
	  /* there should be either zero or one NE instance per DD */
	  remove_first_dd_instance();
	}
      else
	first_dd_exists = True;
    }
  
  /* extract the information from the tag header */
  parse_ne_tag(open_tag);

  /* push cur_instance on the tag stack */
  tag_stack[tag_stack_ptr++] = cur_inst;
  
  /* set the 'text' slot as the cur_slot */
  start_ne_slot("text");
  
  return;
}



/*
 * finish_ne_instance -- the parser has found an NE
 *      close tag in the text.  Pop the NE instance
 *      from the NE instance ("tag") stack.
 */

#if UsePrototypes
static
void
finish_ne_instance(char *close_tag)
#else
static
void
finish_ne_instance(close_tag)
char *close_tag;
#endif	/* UsePrototypes */
{
    Class_instance *prev_inst;
    
    /* is there nothing left on the 'stack' to remove ? */
    if (tag_stack_ptr == 0)
	fprintf(stderr,
		"unmatched '%s' in %s, line %d\n",
		close_tag, cur_docnum, doc_lineno);
    else
    {
	/* set the end offset for the cur_inst */
	cur_inst->end_offset = doc_charno;

	/* are there nested ne tags ? */
	if (tag_stack_ptr >= 2)
	{
	    prev_inst = tag_stack[tag_stack_ptr-2];

	    /* since this is a nested ne, add its text to the
	       enclosing ne */
	    combine_text_fills(prev_inst, cur_inst);

	    /* reset cur_inst and set cur_slot to 'text' */
	    cur_inst = prev_inst;
	    start_ne_slot("text");
	}

	/* decrement the 'stack' level */
	tag_stack_ptr--;
    }
    
    return;
}

/*
 * get_GI -- given an SGML tag, return the generic identifier
 *           (tag name) string
 */

#if UsePrototypes
static
char *
get_GI(char *tag_string)
#else
static
char *
get_GI(tag_string)
char *tag_string;
#endif	/* UsePrototypes */
{
  char *result, *tag_copy;

  result = tag_copy = STRALLOC( tag_string+1 ); /* skip "<" */

  while( isalpha( *tag_copy ) || isdigit( *tag_copy ))
    tag_copy++;

  *tag_copy = '\0';

  return result;
}


/*
 * combine_text_fills -- the end of nested_inst has been reached
 *       in the text.  Append the text fill of nested_inst 
 *       to the text fill of this_inst.
 */


#if UsePrototypes
static
void
combine_text_fills(Class_instance *this_inst, Class_instance *nested_inst)
#else
static
void
combine_text_fills(this_inst, nested_inst)
Class_instance *this_inst;
Class_instance *nested_inst;
#endif	/* UsePrototypes */
{
    TABLE *this_slots = this_inst->slot_fills;
    TABLE *nested_slots = nested_inst->slot_fills;
    
    Slot_fill *this_slot = find_slot_fill(this_slots, "text");
    Slot_fill *nested_slot = find_slot_fill(nested_slots, "text");

    String_fill *this_strfill, *nested_strfill;
    int this_len, nested_len;

    /* for NE, there are no multi fills, so index is 0 */
    nested_strfill = nested_slot->strfills[0];

    /* will this be the first text fill for the enclosing NE? */
    if (this_slot->strfills == (String_fill **) NULL)
	add_string_fill(this_slot, nested_strfill->fill);

    /* previous text exists in the enclosing NE */
    else
    {
	this_strfill = this_slot->strfills[0];

	this_len = strlen(this_strfill->fill);
	nested_len = strlen(nested_strfill->fill);

	this_strfill->fill = UREALLOC(this_strfill->fill,
				      char,
				      (this_len + nested_len + 1));

	strcpy((this_strfill->fill + this_len), nested_strfill->fill);
    }

    return;
}

/*
 * add_to_text_fill -- The parser has found a 
 *        substring of a string which is enclosed by NE tags.  
 *        Append the substring to the text fill of 
 *        the "instance" corresponding to the 
 *        (innermost) pair of NE tags
 */
#if UsePrototypes
static
void
add_to_text_fill(char *new_fill)
#else
static
void
add_to_text_fill(new_fill)
char *new_fill;
#endif	/* UsePrototypes */
{
    String_fill *cur_strfill;
    int cur_len, new_len;

    /* will this be the first text fill? */
    if (cur_slot->strfills == (String_fill **) NULL)
	add_string_fill(cur_slot, new_fill);
    else
    {
	/* append this new_fill to an existing fill */
	cur_strfill = cur_slot->strfills[0];

	cur_len = strlen(cur_strfill->fill);
	new_len = strlen(new_fill);

	cur_strfill->fill = UREALLOC(cur_strfill->fill,
				     char,
				     (cur_len + new_len + 1));

	/* copy the new fill at the end of the current fill */
	strcpy((cur_strfill->fill + cur_len), new_fill);
    }
    
    return;
}

/* start_current_ne_entry -- the parser is ready to insert
 *             NE tag info into the muc catalog (a document
 *             number has been found in either the DOCNO or
 *             DOCID part of the text).  Find the catalog 
 *             entry corresponding to the current document,
 *             or create it if it doesn't exist.
 */

#if UsePrototypes
static
void
start_current_ne_entry(void)
#else
static
void
start_current_ne_entry()
#endif	/* UsePrototypes */
{
    printf("parsing %s %s...",
	   (MUC_instance_type == KEY) ? "key" : "response",
	   cur_docnum);

    cur_entry = catalog_entry_create(cur_docnum);

    /* create a File_section for this document and set its
       begin point. */
    cur_section = file_section_create(markup_fname);
    cur_section->begin = doc_begin;

    cur_root = root_create(cur_docnum);
    cur_classinstlists = cur_root->class_instance_lists;
    
    if (MUC_instance_type == KEY)
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

/*
 *  finish_current_ne_entry -- the parser has found </DOC> in the
 *              text.  Note where in the input file the document ends,
 *              and check for unmatched NE tags.
 */

#if UsePrototypes
static
void
finish_current_ne_entry(void)
#else
static
void
finish_current_ne_entry()
#endif	/* UsePrototypes */
{
  cur_section->end = total_charno;
  if( tag_stack_ptr > 0)
    fprintf(stderr, "not enough closing NE/CO tags in %s\n",
	    cur_docnum);
  printf("%s done.\n", cur_docnum);
}

/*
 * parse_ne_tag -- get the attributes of an NE tag and insert
 *                 them into the slots of the scorer's catalog.
 */

#if UsePrototypes
static void
parse_ne_tag(char *ne_tag)
#else
static void
parse_ne_tag(ne_tag)
char *ne_tag;
#endif	/* UsePrototypes */
{
  char *tokenstr, *tag_token, *attr_token, *val_token;

  tokenstr = STRALLOC(ne_tag);
    
  /* grab the first token:  it should be the tag name */
  if(tag_token = strtok(tokenstr, " >"))
    /* get the next attribute */
    while(attr_token = strtok(NULL, " =>"))
      /* 'status' indicates optionality */
      if (STRCASEEQ(attr_token, "status"))
	{
	  val_token = strtok(NULL, "\"");

	  /* check if the value is 'optional' or 'opt */
	  if ( (STRCASEEQ(val_token, "optional"))
	      || (STRCASEEQ(val_token, "opt")) )
	    cur_inst->is_optional = True;
	}
      else if (STRCASEEQ(attr_token, "alt"))
	/* 'alt' fills are for the 'text' slot only, so start
	   the 'text' slot and initialize its alt set */
	{
	  start_ne_slot("text");
	  start_ne_alt_set();

	  /* grab the first of possibly multiple values */
	  val_token = strtok(NULL, "|\"");
	
	  /* get the next value at the end of the loop */
	  while(val_token)
	    {
	      trim_whitespace(val_token);
	      add_alt_string_fill(cur_altfillset, val_token);

	      /* get the next value */
	      val_token = strtok((char *)NULL, "|\"");
	    }
	}
      else
	{
	  start_ne_slot(attr_token);
	  val_token = strtok(NULL, " \">");
	  add_string_fill(cur_slot, val_token);
	}

  /* after parsing the tag, there will be no more alt sets */
  cur_altfillset = (Alt_fill_set *) NULL;
    
  UFREE(tokenstr);
}

/*
 * set_cur_ne_class_def -- find the class definition in the scorer
 *                         catalog of the class named cname, and
 *                         set the cur_class_def global variable
 *                         to point to it.
 */

#if UsePrototypes
static
void
set_cur_ne_class_def(char *cname)
#else
static
void
set_cur_ne_class_def(cname)
char *cname;
#endif	/* UsePrototypes */
{
    /* search the table for a class_def having the same name as cname */

    if ((cur_class_def = (find_class_def_by_name(cname)))
	== (Class_def *) NULL)
    {
	/* not found */
	fprintf(stderr, "Error: [parse named-entity]: ");
	fprintf(stderr, "class '%s' has not been defined.\n", cname);
	exit(1);
    }

    return;
}

/*
 * start_ne_slot -- given the name of a slot, find its slot definition 
 *                  (for the current class), and set the global variables 
 *                  cur_slot and cur_slot_def. (Create a new slot fill
 *                  if it doesn't already exist)
 */


#if UsePrototypes
static
void
start_ne_slot(char *slotstr)
#else
static
void
start_ne_slot(slotstr)
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
	fprintf(stderr, "Error: [parse named-entity]: slot '%s'", spattern);
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

/*
 * start_ne_alt_set -- create an alternate fill set, and
 *                     add it to the list of alt_fill_sets
 *                     in the current slot.  Also, set
 *                     global variable cur_altfillset.
 */

#if UsePrototypes
static
void
start_ne_alt_set(void)
#else
static
void
start_ne_alt_set()
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

/* 
 * remove_first_dd_instance -- For the texts used
 *        so far in MUC, there have been at most two
 *        DD sections per document.  Only the last
 *        section should be marked up.  If the first
 *        DD section was marked up, and the parser encounters
 *        a second, the data from the first DD section
 *        must be removed from the scorer's catalog.
 */
#if UsePrototypes
static
void
remove_first_dd_instance(void)
#else
static
void
remove_first_dd_instance()
#endif	/* UsePrototypes */
{
    Class_instance_list  *instlist;
    TABLE  *instances;
    Class_instance *dd_inst, *target;

    table_first(cur_classinstlists);
    for (instlist = (Class_instance_list *) table_next(cur_classinstlists);
	 instlist;
	 instlist = (Class_instance_list *) table_next(cur_classinstlists))
    {
	instances = (TABLE *) instlist->instances;
	
	table_first(instances);
	for (dd_inst = (Class_instance *) table_next(instances);
	     dd_inst;
	     dd_inst = (Class_instance *) table_next(instances))
	{
	    if (dd_inst->doc_section == DD)
	    {
		/* create a new class_instance */

		if((target = UALLOC(Class_instance, 1)) == (Class_instance *)NULL)
		{
		    fprintf(stderr, "Error: [remove_first_dd_instance]: ");
		    fprintf(stderr, "Allocation failed.\n");
		    exit(1);
		}

		/* initialize */
		target->name = (char *) NULL;
		target->docnum = (char *) NULL;
		target->id = STRALLOC(dd_inst->id);
		/* create an empty table for the slot_fills */
		target->slot_fills = (TABLE *) NULL;
		target->mapped_to = (Class_instance *) NULL;
		target->connected_to = (Class_instance *) NULL;
		target->ident_in_and_out_ptrs = (Pointer *) NULL;
		target->back_pointers = (Pointer *) NULL;
	
		printf("\n\tremoving first DD instance %s-%s-%s...",
		       dd_inst->name, dd_inst->docnum, dd_inst->id);
		if(table_delete((char *) target, instances) == TABLE_OK)
		{
		    printf("done\n");
		    class_instance_release((char *) target);
		}
		else
		    printf("failed\n");

		/* there should be at most one instance to be deleted,
		   so there's no need to continue after this deletion. */
		return;
	    }
	}
    }
	
    return;
}

/*
 * yyerror -- what to do in the case of a syntax error.
 */

#if UsePrototypes
void
yyerror(const char *msg)
#else
void
yyerror(msg)
char *msg;
#endif  /* UsePrototypes */
{
    printf("(named_entity) %d: %s at '%s'\n", total_lineno, msg, yytext);
}

/* 
 * yywrap -- what to do at the end of the file
 */

#if UsePrototypes
int
yywrap(void)
#else
int
yywrap()
#endif  /* UsePrototypes */
{
    UFREE(cur_cname);
    UFREE(cur_docnum);
    UFREE(cur_id);
    return(1);
}

#if UsePrototypes
void
print_tag_stack(void)
#else
void
print_tag_stack()
#endif  /* UsePrototypes */
{
  int i;

  for( i=0; i<tag_stack_ptr; i++ )
    print_class_instance( tag_stack[i], 0 );
}

