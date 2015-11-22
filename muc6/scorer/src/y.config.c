
# line 2 "config.y"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	config.y
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the yacc specifications for the MUC
 *	configuration file parser.
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
static	char	SccsId[] = "@(#)config.y	1.2 05 Sep 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc_protos.h"	/* nested include of muc.h */


extern void configerror();
extern int configleng;
extern int configlineno;

Option  *newoption;
char	*unqstr;


# line 55 "config.y"
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
# define OPTION 259
# define VALUE 260
# define QSTRING 261

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef configerror
	void configerror(const char *);
#endif

#ifndef configlex
#ifdef __EXTERN_C__
	extern "C" { int configlex(void); }
#else
	int configlex(void);
#endif
#endif
	int configparse(void);

#endif
#define configclearin configchar = -1
#define configerrok configerrflag = 0
extern int configchar;
extern int configerrflag;
YYSTYPE configlval;
YYSTYPE configval;
typedef int configtabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int config_configs[YYMAXDEPTH], *configs = config_configs;
YYSTYPE config_configv[YYMAXDEPTH], *configv = config_configv;
#else	/* user does initial allocation */
int *configs;
YYSTYPE *configv;
#endif
static int configmaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 109 "config.y"


configtabelem configexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 10
# define YYLAST 11
configtabelem configact[]={

     8,     9,     4,     7,     2,     6,     5,     3,     1,     0,
    10 };
configtabelem configpact[]={

  -257,  -257,-10000000,  -260,-10000000,-10000000,  -260,-10000000,-10000000,-10000000,
-10000000 };
configtabelem configpgo[]={

     0,     8,     4,     7,     5,     3 };
configtabelem configr1[]={

     0,     1,     1,     2,     2,     3,     4,     4,     5,     5 };
configtabelem configr2[]={

     0,     2,     4,     4,     2,     3,     2,     4,     3,     3 };
configtabelem configchk[]={

-10000000,    -1,    -2,    -3,   259,    -2,    -4,    -5,   260,   261,
    -5 };
configtabelem configdef[]={

     0,    -2,     1,     4,     5,     2,     3,     6,     8,     9,
     7 };
typedef struct
#ifdef __cplusplus
	configtoktype
#endif
{ char *t_name; int t_val; } configtoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

configtoktype configtoks[] =
{
	"NUMBER",	257,
	"STRING",	258,
	"OPTION",	259,
	"VALUE",	260,
	"QSTRING",	261,
	"-unknown-",	-1	/* ends search */
};

char * configreds[] =
{
	"-no such reduction-",
	"config_specs : option_spec",
	"config_specs : config_specs option_spec",
	"option_spec : option value_list",
	"option_spec : option",
	"option : OPTION",
	"value_list : value",
	"value_list : value_list value",
	"value : VALUE",
	"value : QSTRING",
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
#define YYERROR		goto configerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( configchar >= 0 || ( configr2[ configtmp ] >> 1 ) != 1 )\
	{\
		configerror( "syntax error - cannot backup" );\
		goto configerrlab;\
	}\
	configchar = newtoken;\
	configstate = *configps;\
	configlval = newvalue;\
	goto confignewstate;\
}
#define YYRECOVERING()	(!!configerrflag)
#define YYNEW(type)	malloc(sizeof(type) * confignewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, confignewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, confignewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int configdebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *configpv;			/* top of value stack */
int *configps;			/* top of state stack */

int configstate;			/* current state */
int configtmp;			/* extra var (lasts between blocks) */

int confignerrs;			/* number of errors */
int configerrflag;			/* error recovery flag */
int configchar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		configcvtok(configlex())
/*
** configcvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int configcvtok(int i)
#else
int configcvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( configmbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = configmbchars[mid].character;
			if( j==i ){/*Found*/ 
				return configmbchars[mid].tvalue;
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
#define YYLEX()		configlex()
#endif/*!YYNMBCHARS*/

/*
** configparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int configparse(void)
#else
int configparse()
#endif
{
	register YYSTYPE *configpvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; configpvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto configerrlab;
		case 2: goto confignewstate;
	}
	configpvt = 0;
#endif

	/*
	** Initialize externals - configparse may be called more than once
	*/
	configpv = &configv[-1];
	configps = &configs[-1];
	configstate = 0;
	configtmp = 0;
	confignerrs = 0;
	configerrflag = 0;
	configchar = -1;

#if YYMAXDEPTH <= 0
	if (configmaxdepth <= 0)
	{
		if ((configmaxdepth = YYEXPAND(0)) <= 0)
		{
			configerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *config_pv;	/* top of value stack */
		register int *config_ps;		/* top of state stack */
		register int config_state;		/* current state */
		register int  config_n;		/* internal state number info */
	goto configstack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	confignewstate:
		config_pv = configpv;
		config_ps = configps;
		config_state = configstate;
		goto config_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	configstack:
		config_pv = configpv;
		config_ps = configps;
		config_state = configstate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	config_stack:
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
		if ( configdebug )
		{
			register int config_i;

			printf( "State %d, token ", config_state );
			if ( configchar == 0 )
				printf( "end-of-file\n" );
			else if ( configchar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( config_i = 0; configtoks[config_i].t_val >= 0;
					config_i++ )
				{
					if ( configtoks[config_i].t_val == configchar )
						break;
				}
				printf( "%s\n", configtoks[config_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++config_ps >= &configs[ configmaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int configps_index = (config_ps - configs);
			int configpv_index = (config_pv - configv);
			int configpvt_index = (configpvt - configv);
			int confignewmax;
#ifdef YYEXPAND
			confignewmax = YYEXPAND(configmaxdepth);
#else
			confignewmax = 2 * configmaxdepth;	/* double table size */
			if (configmaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newconfigs = (char *)YYNEW(int);
				char *newconfigv = (char *)YYNEW(YYSTYPE);
				if (newconfigs != 0 && newconfigv != 0)
				{
					configs = YYCOPY(newconfigs, configs, int);
					configv = YYCOPY(newconfigv, configv, YYSTYPE);
				}
				else
					confignewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				configs = YYENLARGE(configs, int);
				configv = YYENLARGE(configv, YYSTYPE);
				if (configs == 0 || configv == 0)
					confignewmax = 0;	/* failed */
			}
#endif
			if (confignewmax <= configmaxdepth)	/* tables not expanded */
			{
				configerror( "yacc stack overflow" );
				YYABORT;
			}
			configmaxdepth = confignewmax;

			config_ps = configs + configps_index;
			config_pv = configv + configpv_index;
			configpvt = configv + configpvt_index;
		}
		*config_ps = config_state;
		*++config_pv = configval;

		/*
		** we have a new state - find out what to do
		*/
	config_newstate:
		if ( ( config_n = configpact[ config_state ] ) <= YYFLAG )
			goto configdefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		configtmp = configchar < 0;
#endif
		if ( ( configchar < 0 ) && ( ( configchar = YYLEX() ) < 0 ) )
			configchar = 0;		/* reached EOF */
#if YYDEBUG
		if ( configdebug && configtmp )
		{
			register int config_i;

			printf( "Received token " );
			if ( configchar == 0 )
				printf( "end-of-file\n" );
			else if ( configchar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( config_i = 0; configtoks[config_i].t_val >= 0;
					config_i++ )
				{
					if ( configtoks[config_i].t_val == configchar )
						break;
				}
				printf( "%s\n", configtoks[config_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( config_n += configchar ) < 0 ) || ( config_n >= YYLAST ) )
			goto configdefault;
		if ( configchk[ config_n = configact[ config_n ] ] == configchar )	/*valid shift*/
		{
			configchar = -1;
			configval = configlval;
			config_state = config_n;
			if ( configerrflag > 0 )
				configerrflag--;
			goto config_stack;
		}

	configdefault:
		if ( ( config_n = configdef[ config_state ] ) == -2 )
		{
#if YYDEBUG
			configtmp = configchar < 0;
#endif
			if ( ( configchar < 0 ) && ( ( configchar = YYLEX() ) < 0 ) )
				configchar = 0;		/* reached EOF */
#if YYDEBUG
			if ( configdebug && configtmp )
			{
				register int config_i;

				printf( "Received token " );
				if ( configchar == 0 )
					printf( "end-of-file\n" );
				else if ( configchar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( config_i = 0;
						configtoks[config_i].t_val >= 0;
						config_i++ )
					{
						if ( configtoks[config_i].t_val
							== configchar )
						{
							break;
						}
					}
					printf( "%s\n", configtoks[config_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *configxi = configexca;

				while ( ( *configxi != -1 ) ||
					( configxi[1] != config_state ) )
				{
					configxi += 2;
				}
				while ( ( *(configxi += 2) >= 0 ) &&
					( *configxi != configchar ) )
					;
				if ( ( config_n = configxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( config_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( configerrflag )
			{
			case 0:		/* new error */
				configerror( "syntax error" );
				goto skip_init;
			configerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				config_pv = configpv;
				config_ps = configps;
				config_state = configstate;
			skip_init:
				confignerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				configerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( config_ps >= configs )
				{
					config_n = configpact[ *config_ps ] + YYERRCODE;
					if ( config_n >= 0 && config_n < YYLAST &&
						configchk[configact[config_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						config_state = configact[ config_n ];
						goto config_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( configdebug )
						printf( _POP_, *config_ps,
							config_ps[-1] );
#	undef _POP_
#endif
					config_ps--;
					config_pv--;
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
				if ( configdebug )
				{
					register int config_i;

					printf( "Error recovery discards " );
					if ( configchar == 0 )
						printf( "token end-of-file\n" );
					else if ( configchar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( config_i = 0;
							configtoks[config_i].t_val >= 0;
							config_i++ )
						{
							if ( configtoks[config_i].t_val
								== configchar )
							{
								break;
							}
						}
						printf( "token %s\n",
							configtoks[config_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( configchar == 0 )	/* reached EOF. quit */
					YYABORT;
				configchar = -1;
				goto config_newstate;
			}
		}/* end if ( config_n == 0 ) */
		/*
		** reduction by production config_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( configdebug )
			printf( "Reduce by (%d) \"%s\"\n",
				config_n, configreds[ config_n ] );
#endif
		configtmp = config_n;			/* value to switch over */
		configpvt = config_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using config_state here as temporary
		** register variable, but why not, if it works...
		** If configr2[ config_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto config_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int config_len = configr2[ config_n ];

			if ( !( config_len & 01 ) )
			{
				config_len >>= 1;
				configval = ( config_pv -= config_len )[1];	/* $$ = $1 */
				config_state = configpgo[ config_n = configr1[ config_n ] ] +
					*( config_ps -= config_len ) + 1;
				if ( config_state >= YYLAST ||
					configchk[ config_state =
					configact[ config_state ] ] != -config_n )
				{
					config_state = configact[ configpgo[ config_n ] ];
				}
				goto config_stack;
			}
			config_len >>= 1;
			configval = ( config_pv -= config_len )[1];	/* $$ = $1 */
			config_state = configpgo[ config_n = configr1[ config_n ] ] +
				*( config_ps -= config_len ) + 1;
			if ( config_state >= YYLAST ||
				configchk[ config_state = configact[ config_state ] ] != -config_n )
			{
				config_state = configact[ configpgo[ config_n ] ];
			}
		}
					/* save until reenter driver code */
		configstate = config_state;
		configps = config_ps;
		configpv = config_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( configtmp )
	{
		
case 5:
# line 80 "config.y"
{
	    newoption = find_muc_option(configlval.sval);
	    UFREE(configlval.sval);
	} break;
case 8:
# line 91 "config.y"
{
	    if (newoption != (Option *)NULL)
		add_muc_option_value(newoption, configlval.sval);
	    UFREE(configlval.sval);
	} break;
case 9:
# line 97 "config.y"
{
	    if (newoption != (Option *)NULL)
	    {
		unqstr = trim_quotes(configlval.sval);
		trim_whitespace(unqstr);
		add_muc_option_value(newoption, unqstr);
		UFREE(unqstr);
		UFREE(configlval.sval);
	    }
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto configstack;		/* reset registers in driver code */
}

