%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	template.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of MUC template files.
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
static	char	SccsId[] = "@(#)template.l	1.3 31 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include "cdefs.h"
#include "y.template.h"

#ifndef FLEX_SCANNER
#undef yywrap
#endif

/*
 *  lex definitions to parse MUC template file.
 */

Proto    (extern void, yyerror, (const char *msg));
Proto    (extern int, yywrap, (void));
Proto    (extern void, finish_template, (void));

long  templlineno, templcharno;
long  templbegin;

%}

%s SLOT

%%

%{
/* comments */
%}

^[#;].*				{ templcharno += yyleng; }

%{
/* instance header */
%}

^"<"[^ \t>]+">"[ \t]*":="	{ templbegin = templcharno;
				  templcharno += yyleng;
				  yylval.sval = (char *) strdup(yytext);
				  return INST_START; }

%{
/* slot name */
%}

^[ \t]*[^ \t]+":"		{ templcharno += yyleng;
				  yylval.sval = (char *) strdup(yytext);
				  BEGIN SLOT;
				  return SLOTNAME; }

%{
/* eol while parsing slot */
%}

<SLOT>(\n)			{ templlineno++;
				  templcharno++;
				  BEGIN INITIAL; }

%{
/* optional marker while parsing slot */
%}

<SLOT>[ \t]*"/"			{ templcharno += yyleng;
				  BEGIN INITIAL;
				  return OPTMARK; }

%{
/* alternate marker */
%}

^[ \t]*"/"			{ templcharno += yyleng;
				  return ALTMARK; }

%{
/* instance pointer */
%}

"<"[^ \t>]+">"			{ templcharno += yyleng;
				  yylval.sval = (char *) strdup(yytext);
				  return POINTER; }

%{
/* double-quoted string */
%}

\"[^\"\n]*\"			{ templcharno += yyleng;
				  yylval.sval = (char *) strdup(yytext);
				  return QSTRING; }

%{
/* single-quoted string */
%}

\'[^\'\n]*\'			{ templcharno += yyleng;
				  yylval.sval = (char *) strdup(yytext);
				  return QSTRING; }

%{
/* slot filler */
%}

[^ \t\n\/].*[^ \t\n]		{ templcharno += yyleng;
				  yylval.sval = (char *) strdup(yytext);
				  return SLOT_FILL; }

%{
/* whitespace */
%}

[ \t]				{ templcharno += yyleng; }

%{
/* eol */
%}

\n				{ templlineno++;
				  templcharno++; }

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
#endif  /* UsePrototypes */
{
    printf("(template) %d: %s at '%s'\n", templlineno, msg, yytext);
}


#if UsePrototypes
int
yywrap(void)
#else
int
yywrap()
#endif  /* UsePrototypes */
{
    finish_template();
    return(1);
}
