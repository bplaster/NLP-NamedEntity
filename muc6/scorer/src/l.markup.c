#include <stdio.h>
# define U(x) x
# define NLSTATE markup_previous=YYNEWLINE
# define BEGIN markup_bgin = markup_svec + 1 +
# define INITIAL 0
# define YYLERR markup_svec
# define YYSTATE (markup_estate-markup_svec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,markup_out)
#else
# define lex_output(c) (void)putc(c,markup_out)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int markup_back(int *, int);
	int markup_input(void);
	int markup_look(void);
	void markup_output(int);
	int markup_racc(int);
	int markup_reject(void);
	void markup_unput(int);
	int markup_lex(void);
#ifdef YYLEX_E
	void markup_woutput(wchar_t);
	wchar_t markup_winput(void);
#endif
#ifndef markup_less
	int markup_less(int);
#endif
#ifndef markup_wrap
	int markup_wrap(void);
#endif
#ifdef LEXDEBUG
	void allprint(char);
	void sprint(char *);
#endif
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void exit(int);
#ifdef __cplusplus
}
#endif

#endif
# define unput(c) {markup_tchar= (c);if(markup_tchar=='\n')markup_lineno--;*markup_sptr++=markup_tchar;}
# define markup_more() (markup_morfg=1)
#ifndef __cplusplus
# define input() (((markup_tchar=markup_sptr>markup_sbuf?U(*--markup_sptr):getc(markup_in))==10?(markup_lineno++,markup_tchar):markup_tchar)==EOF?0:markup_tchar)
#else
# define lex_input() (((markup_tchar=markup_sptr>markup_sbuf?U(*--markup_sptr):getc(markup_in))==10?(markup_lineno++,markup_tchar):markup_tchar)==EOF?0:markup_tchar)
#endif
#define ECHO fprintf(markup_out, "%s",markup_text)
# define REJECT { nstr = markup_reject(); goto markup_fussy;}
int markup_leng;
char markup_text[YYLMAX];
int markup_morfg;
extern char *markup_sptr, markup_sbuf[];
int markup_tchar;
FILE *markup_in = {stdin}, *markup_out = {stdout};
extern int markup_lineno;
struct markup_svf { 
	struct markup_work *markup_stoff;
	struct markup_svf *markup_other;
	int *markup_stops;};
struct markup_svf *markup_estate;
extern struct markup_svf markup_svec[], *markup_bgin;

# line 3 "markup.l"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */


# line 7 "markup.l"
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
#undef markup_wrap
#endif


# line 57 "markup.l"
/* max depth of tag nesting in document */

#define NEST_DEPTH 10



# line 62 "markup.l"
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


# line 112 "markup.l"
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



# line 143 "markup.l"
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

Proto	(extern void, markup_error, (const char *msg));
Proto	(extern int, markup_wrap, (void));


# define DOCNO_STATE 2
# define YYNEWLINE 10
markup_lex(){
int nstr; extern int markup_previous;
   /* non-muc-task open tag */
#ifdef __cplusplus
/* to avoid CC and lint complaining markup_fussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto markup_fussy;
#endif
while((nstr = markup_look()) >= 0)
markup_fussy: switch(nstr){
case 0:
if(markup_wrap()) return(0); break;
case 1:

# line 178 "markup.l"
                {
                                  this_tag = which_tag_string( markup_text + 1 );
				  switch( this_tag ){

				  case IGNORE:
				    total_charno += markup_leng;
				    doc_charno += markup_leng;
				    break;

				  case DOC:
				    doc_begin = total_charno;
				    doc_lineno = 1;
				    doc_ignore = 0;

				    total_charno += markup_leng;
				    doc_charno = markup_leng;

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
				    total_charno += markup_leng;
				    doc_charno += markup_leng;
				    BEGIN DOCNO_STATE;
				    break;
				    
				  case DOCID:
				    total_charno += markup_leng;
				    doc_charno += markup_leng;
				    if( cur_docnum == NULL )
				      BEGIN DOCNO_STATE;
				    break;

				  default:
				    total_charno += markup_leng;
				    doc_charno += markup_leng;
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
break;
   /* non-muc-task close tag */
case 2:

# line 242 "markup.l"
                 {
                                  this_tag = which_tag_string( markup_text + 2 );

				  total_charno += markup_leng;
				  doc_charno += markup_leng;

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
break;
   /* document number text */
case 3:

# line 269 "markup.l"
	{
				    total_charno += markup_leng;
				    doc_charno += markup_leng;
                                    /* extract the docnum, and 
				       save it in cur_docnum */
				    UFREE(cur_docnum);
				    cur_docnum = nice_docnum(markup_text);
				    BEGIN INITIAL;
				}
break;
case 4:

# line 279 "markup.l"
	{
				    total_charno++;
				    doc_charno++;
				}
break;
case 5:

# line 283 "markup.l"
                {
				    total_charno++;
				    total_lineno++;
				    doc_charno++;
				    doc_lineno++;
				}
break;
   /* any non-task char (except eol) */
case 6:

# line 293 "markup.l"
			{
				    total_charno++;
				    doc_charno++;
				    if (tag_stack_ptr > 0)
				      add_to_text_fill(markup_text);
				}
break;
   /* eol */
case 7:

# line 303 "markup.l"
			{
				    total_lineno++;
				    total_charno++;
				    doc_lineno++;
				    doc_charno++;
				    if (tag_stack_ptr > 0)
				      add_to_text_fill(" ");
				}
break;
   /* NE open tags (including attributes) */
case 8:

# line 314 "markup.l"
  case 9:

# line 315 "markup.l"
      case 10:

# line 316 "markup.l"
      {
				    total_ignore += markup_leng;
				    doc_ignore += markup_leng;
				    if ( section_stack[section_stack_ptr-1] == IGNORE)
					;
				    else
				      start_ne_instance(markup_text);
				}
break;
   /* NE close tags */
case 11:

# line 328 "markup.l"
 case 12:

# line 329 "markup.l"
     case 13:

# line 330 "markup.l"
     {
				    total_ignore += markup_leng;
				    doc_ignore += markup_leng;
				    if ( section_stack[section_stack_ptr-1] == IGNORE)
					;
				    else
				      finish_ne_instance(markup_text);
				}
break;
case -1:
break;
default:
(void)fprintf(markup_out,"bad switch markup_look %d",nstr);
} return(0); }
/* end of markup_lex */

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
 * markup_error -- what to do in the case of a syntax error.
 */

#if UsePrototypes
void
markup_error(const char *msg)
#else
void
markup_error(msg)
char *msg;
#endif  /* UsePrototypes */
{
    printf("(named_entity) %d: %s at '%s'\n", total_lineno, msg, markup_text);
}

/*
 * markup_wrap -- what to do at the end of the file
 */

#if UsePrototypes
int
markup_wrap(void)
#else
int
markup_wrap()
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

int markup_vstop[] = {
0,

6,
0,

7,
0,

6,
0,

3,
6,
0,

4,
6,
0,

5,
7,
0,

3,
6,
0,

1,
0,

1,
0,

1,
0,

1,
0,

3,
0,

3,
0,

1,
3,
0,

1,
3,
0,

1,
3,
0,

1,
3,
0,

2,
0,

2,
0,

2,
0,

2,
0,

1,
0,

1,
0,

1,
0,

2,
3,
0,

2,
3,
0,

2,
3,
0,

2,
3,
0,

1,
3,
0,

1,
3,
0,

1,
3,
0,

2,
0,

2,
0,

2,
0,

1,
0,

1,
0,

1,
0,

2,
3,
0,

2,
3,
0,

2,
3,
0,

1,
3,
0,

1,
3,
0,

1,
3,
0,

2,
0,

2,
0,

2,
0,

1,
0,

1,
0,

1,
0,

2,
3,
0,

2,
3,
0,

2,
3,
0,

1,
3,
0,

1,
3,
0,

1,
3,
0,

2,
0,

2,
0,

2,
0,

1,
0,

1,
0,

1,
0,

2,
3,
0,

2,
3,
0,

2,
3,
0,

1,
3,
0,

1,
3,
0,

1,
3,
0,

2,
0,

2,
0,

2,
0,

1,
0,

9,
0,

10,
0,

2,
3,
0,

2,
3,
0,

2,
3,
0,

1,
3,
0,

3,
0,

3,
0,

2,
0,

12,
0,

13,
0,

8,
0,

2,
3,
0,

3,
0,

11,
0,
0};
# define YYTYPE unsigned char
struct markup_work { YYTYPE verify, advance; } markup_crank[] = {
0,0,	0,0,	1,5,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,5,	1,6,	
17,0,	17,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	3,8,	0,0,	0,0,	
0,0,	0,0,	0,0,	17,0,	
0,0,	3,9,	3,10,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,5,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,7,	2,7,	1,5,	
4,11,	17,0,	1,5,	0,0,	
27,40,	0,0,	1,5,	0,0,	
41,53,	0,0,	1,5,	42,54,	
3,8,	16,29,	1,5,	1,5,	
0,0,	0,0,	14,27,	28,41,	
0,0,	1,5,	1,5,	0,0,	
3,11,	1,5,	3,9,	0,0,	
0,0,	3,8,	0,0,	0,0,	
15,28,	3,8,	29,42,	40,52,	
27,40,	3,8,	52,64,	0,0,	
41,53,	3,8,	3,8,	42,54,	
0,0,	16,29,	0,0,	0,0,	
3,8,	3,8,	14,27,	28,41,	
3,8,	7,12,	7,13,	7,13,	
7,13,	7,13,	7,13,	7,13,	
7,13,	7,13,	7,13,	7,13,	
15,28,	53,65,	29,42,	40,52,	
54,66,	64,76,	52,64,	7,13,	
7,13,	7,13,	7,13,	7,14,	
7,13,	7,13,	7,13,	7,13,	
7,13,	7,13,	7,13,	7,13,	
7,15,	7,13,	7,13,	7,13,	
7,13,	7,13,	7,16,	7,13,	
7,13,	7,13,	7,13,	7,13,	
7,13,	53,65,	0,0,	0,0,	
54,66,	64,76,	0,0,	7,13,	
7,13,	7,13,	7,13,	7,14,	
7,13,	7,13,	7,13,	7,13,	
7,13,	7,13,	7,13,	7,13,	
7,15,	7,13,	7,13,	7,13,	
7,13,	7,13,	7,16,	7,13,	
7,13,	7,13,	7,13,	7,13,	
7,13,	8,17,	0,0,	11,17,	
23,23,	24,23,	25,23,	0,0,	
0,0,	8,0,	8,0,	11,0,	
11,0,	23,23,	24,37,	25,23,	
0,0,	0,0,	0,0,	23,23,	
24,23,	25,23,	25,38,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
8,0,	0,0,	11,0,	0,0,	
23,23,	24,23,	25,23,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	23,23,	24,37,	25,23,	
8,17,	11,18,	11,19,	23,23,	
24,23,	25,23,	25,38,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	8,0,	0,0,	
11,0,	8,17,	0,0,	11,19,	
0,0,	8,17,	0,0,	11,20,	
0,0,	8,17,	0,0,	11,19,	
0,0,	8,17,	8,17,	11,19,	
11,21,	0,0,	0,0,	0,0,	
8,17,	8,17,	11,22,	11,19,	
8,17,	0,0,	11,19,	12,23,	
12,23,	12,23,	12,23,	12,23,	
12,23,	12,23,	12,23,	12,23,	
12,23,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
12,23,	12,23,	12,23,	12,23,	
12,24,	12,23,	12,23,	12,23,	
12,23,	12,23,	12,23,	12,23,	
12,23,	12,25,	12,23,	12,23,	
12,23,	12,23,	12,23,	12,26,	
12,23,	12,23,	12,23,	12,23,	
12,23,	12,23,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
12,23,	12,23,	12,23,	12,23,	
12,24,	12,23,	12,23,	12,23,	
12,23,	12,23,	12,23,	12,23,	
12,23,	12,25,	12,23,	12,23,	
12,23,	12,23,	12,23,	12,26,	
12,23,	12,23,	12,23,	12,23,	
12,23,	12,23,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	13,13,	13,13,	13,13,	
13,13,	18,0,	18,0,	19,0,	
19,0,	26,23,	0,0,	0,0,	
0,0,	26,39,	0,0,	0,0,	
0,0,	0,0,	26,23,	37,49,	
0,0,	0,0,	0,0,	37,23,	
26,23,	0,0,	0,0,	0,0,	
18,0,	0,0,	19,0,	0,0,	
37,23,	0,0,	0,0,	0,0,	
0,0,	0,0,	37,23,	20,0,	
20,0,	26,23,	0,0,	0,0,	
18,30,	26,39,	19,19,	0,0,	
0,0,	0,0,	26,23,	37,49,	
0,0,	0,0,	0,0,	37,23,	
26,23,	0,0,	18,0,	0,0,	
19,0,	18,30,	20,0,	19,19,	
37,23,	18,31,	0,0,	19,19,	
0,0,	18,30,	37,23,	19,19,	
0,0,	18,30,	18,32,	19,19,	
19,19,	0,0,	20,19,	0,0,	
18,33,	18,30,	19,19,	19,19,	
18,30,	0,0,	19,19,	21,0,	
21,0,	22,0,	22,0,	50,62,	
20,0,	0,0,	0,0,	20,19,	
0,0,	0,0,	0,0,	20,19,	
50,23,	0,0,	0,0,	20,19,	
0,0,	0,0,	50,23,	20,19,	
20,34,	0,0,	21,0,	0,0,	
22,0,	0,0,	20,19,	20,19,	
0,0,	0,0,	20,19,	0,0,	
51,63,	30,0,	30,0,	50,62,	
0,0,	0,0,	21,19,	0,0,	
22,19,	51,23,	0,0,	0,0,	
50,23,	0,0,	0,0,	51,23,	
0,0,	0,0,	50,23,	0,0,	
21,0,	0,0,	22,0,	21,19,	
30,0,	22,19,	0,0,	21,19,	
0,0,	22,19,	0,0,	21,19,	
51,63,	22,36,	0,0,	21,19,	
21,19,	22,19,	22,19,	0,0,	
30,30,	51,23,	21,19,	21,35,	
22,19,	22,19,	21,19,	51,23,	
22,19,	31,0,	31,0,	32,0,	
32,0,	61,73,	30,0,	0,0,	
0,0,	30,30,	0,0,	0,0,	
0,0,	30,30,	61,23,	0,0,	
0,0,	30,30,	0,0,	0,0,	
61,23,	30,30,	30,30,	0,0,	
31,0,	0,0,	32,0,	74,88,	
30,30,	30,30,	0,0,	0,0,	
30,30,	0,0,	74,23,	33,0,	
33,0,	61,73,	0,0,	0,0,	
31,30,	0,0,	32,30,	74,23,	
0,0,	0,0,	61,23,	0,0,	
0,0,	74,23,	0,0,	0,0,	
61,23,	0,0,	31,0,	0,0,	
32,0,	31,30,	33,0,	32,30,	
0,0,	31,30,	0,0,	32,30,	
0,0,	31,30,	74,23,	32,30,	
0,0,	31,30,	31,43,	32,30,	
32,30,	0,0,	33,30,	74,23,	
31,30,	31,30,	32,30,	32,44,	
31,30,	74,23,	32,30,	34,0,	
34,0,	35,0,	35,0,	0,0,	
33,0,	0,0,	0,0,	33,30,	
0,0,	0,0,	0,0,	33,30,	
0,0,	0,0,	0,0,	33,45,	
0,0,	0,0,	0,0,	33,30,	
33,30,	0,0,	34,0,	0,0,	
35,0,	0,0,	33,30,	33,30,	
0,0,	0,0,	33,30,	0,0,	
0,0,	36,0,	36,0,	0,0,	
0,0,	0,0,	34,19,	0,0,	
35,19,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
34,0,	0,0,	35,0,	34,46,	
36,0,	35,19,	0,0,	34,19,	
0,0,	35,19,	0,0,	34,19,	
0,0,	35,19,	0,0,	34,19,	
34,19,	35,47,	35,19,	0,0,	
36,19,	38,23,	34,19,	34,19,	
35,19,	35,19,	34,19,	0,0,	
35,19,	38,50,	38,23,	39,23,	
0,0,	0,0,	36,0,	0,0,	
38,23,	36,19,	0,0,	39,51,	
39,23,	36,19,	43,0,	43,0,	
0,0,	36,19,	39,23,	0,0,	
0,0,	36,48,	36,19,	0,0,	
0,0,	38,23,	0,0,	0,0,	
36,19,	36,19,	0,0,	0,0,	
36,19,	38,50,	38,23,	39,23,	
0,0,	43,0,	44,0,	44,0,	
38,23,	0,0,	0,0,	39,51,	
39,23,	0,0,	0,0,	0,0,	
0,0,	0,0,	39,23,	0,0,	
0,0,	43,30,	0,0,	0,0,	
45,0,	45,0,	0,0,	0,0,	
0,0,	44,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	43,0,	
0,0,	0,0,	43,55,	0,0,	
0,0,	0,0,	43,30,	0,0,	
0,0,	44,30,	43,30,	45,0,	
46,0,	46,0,	43,30,	43,30,	
0,0,	0,0,	0,0,	0,0,	
0,0,	43,30,	43,30,	44,0,	
0,0,	43,30,	44,30,	45,30,	
0,0,	0,0,	44,30,	0,0,	
0,0,	0,0,	44,30,	46,0,	
47,0,	47,0,	44,56,	44,30,	
0,0,	45,0,	0,0,	0,0,	
45,30,	44,30,	44,30,	0,0,	
45,30,	44,30,	0,0,	46,19,	
45,30,	0,0,	48,0,	48,0,	
45,57,	45,30,	0,0,	47,0,	
0,0,	0,0,	0,0,	45,30,	
45,30,	46,0,	0,0,	45,30,	
46,19,	0,0,	0,0,	0,0,	
46,19,	0,0,	0,0,	47,19,	
46,19,	48,0,	0,0,	0,0,	
46,58,	46,19,	0,0,	0,0,	
0,0,	0,0,	0,0,	46,19,	
46,19,	47,0,	0,0,	46,19,	
47,19,	48,19,	49,23,	0,0,	
47,59,	55,0,	55,0,	0,0,	
47,19,	0,0,	49,61,	49,23,	
47,19,	47,19,	0,0,	48,0,	
0,0,	49,23,	48,19,	47,19,	
47,19,	0,0,	48,60,	47,19,	
0,0,	0,0,	48,19,	0,0,	
55,0,	75,89,	48,19,	48,19,	
56,0,	56,0,	49,23,	0,0,	
75,23,	48,19,	48,19,	0,0,	
0,0,	48,19,	49,61,	49,23,	
55,30,	75,23,	0,0,	0,0,	
0,0,	49,23,	0,0,	75,23,	
0,0,	0,0,	0,0,	56,0,	
57,0,	57,0,	55,0,	0,0,	
0,0,	55,30,	0,0,	0,0,	
0,0,	55,30,	0,0,	0,0,	
75,23,	55,30,	0,0,	56,30,	
0,0,	55,67,	55,30,	0,0,	
0,0,	75,23,	0,0,	57,0,	
55,30,	55,30,	87,94,	75,23,	
55,30,	56,0,	0,0,	0,0,	
56,30,	87,23,	58,0,	58,0,	
56,68,	0,0,	0,0,	57,30,	
56,30,	0,0,	87,23,	0,0,	
56,30,	56,30,	0,0,	0,0,	
87,23,	0,0,	0,0,	56,30,	
56,30,	57,0,	0,0,	56,30,	
57,30,	58,0,	59,0,	59,0,	
57,69,	0,0,	0,0,	0,0,	
57,30,	87,23,	0,0,	0,0,	
57,30,	57,30,	0,0,	0,0,	
0,0,	58,19,	87,23,	57,30,	
57,30,	0,0,	0,0,	57,30,	
87,23,	59,0,	60,0,	60,0,	
0,0,	0,0,	0,0,	58,0,	
0,0,	0,0,	58,19,	0,0,	
0,0,	0,0,	58,70,	0,0,	
0,0,	59,19,	58,19,	0,0,	
0,0,	0,0,	58,19,	58,19,	
0,0,	60,0,	0,0,	0,0,	
0,0,	58,19,	58,19,	59,0,	
0,0,	58,19,	59,19,	0,0,	
0,0,	0,0,	59,19,	0,0,	
0,0,	60,19,	59,19,	0,0,	
0,0,	0,0,	59,19,	59,19,	
0,0,	0,0,	62,23,	0,0,	
65,77,	59,19,	59,19,	60,0,	
0,0,	59,71,	60,19,	62,23,	
65,77,	65,77,	60,19,	0,0,	
63,23,	62,23,	60,19,	0,0,	
0,0,	62,74,	60,19,	60,19,	
0,0,	63,23,	0,0,	0,0,	
66,79,	60,19,	60,19,	63,23,	
0,0,	60,72,	62,23,	63,75,	
66,79,	66,79,	0,0,	0,0,	
0,0,	0,0,	0,0,	62,23,	
0,0,	0,0,	0,0,	0,0,	
63,23,	62,23,	0,0,	65,65,	
0,0,	62,74,	67,0,	67,0,	
0,0,	63,23,	0,0,	0,0,	
0,0,	0,0,	0,0,	63,23,	
0,0,	65,78,	0,0,	63,75,	
65,65,	0,0,	0,0,	0,0,	
65,65,	0,0,	0,0,	66,66,	
65,65,	67,0,	68,0,	68,0,	
65,65,	65,65,	0,0,	0,0,	
0,0,	0,0,	0,0,	65,65,	
65,65,	66,80,	0,0,	65,65,	
66,66,	67,30,	0,0,	0,0,	
66,66,	0,0,	0,0,	0,0,	
66,66,	68,0,	69,0,	69,0,	
66,66,	66,66,	0,0,	67,0,	
0,0,	0,0,	67,30,	66,66,	
66,66,	0,0,	67,81,	66,66,	
0,0,	68,30,	67,30,	0,0,	
70,0,	70,0,	67,30,	67,30,	
0,0,	69,0,	0,0,	0,0,	
0,0,	67,30,	67,30,	68,0,	
0,0,	67,30,	68,30,	0,0,	
71,85,	0,0,	68,30,	0,0,	
0,0,	69,30,	68,30,	70,0,	
0,0,	0,0,	68,30,	68,30,	
0,0,	0,0,	72,86,	0,0,	
0,0,	68,30,	68,30,	69,0,	
0,0,	68,82,	69,30,	70,19,	
73,23,	0,0,	69,30,	0,0,	
0,0,	0,0,	69,30,	0,0,	
0,0,	73,23,	69,30,	69,30,	
0,0,	70,0,	0,0,	73,23,	
70,19,	69,30,	69,30,	73,87,	
70,19,	69,83,	76,90,	71,71,	
70,19,	0,0,	0,0,	0,0,	
70,19,	70,19,	76,90,	76,90,	
73,23,	0,0,	0,0,	70,19,	
70,19,	72,72,	0,0,	70,84,	
71,71,	73,23,	0,0,	0,0,	
71,71,	0,0,	0,0,	73,23,	
71,71,	0,0,	0,0,	73,87,	
71,71,	71,71,	72,72,	77,77,	
0,0,	0,0,	72,72,	71,71,	
71,71,	0,0,	72,72,	71,71,	
84,93,	0,0,	72,72,	72,72,	
0,0,	76,76,	0,0,	0,0,	
77,77,	72,72,	72,72,	0,0,	
77,77,	72,72,	0,0,	0,0,	
77,77,	0,0,	0,0,	76,91,	
77,77,	77,77,	76,76,	79,79,	
0,0,	0,0,	76,76,	77,77,	
77,77,	0,0,	76,76,	77,77,	
81,0,	81,0,	76,76,	76,76,	
0,0,	0,0,	0,0,	0,0,	
79,79,	76,76,	76,76,	0,0,	
79,79,	76,76,	0,0,	84,84,	
79,79,	0,0,	82,0,	82,0,	
79,79,	79,79,	0,0,	81,0,	
0,0,	0,0,	0,0,	79,79,	
79,79,	0,0,	0,0,	79,79,	
84,84,	0,0,	0,0,	0,0,	
84,84,	0,0,	0,0,	81,30,	
84,84,	82,0,	83,0,	83,0,	
84,84,	84,84,	0,0,	0,0,	
0,0,	0,0,	0,0,	84,84,	
84,84,	81,0,	0,0,	84,84,	
81,30,	82,30,	85,85,	0,0,	
81,30,	0,0,	0,0,	0,0,	
81,30,	83,0,	0,0,	0,0,	
81,30,	81,30,	0,0,	82,88,	
86,86,	0,0,	82,30,	81,30,	
81,30,	0,0,	82,30,	81,92,	
0,0,	83,30,	82,30,	0,0,	
0,0,	0,0,	82,30,	82,30,	
0,0,	0,0,	0,0,	0,0,	
0,0,	82,30,	82,30,	83,89,	
0,0,	82,30,	83,30,	0,0,	
0,0,	0,0,	83,30,	0,0,	
0,0,	85,85,	83,30,	0,0,	
92,0,	92,0,	83,30,	83,30,	
0,0,	0,0,	0,0,	0,0,	
0,0,	83,30,	83,30,	86,86,	
0,0,	83,30,	85,85,	0,0,	
93,93,	0,0,	85,85,	0,0,	
0,0,	0,0,	85,85,	92,0,	
0,0,	90,90,	85,85,	85,85,	
86,86,	0,0,	0,0,	0,0,	
86,86,	85,85,	85,85,	0,0,	
86,86,	85,85,	0,0,	92,30,	
86,86,	86,86,	90,90,	0,0,	
0,0,	0,0,	90,90,	86,86,	
86,86,	0,0,	90,90,	86,86,	
0,0,	92,94,	90,90,	90,90,	
92,30,	0,0,	0,0,	0,0,	
92,30,	90,90,	90,90,	93,93,	
92,30,	90,90,	0,0,	0,0,	
92,30,	92,30,	0,0,	0,0,	
0,0,	0,0,	0,0,	92,30,	
92,30,	0,0,	0,0,	92,30,	
93,93,	0,0,	0,0,	0,0,	
93,93,	0,0,	0,0,	0,0,	
93,93,	0,0,	0,0,	0,0,	
93,93,	93,93,	0,0,	0,0,	
0,0,	0,0,	0,0,	93,93,	
93,93,	0,0,	0,0,	93,93,	
0,0};
struct markup_svf markup_svec[] = {
0,	0,	0,
markup_crank+-1,	0,		0,	
markup_crank+-2,	markup_svec+1,	0,	
markup_crank+-28,	0,		0,	
markup_crank+-4,	markup_svec+3,	0,	
markup_crank+0,	0,		markup_vstop+1,
markup_crank+0,	0,		markup_vstop+3,
markup_crank+70,	0,		markup_vstop+5,
markup_crank+-192,	0,		markup_vstop+7,
markup_crank+0,	0,		markup_vstop+10,
markup_crank+0,	0,		markup_vstop+13,
markup_crank+-194,	0,		markup_vstop+16,
markup_crank+235,	0,		0,	
markup_crank+310,	0,		markup_vstop+19,
markup_crank+4,	markup_svec+13,	markup_vstop+21,
markup_crank+11,	markup_svec+13,	markup_vstop+23,
markup_crank+4,	markup_svec+13,	markup_vstop+25,
markup_crank+-3,	markup_svec+8,	markup_vstop+27,
markup_crank+-424,	markup_svec+8,	markup_vstop+29,
markup_crank+-426,	markup_svec+8,	markup_vstop+31,
markup_crank+-458,	markup_svec+8,	markup_vstop+34,
markup_crank+-506,	markup_svec+8,	markup_vstop+37,
markup_crank+-508,	markup_svec+8,	markup_vstop+40,
markup_crank+127,	markup_svec+12,	markup_vstop+43,
markup_crank+128,	markup_svec+12,	markup_vstop+45,
markup_crank+129,	markup_svec+12,	markup_vstop+47,
markup_crank+368,	markup_svec+12,	markup_vstop+49,
markup_crank+3,	markup_svec+13,	markup_vstop+51,
markup_crank+6,	markup_svec+13,	markup_vstop+53,
markup_crank+21,	markup_svec+13,	markup_vstop+55,
markup_crank+-540,	markup_svec+8,	markup_vstop+57,
markup_crank+-588,	markup_svec+8,	markup_vstop+60,
markup_crank+-590,	markup_svec+8,	markup_vstop+63,
markup_crank+-622,	markup_svec+8,	markup_vstop+66,
markup_crank+-670,	markup_svec+8,	markup_vstop+69,
markup_crank+-672,	markup_svec+8,	markup_vstop+72,
markup_crank+-704,	markup_svec+8,	markup_vstop+75,
markup_crank+382,	markup_svec+12,	markup_vstop+78,
markup_crank+684,	markup_svec+12,	markup_vstop+80,
markup_crank+694,	markup_svec+12,	markup_vstop+82,
markup_crank+22,	markup_svec+13,	markup_vstop+84,
markup_crank+3,	markup_svec+13,	markup_vstop+86,
markup_crank+6,	markup_svec+13,	markup_vstop+88,
markup_crank+-765,	markup_svec+8,	markup_vstop+90,
markup_crank+-789,	markup_svec+8,	markup_vstop+93,
markup_crank+-807,	markup_svec+8,	markup_vstop+96,
markup_crank+-831,	markup_svec+8,	markup_vstop+99,
markup_crank+-855,	markup_svec+8,	markup_vstop+102,
markup_crank+-873,	markup_svec+8,	markup_vstop+105,
markup_crank+853,	markup_svec+12,	markup_vstop+108,
markup_crank+450,	markup_svec+12,	markup_vstop+110,
markup_crank+479,	markup_svec+12,	markup_vstop+112,
markup_crank+33,	markup_svec+13,	markup_vstop+114,
markup_crank+41,	markup_svec+13,	markup_vstop+116,
markup_crank+44,	markup_svec+13,	markup_vstop+118,
markup_crank+-916,	markup_svec+8,	markup_vstop+120,
markup_crank+-943,	markup_svec+8,	markup_vstop+123,
markup_crank+-967,	markup_svec+8,	markup_vstop+126,
markup_crank+-1001,	markup_svec+8,	markup_vstop+129,
markup_crank+-1025,	markup_svec+8,	markup_vstop+132,
markup_crank+-1049,	markup_svec+8,	markup_vstop+135,
markup_crank+532,	markup_svec+12,	markup_vstop+138,
markup_crank+1037,	markup_svec+12,	markup_vstop+140,
markup_crank+1051,	markup_svec+12,	markup_vstop+142,
markup_crank+45,	markup_svec+13,	markup_vstop+144,
markup_crank+-1107,	0,		markup_vstop+146,
markup_crank+-1131,	0,		markup_vstop+148,
markup_crank+-1149,	markup_svec+8,	markup_vstop+150,
markup_crank+-1173,	markup_svec+8,	markup_vstop+153,
markup_crank+-1197,	markup_svec+8,	markup_vstop+156,
markup_crank+-1215,	markup_svec+8,	markup_vstop+159,
markup_crank+-1239,	markup_svec+65,	markup_vstop+162,
markup_crank+-1253,	markup_svec+66,	markup_vstop+165,
markup_crank+1195,	markup_svec+12,	markup_vstop+168,
markup_crank+561,	markup_svec+12,	markup_vstop+170,
markup_crank+887,	markup_svec+12,	markup_vstop+172,
markup_crank+-1285,	0,		markup_vstop+174,
markup_crank+-1271,	markup_svec+65,	0,	
markup_crank+0,	0,		markup_vstop+176,
markup_crank+-1303,	markup_svec+66,	0,	
markup_crank+0,	0,		markup_vstop+178,
markup_crank+-1351,	markup_svec+8,	markup_vstop+180,
markup_crank+-1369,	markup_svec+8,	markup_vstop+183,
markup_crank+-1393,	markup_svec+8,	markup_vstop+186,
markup_crank+-1327,	markup_svec+76,	markup_vstop+189,
markup_crank+-1417,	markup_svec+65,	markup_vstop+192,
markup_crank+-1431,	markup_svec+66,	markup_vstop+194,
markup_crank+940,	markup_svec+12,	markup_vstop+196,
markup_crank+0,	0,		markup_vstop+198,
markup_crank+0,	0,		markup_vstop+200,
markup_crank+-1445,	markup_svec+76,	0,	
markup_crank+0,	0,		markup_vstop+202,
markup_crank+-1459,	markup_svec+8,	markup_vstop+204,
markup_crank+-1483,	markup_svec+76,	markup_vstop+207,
markup_crank+0,	0,		markup_vstop+209,
0,	0,	0};
struct markup_work *markup_top = markup_crank+1571;
struct markup_svf *markup_bgin = markup_svec+1;
char markup_match[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
 48,  48,  48,  48,  48,  48,  48,  48, 
 48,  48,   1,   1,   1,   1,  62,   1, 
  1,  65,  48,  48,  48,  69,  48,  48, 
 48,  73,  48,  48,  48,  77,  78,  48, 
 48,  48,  48,  48,  84,  85,  48,  48, 
 88,  48,  48,   1,   1,   1,   1,   1, 
  1,  65,  48,  48,  48,  69,  48,  48, 
 48,  73,  48,  48,  48,  77,  78,  48, 
 48,  48,  48,  48,  84,  85,  48,  48, 
 88,  48,  48,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
0};
char markup_extra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int markup_lineno =1;
# define YYU(x) x
# define NLSTATE markup_previous=YYNEWLINE
struct markup_svf *markup_lstate [YYLMAX], **markup_lsp, **markup_olsp;
char markup_sbuf[YYLMAX];
char *markup_sptr = markup_sbuf;
int *markup_fnd;
extern struct markup_svf *markup_estate;
int markup_previous = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int markup_look(void)
#else
markup_look()
#endif
{
	register struct markup_svf *markup_state, **lsp;
	register struct markup_work *markup_t;
	struct markup_svf *markup_z;
	int markup_ch, markup_first;
	struct markup_work *markup_r;
# ifdef LEXDEBUG
	int debug;
# endif
	char *markup_lastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	markup_first=1;
	if (!markup_morfg)
		markup_lastch = markup_text;
	else {
		markup_morfg=0;
		markup_lastch = markup_text+markup_leng;
		}
	for(;;){
		lsp = markup_lstate;
		markup_estate = markup_state = markup_bgin;
		if (markup_previous==YYNEWLINE) markup_state++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(markup_out,"state %d\n",markup_state-markup_svec-1);
# endif
			markup_t = markup_state->markup_stoff;
			if(markup_t == markup_crank && !markup_first){  /* may not be any transitions */
				markup_z = markup_state->markup_other;
				if(markup_z == 0)break;
				if(markup_z->markup_stoff == markup_crank)break;
				}
#ifndef __cplusplus
			*markup_lastch++ = markup_ch = input();
#else
			*markup_lastch++ = markup_ch = lex_input();
#endif
			if(markup_lastch > &markup_text[YYLMAX]) {
				fprintf(markup_out,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			markup_first=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(markup_out,"char ");
				allprint(markup_ch);
				putchar('\n');
				}
# endif
			markup_r = markup_t;
			if ( (int)markup_t > (int)markup_crank){
				markup_t = markup_r + markup_ch;
				if (markup_t <= markup_top && markup_t->verify+markup_svec == markup_state){
					if(markup_t->advance+markup_svec == YYLERR)	/* error transitions */
						{unput(*--markup_lastch);break;}
					*lsp++ = markup_state = markup_t->advance+markup_svec;
					if(lsp > &markup_lstate[YYLMAX]) {
						fprintf(markup_out,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)markup_t < (int)markup_crank) {		/* r < markup_crank */
				markup_t = markup_r = markup_crank+(markup_crank-markup_t);
# ifdef LEXDEBUG
				if(debug)fprintf(markup_out,"compressed state\n");
# endif
				markup_t = markup_t + markup_ch;
				if(markup_t <= markup_top && markup_t->verify+markup_svec == markup_state){
					if(markup_t->advance+markup_svec == YYLERR)	/* error transitions */
						{unput(*--markup_lastch);break;}
					*lsp++ = markup_state = markup_t->advance+markup_svec;
					if(lsp > &markup_lstate[YYLMAX]) {
						fprintf(markup_out,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				markup_t = markup_r + YYU(markup_match[markup_ch]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(markup_out,"try fall back character ");
					allprint(YYU(markup_match[markup_ch]));
					putchar('\n');
					}
# endif
				if(markup_t <= markup_top && markup_t->verify+markup_svec == markup_state){
					if(markup_t->advance+markup_svec == YYLERR)	/* error transition */
						{unput(*--markup_lastch);break;}
					*lsp++ = markup_state = markup_t->advance+markup_svec;
					if(lsp > &markup_lstate[YYLMAX]) {
						fprintf(markup_out,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((markup_state = markup_state->markup_other) && (markup_t= markup_state->markup_stoff) != markup_crank){
# ifdef LEXDEBUG
				if(debug)fprintf(markup_out,"fall back to state %d\n",markup_state-markup_svec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--markup_lastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(markup_out,"state %d char ",markup_state-markup_svec-1);
				allprint(markup_ch);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(markup_out,"stopped at %d with ",*(lsp-1)-markup_svec-1);
			allprint(markup_ch);
			putchar('\n');
			}
# endif
		while (lsp-- > markup_lstate){
			*markup_lastch-- = 0;
			if (*lsp != 0 && (markup_fnd= (*lsp)->markup_stops) && *markup_fnd > 0){
				markup_olsp = lsp;
				if(markup_extra[*markup_fnd]){		/* must backup */
					while(markup_back((*lsp)->markup_stops,-*markup_fnd) != 1 && lsp > markup_lstate){
						lsp--;
						unput(*markup_lastch--);
						}
					}
				markup_previous = YYU(*markup_lastch);
				markup_lsp = lsp;
				markup_leng = markup_lastch-markup_text+1;
				markup_text[markup_leng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(markup_out,"\nmatch ");
					sprint(markup_text);
					fprintf(markup_out," action %d\n",*markup_fnd);
					}
# endif
				return(*markup_fnd++);
				}
			unput(*markup_lastch);
			}
		if (markup_text[0] == 0  /* && feof(markup_in) */)
			{
			markup_sptr=markup_sbuf;
			return(0);
			}
#ifndef __cplusplus
		markup_previous = markup_text[0] = input();
		if (markup_previous>0)
			output(markup_previous);
#else
		markup_previous = markup_text[0] = lex_input();
		if (markup_previous>0)
			lex_output(markup_previous);
#endif
		markup_lastch=markup_text;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int markup_back(int *p, int m)
#else
markup_back(p, m)
	int *p;
#endif
{
	if (p==0) return(0);
	while (*p) {
		if (*p++ == m)
			return(1);
	}
	return(0);
}
	/* the following are only used in the lex library */
#if defined(__cplusplus) || defined(__STDC__)
int markup_input(void)
#else
markup_input()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void markup_output(int c)
#else
markup_output(c)
  int c; 
#endif
{
#ifndef __cplusplus
	output(c);
#else
	lex_output(c);
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void markup_unput(int c)
#else
markup_unput(c)
   int c; 
#endif
{
	unput(c);
	}
