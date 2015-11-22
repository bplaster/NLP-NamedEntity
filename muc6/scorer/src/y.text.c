
# line 2 "text.y"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	text.y
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the yacc specifications for the MUC
 *	text file parser.
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
static	char	SccsId[] = "@(#)text.y	1.4 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc_protos.h"	/* nested include of muc.h */


extern void textserror();

extern	int	textsleng;
extern	long	textlineno;
extern	long	textcharno;
extern	long	textbegin;
extern	long	textend;
extern	char	*textfname;


Catalog_entry	*newentry;
File_section    *newsection;
char		*docnum;


# line 62 "text.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 
{
    int   ival;
    char  *sval;
} YYSTYPE;
# define DOC_START 257
# define DOC_END 258
# define DOCNO_START 259
# define DOCNO_END 260
# define SGML 261
# define STRING 262

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef textserror
	void textserror(const char *);
#endif

#ifndef textslex
#ifdef __EXTERN_C__
	extern "C" { int textslex(void); }
#else
	int textslex(void);
#endif
#endif
	int textsparse(void);

#endif
#define textsclearin textschar = -1
#define textserrok textserrflag = 0
extern int textschar;
extern int textserrflag;
YYSTYPE textslval;
YYSTYPE textsval;
typedef int textstabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int texts_textss[YYMAXDEPTH], *textss = texts_textss;
YYSTYPE texts_textsv[YYMAXDEPTH], *textsv = texts_textsv;
#else	/* user does initial allocation */
int *textss;
YYSTYPE *textsv;
#endif
static int textsmaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 148 "text.y"


textstabelem textsexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 17
# define YYLAST 36
textstabelem textsact[]={

    20,     5,    12,     6,     6,     6,     6,    18,    23,    11,
     4,     4,     9,    19,     8,     2,    17,     7,    22,    15,
    16,    13,    10,    15,    14,     3,     1,    21,     0,     0,
    24,    15,     0,     0,    15,    25 };
textstabelem textspact[]={

  -256,  -256,-10000000,  -257,-10000000,-10000000,-10000000,-10000000,  -259,  -257,
  -255,-10000000,-10000000,  -258,  -259,-10000000,  -252,  -259,-10000000,-10000000,
-10000000,  -258,-10000000,-10000000,  -259,-10000000 };
textstabelem textspgo[]={

     0,    26,    15,    25,    14,    12,    13,     9,    22,    20,
    18,    16 };
textstabelem textsr1[]={

     0,     1,     1,     2,     2,     2,     3,     6,     4,     8,
     9,     9,    11,    10,     5,     5,     7 };
textstabelem textsr2[]={

     0,     2,     4,     8,    10,     2,     3,     3,     6,     3,
     2,     4,     3,     3,     2,     4,     3 };
textstabelem textschk[]={

-10000000,    -1,    -2,    -3,    -7,   257,   262,    -2,    -4,    -5,
    -8,    -7,   259,    -5,    -4,    -7,    -9,   -11,   262,    -6,
   258,    -5,   -10,   260,    -5,    -6 };
textstabelem textsdef[]={

     0,    -2,     1,     0,     5,     6,    16,     2,     0,     0,
     0,    14,     9,     0,     0,    15,     0,    10,    12,     3,
     7,     0,     8,    13,    11,     4 };
typedef struct
#ifdef __cplusplus
	textstoktype
#endif
{ char *t_name; int t_val; } textstoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

textstoktype textstoks[] =
{
	"DOC_START",	257,
	"DOC_END",	258,
	"DOCNO_START",	259,
	"DOCNO_END",	260,
	"SGML",	261,
	"STRING",	262,
	"-unknown-",	-1	/* ends search */
};

char * textsreds[] =
{
	"-no such reduction-",
	"doc_specs : doc_spec",
	"doc_specs : doc_specs doc_spec",
	"doc_spec : doc_start docno_spec text_fills doc_end",
	"doc_spec : doc_start text_fills docno_spec text_fills doc_end",
	"doc_spec : text",
	"doc_start : DOC_START",
	"doc_end : DOC_END",
	"docno_spec : docno_start docno_text docno_end",
	"docno_start : DOCNO_START",
	"docno_text : docno",
	"docno_text : docno text_fills",
	"docno : STRING",
	"docno_end : DOCNO_END",
	"text_fills : text",
	"text_fills : text_fills text",
	"text : STRING",
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
#define YYERROR		goto textserrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( textschar >= 0 || ( textsr2[ textstmp ] >> 1 ) != 1 )\
	{\
		textserror( "syntax error - cannot backup" );\
		goto textserrlab;\
	}\
	textschar = newtoken;\
	textsstate = *textsps;\
	textslval = newvalue;\
	goto textsnewstate;\
}
#define YYRECOVERING()	(!!textserrflag)
#define YYNEW(type)	malloc(sizeof(type) * textsnewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, textsnewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, textsnewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int textsdebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *textspv;			/* top of value stack */
int *textsps;			/* top of state stack */

int textsstate;			/* current state */
int textstmp;			/* extra var (lasts between blocks) */

int textsnerrs;			/* number of errors */
int textserrflag;			/* error recovery flag */
int textschar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		textscvtok(textslex())
/*
** textscvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int textscvtok(int i)
#else
int textscvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( textsmbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = textsmbchars[mid].character;
			if( j==i ){/*Found*/ 
				return textsmbchars[mid].tvalue;
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
#define YYLEX()		textslex()
#endif/*!YYNMBCHARS*/

/*
** textsparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int textsparse(void)
#else
int textsparse()
#endif
{
	register YYSTYPE *textspvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; textspvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto textserrlab;
		case 2: goto textsnewstate;
	}
	textspvt = 0;
#endif

	/*
	** Initialize externals - textsparse may be called more than once
	*/
	textspv = &textsv[-1];
	textsps = &textss[-1];
	textsstate = 0;
	textstmp = 0;
	textsnerrs = 0;
	textserrflag = 0;
	textschar = -1;

#if YYMAXDEPTH <= 0
	if (textsmaxdepth <= 0)
	{
		if ((textsmaxdepth = YYEXPAND(0)) <= 0)
		{
			textserror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *texts_pv;	/* top of value stack */
		register int *texts_ps;		/* top of state stack */
		register int texts_state;		/* current state */
		register int  texts_n;		/* internal state number info */
	goto textsstack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	textsnewstate:
		texts_pv = textspv;
		texts_ps = textsps;
		texts_state = textsstate;
		goto texts_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	textsstack:
		texts_pv = textspv;
		texts_ps = textsps;
		texts_state = textsstate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	texts_stack:
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
		if ( textsdebug )
		{
			register int texts_i;

			printf( "State %d, token ", texts_state );
			if ( textschar == 0 )
				printf( "end-of-file\n" );
			else if ( textschar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( texts_i = 0; textstoks[texts_i].t_val >= 0;
					texts_i++ )
				{
					if ( textstoks[texts_i].t_val == textschar )
						break;
				}
				printf( "%s\n", textstoks[texts_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++texts_ps >= &textss[ textsmaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int textsps_index = (texts_ps - textss);
			int textspv_index = (texts_pv - textsv);
			int textspvt_index = (textspvt - textsv);
			int textsnewmax;
#ifdef YYEXPAND
			textsnewmax = YYEXPAND(textsmaxdepth);
#else
			textsnewmax = 2 * textsmaxdepth;	/* double table size */
			if (textsmaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newtextss = (char *)YYNEW(int);
				char *newtextsv = (char *)YYNEW(YYSTYPE);
				if (newtextss != 0 && newtextsv != 0)
				{
					textss = YYCOPY(newtextss, textss, int);
					textsv = YYCOPY(newtextsv, textsv, YYSTYPE);
				}
				else
					textsnewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				textss = YYENLARGE(textss, int);
				textsv = YYENLARGE(textsv, YYSTYPE);
				if (textss == 0 || textsv == 0)
					textsnewmax = 0;	/* failed */
			}
#endif
			if (textsnewmax <= textsmaxdepth)	/* tables not expanded */
			{
				textserror( "yacc stack overflow" );
				YYABORT;
			}
			textsmaxdepth = textsnewmax;

			texts_ps = textss + textsps_index;
			texts_pv = textsv + textspv_index;
			textspvt = textsv + textspvt_index;
		}
		*texts_ps = texts_state;
		*++texts_pv = textsval;

		/*
		** we have a new state - find out what to do
		*/
	texts_newstate:
		if ( ( texts_n = textspact[ texts_state ] ) <= YYFLAG )
			goto textsdefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		textstmp = textschar < 0;
#endif
		if ( ( textschar < 0 ) && ( ( textschar = YYLEX() ) < 0 ) )
			textschar = 0;		/* reached EOF */
#if YYDEBUG
		if ( textsdebug && textstmp )
		{
			register int texts_i;

			printf( "Received token " );
			if ( textschar == 0 )
				printf( "end-of-file\n" );
			else if ( textschar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( texts_i = 0; textstoks[texts_i].t_val >= 0;
					texts_i++ )
				{
					if ( textstoks[texts_i].t_val == textschar )
						break;
				}
				printf( "%s\n", textstoks[texts_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( texts_n += textschar ) < 0 ) || ( texts_n >= YYLAST ) )
			goto textsdefault;
		if ( textschk[ texts_n = textsact[ texts_n ] ] == textschar )	/*valid shift*/
		{
			textschar = -1;
			textsval = textslval;
			texts_state = texts_n;
			if ( textserrflag > 0 )
				textserrflag--;
			goto texts_stack;
		}

	textsdefault:
		if ( ( texts_n = textsdef[ texts_state ] ) == -2 )
		{
#if YYDEBUG
			textstmp = textschar < 0;
#endif
			if ( ( textschar < 0 ) && ( ( textschar = YYLEX() ) < 0 ) )
				textschar = 0;		/* reached EOF */
#if YYDEBUG
			if ( textsdebug && textstmp )
			{
				register int texts_i;

				printf( "Received token " );
				if ( textschar == 0 )
					printf( "end-of-file\n" );
				else if ( textschar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( texts_i = 0;
						textstoks[texts_i].t_val >= 0;
						texts_i++ )
					{
						if ( textstoks[texts_i].t_val
							== textschar )
						{
							break;
						}
					}
					printf( "%s\n", textstoks[texts_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *textsxi = textsexca;

				while ( ( *textsxi != -1 ) ||
					( textsxi[1] != texts_state ) )
				{
					textsxi += 2;
				}
				while ( ( *(textsxi += 2) >= 0 ) &&
					( *textsxi != textschar ) )
					;
				if ( ( texts_n = textsxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( texts_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( textserrflag )
			{
			case 0:		/* new error */
				textserror( "syntax error" );
				goto skip_init;
			textserrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				texts_pv = textspv;
				texts_ps = textsps;
				texts_state = textsstate;
			skip_init:
				textsnerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				textserrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( texts_ps >= textss )
				{
					texts_n = textspact[ *texts_ps ] + YYERRCODE;
					if ( texts_n >= 0 && texts_n < YYLAST &&
						textschk[textsact[texts_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						texts_state = textsact[ texts_n ];
						goto texts_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( textsdebug )
						printf( _POP_, *texts_ps,
							texts_ps[-1] );
#	undef _POP_
#endif
					texts_ps--;
					texts_pv--;
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
				if ( textsdebug )
				{
					register int texts_i;

					printf( "Error recovery discards " );
					if ( textschar == 0 )
						printf( "token end-of-file\n" );
					else if ( textschar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( texts_i = 0;
							textstoks[texts_i].t_val >= 0;
							texts_i++ )
						{
							if ( textstoks[texts_i].t_val
								== textschar )
							{
								break;
							}
						}
						printf( "token %s\n",
							textstoks[texts_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( textschar == 0 )	/* reached EOF. quit */
					YYABORT;
				textschar = -1;
				goto texts_newstate;
			}
		}/* end if ( texts_n == 0 ) */
		/*
		** reduction by production texts_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( textsdebug )
			printf( "Reduce by (%d) \"%s\"\n",
				texts_n, textsreds[ texts_n ] );
#endif
		textstmp = texts_n;			/* value to switch over */
		textspvt = texts_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using texts_state here as temporary
		** register variable, but why not, if it works...
		** If textsr2[ texts_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto texts_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int texts_len = textsr2[ texts_n ];

			if ( !( texts_len & 01 ) )
			{
				texts_len >>= 1;
				textsval = ( texts_pv -= texts_len )[1];	/* $$ = $1 */
				texts_state = textspgo[ texts_n = textsr1[ texts_n ] ] +
					*( texts_ps -= texts_len ) + 1;
				if ( texts_state >= YYLAST ||
					textschk[ texts_state =
					textsact[ texts_state ] ] != -texts_n )
				{
					texts_state = textsact[ textspgo[ texts_n ] ];
				}
				goto texts_stack;
			}
			texts_len >>= 1;
			textsval = ( texts_pv -= texts_len )[1];	/* $$ = $1 */
			texts_state = textspgo[ texts_n = textsr1[ texts_n ] ] +
				*( texts_ps -= texts_len ) + 1;
			if ( texts_state >= YYLAST ||
				textschk[ texts_state = textsact[ texts_state ] ] != -texts_n )
			{
				texts_state = textsact[ textspgo[ texts_n ] ];
			}
		}
					/* save until reenter driver code */
		textsstate = texts_state;
		textsps = texts_ps;
		textspv = texts_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( textstmp )
	{
		
case 6:
# line 86 "text.y"
{
	    ;
	} break;
case 7:
# line 92 "text.y"
{
	    /* set the end point for the text_doc File_section of this
	       entry. */
	    newsection->end = textend;
	} break;
case 9:
# line 103 "text.y"
{
	    ;
	} break;
case 12:
# line 113 "text.y"
{
	    /* extract the docnum, and create a catalog entry for this
	       document - it may be a new entry, or perhaps a parsed
	       template has already created one. */

	    docnum = nice_docnum(textslval.sval);
	    newentry = catalog_entry_create(docnum);

	    /* create a File_section for this document and set its
	       begin point. */
	    newsection = file_section_create(textfname);
	    newsection->begin = textbegin;

	    newentry->text_doc = newsection;
	    UFREE(textslval.sval);
	    UFREE(docnum);
	} break;
case 13:
# line 133 "text.y"
{
	    ;
	} break;
case 16:
# line 143 "text.y"
{
	    UFREE(textslval.sval);
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto textsstack;		/* reset registers in driver code */
}

