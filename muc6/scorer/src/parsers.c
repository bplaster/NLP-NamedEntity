/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	parsers.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	entry functions for the various parsers in the scorer program
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *	Put all functions from parse_*.c into this file. -ad 12/27/96
 *
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)parse.c	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc_protos.h"	/* nested include of muc.h */
#include  "muc_globals.h"

/* global functions */

Proto (void, parse_config, (char *fname));
Proto (void, parse_mapconfig, (void));
Proto (void, parse_slotconfig, (void));
Proto (void, parse_keys, (void));
Proto (void, parse_responses, (void));
Proto (void, parse_texts, (void));

/* local functions */

Proto	(static void, parse_templates, (void));
Proto	(static void, parse_markup, (void));

char	*templfname;
char	*markup_fname;
char	*textfname;

Bool	first_entry;        /* flag used in template parser */

/*****************************************/
/*****************************************/
/*****************************************/

#if UsePrototypes
void
parse_config(char *fname)
#else
void
parse_config(fname)
char *fname;
#endif	/* UsePrototypes */
{
    FILE	*fopen();
    extern FILE	*configin;
    extern configlineno;
    
    configlineno = 1;
    
    if ((configin = fopen(fname, "r")) == (FILE *)NULL)
    {
	fprintf(stderr, "Could not open configuration file '%s'.\n",
		fname);
	exit(1);
    }

    while (!feof(configin))
    {
	/* configparse() returns 0 if successful, 1 on failure */
	if (configparse())
	{
            printf("parse of configuration '%s' failed.\n", fname);
	    exit(1);
	}
    }

    fclose(configin);
}

/*****************************************/
/*****************************************/
/*****************************************/
   


#if UsePrototypes
void
parse_mapconfig(void)
#else
void
parse_mapconfig()
#endif	/* UsePrototypes */    
{
    FILE	*fopen();

    extern FILE *mapconfigin;
    extern int  maplineno;
    
    Option	*basediropt, *mapfileopt;
    char	*basedir, *mapname, *fname;
    
    Map_order = (Option *)find_muc_option(":map_order");
    
    maplineno = 1;

    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *)NULL)
	basedir = basediropt->default_value;
    
    mapfileopt = find_muc_option(":map_file");
    if ((mapname = mapfileopt->value) == (char *)NULL)
    {
	fprintf(stderr, "Warning: Map-configuration file unspecified.\n");
	return;
    }
    else
    {
	fname = (char *) malloc (strlen(basedir) + strlen(mapname) + 1);
	if (fname == (char *)NULL)
	{
	    fprintf(stderr, "[parse_mapconfig] Can't malloc for fname.\n");
	    exit(1);
	}
    
	fname = strcat(strcpy(fname, basedir), mapname);
    
	if ((mapconfigin = fopen(fname, "r")) == (FILE *)NULL)
	{
	    fprintf(stderr, "Could not open mapconfiguration file '%s'.\n",
		    fname);
	    exit(1);
	}
	
	while (!feof(mapconfigin))
	{
	    /* mapconfigparse() returns 0 if successful, 1 on failure */
	    if(mapconfigparse())
	    {
		printf("parse of mapconfiguration '%s' failed.\n", fname);
		exit(1);
	    }
	}

	fclose(mapconfigin);
	UFREE(fname);

	return;
    }
}

/*****************************************/
/*****************************************/
/*****************************************/


#if UsePrototypes
void
parse_slotconfig(void)
#else
void
parse_slotconfig()
#endif	/* UsePrototypes */    
{
    FILE	*fopen();

    extern FILE *slotconfigin;
    extern int  slotlineno;
    
    Option	*basediropt, *slotfileopt;
    char	*basedir, *slotname, *fname;
    
    slotlineno = 1;

    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *)NULL)
	basedir = basediropt->default_value;
    
    slotfileopt = find_muc_option(":slot_file");

    if ((slotname = slotfileopt->value) == (char *)NULL)
    {
	fprintf(stderr, "Error: Slot-configuration file unspecified.\n");
	exit(1);
    }
    
    fname = (char *) malloc (strlen(basedir) + strlen(slotname) + 1);
    if (fname == (char *)NULL)
    {
	fprintf(stderr, "[parse_slotconfig] Can't malloc for fname.\n");
	exit(1);
    }
    
    fname = strcat(strcpy(fname, basedir), slotname);
    
    if ((slotconfigin = fopen(fname, "r")) == (FILE *)NULL)
    {
	fprintf(stderr, "Could not open slotconfiguration file '%s'.\n",
		fname);
	exit(1);
    }

    while (!feof(slotconfigin))
    {
	/* slotconfigparse() returns 0 if successful, 1 on failure */
	if(slotconfigparse())
	{
            printf("parse of slotconfiguration '%s' failed.\n", fname);
	    exit(1);
	}
    }

    fclose(slotconfigin);
    UFREE(fname);
}

/*****************************************/
/*****************************************/
/*****************************************/


#if UsePrototypes
void
parse_keys(void)
#else
void
parse_keys()
#endif	/* UsePrototypes */
{
    first_entry = True;
    MUC_instance_type = KEY;

    if ( (MUC_scoring_task == SCENARIO_TEMPLATE)
	|| (MUC_scoring_task == TEMPLATE_ELEMENT) )
    {
	MUC_file_type = KEY_TEMPLATE;
	printf("Begin Parsing Key Templates.\n");
	parse_templates();
	printf("Parsing Key Templates Completed.\n");
    }
    else if ( (MUC_scoring_task == NAMED_ENTITY)
	     || (MUC_scoring_task == COREFERENCE) )
    {
	printf("Begin Parsing NE/CO Key Documents.\n");
	MUC_file_type = KEY_DOCUMENT;
	parse_markup();
	printf("Parsing NE/CO Key Documents Completed.\n");
    }
    
    MUC_file_type = 0;
    MUC_instance_type = 0;
}

/*****************************************/
/*****************************************/
/*****************************************/


#if UsePrototypes
void
parse_responses(void)
#else
void
parse_responses()
#endif	/* UsePrototypes */
{
    first_entry = True;
    MUC_instance_type = RESPONSE;

    if ( (MUC_scoring_task == SCENARIO_TEMPLATE)
	|| (MUC_scoring_task == TEMPLATE_ELEMENT) )
    {
	MUC_file_type = RESPONSE_TEMPLATE;
	printf("Begin Parsing Response Templates.\n");
	parse_templates();
	printf("Parsing Response Templates Completed.\n");
    }
    else if ( (MUC_scoring_task == NAMED_ENTITY)
	     || (MUC_scoring_task == COREFERENCE) )
    {
	printf("Begin Parsing NE/CO Response Documents.\n");
	MUC_file_type = RESPONSE_DOCUMENT;
	parse_markup();
	printf("Parsing NE/CO Response Documents Completed.\n");
    }
    
    MUC_file_type = 0;
    MUC_instance_type = 0;
}

/*****************************************/
/*****************************************/
/*****************************************/


#if UsePrototypes
static
void
parse_templates(void)
#else
static
void
parse_templates()
#endif	/* UsePrototypes */    
{
    FILE	*fopen();

    extern FILE *templatesin;
    extern int  templlineno;
    extern int  templcharno;
    extern int  templbegin;
    
    Option	*basediropt, *templfileopt;
    char	*basedir, *fname;
    
    templlineno = templcharno = templbegin = 1;

    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *)NULL)
	basedir = basediropt->default_value;

    if (MUC_instance_type == KEY)
	templfileopt = find_muc_option(":key_file");
    else
	templfileopt = find_muc_option(":response_file");
    
    if ((fname = templfileopt->value) == (char *)NULL)
    {
	fprintf(stderr,
		"[parse_templates]: %s template file not specified.\n",
		( (MUC_instance_type == KEY) ? "key" : "response"));
	return;
    }
    else
    {
	templfname = (char *) malloc (strlen(basedir) + strlen(fname) + 1);
	if (templfname == (char *)NULL)
	{
	    fprintf(stderr, "[parse_templates] Can't malloc for templfname.\n");
	    exit(1);
	}
    
	templfname = strcat(strcpy(templfname, basedir), fname);
    
	if ((templatesin = fopen(templfname, "r")) == (FILE *)NULL)
	{
	    fprintf(stderr, "Could not open template file '%s'.\n",
		    templfname);
	    exit(1);
	}

	while (!feof(templatesin))
	{
	    /* templatesparse() returns 0 if successful, 1 on failure */
	    if(templatesparse())
	    {
		printf("parse of template file '%s' failed.\n", templfname);
		exit(1);
	    }
	}

	fclose(templatesin);
	UFREE(templfname);

	return;
    }
}

/*****************************************/
/*****************************************/
/*****************************************/


#if UsePrototypes
static
void
parse_markup(void)
#else
static
void
parse_markup()
#endif	/* UsePrototypes */
{
    FILE	 *fopen();

    extern FILE	 *markup_in;
    extern long	 total_lineno;
    extern long	 total_charno;
    extern long	 total_ignore;
    
    Option	 *basediropt, *markup_fileopt;
    char	 *basedir, *fname;
    
    total_lineno = total_charno = 1;
    total_ignore = 0;

    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *)NULL)
	basedir = basediropt->default_value;

    if (MUC_instance_type == KEY)
	markup_fileopt = find_muc_option(":key_file");
    else
	markup_fileopt = find_muc_option(":response_file");
    
    if ((fname = markup_fileopt->value) == (char *)NULL)
	fname = markup_fileopt->default_value;
    
    markup_fname = (char *) malloc (strlen(basedir) + strlen(fname) + 1);
    if (markup_fname == (char *)NULL)
    {
	fprintf(stderr, "[parse_markup] Can't malloc for markup_fname.\n");
	exit(1);
    }
    
    markup_fname = strcat(strcpy(markup_fname, basedir), fname);
    
    if ((markup_in = fopen(markup_fname, "r")) == (FILE *)NULL)
    {
	fprintf(stderr, "Could not open ne/co document file '%s'.\n",
		markup_fname);
	exit(1);
    }

    while (!feof(markup_in))
    {
	/* markup_lex() returns 0 if successful, 1 on failure */
	if(markup_lex())
	{
            printf("parse of ne/co document file '%s' failed.\n", markup_fname);
	    exit(1);
	}
    }

    fclose(markup_in);
    UFREE(markup_fname);
}

/*****************************************/
/*****************************************/
/*****************************************/


#if UsePrototypes
void
parse_texts(void)
#else
void
parse_texts()
#endif	/* UsePrototypes */    
{
    FILE	*fopen();

    extern FILE *textsin;
    extern int  textlineno;
    extern int  textcharno;
    extern int  textbegin;
    extern int  textend;
    
    Option	*basediropt, *textfileopt;
    char	*basedir, *fname;
    
    textlineno = textcharno = textbegin = textend = 1;

    basediropt = find_muc_option(":muc_base_directory");
    if ((basedir = basediropt->value) == (char *)NULL)
	basedir = basediropt->default_value;
    
    textfileopt = find_muc_option(":text_file");
    if ((fname = textfileopt->value) == (char *)NULL)
    {
	fprintf(stderr, "[parse_texts] Text file not specified.  ");
	fprintf(stderr, "Skipping parse.\n");
    }
    else
    {
	textfname = (char *) malloc (strlen(basedir) + strlen(fname) + 1);
	if (textfname == (char *)NULL)
	{
	    fprintf(stderr, "[parse_texts] Can't malloc for textfname.\n");
	    exit(1);
	}
    
	textfname = strcat(strcpy(textfname, basedir), fname);
    
	if ((textsin = fopen(textfname, "r")) == (FILE *)NULL)
	{
	    fprintf(stderr, "Could not open text file '%s'.  ",
		    textfname);
	    fprintf(stderr, "Skipping parse.\n");
	}
	else
	{
	    while (!feof(textsin))
	    {
		/* textsparse() returns 0 if successful, 1 on failure */
		if(textsparse())
		{
		    printf("[parse_text] Parse of '%s' failed.\n",
			   textfname);
		    exit(1);
		}
	    }

	    fclose(textsin);
	    UFREE(textfname);
	}
    }

    return;
}
