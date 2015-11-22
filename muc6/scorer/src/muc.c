/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	muc.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	main function for MUC scorer.
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
static	char	SccsId[] = "%W%  %G%  Copyright 1995 Science Applications International Corporation.";
#endif


#include  <stdio.h>
#include  <stdlib.h>
#include  "muc.h"
#include  "muc_protos.h"
#include  "muc_globals.h"


#if UsePrototypes
main(int argc, char **argv)
#else
main(argc, argv)
int argc;
char **argv;
#endif	/* UsePrototypes */
{

    if (argc == 1)
    {
	fprintf(stderr, "Usage: %s <configuration_filename>\n", argv[0]);
	exit(1);
    }

    MUC_configuration_filename = (char *) malloc (strlen(argv[1]) + 1);
    strcpy(MUC_configuration_filename, argv[1]);
    
    initialize_MUC_scorer();                 /* init data, read input */

    map_MUC_catalog();                       /* "align" objects       */

    display_MUC_map_history();               /* output alignment      */
    
    if ( (MUC_scoring_task == SCENARIO_TEMPLATE)
	|| (MUC_scoring_task == TEMPLATE_ELEMENT)
	|| (MUC_scoring_task == NAMED_ENTITY) )
	find_and_remove_MUC_optional_instances();  /* remove *unaligned* opt. key objs */

    if ( (MUC_scoring_task == SCENARIO_TEMPLATE)
	|| (MUC_scoring_task == TEMPLATE_ELEMENT) )
	connect_MUC_catalog();                     /* "connect" unaligned objs */
    
    score_MUC_catalog();                           /* traverse catalog, counting beans */
/*   print_muc_class_defs(); */
/*   print_muc_catalog();   */
    display_MUC_catalog_scores();
    exit(0);
}









