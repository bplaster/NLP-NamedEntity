%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	mapconfig.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of the MUC map configuration file.
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
static	char	SccsId[] = "@(#)mapconfig.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "muc_defs.h"
#include  "y.mapconfig.h"

#ifndef FLEX_SCANNER
#undef yywrap
#endif

/*
 *  lex definitions to parse MUC map configuration file.
 */

Proto    (extern void, yyerror, (const char *msg));

int  maplineno;

%}

%%

%{
/* comments */
%}

^#.*				{ ; }

%{
/* open or close parenthesis */
%}

[()]				{ return(yytext[0]); }

%{
/* option name */
%}

^:([a-zA-Z][a-zA-Z0-9_\-]*)	{ if STRCASEEQ(yytext, ":scored_slots")
				      return SCORED_SLOTS;
				  else if STRCASEEQ(yytext, ":unscored_slots")
				      return UNSCORED_SLOTS;
				  else if STRCASEEQ(yytext, ":scored_templates")
				      return SCORED_TEMPLATES;
				  else if STRCASEEQ(yytext, ":map_order")
				      return MAP_ORDER;
				  else if STRCASEEQ(yytext, ":map_weights")
				      return MAP_WEIGHTS;
				  else
				      yyerror("unrecognized option"); }

%{
/* literal 'all' */
%}

all				{ return SCORE_ALL; }

%{
/* text */
%}

[^ \t\n\(\)]+			{ yylval.sval = (char *) strdup(yytext);
				  return STRING; }
%{
/* single-quoted string */
%}

(\'[^\'\n]*\')			{ yylval.sval = (char *) strdup(yytext);
				  return QSTRING; }
%{
/* double-quoted string */
%}

(\"[^\"\n]*\")			{ yylval.sval = (char *) strdup(yytext);
				  return QSTRING; }

%{
/* eol */
%}

\n				{ maplineno++; }

%{
/* whitespace */
%}

[ \t]				{ ; }

%{
/* other */
%}

.				{ yyerror("invalid character"); }

%%

#if UsePrototypes
void
yyerror(const char *msg)
#else
void
yyerror(msg)
char *msg;
#endif	/* UsePrototypes */
{
    printf("(mapconfig) %d: %s at '%s'\n", maplineno, msg, yytext);
}

#if UsePrototypes
int
yywrap(void)
#else
int
yywrap()
#endif  /* UsePrototypes */
{
    return(1);
}

