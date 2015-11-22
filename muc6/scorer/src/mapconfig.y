%{
/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	mapconfig.y
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the yacc specifications for the MUC
 *	map configuration file parser.
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
static	char	SccsId[] = "@(#)mapconfig.y	1.1 09 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"

Proto	(extern	void, yyerror, (const char *msg));
Proto	(static	void, score_all_slots, (void));


extern int yyleng;
extern int maplineno;

TABLE	   *Class_defs;
Class_def  *cur_class;
TABLE	   *cur_slots;
Slot_def   *cur_slot;
char	   *unqstr;


%}


%union 
{
    int   ival;
    char  *sval;
}

%token <sval> STRING
%token <sval> QSTRING

%token SCORED_SLOTS UNSCORED_SLOTS SCORED_TEMPLATES SCORE_ALL
%token MAP_ORDER MAP_WEIGHTS

%%

/*  parsing a map configuration file */

map_specs:	map_spec
	|	map_specs map_spec
	;

map_spec:	SCORED_SLOTS scored_slots_specs
	|	UNSCORED_SLOTS unscored_slots_specs
	|	SCORED_TEMPLATES scored_templates_specs
	|	MAP_ORDER map_order_spec
	|	MAP_WEIGHTS map_weights_specs
	;

scored_slots_specs:
		/* empty */
	{
	    score_all_slots();
	}
	|	score_all_slots
	|	scored_class_specs
	;

score_all_slots:
		SCORE_ALL
	{
	    score_all_slots();
	}
	;

scored_class_specs:	scored_class_spec
	|		scored_class_specs scored_class_spec
	;

scored_class_spec:	'(' scored_class scored_slots ')'

scored_class:	STRING		
        {
	    cur_class = find_class_def_by_name(yylval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: scored_slots spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", yylval.sval);
		UFREE(yylval.sval);
		exit(1);
	    }
	    else
	    {
		cur_class->is_scored_class = True;
		cur_slots = (TABLE *) cur_class->slot_defs;
		UFREE(yylval.sval);
	    }
	}
	;

scored_slots:  	/* empty */
	|	scored_slots scored_slot
	;

scored_slot:	STRING
	{
	    cur_slot = find_slot_def_by_name(cur_class, yylval.sval);
	    if (cur_slot == (Slot_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: scored_slots spec:\n");
		fprintf(stderr, "  slot '%s' undefined for class '%s'.\n",
			yylval.sval, cur_class->name);
		UFREE(yylval.sval);
		exit(1);
	    }
	    else
	    {
		cur_slot->is_scored_slot = True;
		UFREE(yylval.sval);
	    }
	}
	;

unscored_slots_specs:
		/* empty */
	{
	    ;
	}
	|	unscored_class_specs
	;

unscored_class_specs:
		unscored_class_spec
	|	unscored_class_specs unscored_class_spec
	;

unscored_class_spec:
		'(' unscored_class unscored_slot_specs ')'

unscored_class:
		STRING		
        {
	    cur_class = find_class_def_by_name(yylval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: unscored_slots spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", yylval.sval);
		UFREE(yylval.sval);
		exit(1);
	    }
	    else
	    {
		cur_slots = (TABLE *) cur_class->slot_defs;
		UFREE(yylval.sval);
	    }
	}
	;

unscored_slot_specs:
		/* empty */
	{
	    /* an empty slot list indicates that the class itself
	       is to be unscored. */
	    cur_class->is_scored_class = False;
	}
	|	unscored_slots
	;

unscored_slots:
		unscored_slot
	|	unscored_slots unscored_slot
	;

unscored_slot:	STRING
	{
	    cur_slot = find_slot_def_by_name(cur_class, yylval.sval);
	    if (cur_slot == (Slot_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: unscored_slots spec:\n");
		fprintf(stderr, "  slot '%s' undefined for class '%s'.\n",
			yylval.sval, cur_class->name);
		UFREE(yylval.sval);
		exit(1);
	    }
	    else
	    {
		cur_slot->is_scored_slot = False;
		UFREE(yylval.sval);
	    }
	}
	;

scored_templates_specs:
		score_all_templates
	;

score_all_templates:
		SCORE_ALL
	{
	    ;
	}
	;

map_order_spec:	/* empty */
	{
	    Class_defs = (TABLE *) MUC_Class_defs;
	    table_first(Class_defs);
	    for (cur_class = (Class_def *) table_next(Class_defs);
		 cur_class;
		 cur_class = (Class_def *) table_next(Class_defs))
		add_muc_option_value(Map_order, cur_class->name);
	}
	|	map_class_list
	;

map_class_list:	map_class
	|	map_class_list map_class
	;

map_class:	STRING
	{
	    cur_class = find_class_def_by_name(yylval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: map_order spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", yylval.sval);
		UFREE(yylval.sval);
		exit(1);
	    }
	    else
	    {
		add_muc_option_value(Map_order, yylval.sval);
		UFREE(yylval.sval);
	    }
	}
	;

map_weights_specs:
		/* empty */
	|	class_weight_specs
	;

class_weight_specs:
		class_weight_spec
	|	class_weight_specs class_weight_spec
	;

class_weight_spec:
		'(' weighted_class threshold slot_weight_specs ')'
	;

weighted_class:	STRING
	{
	    cur_class = find_class_def_by_name(yylval.sval);
	    if (cur_class == (Class_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: map_weight spec:\n");
		fprintf(stderr, "  class '%s' undefined.\n", yylval.sval);
		UFREE(yylval.sval);
		exit(1);
	    }
	    else
		UFREE(yylval.sval);
	}
	;

threshold:	STRING
	{
	    cur_class->map_threshold = atoi(yylval.sval);
	    UFREE(yylval.sval);
	}
	;

slot_weight_specs:
	    	/* empty */
	|	slot_weight_specs slot_weight_spec
	;

slot_weight_spec:
		'(' weighted_slot slot_weight ')'
	;

weighted_slot:	STRING
        {
	    cur_slot = find_slot_def_by_name(cur_class, yylval.sval);
	    if (cur_slot == (Slot_def *) NULL)
	    {
		fprintf(stderr, "[map_configuration]: map_weight spec:\n");
		fprintf(stderr, "  slot '%s' undefined for class '%s'.\n",
			yylval.sval, cur_class->name);
		UFREE(yylval.sval);
		exit(1);
	    }
	    else
		UFREE(yylval.sval);
	}
	;

slot_weight:	STRING
	{
	    cur_slot->map_weight = atoi(yylval.sval);
	    UFREE(yylval.sval);
	}
	;

%%


#if UsePrototypes
static
void
score_all_slots(void)
#else
static
void
score_all_slots()
#endif	/* UsePrototypes */
{
    TABLE *classes = (TABLE *) MUC_Class_defs;
    Class_def *this_class;
    TABLE *slots;
    Slot_def *this_slot;
    
    table_first(classes);
    for (this_class = (Class_def *) table_next(classes);
	 this_class;
	 this_class = (Class_def *) table_next(classes))
    {
	this_class->is_scored_class = True;
	
	slots = (TABLE *) this_class->slot_defs;
	table_first(slots);
	for (this_slot = (Slot_def *) table_next(slots);
	     this_slot;
	     this_slot = (Slot_def *) table_next(slots))
	    this_slot->is_scored_slot = True;
    }
    
    return;
}
