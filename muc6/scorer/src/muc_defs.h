/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 *  muc_defs.h include file.
 *
 *  The contents of this file are, for the most part, from
 *  /prj/shared/include/aesir.h.
 *
 *  SccsId:  @(#)muc_defs.h	1.4  3/7/96
 */


#ifndef _MUC_DEFS_H_
#define _MUC_DEFS_H_	1

	/*
	 *  Since malloc() & free() are referenced here let's 
	 *  automatically include malloc.h so they are defined.
	 */

#ifndef __malloc_h
#include <malloc.h>
#endif /* __malloc.h */

#ifndef _alloca_h
#include <alloca.h>
#endif /* _alloca_h */

        /*
	 *  Since strcpy() & strlen() are referenced here let's 
	 *  automatically include string.h so they are defined.
	 *
	 *  Assume that the include files protect themselves from
	 *  being included twice - other wise we have to assume the
	 *  named defines (like __malloc_h) are OS independent and
         *  they are not (like __malloc_h).
	 */

#include <string.h>

#include "cdefs.h"	/* To support existing uses of Proto. */

#define MUC_VERSION	"0.0"	/* string for printing */

#ifdef Bool
#undef Bool
#endif
typedef int	Bool;

#ifndef NULL
#define NULL	0
#endif

        /*
	 *  Assume that TRUE is defined along with FALSE.
	 *  Likewise for True and False, ERR and OK.
	 */

#ifndef FALSE
#define FALSE		0
#define TRUE		1
#endif

#ifndef False
#define False		0
#define True		1
#endif

#ifndef ERR
#define ERR		-1		/* error return value */
#define OK		0		/* ok return value */
#endif

#ifndef  OFF
#define  OFF		"off"
#define  ON		"on"
#endif


/*
 * define muc_min() and muc_max() as macros.
 */

#define	muc_min(a, b)	((a) < (b) ? (a) : (b))
#define	muc_max(a, b)	((a) > (b) ? (a) : (b))


/* muc option types */

#define  OPTVAL      1	/* a user-defined option value */
#define  VAL_LIST    2	/* multi-valued option */
#define  TOGGLE      3	/* on or off? */
#define  SELECT      4	/* select from a defined set of values */
#define  SLOTS       5	/* for scored_slots */
#define  TEMPLATES   6	/* for scored_templates */

/* muc scoring_task types */

#define	SCENARIO_TEMPLATE  1
#define	TEMPLATE_ELEMENT   2
#define	NAMED_ENTITY	   3
#define	COREFERENCE	   4

/* muc file types */

#define	TEXT_DOCUMENT	   1
#define	KEY_TEMPLATE	   2
#define	RESPONSE_TEMPLATE  3
#define	KEY_DOCUMENT	   4
#define	RESPONSE_DOCUMENT  5

/* muc instance types */

#define	KEY	  1
#define	RESPONSE  2

/* major muc modes */

#define  NO_MODE       0
#define  INIT_MODE     1
#define  PARSE_MODE    2
#define  MAP_MODE      3
#define  SCORE_MODE    4
#define  DISPLAY_MODE  5

/* score results */

#define  MISMATCH  0
#define  MATCH     1
#define  PARTIAL   2
#define  QUERY     3

/* f-measure types */

#define  F_PR   1
#define  F_2PR  2
#define  F_P2R  3

/* basic fill types */

#define	 STRING_FILL	   1
#define  REGULARIZED_FILL  2
#define  POINTER_FILL	   3
#define  SET_FILL	   4
#define  LOCATION_FILL	   5
#define  MULTI_FILL	   6

/* named-entity types */

#define  ENAMEX  1
#define  NUMEX   2
#define  TIMEX   3

/* named-entity doc_section types             */
/* if you change this, change the array       */
/* non_muc_tag_strings in markup.l to match it  */

#define DOC       0
#define DOCNO     1
#define DOCID     2
#define HL        3
#define hl        4
#define HEADLINE  5
#define headline  6
#define SLUG      7
#define PREAMBLE  8
#define IDS       9
#define ids      10
#define TRAILER  11
#define trailer  12
#define DD       13
#define dd       14
#define DATE     15
#define date     16
#define DATELINE 17
#define dateline 18
#define TXT      19
#define txt      20
#define TEXT     21
#define text     22
#define IGNORE   23



/************************************************************************
 *									*
 *  tally macros							*
 *									*
 ***********************************************************************/

#define	tally_possible(tallies, count)	((tallies)->possible += (count))
#define	tally_actual(tallies, count)	((tallies)->actual += (count))
#define	tally_correct(tallies, count)	((tallies)->correct += (count))
#define	tally_partial(tallies, count)	((tallies)->partial += (count))
#define	tally_incorrect(tallies, count)	((tallies)->incorrect += (count))
#define	tally_missing(tallies, count)	((tallies)->missing += (count))
#define	tally_spurious(tallies, count)	((tallies)->spurious += (count))
#define	tally_noncommittal(tallies, count)  ((tallies)->noncommittal += (count))

#define	tally1_possible(tallies)	((tallies)->possible++)
#define	tally1_actual(tallies)		((tallies)->actual++)
#define	tally1_correct(tallies)		((tallies)->correct++)
#define	tally1_partial(tallies)		((tallies)->partial++)
#define	tally1_incorrect(tallies)	((tallies)->incorrect++)
#define	tally1_missing(tallies)		((tallies)->missing++)
#define	tally1_spurious(tallies)	((tallies)->spurious++)
#define	tally1_noncommittal(tallies)	((tallies)->noncommittal++)


/* non-MUC-specific defines */

#define REG		register
#define EOS		'\0'		/* end of string */
#define EOL		'\n'		/* end of line */
#define EOP		'\14'		/* end of page (form feed) */

#define FILENAMELEN	255		/* longest file name */
#define	MAXHOSTNAME	256
#define MAXPATH		FILENAMELEN	/* longest file name */
#define NBITS		8		/* number of bits in a byte */
#define BUFLENGTH	256		/* convenient size for getting input */
#define	ID_SIZE		32
#define MAX_MESSSIZE	1024
#define	PPOID_SIZE	32
#define LPOID_SIZE	PPOID_SIZE
#define LONGTIME	(60*60*24*7)	/* seven days */


/*
 * The following make the use of malloc much easier.  You generally want
 * to use UALLOC() to dynamically allocate space and UFREE() to free
 * the space up.
 *
 * UALLOC	- Allocate permanent memory.
 * UALLOCA	- Allocate temporary (stack) memory that is automatically
 *		  free'd when calling procedure returns.
 * UREALLOC	- Change the size of allocated memory from a previous
 *		  UALLOC() call (only on permanent memory).
 * STRALLOC	- Returns a pointer to the string copied into permanent
 *		  memory.
 * STRALLOCA	- Same as STRALLOC except uses temporary memory.
 * UFREE	- Free permanent memory and set pointer to NULL.
 */

#define UALLOC(type, count)	(type *) malloc ((unsigned) (count) * \
						             sizeof (type))
#define UALLOCA(type, count)	(type *) alloca ((count) * sizeof (type))

/*
 *  Please excuse this ternary statement but it allows us to use REALLOC 
 *  even if ptr is NULL.
 */
#define UREALLOC(ptr,type,count) ( (ptr) ? \
				   (type *) realloc ((char *)ptr, \
						     (unsigned) sizeof (type) \
						      * (count)) \
				  : \
				   UALLOC(type,(count)) \
				 )
#define STRALLOC(string)	strcpy (UALLOC (char, strlen (string)+1), \
					string)
#define STRALLOCA(string)	strcpy (UALLOCA (char, strlen (string)+1), \
					string)

	/*
	 *  This sort of weirdness is so it becomes one statement.
	 */
#define UFREE(ptr)		do \
				{	if (ptr)\
					{	(void) free ((char *) (ptr));\
						(ptr) = NULL;\
					} \
				} while (0)

/* Return the number of elements in an array. */
#define DIM(ar)		(sizeof (ar) / sizeof (*(ar)))

/* Are two strings equal? */
#define STREQ(a,b)		(strcmp ((a), (b)) == 0)

/* Are two strings equal? (case insensitive version) */
/*#define STRCASEEQ(a,b)		(muc_strcasecmp ((a), (b)) == 0)*/
#define STRCASEEQ(a,b)		(muc_cmp_special((a), (b)) == 0)


#ifdef DO_DEBUG

/*
 * Provide varying levels of debug information.  You should define your
 * the areas you are interested in debugging (depends on the application
 * what those areas may be) up to a maximum of MAX_DEBUG.  Within each of
 * the areas of interest, you can various levels of debugging information.
 *
 * Typically invoked as:
 *	DEBUG(BUG_INIT, 3, ("Opening file %s\n", filename));
 * and if you have set aDebug[BUG_INIT] >= 3, you will see
 *	"Opening file foo"
 * printed on the screen.  If you set aDebug[BUG_INIT] = 0, you won't
 * see the message.  If you do not define DO_DEBUG, then the debugging
 * statements are not compiled into the code.
 */

#define DEBUG(what,lev,mess)	do {if (aDebug[what] >= lev) printf mess;fflush (stdout);} while(0)
#define MAX_DEBUG	4
extern int		aDebug[MAX_DEBUG+1];
#else
#define DEBUG(lev,what,mess)
#endif	/* DO_DEBUG */

#endif /*_MUC_DEFS_H_ */
