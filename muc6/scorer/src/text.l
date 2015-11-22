%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	text.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of MUC text files.
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
static	char	SccsId[] = "@(#)text.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "y.text.h"

#ifndef FLEX_SCANNER
#undef yywrap
#endif

/*
 *  lex definitions to parse MUC text file.
 */

Proto    (extern void, yyerror, (const char *msg));

long  textlineno, textcharno;
long  textbegin, textend;

%}

%%

%{
/*  start of document */
%}

"<"[Dd][Oo][Cc]">"		{ textbegin = textcharno;
				  textcharno += yyleng;
				  return DOC_START; }

%{
/*  end of document */
%}

"</"[Dd][Oo][Cc]">"		{ textcharno += yyleng;
				  textend = textcharno;
				  return DOC_END; }

%{
/*  start of document number sgml tag */
%}

"<"[Dd][Oo][Cc][Nn][Oo]">"	{ textcharno += yyleng;
				  return DOCNO_START; }

%{
/*  end of document number */
%}

"</"[Dd][Oo][Cc][Nn][Oo]">"	{ textcharno += yyleng;
				  return DOCNO_END; }

%{
/*  text */
%}

[^ \t\n]+			{ yylval.sval = (char *) strdup(yytext);
				  textcharno += yyleng;
				  return STRING; }

%{
/*  whitespace */
%}

[ \t]				{ textcharno += yyleng; }

%{
/*  eol */
%}

\n				{ textlineno++;
				  textcharno++; }

%{
/*  other */
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
    printf("(text) %d: %s at '%s'\n", textlineno, msg, yytext);
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

