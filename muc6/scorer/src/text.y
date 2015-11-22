%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	text.y
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the yacc specifications for the MUC
 *	text file parser.
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
static	char	SccsId[] = "@(#)text.y	1.4 24 Jan 1996  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc_protos.h"	/* nested include of muc.h */


extern void yyerror();

extern	int	yyleng;
extern	long	textlineno;
extern	long	textcharno;
extern	long	textbegin;
extern	long	textend;
extern	char	*textfname;


Catalog_entry	*newentry;
File_section    *newsection;
char		*docnum;

%}


%union 
{
    int   ival;
    char  *sval;
}

%token DOC_START  DOC_END  DOCNO_START  DOCNO_END
%token <sval> SGML
%token <sval> STRING

%%

/*  parsing a text file */

doc_specs:	doc_spec
	|	doc_specs doc_spec
	;

doc_spec:	doc_start docno_spec text_fills doc_end
	|	doc_start text_fills docno_spec text_fills doc_end
	|	text
	;

doc_start:	DOC_START
	{
	    ;
	}
	;

doc_end:	DOC_END
	{
	    /* set the end point for the text_doc File_section of this
	       entry. */
	    newsection->end = textend;
	}
	;

docno_spec:	docno_start docno_text docno_end
	;

docno_start:	DOCNO_START
	{
	    ;
	}
	;

docno_text:	docno
	|	docno text_fills
	;

docno:		STRING
	{
	    /* extract the docnum, and create a catalog entry for this
	       document - it may be a new entry, or perhaps a parsed
	       template has already created one. */

	    docnum = nice_docnum(yylval.sval);
	    newentry = catalog_entry_create(docnum);

	    /* create a File_section for this document and set its
	       begin point. */
	    newsection = file_section_create(textfname);
	    newsection->begin = textbegin;

	    newentry->text_doc = newsection;
	    UFREE(yylval.sval);
	    UFREE(docnum);
	}
	;

docno_end:	DOCNO_END
	{
	    ;
	}
	;

text_fills:	text
	|	text_fills text
	;

text:		STRING
	{
	    UFREE(yylval.sval);
	}
	;

%%

