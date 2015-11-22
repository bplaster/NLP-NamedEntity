
# line 2 "template.y"
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


Proto	(extern void, templateserror, (const char *msg));
Proto	(extern int, templateswrap, (void));
Proto	(extern void, finish_template, (void));

Proto	(static void, start_instance, (char *instheader));
Proto	(static void, find_current_entry, ());
Proto	(static void, set_cur_entry, ());
Proto	(static void, set_cur_class_def, (char *pattern));

Proto	(static void, start_slot, (char *slotstr));
Proto	(static void, start_alt_set, (void));
Proto	(static void, start_pointer, (char *instheader));

extern	int	templatesleng;
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



# line 93 "template.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 
{
    int   ival;
    char  *sval;
} YYSTYPE;
# define INST_START 257
# define SLOTNAME 258
# define POINTER 259
# define SLOT_FILL 260
# define QSTRING 261
# define OPTMARK 262
# define ALTMARK 263

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef templateserror
	void templateserror(const char *);
#endif

#ifndef templateslex
#ifdef __EXTERN_C__
	extern "C" { int templateslex(void); }
#else
	int templateslex(void);
#endif
#endif
	int templatesparse(void);

#endif
#define templatesclearin templateschar = -1
#define templateserrok templateserrflag = 0
extern int templateschar;
extern int templateserrflag;
YYSTYPE templateslval;
YYSTYPE templatesval;
typedef int templatestabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int templates_templatess[YYMAXDEPTH], *templatess = templates_templatess;
YYSTYPE templates_templatesv[YYMAXDEPTH], *templatesv = templates_templatesv;
#else	/* user does initial allocation */
int *templatess;
YYSTYPE *templatesv;
#endif
static int templatesmaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 208 "template.y"



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

templatestabelem templatesexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 23
# define YYLAST 24
templatestabelem templatesact[]={

    21,    22,    23,    17,    18,     9,     4,    12,     7,     2,
    20,     5,    19,    16,    15,    10,    14,    13,    11,    24,
     8,     6,     3,     1 };
templatestabelem templatespact[]={

  -251,  -251,-10000000,  -253,-10000000,-10000000,  -253,-10000000,  -259,-10000000,
-10000000,  -259,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000 };
templatestabelem templatespgo[]={

     0,    23,     9,    22,    21,     8,    20,    18,     7,    17,
    16,    14,    13,    12,    10 };
templatestabelem templatesr1[]={

     0,     1,     1,     2,     3,     4,     4,     5,     5,     6,
     7,     7,     8,     8,     8,     8,     9,    10,    11,    11,
    13,    14,    12 };
templatestabelem templatesr2[]={

     0,     2,     4,     4,     3,     2,     4,     2,     4,     3,
     2,     4,     2,     2,     2,     2,     3,     3,     2,     2,
     3,     3,     3 };
templatestabelem templateschk[]={

-10000000,    -1,    -2,    -3,   257,    -2,    -4,    -5,    -6,   258,
    -5,    -7,    -8,    -9,   -10,   -11,   -12,   262,   263,   -13,
   -14,   259,   260,   261,    -8 };
templatestabelem templatesdef[]={

     0,    -2,     1,     0,     4,     2,     3,     5,     7,     9,
     6,     8,    10,    12,    13,    14,    15,    16,    17,    18,
    19,    22,    20,    21,    11 };
typedef struct
#ifdef __cplusplus
	templatestoktype
#endif
{ char *t_name; int t_val; } templatestoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

templatestoktype templatestoks[] =
{
	"INST_START",	257,
	"SLOTNAME",	258,
	"POINTER",	259,
	"SLOT_FILL",	260,
	"QSTRING",	261,
	"OPTMARK",	262,
	"ALTMARK",	263,
	"-unknown-",	-1	/* ends search */
};

char * templatesreds[] =
{
	"-no such reduction-",
	"inst_specs : inst_spec",
	"inst_specs : inst_specs inst_spec",
	"inst_spec : inst_start slot_specs",
	"inst_start : INST_START",
	"slot_specs : slot_spec",
	"slot_specs : slot_specs slot_spec",
	"slot_spec : slotname",
	"slot_spec : slotname slot_fills",
	"slotname : SLOTNAME",
	"slot_fills : slot_fill",
	"slot_fills : slot_fills slot_fill",
	"slot_fill : optmark",
	"slot_fill : altmark",
	"slot_fill : text",
	"slot_fill : pointer",
	"optmark : OPTMARK",
	"altmark : ALTMARK",
	"text : string",
	"text : qstring",
	"string : SLOT_FILL",
	"qstring : QSTRING",
	"pointer : POINTER",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.12	93/06/07 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto templateserrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( templateschar >= 0 || ( templatesr2[ templatestmp ] >> 1 ) != 1 )\
	{\
		templateserror( "syntax error - cannot backup" );\
		goto templateserrlab;\
	}\
	templateschar = newtoken;\
	templatesstate = *templatesps;\
	templateslval = newvalue;\
	goto templatesnewstate;\
}
#define YYRECOVERING()	(!!templateserrflag)
#define YYNEW(type)	malloc(sizeof(type) * templatesnewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, templatesnewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, templatesnewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int templatesdebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *templatespv;			/* top of value stack */
int *templatesps;			/* top of state stack */

int templatesstate;			/* current state */
int templatestmp;			/* extra var (lasts between blocks) */

int templatesnerrs;			/* number of errors */
int templateserrflag;			/* error recovery flag */
int templateschar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		templatescvtok(templateslex())
/*
** templatescvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int templatescvtok(int i)
#else
int templatescvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( templatesmbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = templatesmbchars[mid].character;
			if( j==i ){/*Found*/ 
				return templatesmbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		templateslex()
#endif/*!YYNMBCHARS*/

/*
** templatesparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int templatesparse(void)
#else
int templatesparse()
#endif
{
	register YYSTYPE *templatespvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; templatespvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto templateserrlab;
		case 2: goto templatesnewstate;
	}
	templatespvt = 0;
#endif

	/*
	** Initialize externals - templatesparse may be called more than once
	*/
	templatespv = &templatesv[-1];
	templatesps = &templatess[-1];
	templatesstate = 0;
	templatestmp = 0;
	templatesnerrs = 0;
	templateserrflag = 0;
	templateschar = -1;

#if YYMAXDEPTH <= 0
	if (templatesmaxdepth <= 0)
	{
		if ((templatesmaxdepth = YYEXPAND(0)) <= 0)
		{
			templateserror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *templates_pv;	/* top of value stack */
		register int *templates_ps;		/* top of state stack */
		register int templates_state;		/* current state */
		register int  templates_n;		/* internal state number info */
	goto templatesstack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	templatesnewstate:
		templates_pv = templatespv;
		templates_ps = templatesps;
		templates_state = templatesstate;
		goto templates_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	templatesstack:
		templates_pv = templatespv;
		templates_ps = templatesps;
		templates_state = templatesstate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	templates_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( templatesdebug )
		{
			register int templates_i;

			printf( "State %d, token ", templates_state );
			if ( templateschar == 0 )
				printf( "end-of-file\n" );
			else if ( templateschar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( templates_i = 0; templatestoks[templates_i].t_val >= 0;
					templates_i++ )
				{
					if ( templatestoks[templates_i].t_val == templateschar )
						break;
				}
				printf( "%s\n", templatestoks[templates_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++templates_ps >= &templatess[ templatesmaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int templatesps_index = (templates_ps - templatess);
			int templatespv_index = (templates_pv - templatesv);
			int templatespvt_index = (templatespvt - templatesv);
			int templatesnewmax;
#ifdef YYEXPAND
			templatesnewmax = YYEXPAND(templatesmaxdepth);
#else
			templatesnewmax = 2 * templatesmaxdepth;	/* double table size */
			if (templatesmaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newtemplatess = (char *)YYNEW(int);
				char *newtemplatesv = (char *)YYNEW(YYSTYPE);
				if (newtemplatess != 0 && newtemplatesv != 0)
				{
					templatess = YYCOPY(newtemplatess, templatess, int);
					templatesv = YYCOPY(newtemplatesv, templatesv, YYSTYPE);
				}
				else
					templatesnewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				templatess = YYENLARGE(templatess, int);
				templatesv = YYENLARGE(templatesv, YYSTYPE);
				if (templatess == 0 || templatesv == 0)
					templatesnewmax = 0;	/* failed */
			}
#endif
			if (templatesnewmax <= templatesmaxdepth)	/* tables not expanded */
			{
				templateserror( "yacc stack overflow" );
				YYABORT;
			}
			templatesmaxdepth = templatesnewmax;

			templates_ps = templatess + templatesps_index;
			templates_pv = templatesv + templatespv_index;
			templatespvt = templatesv + templatespvt_index;
		}
		*templates_ps = templates_state;
		*++templates_pv = templatesval;

		/*
		** we have a new state - find out what to do
		*/
	templates_newstate:
		if ( ( templates_n = templatespact[ templates_state ] ) <= YYFLAG )
			goto templatesdefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		templatestmp = templateschar < 0;
#endif
		if ( ( templateschar < 0 ) && ( ( templateschar = YYLEX() ) < 0 ) )
			templateschar = 0;		/* reached EOF */
#if YYDEBUG
		if ( templatesdebug && templatestmp )
		{
			register int templates_i;

			printf( "Received token " );
			if ( templateschar == 0 )
				printf( "end-of-file\n" );
			else if ( templateschar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( templates_i = 0; templatestoks[templates_i].t_val >= 0;
					templates_i++ )
				{
					if ( templatestoks[templates_i].t_val == templateschar )
						break;
				}
				printf( "%s\n", templatestoks[templates_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( templates_n += templateschar ) < 0 ) || ( templates_n >= YYLAST ) )
			goto templatesdefault;
		if ( templateschk[ templates_n = templatesact[ templates_n ] ] == templateschar )	/*valid shift*/
		{
			templateschar = -1;
			templatesval = templateslval;
			templates_state = templates_n;
			if ( templateserrflag > 0 )
				templateserrflag--;
			goto templates_stack;
		}

	templatesdefault:
		if ( ( templates_n = templatesdef[ templates_state ] ) == -2 )
		{
#if YYDEBUG
			templatestmp = templateschar < 0;
#endif
			if ( ( templateschar < 0 ) && ( ( templateschar = YYLEX() ) < 0 ) )
				templateschar = 0;		/* reached EOF */
#if YYDEBUG
			if ( templatesdebug && templatestmp )
			{
				register int templates_i;

				printf( "Received token " );
				if ( templateschar == 0 )
					printf( "end-of-file\n" );
				else if ( templateschar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( templates_i = 0;
						templatestoks[templates_i].t_val >= 0;
						templates_i++ )
					{
						if ( templatestoks[templates_i].t_val
							== templateschar )
						{
							break;
						}
					}
					printf( "%s\n", templatestoks[templates_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *templatesxi = templatesexca;

				while ( ( *templatesxi != -1 ) ||
					( templatesxi[1] != templates_state ) )
				{
					templatesxi += 2;
				}
				while ( ( *(templatesxi += 2) >= 0 ) &&
					( *templatesxi != templateschar ) )
					;
				if ( ( templates_n = templatesxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( templates_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( templateserrflag )
			{
			case 0:		/* new error */
				templateserror( "syntax error" );
				goto skip_init;
			templateserrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				templates_pv = templatespv;
				templates_ps = templatesps;
				templates_state = templatesstate;
			skip_init:
				templatesnerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				templateserrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( templates_ps >= templatess )
				{
					templates_n = templatespact[ *templates_ps ] + YYERRCODE;
					if ( templates_n >= 0 && templates_n < YYLAST &&
						templateschk[templatesact[templates_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						templates_state = templatesact[ templates_n ];
						goto templates_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( templatesdebug )
						printf( _POP_, *templates_ps,
							templates_ps[-1] );
#	undef _POP_
#endif
					templates_ps--;
					templates_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( templatesdebug )
				{
					register int templates_i;

					printf( "Error recovery discards " );
					if ( templateschar == 0 )
						printf( "token end-of-file\n" );
					else if ( templateschar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( templates_i = 0;
							templatestoks[templates_i].t_val >= 0;
							templates_i++ )
						{
							if ( templatestoks[templates_i].t_val
								== templateschar )
							{
								break;
							}
						}
						printf( "token %s\n",
							templatestoks[templates_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( templateschar == 0 )	/* reached EOF. quit */
					YYABORT;
				templateschar = -1;
				goto templates_newstate;
			}
		}/* end if ( templates_n == 0 ) */
		/*
		** reduction by production templates_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( templatesdebug )
			printf( "Reduce by (%d) \"%s\"\n",
				templates_n, templatesreds[ templates_n ] );
#endif
		templatestmp = templates_n;			/* value to switch over */
		templatespvt = templates_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using templates_state here as temporary
		** register variable, but why not, if it works...
		** If templatesr2[ templates_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto templates_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int templates_len = templatesr2[ templates_n ];

			if ( !( templates_len & 01 ) )
			{
				templates_len >>= 1;
				templatesval = ( templates_pv -= templates_len )[1];	/* $$ = $1 */
				templates_state = templatespgo[ templates_n = templatesr1[ templates_n ] ] +
					*( templates_ps -= templates_len ) + 1;
				if ( templates_state >= YYLAST ||
					templateschk[ templates_state =
					templatesact[ templates_state ] ] != -templates_n )
				{
					templates_state = templatesact[ templatespgo[ templates_n ] ];
				}
				goto templates_stack;
			}
			templates_len >>= 1;
			templatesval = ( templates_pv -= templates_len )[1];	/* $$ = $1 */
			templates_state = templatespgo[ templates_n = templatesr1[ templates_n ] ] +
				*( templates_ps -= templates_len ) + 1;
			if ( templates_state >= YYLAST ||
				templateschk[ templates_state = templatesact[ templates_state ] ] != -templates_n )
			{
				templates_state = templatesact[ templatespgo[ templates_n ] ];
			}
		}
					/* save until reenter driver code */
		templatesstate = templates_state;
		templatesps = templates_ps;
		templatespv = templates_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( templatestmp )
	{
		
case 4:
# line 118 "template.y"
{
	    start_instance(templateslval.sval);
	    UFREE(templateslval.sval);
	} break;
case 9:
# line 134 "template.y"
{
	    start_slot(templateslval.sval);
	    UFREE(templateslval.sval);
	} break;
case 16:
# line 152 "template.y"
{
	    cur_slot->is_optional = True;
	} break;
case 17:
# line 158 "template.y"
{
	    start_alt_set();
	} break;
case 20:
# line 168 "template.y"
{
	    this_filler = trim_quotes(templateslval.sval);

	    if (cur_altfillset)
		add_alt_string_fill(cur_altfillset, this_filler);
	    else
		add_string_fill(cur_slot, this_filler);

	    UFREE(this_filler);
	    UFREE(templateslval.sval);
	} break;
case 21:
# line 182 "template.y"
{
	    this_filler = trim_quotes(templateslval.sval);
	    trim_whitespace(this_filler);

	    if (cur_altfillset)
		add_alt_string_fill(cur_altfillset, this_filler);
	    else
		add_string_fill(cur_slot, this_filler);

	    UFREE(this_filler);
	    UFREE(templateslval.sval);
	} break;
case 22:
# line 197 "template.y"
{
	    start_pointer(templateslval.sval);
	    if (cur_altfillset)
		add_alt_pointer_fill(cur_altfillset, this_pointer);
	    else
		add_pointer_fill(cur_slot, this_pointer);
	    UFREE(templateslval.sval);
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto templatesstack;		/* reset registers in driver code */
}

