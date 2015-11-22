
# line 2 "mapconfig.y"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	mapconfig.y
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the yacc specifications for the MUC
 *	map configuration file parser.
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
static	char	SccsId[] = "@(#)mapconfig.y	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"

Proto	(extern	void, mapconfigerror, (const char *msg));
Proto	(static	void, score_all_slots, (void));


extern int mapconfigleng;
extern int maplineno;

TABLE	   *Class_defs;
Class_def  *cur_class;
TABLE	   *cur_slots;
Slot_def   *cur_slot;
char	   *unqstr;



# line 63 "mapconfig.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 
{
    int   ival;
    char  *sval;
} YYSTYPE;
# define STRING 257
# define QSTRING 258
# define SCORED_SLOTS 259
# define UNSCORED_SLOTS 260
# define SCORED_TEMPLATES 261
# define SCORE_ALL 262
# define MAP_ORDER 263
# define MAP_WEIGHTS 264

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef mapconfigerror
	void mapconfigerror(const char *);
#endif

#ifndef mapconfiglex
#ifdef __EXTERN_C__
	extern "C" { int mapconfiglex(void); }
#else
	int mapconfiglex(void);
#endif
#endif
	int mapconfigparse(void);

#endif
#define mapconfigclearin mapconfigchar = -1
#define mapconfigerrok mapconfigerrflag = 0
extern int mapconfigchar;
extern int mapconfigerrflag;
YYSTYPE mapconfiglval;
YYSTYPE mapconfigval;
typedef int mapconfigtabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int mapconfig_mapconfigs[YYMAXDEPTH], *mapconfigs = mapconfig_mapconfigs;
YYSTYPE mapconfig_mapconfigv[YYMAXDEPTH], *mapconfigv = mapconfig_mapconfigv;
#else	/* user does initial allocation */
int *mapconfigs;
YYSTYPE *mapconfigv;
#endif
static int mapconfigmaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 336 "mapconfig.y"



#if UsePrototypes
static
void
score_all_slots(void)
#else
static
void
score_all_slots()
#endif	/* UsePrototypes */
{
    TABLE *classes = (TABLE *) MUC_Class_defs;
    Class_def *this_class;
    TABLE *slots;
    Slot_def *this_slot;
    
    table_first(classes);
    for (this_class = (Class_def *) table_next(classes);
	 this_class;
	 this_class = (Class_def *) table_next(classes))
    {
	this_class->is_scored_class = True;
	
	slots = (TABLE *) this_class->slot_defs;
	table_first(slots);
	for (this_slot = (Slot_def *) table_next(slots);
	     this_slot;
	     this_slot = (Slot_def *) table_next(slots))
	    this_slot->is_scored_slot = True;
    }
    
    return;
}
mapconfigtabelem mapconfigexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 49
# define YYLAST 226
mapconfigtabelem mapconfigact[]={

     3,     4,     5,    14,     6,     7,    21,    47,    59,    57,
    44,    46,    39,    25,    35,    32,    43,    55,    53,    60,
    50,    29,    18,    14,    28,    24,    17,    13,     2,    58,
     8,    56,    54,    52,    45,    38,    27,    23,    20,    30,
    42,    41,    34,    33,    16,    48,    40,    31,    11,    36,
    10,    26,    37,    22,    19,    15,     9,     1,     0,    51,
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
     0,     0,     0,    49,     0,    12 };
mapconfigtabelem mapconfigpact[]={

  -259,  -259,-10000000,   -37,   -18,  -256,  -244,   -19,-10000000,-10000000,
-10000000,   -17,-10000000,-10000000,  -242,-10000000,   -18,-10000000,  -243,-10000000,
-10000000,-10000000,-10000000,  -244,-10000000,-10000000,-10000000,   -19,-10000000,  -245,
-10000000,-10000000,-10000000,-10000000,  -247,-10000000,-10000000,-10000000,  -246,-10000000,
   -34,   -21,  -247,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,   -23,-10000000,-10000000,  -248,  -249,-10000000,   -22,-10000000,
-10000000 };
mapconfigtabelem mapconfigpgo[]={

     0,    57,    28,    56,    55,    54,    53,    51,    50,    48,
    27,    47,    46,    45,    44,    26,    42,    41,    40,    16,
    38,    37,    25,    36,    24,    35,    34,    33,    32,    31,
    29 };
mapconfigtabelem mapconfigr1[]={

     0,     1,     1,     2,     2,     2,     2,     2,     3,     3,
     3,     8,     9,     9,    10,    11,    12,    12,    13,     4,
     4,    14,    14,    15,    16,    17,    17,    18,    18,    19,
     5,    20,     6,     6,    21,    21,    22,     7,     7,    23,
    23,    24,    25,    26,    27,    27,    28,    29,    30 };
mapconfigtabelem mapconfigr2[]={

     0,     2,     4,     4,     4,     4,     4,     4,     1,     2,
     2,     3,     2,     4,     8,     3,     0,     4,     3,     1,
     2,     2,     4,     8,     3,     1,     2,     2,     4,     3,
     2,     3,     1,     2,     2,     4,     3,     0,     2,     2,
     4,    10,     3,     3,     0,     4,     8,     3,     3 };
mapconfigtabelem mapconfigchk[]={

-10000000,    -1,    -2,   259,   260,   261,   263,   264,    -2,    -3,
    -8,    -9,   262,   -10,    40,    -4,   -14,   -15,    40,    -5,
   -20,   262,    -6,   -21,   -22,   257,    -7,   -23,   -24,    40,
   -10,   -11,   257,   -15,   -16,   257,   -22,   -24,   -25,   257,
   -12,   -17,   -18,   -19,   257,   -26,   257,    41,   -13,   257,
    41,   -19,   -27,    41,   -28,    40,   -29,   257,   -30,   257,
    41 };
mapconfigtabelem mapconfigdef[]={

     0,    -2,     1,     8,    19,     0,    32,    37,     2,     3,
     9,    10,    11,    12,     0,     4,    20,    21,     0,     5,
    30,    31,     6,    33,    34,    36,     7,    38,    39,     0,
    13,    16,    15,    22,    25,    24,    35,    40,     0,    42,
     0,     0,    26,    27,    29,    44,    43,    14,    17,    18,
    23,    28,     0,    41,    45,     0,     0,    47,     0,    48,
    46 };
typedef struct
#ifdef __cplusplus
	mapconfigtoktype
#endif
{ char *t_name; int t_val; } mapconfigtoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

mapconfigtoktype mapconfigtoks[] =
{
	"STRING",	257,
	"QSTRING",	258,
	"SCORED_SLOTS",	259,
	"UNSCORED_SLOTS",	260,
	"SCORED_TEMPLATES",	261,
	"SCORE_ALL",	262,
	"MAP_ORDER",	263,
	"MAP_WEIGHTS",	264,
	"-unknown-",	-1	/* ends search */
};

char * mapconfigreds[] =
{
	"-no such reduction-",
	"map_specs : map_spec",
	"map_specs : map_specs map_spec",
	"map_spec : SCORED_SLOTS scored_slots_specs",
	"map_spec : UNSCORED_SLOTS unscored_slots_specs",
	"map_spec : SCORED_TEMPLATES scored_templates_specs",
	"map_spec : MAP_ORDER map_order_spec",
	"map_spec : MAP_WEIGHTS map_weights_specs",
	"scored_slots_specs : /* empty */",
	"scored_slots_specs : score_all_slots",
	"scored_slots_specs : scored_class_specs",
	"score_all_slots : SCORE_ALL",
	"scored_class_specs : scored_class_spec",
	"scored_class_specs : scored_class_specs scored_class_spec",
	"scored_class_spec : '(' scored_class scored_slots ')'",
	"scored_class : STRING",
	"scored_slots : /* empty */",
	"scored_slots : scored_slots scored_slot",
	"scored_slot : STRING",
	"unscored_slots_specs : /* empty */",
	"unscored_slots_specs : unscored_class_specs",
	"unscored_class_specs : unscored_class_spec",
	"unscored_class_specs : unscored_class_specs unscored_class_spec",
	"unscored_class_spec : '(' unscored_class unscored_slot_specs ')'",
	"unscored_class : STRING",
	"unscored_slot_specs : /* empty */",
	"unscored_slot_specs : unscored_slots",
	"unscored_slots : unscored_slot",
	"unscored_slots : unscored_slots unscored_slot",
	"unscored_slot : STRING",
	"scored_templates_specs : score_all_templates",
	"score_all_templates : SCORE_ALL",
	"map_order_spec : /* empty */",
	"map_order_spec : map_class_list",
	"map_class_list : map_class",
	"map_class_list : map_class_list map_class",
	"map_class : STRING",
	"map_weights_specs : /* empty */",
	"map_weights_specs : class_weight_specs",
	"class_weight_specs : class_weight_spec",
	"class_weight_specs : class_weight_specs class_weight_spec",
	"class_weight_spec : '(' weighted_class threshold slot_weight_specs ')'",
	"weighted_class : STRING",
	"threshold : STRING",
	"slot_weight_specs : /* empty */",
	"slot_weight_specs : slot_weight_specs slot_weight_spec",
	"slot_weight_spec : '(' weighted_slot slot_weight ')'",
	"weighted_slot : STRING",
	"slot_weight : STRING",
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
#define YYERROR		goto mapconfigerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( mapconfigchar >= 0 || ( mapconfigr2[ mapconfigtmp ] >> 1 ) != 1 )\
	{\
		mapconfigerror( "syntax error - cannot backup" );\
		goto mapconfigerrlab;\
	}\
	mapconfigchar = newtoken;\
	mapconfigstate = *mapconfigps;\
	mapconfiglval = newvalue;\
	goto mapconfignewstate;\
}
#define YYRECOVERING()	(!!mapconfigerrflag)
#define YYNEW(type)	malloc(sizeof(type) * mapconfignewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, mapconfignewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, mapconfignewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int mapconfigdebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *mapconfigpv;			/* top of value stack */
int *mapconfigps;			/* top of state stack */

int mapconfigstate;			/* current state */
int mapconfigtmp;			/* extra var (lasts between blocks) */

int mapconfignerrs;			/* number of errors */
int mapconfigerrflag;			/* error recovery flag */
int mapconfigchar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		mapconfigcvtok(mapconfiglex())
/*
** mapconfigcvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int mapconfigcvtok(int i)
#else
int mapconfigcvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( mapconfigmbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = mapconfigmbchars[mid].character;
			if( j==i ){/*Found*/ 
				return mapconfigmbchars[mid].tvalue;
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
#define YYLEX()		mapconfiglex()
#endif/*!YYNMBCHARS*/

/*
** mapconfigparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int mapconfigparse(void)
#else
int mapconfigparse()
#endif
{
	register YYSTYPE *mapconfigpvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; mapconfigpvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto mapconfigerrlab;
		case 2: goto mapconfignewstate;
	}
	mapconfigpvt = 0;
#endif

	/*
	** Initialize externals - mapconfigparse may be called more than once
	*/
	mapconfigpv = &mapconfigv[-1];
	mapconfigps = &mapconfigs[-1];
	mapconfigstate = 0;
	mapconfigtmp = 0;
	mapconfignerrs = 0;
	mapconfigerrflag = 0;
	mapconfigchar = -1;

#if YYMAXDEPTH <= 0
	if (mapconfigmaxdepth <= 0)
	{
		if ((mapconfigmaxdepth = YYEXPAND(0)) <= 0)
		{
			mapconfigerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *mapconfig_pv;	/* top of value stack */
		register int *mapconfig_ps;		/* top of state stack */
		register int mapconfig_state;		/* current state */
		register int  mapconfig_n;		/* internal state number info */
	goto mapconfigstack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	mapconfignewstate:
		mapconfig_pv = mapconfigpv;
		mapconfig_ps = mapconfigps;
		mapconfig_state = mapconfigstate;
		goto mapconfig_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	mapconfigstack:
		mapconfig_pv = mapconfigpv;
		mapconfig_ps = mapconfigps;
		mapconfig_state = mapconfigstate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	mapconfig_stack:
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
		if ( mapconfigdebug )
		{
			register int mapconfig_i;

			printf( "State %d, token ", mapconfig_state );
			if ( mapconfigchar == 0 )
				printf( "end-of-file\n" );
			else if ( mapconfigchar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( mapconfig_i = 0; mapconfigtoks[mapconfig_i].t_val >= 0;
					mapconfig_i++ )
				{
					if ( mapconfigtoks[mapconfig_i].t_val == mapconfigchar )
						break;
				}
				printf( "%s\n", mapconfigtoks[mapconfig_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++mapconfig_ps >= &mapconfigs[ mapconfigmaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int mapconfigps_index = (mapconfig_ps - mapconfigs);
			int mapconfigpv_index = (mapconfig_pv - mapconfigv);
			int mapconfigpvt_index = (mapconfigpvt - mapconfigv);
			int mapconfignewmax;
#ifdef YYEXPAND
			mapconfignewmax = YYEXPAND(mapconfigmaxdepth);
#else
			mapconfignewmax = 2 * mapconfigmaxdepth;	/* double table size */
			if (mapconfigmaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newmapconfigs = (char *)YYNEW(int);
				char *newmapconfigv = (char *)YYNEW(YYSTYPE);
				if (newmapconfigs != 0 && newmapconfigv != 0)
				{
					mapconfigs = YYCOPY(newmapconfigs, mapconfigs, int);
					mapconfigv = YYCOPY(newmapconfigv, mapconfigv, YYSTYPE);
				}
				else
					mapconfignewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				mapconfigs = YYENLARGE(mapconfigs, int);
				mapconfigv = YYENLARGE(mapconfigv, YYSTYPE);
				if (mapconfigs == 0 || mapconfigv == 0)
					mapconfignewmax = 0;	/* failed */
			}
#endif
			if (mapconfignewmax <= mapconfigmaxdepth)	/* tables not expanded */
			{
				mapconfigerror( "yacc stack overflow" );
				YYABORT;
			}
			mapconfigmaxdepth = mapconfignewmax;

			mapconfig_ps = mapconfigs + mapconfigps_index;
			mapconfig_pv = mapconfigv + mapconfigpv_index;
			mapconfigpvt = mapconfigv + mapconfigpvt_index;
		}
		*mapconfig_ps = mapconfig_state;
		*++mapconfig_pv = mapconfigval;

		/*
		** we have a new state - find out what to do
		*/
	mapconfig_newstate:
		if ( ( mapconfig_n = mapconfigpact[ mapconfig_state ] ) <= YYFLAG )
			goto mapconfigdefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		mapconfigtmp = mapconfigchar < 0;
#endif
		if ( ( mapconfigchar < 0 ) && ( ( mapconfigchar = YYLEX() ) < 0 ) )
			mapconfigchar = 0;		/* reached EOF */
#if YYDEBUG
		if ( mapconfigdebug && mapconfigtmp )
		{
			register int mapconfig_i;

			printf( "Received token " );
			if ( mapconfigchar == 0 )
				printf( "end-of-file\n" );
			else if ( mapconfigchar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( mapconfig_i = 0; mapconfigtoks[mapconfig_i].t_val >= 0;
					mapconfig_i++ )
				{
					if ( mapconfigtoks[mapconfig_i].t_val == mapconfigchar )
						break;
				}
				printf( "%s\n", mapconfigtoks[mapconfig_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( mapconfig_n += mapconfigchar ) < 0 ) || ( mapconfig_n >= YYLAST ) )
			goto mapconfigdefault;
		if ( mapconfigchk[ mapconfig_n = mapconfigact[ mapconfig_n ] ] == mapconfigchar )	/*valid shift*/
		{
			mapconfigchar = -1;
			mapconfigval = mapconfiglval;
			mapconfig_state = mapconfig_n;
			if ( mapconfigerrflag > 0 )
				mapconfigerrflag--;
			goto mapconfig_stack;
		}

	mapconfigdefault:
		if ( ( mapconfig_n = mapconfigdef[ mapconfig_state ] ) == -2 )
		{
#if YYDEBUG
			mapconfigtmp = mapconfigchar < 0;
#endif
			if ( ( mapconfigchar < 0 ) && ( ( mapconfigchar = YYLEX() ) < 0 ) )
				mapconfigchar = 0;		/* reached EOF */
#if YYDEBUG
			if ( mapconfigdebug && mapconfigtmp )
			{
				register int mapconfig_i;

				printf( "Received token " );
				if ( mapconfigchar == 0 )
					printf( "end-of-file\n" );
				else if ( mapconfigchar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( mapconfig_i = 0;
						mapconfigtoks[mapconfig_i].t_val >= 0;
						mapconfig_i++ )
					{
						if ( mapconfigtoks[mapconfig_i].t_val
							== mapconfigchar )
						{
							break;
						}
					}
					printf( "%s\n", mapconfigtoks[mapconfig_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *mapconfigxi = mapconfigexca;

				while ( ( *mapconfigxi != -1 ) ||
					( mapconfigxi[1] != mapconfig_state ) )
				{
					mapconfigxi += 2;
				}
				while ( ( *(mapconfigxi += 2) >= 0 ) &&
					( *mapconfigxi != mapconfigchar ) )
					;
				if ( ( mapconfig_n = mapconfigxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( mapconfig_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( mapconfigerrflag )
			{
			case 0:		/* new error */
				mapconfigerror( "syntax error" );
				goto skip_init;
			mapconfigerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				mapconfig_pv = mapconfigpv;
				mapconfig_ps = mapconfigps;
				mapconfig_state = mapconfigstate;
			skip_init:
				mapconfignerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				mapconfigerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( mapconfig_ps >= mapconfigs )
				{
					mapconfig_n = mapconfigpact[ *mapconfig_ps ] + YYERRCODE;
					if ( mapconfig_n >= 0 && mapconfig_n < YYLAST &&
						mapconfigchk[mapconfigact[mapconfig_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						mapconfig_state = mapconfigact[ mapconfig_n ];
						goto mapconfig_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( mapconfigdebug )
						printf( _POP_, *mapconfig_ps,
							mapconfig_ps[-1] );
#	undef _POP_
#endif
					mapconfig_ps--;
					mapconfig_pv--;
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
				if ( mapconfigdebug )
				{
					register int mapconfig_i;

					printf( "Error recovery discards " );
					if ( mapconfigchar == 0 )
						printf( "token end-of-file\n" );
					else if ( mapconfigchar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( mapconfig_i = 0;
							mapconfigtoks[mapconfig_i].t_val >= 0;
							mapconfig_i++ )
						{
							if ( mapconfigtoks[mapconfig_i].t_val
								== mapconfigchar )
							{
								break;
							}
						}
						printf( "token %s\n",
							mapconfigtoks[mapconfig_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( mapconfigchar == 0 )	/* reached EOF. quit */
					YYABORT;
				mapconfigchar = -1;
				goto mapconfig_newstate;
			}
		}/* end if ( mapconfig_n == 0 ) */
		/*
		** reduction by production mapconfig_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( mapconfigdebug )
			printf( "Reduce by (%d) \"%s\"\n",
				mapconfig_n, mapconfigreds[ mapconfig_n ] );
#endif
		mapconfigtmp = mapconfig_n;			/* value to switch over */
		mapconfigpvt = mapconfig_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using mapconfig_state here as temporary
		** register variable, but why not, if it works...
		** If mapconfigr2[ mapconfig_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto mapconfig_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int mapconfig_len = mapconfigr2[ mapconfig_n ];

			if ( !( mapconfig_len & 01 ) )
			{
				mapconfig_len >>= 1;
				mapconfigval = ( mapconfig_pv -= mapconfig_len )[1];	/* $$ = $1 */
				mapconfig_state = mapconfigpgo[ mapconfig_n = mapconfigr1[ mapconfig_n ] ] +
					*( mapconfig_ps -= mapconfig_len ) + 1;
				if ( mapconfig_state >= YYLAST ||
					mapconfigchk[ mapconfig_state =
					mapconfigact[ mapconfig_state ] ] != -mapconfig_n )
				{
					mapconfig_state = mapconfigact[ mapconfigpgo[ mapconfig_n ] ];
				}
				goto mapconfig_stack;
			}
			mapconfig_len >>= 1;
			mapconfigval = ( mapconfig_pv -= mapconfig_len )[1];	/* $$ = $1 */
			mapconfig_state = mapconfigpgo[ mapconfig_n = mapconfigr1[ mapconfig_n ] ] +
				*( mapconfig_ps -= mapconfig_len ) + 1;
			if ( mapconfig_state >= YYLAST ||
				mapconfigchk[ mapconfig_state = mapconfigact[ mapconfig_state ] ] != -mapconfig_n )
			{
				mapconfig_state = mapconfigact[ mapconfigpgo[ mapconfig_n ] ];
			}
		}
					/* save until reenter driver code */
		mapconfigstate = mapconfig_state;
		mapconfigps = mapconfig_ps;
		mapconfigpv = mapconfig_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( mapconfigtmp )
	{
		
case 8:
# line 92 "mapconfig.y"
{
	    score_all_slots();
	} break;
case 11:
# line 101 "mapconfig.y"
{
	    score_all_slots();
	} break;
case 15:
# line 113 "mapconfig.y"
{
	    cur_class = find_class_def_by_name(mapconfiglval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: scored_slots spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", mapconfiglval.sval);
		UFREE(mapconfiglval.sval);
		exit(1);
	    }
	    else
	    {
		cur_class->is_scored_class = True;
		cur_slots = (TABLE *) cur_class->slot_defs;
		UFREE(mapconfiglval.sval);
	    }
	} break;
case 18:
# line 136 "mapconfig.y"
{
	    cur_slot = find_slot_def_by_name(cur_class, mapconfiglval.sval);
	    if (cur_slot == (Slot_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: scored_slots spec:\n");
		fprintf(stderr, "  slot '%s' undefined for class '%s'.\n",
			mapconfiglval.sval, cur_class->name);
		UFREE(mapconfiglval.sval);
		exit(1);
	    }
	    else
	    {
		cur_slot->is_scored_slot = True;
		UFREE(mapconfiglval.sval);
	    }
	} break;
case 19:
# line 156 "mapconfig.y"
{
	    ;
	} break;
case 24:
# line 172 "mapconfig.y"
{
	    cur_class = find_class_def_by_name(mapconfiglval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: unscored_slots spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", mapconfiglval.sval);
		UFREE(mapconfiglval.sval);
		exit(1);
	    }
	    else
	    {
		cur_slots = (TABLE *) cur_class->slot_defs;
		UFREE(mapconfiglval.sval);
	    }
	} break;
case 25:
# line 191 "mapconfig.y"
{
	    /* an empty slot list indicates that the class itself
	       is to be unscored. */
	    cur_class->is_scored_class = False;
	} break;
case 29:
# line 205 "mapconfig.y"
{
	    cur_slot = find_slot_def_by_name(cur_class, mapconfiglval.sval);
	    if (cur_slot == (Slot_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: unscored_slots spec:\n");
		fprintf(stderr, "  slot '%s' undefined for class '%s'.\n",
			mapconfiglval.sval, cur_class->name);
		UFREE(mapconfiglval.sval);
		exit(1);
	    }
	    else
	    {
		cur_slot->is_scored_slot = False;
		UFREE(mapconfiglval.sval);
	    }
	} break;
case 31:
# line 229 "mapconfig.y"
{
	    ;
	} break;
case 32:
# line 235 "mapconfig.y"
{
	    Class_defs = (TABLE *) MUC_Class_defs;
	    table_first(Class_defs);
	    for (cur_class = (Class_def *) table_next(Class_defs);
		 cur_class;
		 cur_class = (Class_def *) table_next(Class_defs))
		add_muc_option_value(Map_order, cur_class->name);
	} break;
case 36:
# line 251 "mapconfig.y"
{
	    cur_class = find_class_def_by_name(mapconfiglval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: map_order spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", mapconfiglval.sval);
		UFREE(mapconfiglval.sval);
		exit(1);
	    }
	    else
	    {
		add_muc_option_value(Map_order, mapconfiglval.sval);
		UFREE(mapconfiglval.sval);
	    }
	} break;
case 42:
# line 283 "mapconfig.y"
{
	    cur_class = find_class_def_by_name(mapconfiglval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: map_weight spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", mapconfiglval.sval);
		UFREE(mapconfiglval.sval);
		exit(1);
	    }
	    else
		UFREE(mapconfiglval.sval);
	} break;
case 43:
# line 298 "mapconfig.y"
{
	    cur_class->map_threshold = atoi(mapconfiglval.sval);
	    UFREE(mapconfiglval.sval);
	} break;
case 47:
# line 314 "mapconfig.y"
{
	    cur_slot = find_slot_def_by_name(cur_class, mapconfiglval.sval);
	    if (cur_slot == (Slot_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: map_weight spec:\n");
		fprintf(stderr, "  slot '%s' undefined for class '%s'.\n",
			mapconfiglval.sval, cur_class->name);
		UFREE(mapconfiglval.sval);
		exit(1);
	    }
	    else
		UFREE(mapconfiglval.sval);
	} break;
case 48:
# line 330 "mapconfig.y"
{
	    cur_slot->map_weight = atoi(mapconfiglval.sval);
	    UFREE(mapconfiglval.sval);
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto mapconfigstack;		/* reset registers in driver code */
}

