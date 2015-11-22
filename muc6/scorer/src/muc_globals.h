/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 *  muc_globals.h include file.
 *
 *  SccsId:  @(#)muc_globals.h	1.2  24 Jan 1996
 */


#ifndef _MUC_GLOBALS_H_
#define _MUC_GLOBALS_H_  1

#include  "muc.h"	/* nested includes of muc_defs.h and cdefs.h */


extern  TABLE   *MUC_Catalog;
extern  TABLE   *MUC_Class_defs;
extern  TABLE   *MUC_Summary_Class_scores;

extern	char	*MUC_configuration_filename;

extern	int	MUC_scoring_task;
extern	int	MUC_file_type;
extern	int	MUC_instance_type;

extern	int	MUC_mode;

extern	Option	MUC_Options[];
extern	int	Num_Options;

extern	Option	*Premodifiers;
extern	Option	*Postmodifiers;
extern	Option	*Corp_designators;
extern	Option	*Scoring_method;

extern	Option	*Map_order;

extern	char	*blanks;

/* NE specific */

extern	int	NE_doc_section;
extern	int	NE_category;
extern  int     NE_headline_type;
extern  int     NE_text_type;

/*** kao add new tallies ***/
extern Tallies *HEADLINE_tallies;
extern Tallies *headline_tallies;
extern Tallies *SLUG_tallies;
extern Tallies *PREAMBLE_tallies;
extern Tallies *hl_tallies;
extern Tallies *HL_tallies;
extern Tallies *TXT_tallies;
extern Tallies *TEXT_tallies;
extern Tallies *IDS_tallies;
extern Tallies *TRAILER_tallies;
extern Tallies *date_tallies;


extern Tallies *dd_tallies;
extern Tallies *DATELINE_tallies;

extern Tallies *enamex_organization_tallies;
extern Tallies *enamex_person_tallies;
extern Tallies *enamex_location_tallies;
extern Tallies *enamex_other_tallies;
extern Tallies *timex_date_tallies;
extern Tallies *timex_time_tallies;
extern Tallies *timex_other_tallies;
extern Tallies *numex_money_tallies;
extern Tallies *numex_percent_tallies;
extern Tallies *numex_other_tallies;

#endif	/* _MUC_GLOBALS_H_ */
