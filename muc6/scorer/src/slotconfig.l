%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	slotconfig.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of the MUC slot configuration file.
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
static	char	SccsId[] = "@(#)slotconfig.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "y.slotconfig.h"

#ifndef FLEX_SCANNER
#undef yywrap
#endif

/*
 *  lex definitions to parse MUC configuration file.
 */

Proto    (extern void, yyerror, (const char *msg));

int  slotlineno;

%}

%%

%{
/* comments */
%}

^#.*			{ ; }

%{
/* open or close parenthesis */
%}

[()]			{ return(yytext[0]); }

%{
/* text */
%}

[^ \t\n\(\)]+		{ yylval.sval = (char *) strdup(yytext);
			  return STRING; }
%{
/* single-quoted string */
%}

(\'[^\'\n]*\')		{ yylval.sval = (char *) strdup(yytext);
			  return QSTRING; }
%{
/* double-quoted string */
%}

(\"[^\"\n]*\")		{ yylval.sval = (char *) strdup(yytext);
			  return QSTRING; }

%{
/* whitespace */
%}

[ \t]			{ ; }

%{
/* eol */
%}

\n			{ slotlineno++; }

%{
/* other */
%}

.			{ yyerror("invalid character"); }

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
    printf("(slotconfig) %d: %s at '%s'\n", slotlineno, msg, yytext);
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

