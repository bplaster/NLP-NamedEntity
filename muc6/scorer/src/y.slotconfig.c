
# line 2 "slotconfig.y"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	slotconfig.y
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the yacc specifications for the MUC
 *	slot configuration file parser.
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
static	char	SccsId[] = "@(#)slotconfig.y	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc_protos.h"	/* nested include of muc.h */


extern void slotconfigerror();
extern int slotconfigleng;
extern int slotlineno;

Class_def  *newclass;
Slot_def   *newslot;
char	   *unqstr;


# line 56 "slotconfig.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 
{
    int   ival;
    char  *sval;
} YYSTYPE;
# define NUMBER 257
# define STRING 258
# define QSTRING 259

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef slotconfigerror
	void slotconfigerror(const char *);
#endif

#ifndef slotconfiglex
#ifdef __EXTERN_C__
	extern "C" { int slotconfiglex(void); }
#else
	int slotconfiglex(void);
#endif
#endif
	int slotconfigparse(void);

#endif
#define slotconfigclearin slotconfigchar = -1
#define slotconfigerrok slotconfigerrflag = 0
extern int slotconfigchar;
extern int slotconfigerrflag;
YYSTYPE slotconfiglval;
YYSTYPE slotconfigval;
typedef int slotconfigtabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int slotconfig_slotconfigs[YYMAXDEPTH], *slotconfigs = slotconfig_slotconfigs;
YYSTYPE slotconfig_slotconfigv[YYMAXDEPTH], *slotconfigv = slotconfig_slotconfigv;
#else	/* user does initial allocation */
int *slotconfigs;
YYSTYPE *slotconfigv;
#endif
static int slotconfigmaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 168 "slotconfig.y"


slotconfigtabelem slotconfigexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 25
# define YYLAST 223
slotconfigtabelem slotconfigact[]={

    35,    15,    33,    20,    31,    32,    22,    23,    17,    18,
     8,     9,     6,    13,    11,    28,    25,     3,    30,    21,
     2,    29,     4,    26,    24,    19,    16,    14,    12,    10,
     7,     5,     1,     0,     0,     0,     0,     0,     0,     0,
    27,     0,     0,     0,     0,     0,    34,     0,    36,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    31,    32,    22,
    23,    22,    23 };
slotconfigtabelem slotconfigpact[]={

   -23,   -23,-10000000,  -246,-10000000,  -248,-10000000,-10000000,-10000000,-10000000,
   -27,-10000000,-10000000,  -257,  -250,-10000000,   -37,-10000000,-10000000,   -24,
  -252,-10000000,-10000000,-10000000,   -26,  -254,   -39,-10000000,-10000000,   -41,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000 };
slotconfigtabelem slotconfigpgo[]={

     0,    32,    20,    31,    30,    29,    28,    27,    26,    25,
    24,    23,    19,    21,    18 };
slotconfigtabelem slotconfigr1[]={

     0,     1,     1,     2,     3,     4,     4,     5,     5,     6,
     7,     8,     8,     9,     9,    11,    11,    12,    12,    10,
    10,    13,    13,    14,    14 };
slotconfigtabelem slotconfigr2[]={

     0,     2,     4,    10,     3,     3,     3,     0,     4,    12,
     3,     3,     3,     6,     2,     2,     4,     3,     3,     0,
     6,     2,     4,     2,     3 };
slotconfigtabelem slotconfigchk[]={

-10000000,    -1,    -2,    40,    -2,    -3,   258,    -4,   258,   259,
    -5,    41,    -6,    40,    -7,   258,    -8,   258,   259,    -9,
    40,   -12,   258,   259,   -10,    40,   -11,   -12,    41,   -13,
   -14,   258,   259,    41,   -12,    41,   -14 };
slotconfigtabelem slotconfigdef[]={

     0,    -2,     1,     0,     2,     0,     4,     7,     5,     6,
     0,     3,     8,     0,     0,    10,     0,    11,    12,    19,
     0,    14,    17,    18,     0,     0,     0,    15,     9,     0,
    21,    23,    24,    13,    16,    20,    22 };
typedef struct
#ifdef __cplusplus
	slotconfigtoktype
#endif
{ char *t_name; int t_val; } slotconfigtoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

slotconfigtoktype slotconfigtoks[] =
{
	"NUMBER",	257,
	"STRING",	258,
	"QSTRING",	259,
	"-unknown-",	-1	/* ends search */
};

char * slotconfigreds[] =
{
	"-no such reduction-",
	"class_specs : class_spec",
	"class_specs : class_specs class_spec",
	"class_spec : '(' class class_sgml slot_specs ')'",
	"class : STRING",
	"class_sgml : STRING",
	"class_sgml : QSTRING",
	"slot_specs : /* empty */",
	"slot_specs : slot_specs slot_spec",
	"slot_spec : '(' slot slot_sgml fill_type_spec set_fill_spec ')'",
	"slot : STRING",
	"slot_sgml : STRING",
	"slot_sgml : QSTRING",
	"fill_type_spec : '(' fill_types ')'",
	"fill_type_spec : fill_type",
	"fill_types : fill_type",
	"fill_types : fill_types fill_type",
	"fill_type : STRING",
	"fill_type : QSTRING",
	"set_fill_spec : /* empty */",
	"set_fill_spec : '(' set_fills ')'",
	"set_fills : set_fill",
	"set_fills : set_fills set_fill",
	"set_fill : STRING",
	"set_fill : QSTRING",
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
#define YYERROR		goto slotconfigerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( slotconfigchar >= 0 || ( slotconfigr2[ slotconfigtmp ] >> 1 ) != 1 )\
	{\
		slotconfigerror( "syntax error - cannot backup" );\
		goto slotconfigerrlab;\
	}\
	slotconfigchar = newtoken;\
	slotconfigstate = *slotconfigps;\
	slotconfiglval = newvalue;\
	goto slotconfignewstate;\
}
#define YYRECOVERING()	(!!slotconfigerrflag)
#define YYNEW(type)	malloc(sizeof(type) * slotconfignewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, slotconfignewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, slotconfignewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int slotconfigdebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *slotconfigpv;			/* top of value stack */
int *slotconfigps;			/* top of state stack */

int slotconfigstate;			/* current state */
int slotconfigtmp;			/* extra var (lasts between blocks) */

int slotconfignerrs;			/* number of errors */
int slotconfigerrflag;			/* error recovery flag */
int slotconfigchar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		slotconfigcvtok(slotconfiglex())
/*
** slotconfigcvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int slotconfigcvtok(int i)
#else
int slotconfigcvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( slotconfigmbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = slotconfigmbchars[mid].character;
			if( j==i ){/*Found*/ 
				return slotconfigmbchars[mid].tvalue;
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
#define YYLEX()		slotconfiglex()
#endif/*!YYNMBCHARS*/

/*
** slotconfigparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int slotconfigparse(void)
#else
int slotconfigparse()
#endif
{
	register YYSTYPE *slotconfigpvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; slotconfigpvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto slotconfigerrlab;
		case 2: goto slotconfignewstate;
	}
	slotconfigpvt = 0;
#endif

	/*
	** Initialize externals - slotconfigparse may be called more than once
	*/
	slotconfigpv = &slotconfigv[-1];
	slotconfigps = &slotconfigs[-1];
	slotconfigstate = 0;
	slotconfigtmp = 0;
	slotconfignerrs = 0;
	slotconfigerrflag = 0;
	slotconfigchar = -1;

#if YYMAXDEPTH <= 0
	if (slotconfigmaxdepth <= 0)
	{
		if ((slotconfigmaxdepth = YYEXPAND(0)) <= 0)
		{
			slotconfigerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *slotconfig_pv;	/* top of value stack */
		register int *slotconfig_ps;		/* top of state stack */
		register int slotconfig_state;		/* current state */
		register int  slotconfig_n;		/* internal state number info */
	goto slotconfigstack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	slotconfignewstate:
		slotconfig_pv = slotconfigpv;
		slotconfig_ps = slotconfigps;
		slotconfig_state = slotconfigstate;
		goto slotconfig_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	slotconfigstack:
		slotconfig_pv = slotconfigpv;
		slotconfig_ps = slotconfigps;
		slotconfig_state = slotconfigstate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	slotconfig_stack:
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
		if ( slotconfigdebug )
		{
			register int slotconfig_i;

			printf( "State %d, token ", slotconfig_state );
			if ( slotconfigchar == 0 )
				printf( "end-of-file\n" );
			else if ( slotconfigchar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( slotconfig_i = 0; slotconfigtoks[slotconfig_i].t_val >= 0;
					slotconfig_i++ )
				{
					if ( slotconfigtoks[slotconfig_i].t_val == slotconfigchar )
						break;
				}
				printf( "%s\n", slotconfigtoks[slotconfig_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++slotconfig_ps >= &slotconfigs[ slotconfigmaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int slotconfigps_index = (slotconfig_ps - slotconfigs);
			int slotconfigpv_index = (slotconfig_pv - slotconfigv);
			int slotconfigpvt_index = (slotconfigpvt - slotconfigv);
			int slotconfignewmax;
#ifdef YYEXPAND
			slotconfignewmax = YYEXPAND(slotconfigmaxdepth);
#else
			slotconfignewmax = 2 * slotconfigmaxdepth;	/* double table size */
			if (slotconfigmaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newslotconfigs = (char *)YYNEW(int);
				char *newslotconfigv = (char *)YYNEW(YYSTYPE);
				if (newslotconfigs != 0 && newslotconfigv != 0)
				{
					slotconfigs = YYCOPY(newslotconfigs, slotconfigs, int);
					slotconfigv = YYCOPY(newslotconfigv, slotconfigv, YYSTYPE);
				}
				else
					slotconfignewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				slotconfigs = YYENLARGE(slotconfigs, int);
				slotconfigv = YYENLARGE(slotconfigv, YYSTYPE);
				if (slotconfigs == 0 || slotconfigv == 0)
					slotconfignewmax = 0;	/* failed */
			}
#endif
			if (slotconfignewmax <= slotconfigmaxdepth)	/* tables not expanded */
			{
				slotconfigerror( "yacc stack overflow" );
				YYABORT;
			}
			slotconfigmaxdepth = slotconfignewmax;

			slotconfig_ps = slotconfigs + slotconfigps_index;
			slotconfig_pv = slotconfigv + slotconfigpv_index;
			slotconfigpvt = slotconfigv + slotconfigpvt_index;
		}
		*slotconfig_ps = slotconfig_state;
		*++slotconfig_pv = slotconfigval;

		/*
		** we have a new state - find out what to do
		*/
	slotconfig_newstate:
		if ( ( slotconfig_n = slotconfigpact[ slotconfig_state ] ) <= YYFLAG )
			goto slotconfigdefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		slotconfigtmp = slotconfigchar < 0;
#endif
		if ( ( slotconfigchar < 0 ) && ( ( slotconfigchar = YYLEX() ) < 0 ) )
			slotconfigchar = 0;		/* reached EOF */
#if YYDEBUG
		if ( slotconfigdebug && slotconfigtmp )
		{
			register int slotconfig_i;

			printf( "Received token " );
			if ( slotconfigchar == 0 )
				printf( "end-of-file\n" );
			else if ( slotconfigchar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( slotconfig_i = 0; slotconfigtoks[slotconfig_i].t_val >= 0;
					slotconfig_i++ )
				{
					if ( slotconfigtoks[slotconfig_i].t_val == slotconfigchar )
						break;
				}
				printf( "%s\n", slotconfigtoks[slotconfig_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( slotconfig_n += slotconfigchar ) < 0 ) || ( slotconfig_n >= YYLAST ) )
			goto slotconfigdefault;
		if ( slotconfigchk[ slotconfig_n = slotconfigact[ slotconfig_n ] ] == slotconfigchar )	/*valid shift*/
		{
			slotconfigchar = -1;
			slotconfigval = slotconfiglval;
			slotconfig_state = slotconfig_n;
			if ( slotconfigerrflag > 0 )
				slotconfigerrflag--;
			goto slotconfig_stack;
		}

	slotconfigdefault:
		if ( ( slotconfig_n = slotconfigdef[ slotconfig_state ] ) == -2 )
		{
#if YYDEBUG
			slotconfigtmp = slotconfigchar < 0;
#endif
			if ( ( slotconfigchar < 0 ) && ( ( slotconfigchar = YYLEX() ) < 0 ) )
				slotconfigchar = 0;		/* reached EOF */
#if YYDEBUG
			if ( slotconfigdebug && slotconfigtmp )
			{
				register int slotconfig_i;

				printf( "Received token " );
				if ( slotconfigchar == 0 )
					printf( "end-of-file\n" );
				else if ( slotconfigchar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( slotconfig_i = 0;
						slotconfigtoks[slotconfig_i].t_val >= 0;
						slotconfig_i++ )
					{
						if ( slotconfigtoks[slotconfig_i].t_val
							== slotconfigchar )
						{
							break;
						}
					}
					printf( "%s\n", slotconfigtoks[slotconfig_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *slotconfigxi = slotconfigexca;

				while ( ( *slotconfigxi != -1 ) ||
					( slotconfigxi[1] != slotconfig_state ) )
				{
					slotconfigxi += 2;
				}
				while ( ( *(slotconfigxi += 2) >= 0 ) &&
					( *slotconfigxi != slotconfigchar ) )
					;
				if ( ( slotconfig_n = slotconfigxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( slotconfig_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( slotconfigerrflag )
			{
			case 0:		/* new error */
				slotconfigerror( "syntax error" );
				goto skip_init;
			slotconfigerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				slotconfig_pv = slotconfigpv;
				slotconfig_ps = slotconfigps;
				slotconfig_state = slotconfigstate;
			skip_init:
				slotconfignerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				slotconfigerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( slotconfig_ps >= slotconfigs )
				{
					slotconfig_n = slotconfigpact[ *slotconfig_ps ] + YYERRCODE;
					if ( slotconfig_n >= 0 && slotconfig_n < YYLAST &&
						slotconfigchk[slotconfigact[slotconfig_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						slotconfig_state = slotconfigact[ slotconfig_n ];
						goto slotconfig_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( slotconfigdebug )
						printf( _POP_, *slotconfig_ps,
							slotconfig_ps[-1] );
#	undef _POP_
#endif
					slotconfig_ps--;
					slotconfig_pv--;
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
				if ( slotconfigdebug )
				{
					register int slotconfig_i;

					printf( "Error recovery discards " );
					if ( slotconfigchar == 0 )
						printf( "token end-of-file\n" );
					else if ( slotconfigchar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( slotconfig_i = 0;
							slotconfigtoks[slotconfig_i].t_val >= 0;
							slotconfig_i++ )
						{
							if ( slotconfigtoks[slotconfig_i].t_val
								== slotconfigchar )
							{
								break;
							}
						}
						printf( "token %s\n",
							slotconfigtoks[slotconfig_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( slotconfigchar == 0 )	/* reached EOF. quit */
					YYABORT;
				slotconfigchar = -1;
				goto slotconfig_newstate;
			}
		}/* end if ( slotconfig_n == 0 ) */
		/*
		** reduction by production slotconfig_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( slotconfigdebug )
			printf( "Reduce by (%d) \"%s\"\n",
				slotconfig_n, slotconfigreds[ slotconfig_n ] );
#endif
		slotconfigtmp = slotconfig_n;			/* value to switch over */
		slotconfigpvt = slotconfig_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using slotconfig_state here as temporary
		** register variable, but why not, if it works...
		** If slotconfigr2[ slotconfig_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto slotconfig_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int slotconfig_len = slotconfigr2[ slotconfig_n ];

			if ( !( slotconfig_len & 01 ) )
			{
				slotconfig_len >>= 1;
				slotconfigval = ( slotconfig_pv -= slotconfig_len )[1];	/* $$ = $1 */
				slotconfig_state = slotconfigpgo[ slotconfig_n = slotconfigr1[ slotconfig_n ] ] +
					*( slotconfig_ps -= slotconfig_len ) + 1;
				if ( slotconfig_state >= YYLAST ||
					slotconfigchk[ slotconfig_state =
					slotconfigact[ slotconfig_state ] ] != -slotconfig_n )
				{
					slotconfig_state = slotconfigact[ slotconfigpgo[ slotconfig_n ] ];
				}
				goto slotconfig_stack;
			}
			slotconfig_len >>= 1;
			slotconfigval = ( slotconfig_pv -= slotconfig_len )[1];	/* $$ = $1 */
			slotconfig_state = slotconfigpgo[ slotconfig_n = slotconfigr1[ slotconfig_n ] ] +
				*( slotconfig_ps -= slotconfig_len ) + 1;
			if ( slotconfig_state >= YYLAST ||
				slotconfigchk[ slotconfig_state = slotconfigact[ slotconfig_state ] ] != -slotconfig_n )
			{
				slotconfig_state = slotconfigact[ slotconfigpgo[ slotconfig_n ] ];
			}
		}
					/* save until reenter driver code */
		slotconfigstate = slotconfig_state;
		slotconfigps = slotconfig_ps;
		slotconfigpv = slotconfig_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( slotconfigtmp )
	{
		
case 4:
# line 78 "slotconfig.y"
{
	    newclass = class_def_create(slotconfiglval.sval);
	    UFREE(slotconfiglval.sval);
	} break;
case 5:
# line 85 "slotconfig.y"
{
	    newclass->pattern = STRALLOC(slotconfiglval.sval);
	    UFREE(slotconfiglval.sval);
	} break;
case 6:
# line 90 "slotconfig.y"
{
	    unqstr = trim_quotes(slotconfiglval.sval);
	    trim_whitespace(unqstr);
	    newclass->pattern = STRALLOC(unqstr);
	    UFREE(unqstr);
	    UFREE(slotconfiglval.sval);
	} break;
case 10:
# line 107 "slotconfig.y"
{
	    newslot = slot_def_create(slotconfiglval.sval,
				      newclass->slot_defs,
				      newclass->name);
	    UFREE(slotconfiglval.sval);
	} break;
case 11:
# line 116 "slotconfig.y"
{
	    newslot->pattern = STRALLOC(slotconfiglval.sval);
	    UFREE(slotconfiglval.sval);
	} break;
case 12:
# line 121 "slotconfig.y"
{
	    unqstr = trim_quotes(slotconfiglval.sval);
	    trim_whitespace(unqstr);
	    newslot->pattern = STRALLOC(unqstr);
	    UFREE(unqstr);
	    UFREE(slotconfiglval.sval);
	} break;
case 17:
# line 139 "slotconfig.y"
{
	    add_slot_def_fill_type(newslot, slotconfiglval.sval);
	    UFREE(slotconfiglval.sval);
	} break;
case 18:
# line 144 "slotconfig.y"
{
	    unqstr = trim_quotes(slotconfiglval.sval);
	    trim_whitespace(unqstr);
	    add_slot_def_fill_type(newslot, unqstr);
	    UFREE(unqstr);
	    UFREE(slotconfiglval.sval);
	} break;
case 24:
# line 163 "slotconfig.y"
{
	    UFREE(slotconfiglval.sval);   /* set fill defs are not used in scoring */
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto slotconfigstack;		/* reset registers in driver code */
}

