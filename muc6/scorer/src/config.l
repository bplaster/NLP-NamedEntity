%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	config.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of the MUC configuration file.
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
static	char	SccsId[] = "@(#)config.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "y.config.h"


#ifndef FLEX_SCANNER
#undef yywrap
#endif

/*********************************************************************
 *                                                                   *
 *  lex definitions to parse MUC configuration file.                 *
 *                                                                   *
 ********************************************************************/

Proto    (extern void,  yyerror, (const char *msg));

int  configlineno;

%}

%%

%{
/* comments */
%}

^#.*				{ ; }

%{
/* option name */
%}

^:([a-zA-Z][a-zA-Z0-9_\-]*)	{ yylval.sval = (char *) strdup(yytext);
				  return OPTION; }

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
/* text */
%}

[^ \t\n]+			{ yylval.sval = (char *) strdup(yytext);
				  return VALUE; }

%{
/* eol */
%}

\n				{ configlineno++; }

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
    printf("(config) %d: %s at '%s'\n", configlineno, msg, yytext);
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

