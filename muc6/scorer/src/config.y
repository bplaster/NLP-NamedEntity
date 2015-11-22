%{
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


extern void yyerror();
extern int yyleng;
extern int configlineno;

Option  *newoption;
char	*unqstr;

%}


%union 
{
    int   ival;
    char  *sval;
}

%token <ival> NUMBER
%token <sval> STRING
%token <sval> OPTION
%token <sval> VALUE
%token <sval> QSTRING

%%

/*  parsing a configuration file */

config_specs:	option_spec
        |	config_specs option_spec
	;

option_spec:	option value_list
	|	option
	;

option:		OPTION
	{
	    newoption = find_muc_option(yylval.sval);
	    UFREE(yylval.sval);
	}
	;

value_list:	value
	|	value_list value
	;

value:		VALUE
	{
	    if (newoption != (Option *)NULL)
		add_muc_option_value(newoption, yylval.sval);
	    UFREE(yylval.sval);
	}
	|	QSTRING
	{
	    if (newoption != (Option *)NULL)
	    {
		unqstr = trim_quotes(yylval.sval);
		trim_whitespace(unqstr);
		add_muc_option_value(newoption, unqstr);
		UFREE(unqstr);
		UFREE(yylval.sval);
	    }
	}
	;
		
%%

