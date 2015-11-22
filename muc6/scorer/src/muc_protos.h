/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 *  muc_protos.h include file.
 *
 *  muc function prototypes.
 *
 *  SccsId:  @(#)muc_protos.h	1.11  3/7/96
 */


#ifndef  _MUC_PROTOS_H_
#define  _MUC_PROTOS_H_   1

#include "muc.h"	/* nested includes of muc_defs.h & cdefs.h */


/************************************************************************
 *									*
 *  externally visible function prototypes				*
 *									*
 ***********************************************************************/

Proto	(extern	void, initialize_MUC_scorer, (void));
Proto	(extern	void,
	 initialize_inst_pairings_list,
	 (TABLE *pairings_list));
Proto	(extern	void, initialize_class_scores, (TABLE *scores,
						char *docnum));
Proto	(extern	void, initialize_slot_scores, (TABLE *scores,
					       Class_def *class,
					       char *docnum));


/* structure create functions */

Proto	(extern	Class_def *, class_def_create, (char *name));
Proto	(extern	Slot_def *, slot_def_create, (char *name,
					      TABLE *table,
					      char *classname));

Proto	(extern	Catalog_entry *, catalog_entry_create, (char *docnum));
Proto	(extern	File_section *, file_section_create, (char *fname));
Proto	(extern	Class_score *, class_score_create, (Class_def *class,
						    char *docnum));
Proto	(extern	Slot_score *, slot_score_create, (char *name,
						  char *docnum));
Proto	(extern	Root *, root_create, (char *docnum));
Proto	(extern	Class_instance_list *,
	 class_instance_list_create,
	 (TABLE *lists, char *name, char *docnum));
Proto	(extern	Class_instance *, class_instance_create, (TABLE *instlist,
							  char *name,
							  char *docnum,
							  char *id));
Proto	(extern	Slot_fill *, slot_fill_create, (TABLE *slots, char *name));
Proto	(extern	Pointer *, pointer_create, (Class_instance *inst));
Proto	(extern	Alt_fill_set *, alt_fill_set_create, (void));
Proto	(extern	String_fill *, string_fill_create, (char *str));
Proto	(extern	Tallies *, tallies_create, (void));

Proto	(extern	Inst_pairings *, inst_pairings_create, (char *cname));
Proto	(extern	Inst_pair *, inst_pair_create, (void));

/* release functions */

Proto	(extern	void, file_section_release,(char *elt));
Proto	(extern	void, tallies_release, (char *elt));
Proto	(extern	void, class_def_release, (char *elt));
Proto	(extern	void, slot_def_release, (char *elt));
Proto	(extern	void, catalog_entry_release, (char *elt));
Proto	(extern	void, class_score_release, (char *elt));
Proto	(extern	void, slot_score_release, (char *elt));
Proto	(extern	void, root_release, (char *elt));
Proto	(extern	void, class_instance_list_release, (char *elt));
Proto	(extern	void, class_instance_release, (char *elt));
Proto	(extern	void, slot_fill_release, (char *elt));
Proto	(extern	void, pointer_release, (char *elt));
Proto	(extern	void, alt_fill_set_release, (char *elt));
Proto	(extern	void, string_fill_release, (char *elt));
Proto	(extern	void, inst_pairings_release, (char *elt));
Proto	(extern	void, inst_pair_release, (char *elt));

/* comparison functions for insertions */

Proto	(extern	int, class_def_compare, (char *elt1, char *elt2));
Proto	(extern	int, slot_def_compare, (char *elt1, char *elt2));
Proto	(extern	int, catalog_entry_compare, (char *elt1, char *elt2));
Proto	(extern	int, class_score_compare, (char *elt1, char *elt2));
Proto	(extern	int, slot_score_compare, (char *elt1, char *elt2));
Proto	(extern	int, class_instance_list_compare, (char *elt1, char *elt2));
Proto	(extern	int, class_instance_compare, (char *elt1, char *elt2));
Proto	(extern	int, slot_fill_compare, (char *elt1, char *elt2));
Proto	(extern	int, inst_pairings_compare, (char *elt1, char *elt2));
Proto	(extern	int, inst_pair_compare, (const void *elt1,
					    const void *elt2));

/* non-TABLE insertion functions */


Proto	(extern	int, add_slot_def_fill_type, (Slot_def *sd, char *ft));

Proto	(extern	void, add_back_pointer, (Class_instance *this,
					 Class_instance *back));
Proto	(extern	void, add_string_fill, (Slot_fill *slot, char *string));
Proto	(extern	void, add_pointer_fill, (Slot_fill *slot, Pointer *ptr));
Proto	(extern	void, add_alt_string_fill, (Alt_fill_set *set,
					    char *string));
Proto	(extern	void, add_alt_pointer_fill, (Alt_fill_set *set,
					     Pointer *ptr));

Proto	(extern	void, add_inst_pair_to_all, (Inst_pairings *pairings,
						Inst_pair *pair));
Proto	(extern	void, add_inst_pair_to_final, (Inst_pairings *pairings,
						  Inst_pair *pair));

/* configuration options utilities */

Proto	(extern	Option *, find_muc_option, (char *optname));
Proto	(extern	int, reset_muc_options, (void));
Proto	(extern	int, add_muc_option_value, (Option *opt, char *val));

/* parse functions */

Proto	(extern	void, parse_config, (char *fname));
Proto	(extern	void, parse_slotconfig, (void));
Proto	(extern	void, parse_mapconfig, (void));
Proto	(extern	void, parse_texts, (void));
Proto	(extern	void, parse_keys, (void));
Proto	(extern	void, parse_responses, (void));

Proto	(extern	int, configparse, (void));
Proto	(extern	int, slotconfigparse, (void));
Proto	(extern	int, mapconfigparse, (void));
Proto	(extern	int, textsparse, (void));
Proto	(extern	int, templatesparse, (void));


/* miscellaneous */

Proto	(extern	Class_def *, find_class_def_by_name, (char *cname));
Proto	(extern	Class_def *, find_class_def_by_pattern, (char *cpattern));
Proto	(extern	Slot_def *, find_slot_def_by_name, (Class_def *class,
						    char *sname));
Proto	(extern	Slot_def *, find_slot_def_by_pattern, (Class_def *class,
						       char *spattern));
Proto	(extern	Class_instance_list *,
	 find_class_instance_list,
	 (TABLE *lists, char *cname));
Proto	(extern	Class_score *, find_class_score, (TABLE *scores,
						  char *cname));
Proto	(extern	Slot_score *, find_slot_score, (TABLE *scores,
						char *sname));
Proto	(extern	Slot_fill *, find_slot_fill, (TABLE *lists, char *sname));
Proto	(extern	Inst_pairings *,
	 find_inst_pairings,
	 (TABLE *pairings_list, char *cname));

Proto	(extern	void, tallies_copy, (Tallies *t1, Tallies *t2));
Proto	(extern	void, tallies_combine, (Tallies *t1, Tallies *t2));
Proto	(extern	void, tallies_subtract, (Tallies *t1, Tallies *t2));
Proto	(extern	void, tallies_clear, (Tallies *t));

/* libmap */

Proto	(extern	void, map_MUC_catalog, (void));
Proto	(extern	void, map_catalog_entry, (Catalog_entry *entry));
Proto	(extern	void, connect_MUC_catalog, (void));
Proto	(extern	void, connect_catalog_entry, (Catalog_entry *entry));
Proto	(extern void, combine_slot_tallies, (Class_score *c_score,
					     Tallies *total));
Proto	(extern	void,
	 find_identical_in_and_out_instances,
	 (Class_instance_list *io_list));

/* libscore */

Proto	(extern	void, score_MUC_catalog, (void));
Proto	(extern	void, score_catalog_entry, (Catalog_entry *entry));
Proto	(extern	void, score_relevancy, (Catalog_entry *entry,
					Tallies *relevancy));
Proto	(extern	void, score_instance_pair, (TABLE *slot_defs,
					    Class_score *c_score,
					    Class_instance *kinst,
					    Class_instance *rinst));
Proto	(extern	void, score_slot, (Slot_def *slot,
				   Slot_score *s_score,
				   Slot_fill *kfill,
				   Slot_fill *rfill));
Proto	(extern void, score_fills, (Slot_def *slot,
				    Tallies *tallies,
				    Slot_fill *kfill,
				    Slot_fill *rfill));
Proto	(extern void, score_alternate_set, (Slot_def *slot,
					    Tallies *tallies,
					    Alt_fill_set *kset,
					    Slot_fill *rfill,
					    String_fill *minstr));
Proto	(extern Bool, is_optional, (Slot_fill *kfill, Slot_fill *rfill));
Proto	(extern Bool, is_a_pointer_fill_type, (Slot_def *slot));

/* libdisplay */

Proto	(extern	void, display_MUC_catalog_scores, (void));
Proto	(extern	void, display_catalog_entry_scores, (Catalog_entry *entry));
Proto	(extern	void, display_MUC_map_history, (void));

Proto	(extern	void,
	 add_to_ne_subcategorization_tallies,
	 (Slot_fill *slot, Tallies *s_tallies));
Proto	(extern	void,
	 subtract_from_ne_subcategorization_tallies,
	 (Slot_fill *slot, Tallies *s_tallies));
Proto	(extern	void, add_to_ne_doc_section_tallies, (Tallies *s_tallies));
Proto	(extern	void, subtract_from_ne_doc_section_tallies, (Tallies *s_tallies));

/* libutils */

Proto	(extern	void, find_and_remove_MUC_optional_instances, (void));

Proto	(extern	char *, string_lower, (char *str));
Proto	(extern	char *, trim_quotes, (char *qstr));
Proto	(extern	void, trim_whitespace, (char *str));
Proto	(extern	char *, nice_docnum, (char *docnum));
Proto	(extern	void, get_class_docnum_id, (char *header,
					    char *cname,
					    char *docnum,
					    char *id));
Proto	(extern	int, muc_strcasecmp, (char *str1, char *str2));
Proto	(extern	int, muc_streq, (char *kstr, char *rstr, char *minstr));
Proto	(extern	float, compute_tallies_f, (Tallies *tallies, int which_f));
Proto	(extern	float, compute_f, (float beta,
				   float precision,
				   float recall));
Proto	(extern	float, compute_recall, (Tallies *tallies));
Proto	(extern	float, compute_precision, (Tallies *tallies));
Proto	(extern	float, compute_undergeneration, (Tallies *tallies));
Proto	(extern	float, compute_overgeneration, (Tallies *tallies));
Proto	(extern	float, compute_error_per_response_fill, (Tallies *tallies));
Proto	(extern	float, compute_substitution, (Tallies *tallies));

/* print functions */

Proto	(extern	int, print_muc_options, (void));
Proto	(extern	void, print_muc_catalog, (void));
Proto	(extern	void, print_catalog_entry, (Catalog_entry *entry,
					    int indent));
Proto   (extern	void, print_muc_class_defs, (void));
Proto	(extern	void, print_class_def, (Class_def *cd, int indent));

Proto	(extern	void, print_file_section, (File_section *section,
					   int indent));
Proto	(extern	void, print_root, (Root *root, int indent));
Proto	(extern	void, print_class_instance_list, (Class_instance_list *list,
						  int indent));
Proto	(extern	void, print_class_instance, (Class_instance *inst,
					     int indent));
Proto	(extern	void, print_slot_fill, (Slot_fill *sfill, int indent));
Proto	(extern	void, print_pointer, (Pointer *ptr, int indent));
Proto	(extern	void, print_alt_fill_set, (Alt_fill_set *set,
					   int setnum,
					   int indent));
Proto	(extern	void, print_string_fill, (String_fill *sfill, int indent));

Proto	(extern	void, print_slot_def, (Slot_def *sd, int indent));

Proto	(extern	void, print_inst_pairings, (Inst_pairings *pairings));
Proto	(extern	void, print_inst_pair, (Inst_pair *pair));

Proto	(extern	void, print_back_pointers, (Class_instance *child));
Proto	(extern	void, print_back_pointer, (Pointer *bptr));

/* test */


#endif   /* _MUC_PROTOS_H_ */

