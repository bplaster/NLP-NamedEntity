%{
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


extern void yyerror();
extern int yyleng;
extern int slotlineno;

Class_def  *newclass;
Slot_def   *newslot;
char	   *unqstr;

%}


%union 
{
    int   ival;
    char  *sval;
}

%token <ival> NUMBER
%token <sval> STRING
%token <sval> QSTRING

%%

/*  parsing a slot configuration file */

class_specs:	class_spec
	|	class_specs class_spec
	;

class_spec:	'(' class class_sgml slot_specs ')'
	;

class:		STRING		
        {
	    newclass = class_def_create(yylval.sval);
	    UFREE(yylval.sval);
	}
	;

class_sgml:	STRING
        {
	    newclass->pattern = STRALLOC(yylval.sval);
	    UFREE(yylval.sval);
	}
	|	QSTRING
	{
	    unqstr = trim_quotes(yylval.sval);
	    trim_whitespace(unqstr);
	    newclass->pattern = STRALLOC(unqstr);
	    UFREE(unqstr);
	    UFREE(yylval.sval);
	}
	;

slot_specs:    	/* empty */
	|	slot_specs slot_spec
	;

slot_spec:	'(' slot slot_sgml fill_type_spec set_fill_spec ')'
	;

slot:		STRING
        {
	    newslot = slot_def_create(yylval.sval,
				      newclass->slot_defs,
				      newclass->name);
	    UFREE(yylval.sval);
	}
	;

slot_sgml:	STRING
        {
	    newslot->pattern = STRALLOC(yylval.sval);
	    UFREE(yylval.sval);
	}
	|	QSTRING
	{
	    unqstr = trim_quotes(yylval.sval);
	    trim_whitespace(unqstr);
	    newslot->pattern = STRALLOC(unqstr);
	    UFREE(unqstr);
	    UFREE(yylval.sval);
	}
	;

fill_type_spec:	'(' fill_types ')'
	|	fill_type
	;

fill_types:	fill_type
	|	fill_types fill_type
	;

fill_type:	STRING
        {
	    add_slot_def_fill_type(newslot, yylval.sval);
	    UFREE(yylval.sval);
	}
	|	QSTRING
	{
	    unqstr = trim_quotes(yylval.sval);
	    trim_whitespace(unqstr);
	    add_slot_def_fill_type(newslot, unqstr);
	    UFREE(unqstr);
	    UFREE(yylval.sval);
	}
	;

set_fill_spec:	/* empty */
	|	'(' set_fills ')'
	;

set_fills:	set_fill
	|	set_fills set_fill
	;

set_fill:	STRING
	|	QSTRING
        {
	    UFREE(yylval.sval);   /* set fill defs are not used in scoring */
	}
	;

%%

