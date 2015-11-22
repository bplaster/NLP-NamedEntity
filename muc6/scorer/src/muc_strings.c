/*
 *  Copyright 1995 Science Applications International Corporation.
 */

/*
 * NAME
 *
 * 
 * FILE 
 *	muc_strings.c
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	string manipulation routines.
 *
 * DIAGNOSTICS
 *
 *
 * FILES
 *
 *
 * NOTES
 *	minor changes to case-insensitive string comparison functions -ad 11/12/96
 *      changed trim_quotes to handle quotes within quotes -ad 11/26/96
 * 
 * SEE ALSO
 *
 * AUTHOR
 *	Gary Dungca	August 1995
 *
 */

#ifndef	lint
static	char	SccsId[] = "@(#)muc_strings.c	1.2 3/7/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "muc.h"
#include "muc_protos.h"
#include "muc_globals.h"


/*  internal function prototypes */

Proto	(static int, muc_min_streq, (char *kstr, char *rstr, char *mstr));
Proto	(static char *, get_minstring, (char *src));
Proto	(static char *, make_clean_string, (char *src));
Proto	(static char *, muc_strstr, (char *s, char *t));
Proto	(static void, remove_postmodifiers, (char *src));
Proto	(static void, remove_premodifiers, (char *src));
Proto	(static void, remove_multi_word_designators, (char *src));
Proto	(static void, remove_corp_designators, (char *src));
Proto	(static Bool, is_postmodifier, (char c));
Proto	(static Bool, is_premodifier, (char *s));
Proto	(static Bool, is_corp_designator, (char *s));
Proto	(static void, remove_word_from_string, (char *str, char *word));


/*  some local globals */

static char *delims = " \t";
static char *space = " ";

/************************************************************************
 *					
 *  trim_quotes				
 *					
 *  take a quoted string and return a copy of the string, which has had
    any whitespace removed from the beginning and end, then any matching
    quotes removed from the beginning and end, and then any whitespace
    removed from the beginning and end.

    example (colons are to make leading whitespace "visible"):

    given:      :   "  John "JD" Dooner   "   :
    return:     :John "JD" Dooner:
 * 
 * 
 ***********************************************************************/

#if UsePrototypes
char *
trim_quotes(char *qstr)
#else
char *
trim_quotes(qstr)
char *qstr;
#endif	/* UsePrototypes */
{
    char *new;
    char qchar;
    int nlen, i;
    
    new = STRALLOC(qstr);
    trim_whitespace(new);
    nlen = strlen(new);
    
    /* make sure first and last characters are matching quotes */
    if ( (new[0] == '\"' && new[nlen-1] == '\"')
	|| (new[0] == '\'' && new[nlen-1] == '\''))
    {
	i=0;
	while( i < nlen-1 )      /* copy char 1,2,...,n-2 to */
	{                        /* position  0,1,...,n-3    */
	  new[i] = new[i+1];
	  i++;
	}
	new[nlen-2] = EOS;            /* set position n-2 to EOS */
	
	trim_whitespace(new);
    }

    return(new);
}


/************************************************************************
 *									*
 *  trim_whitespace							*
 *									*
 *  destructively modifies STR by removing leading and trailing		*
 *  whitespace, as well as leaving only a single space between words.	*
 *									*
 ***********************************************************************/

#if UsePrototypes
void
trim_whitespace(char *str)
#else
void
trim_whitespace(str)
char *str;
#endif	/* UsePrototypes */
{
    char *tokenstr, *token;
    
    tokenstr = STRALLOC(str);
    str[0] = EOS;

    /* grab the first token */
    if(token = strtok(tokenstr, delims))
    {
	strcat(str, token);
	/* extract tokens until there are no more */
	while(token = strtok((char *)NULL, delims))
	    /* append to str, but include a space separator */
	    strcat(strcat(str, space), token);
    }

    UFREE(tokenstr);
    return;
}


/************************************************************************
 *									*
 *  muc_str_str								*
 *									*
 *  a sort-of case-insensitive version of strstr.  this version 	*
 *  creates lower-case versions of S and T, then calls strstr.		*
 *  returns pointer to first occurrence of T in S, or NULL if not	*
 *  present.								*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
char *
muc_strstr(char *s, char *t)
#else
static
char *
muc_strstr(s, t)
char *s;
char *t;
#endif	/* UsePrototypes */
{
    char *s_lower, *t_lower;
    char *pos;
    int offset;
    
    s_lower = string_lower(s);
    t_lower = string_lower(t);
    
    /* the position of t_lower in s_lower provides us with the means
       to determine the offset relative to the start of s_lower.
       use this to derive the position of T in S (but only if non-NULL). */

    if((pos = strstr(s_lower, t_lower)) != (char *) NULL)
    {
	offset = pos - s_lower;
	pos = s + offset;
    }
    
    UFREE(s_lower);
    UFREE(t_lower);
    
    return(pos);
}


/************************************************************************
 *									*
 *  string_lower							*
 *									*
 *  allocates and returns a lower-case version of STR.			*
 *  this is primarily for use in string routines that need		*
 *  case-independence.                                                  *
 *                                                                      *
 *  added casts to tolower call, and null char at end of result -ad     *
 *									*
 ***********************************************************************/

#if UsePrototypes
char *
string_lower(char *str)
#else
char *
string_lower(str)
char *str;
#endif	/* UsePrototypes */
{
    char *newstr;
    int i = 0, slen = strlen(str);
    
    newstr = STRALLOC(str);
    
    /* make an explicit test for whether each character 
       is actually an uppercase Roman letter.  This is 
       because 8 bit chars, like 11111101, were getting
       "lowered" to 00000000 */

    for( i=0; i<slen; i++ )
      if( (newstr[i] >= 'A') && (newstr[i] <= 'Z') )
	newstr[i] = tolower(str[i]);
    
    return(newstr);
}


/************************************************************************
 *									*
 *  nice_docnum								*
 *									*
 *  take an extracted docnum string (from an SGML document), and	*
 *  return a new string that contains no '-' or '.'			*
 *									*
 *  the pattern for the docnum string is as follows:			*
 *									*
 *	[-]<number>[-<number>][.]					*
 *									*
 *    a leading '-' is optional (depends on the calling function)	*
 *    the number sequence is either <6digits>'-'<4digits> or		*
 *	simply <4digits>						*
 *    a trailing '.' may also appear					*
 *									*
 ***********************************************************************/

#if UsePrototypes
char *
nice_docnum(char *docnum)
#else
char *
nice_docnum(docnum)
char *docnum;
#endif	/* UsePrototypes */
{
    char *new, *nptr;
    
    if ((new = (char *) malloc (strlen(docnum) +1)) == (char *)NULL)
    {
	fprintf(stderr, "(nice_docnum): malloc failed.\n");
	exit(1);
    }

    nptr = new;
    
    /* copy docnum onto new, but increment new only if the
       character is neither '-' nor '.' */
    while((*nptr = *docnum++) != '\0')
    {
	if (!((*nptr == '-') || (*nptr == '.')))
	    nptr++;
    }

    return(new);
}


/************************************************************************
 *									*
 *  get_class_docnum_id							*
 *									*
 *  take an instance header (from a template file), and extract		*
 *  the class name, the docnum, and the 1up id.				*
 *									*
 *  the pattern for the docnum string is as follows:			*
 *									*
 *	'<'classname-docnum-1up'>'					*
 *
 *  example 
     given:
       header == "<PERSON-9402240133-1>..."
     return:
       cname  == "PERSON"
       docnum == "9402240133"
       id     == "1"
 *
 *				  					*
 ***********************************************************************/

#if UsePrototypes
void
get_class_docnum_id(char *header, char *cname, char *docnum, char *id)
#else
void
get_class_docnum_id(header, cname, docnum, id)
char *header;
char *cname;
char *docnum;
char *id;
#endif	/* UsePrototypes */
{
    char *hptr, *cptr, *dptr, *iptr;
    
    hptr = header;
    cptr = cname;
    dptr = docnum;
    iptr = id;
    
    /* copy class onto cptr.  assume that header starts with '<'
       and is followed by an alpha series. */

    hptr++;                 /* skip '<' */
    if(!isalpha(*hptr))
    {
	fprintf(stderr, "Error: (1) ill-formed instance header '%s'.\n", header);
	exit(1);
    }
    while((*cptr++ = *hptr++) != '-')
	;
    *(--cptr) = '\0';	/* replace the '-' with string terminator */
    
    /* copy docnum onto dptr.  assume that a numeric series follows
       the class name. */

    if(!isdigit(*hptr))
    {
	fprintf(stderr, "Error: (2) ill-formed instance header '%s'.\n", header);
	exit(1);
    }
    while((*dptr++ = *hptr++) != '-')
	;
    *(--dptr) = '\0';	/* replace the '-' with string terminator */

    /* copy 1up id onto iptr.  assume that a numeric series follows
       the docnum. */

    if(!isdigit(*hptr))
    {
	fprintf(stderr, "Error: (3) ill-formed instance header '%s'.\n", header);
	exit(1);
    }
    while((*iptr++ = *hptr++) != '>')
	;
    *(--iptr) = '\0';	/* replace the '>' with string terminator */

    return;
}
/*******************************************************************
 *
 * muc_cmp_special
 *
 * compare the lowercase versions of str1 and str2.  If str1 is 
 * longer than str2, return 1.  Otherwise return normal strcmp of
 * lowercase versions.
 *
 * Changed call to memcmp to a call to strcmp, in case str2 is
 *    longer than str1 (had been memcmp(str1,str2,|str2|))
 * Also, used string_lower to convert chars
 *    -ad 11/12/96
 * 
 * Will this work for non-ASCII "strings" (e.g. 16-bit chinese chars)?
 *    It *should* do the same thing as muc_strcasecmp below, which
 *    I'll leave in for reference -- the main difference is that
 *    string_lower doesn't use an array index to iterate through the 
 *    string; it just increments the char pointers.
 *
 *******************************************************************/
#if UsePrototypes
int
muc_cmp_special(char *str1, char *str2)
#else
int
muc_cmp_special(str1, str2)
char *str1;
char *str2;
#endif	/* UsePrototypes */
{
    char *lowered_str1;
    char *lowered_str2;
    int   result;

    lowered_str1 = string_lower(str1);
    lowered_str2 = string_lower(str2);

    if ( strlen(str1) > strlen(str2) ) 
        result = 1;
    else 
        result = strcmp(lowered_str1, lowered_str2);

    /* Free lowered versions of str1 and str2 */
    UFREE(lowered_str1);
    UFREE(lowered_str2);

    /* Return result of comparision */
    return(result);
}
/************************************************************************
 *									*
 *  muc_strcasecmp							*
 *									*
 *  takes two strings as arguments. 					*
 *									*
 *  the two strings are compared without regard to case.  This is	*
 *  accomplished by converting all upper case characters in each	*
 *  string to lower case and then comparing the two strings using	*
 *  strcmp().								*
 *									*
 *  NOTE:  This function should be used instead of the system-supplied	*
 *  strcasecmp() when the strings to be compared may contain 8-bit	*
 *  characters.								*
 *
 *  Not used in STRCASEEQ -- muc_cmp_special is used
 * 
 *  return less than zero, zero or greater than zero indicating		*
 *  whether the first string is less than, equal to or greater than	*
 *  the second string (without regard to case).				*
 *									*
 ***********************************************************************/

#if UsePrototypes
int
muc_strcasecmp(char *str1, char *str2)
#else
int
muc_strcasecmp(str1, str2)
char *str1;
char *str2;
#endif	/* UsePrototypes */
{
    int   i;
    int   length_str1;
    int   length_str2;
    char *lowered_str1;
    char *lowered_str2;
    int   status;

    /* Get length of each string */
    length_str1 = strlen(str1);
    length_str2 = strlen(str2);

    /* Create memory for lowered version of s1 and s2 */
    lowered_str1 = UALLOC(char, (length_str1 + 1));
    lowered_str2 = UALLOC(char, (length_str2 + 1));

    /* Lower each character in str1 and place it in lowered_str1 */
    for (i = 0; i < length_str1; i++)
	lowered_str1[i] = (char)tolower((int)str1[i]);
    lowered_str1[length_str1] = '\0';

    /* Lower each character in str2 and place it in lowered_str2 */
    for (i = 0; i < length_str2; i++)
	lowered_str2[i] = (char)tolower((int)str2[i]);
    lowered_str2[length_str2] = '\0';

    /* Compare the lowered versions of str1 and str2 */
    status = strcmp(lowered_str1, lowered_str2);

    /* Free lowered versions of str1 and str2 */
    UFREE(lowered_str1);
    UFREE(lowered_str2);

    /* Return status of comparision */
    return(status);
}


/************************************************************************
 *									*
 *  muc_streq								*
 *									*
 *  takes a key string and a response string as arguments. 		*
 *  a minimum substring is extracted from the key (it may not exist).	*
 *  make_clean_string() is then called on key, response, and min	*
 *  strings to remove all premodifiers, postmodifiers, and corporate	*
 *  designators.  muc_min_streq is called if there is a min string,	*
 *  otherwise the key and the response are compared.			*
 *									*
 *  return MATCH if key and response are STRCASEEQ.			*
 *  return MISMATCH if not STRCASEEQ.					*
 *
 *  key2key not supported yet
 *									*
 ***********************************************************************/

#if UsePrototypes
int
muc_streq(char *kstr, char *rstr, char *minstr)
#else
int
muc_streq(kstr, rstr, minstr)
char *kstr;
char *rstr;
char *minstr;
#endif	/* UsePrototypes */
{
    char *kfull, *kclean, *rfull, *rclean, *min, *mclean;
    int status;

    if ( STRCASEEQ(kstr, rstr) )
     {
	status = MATCH;
       /* printf("length key: %i  length response: %i \n",strlen(kstr),strlen(rstr));*/
    }
    else
    {
	kfull = STRALLOC(kstr);
	rfull = STRALLOC(rstr);

	/* get minimum string, either from minstr or from the key string */

	if (minstr == (char *) NULL)
	    min = get_minstring(kfull);
	else
	    min = STRALLOC(minstr);

	/* remove string extras */
	kclean = make_clean_string(kfull);
	rclean = make_clean_string(rfull);

	/* are they equal now after make_clean_string? */
	if (STRCASEEQ(kclean, rclean))
	    status = MATCH;
	else if (min != (char *) NULL)
	/* if min exists, then proceed to muc_min_streq */
	{
	    mclean = STRALLOC(min);
	    mclean = make_clean_string(min);
	    status = muc_min_streq(kclean, rclean, mclean);
	    UFREE(mclean);
	}
	else
	    status = MISMATCH;

	UFREE(kfull);
	UFREE(rfull);
	UFREE(kclean);
	UFREE(rclean);
	UFREE(min);
    }
    
    return(status);
}


/************************************************************************
 *									*
 *  muc_min_streq							*
 *									*
 *  takes a key, a response, and min string as arguments. 		*
 *  this function is called when KSTR is not equal to RSTR, but		*
 *  KSTR contains a minimum substring MSTR.  the comparison and		*
 *  scoring is as follows:						*
 *  									*
 *  	RSTR = MSTR			yields		MATCH		*
 *  	RSTR does not contain MSTR	yields		MISMATCH	*
 *  	RSTR contains MSTR and:						*
 *  	  RSTR is substring of KSTR	yields		MATCH		*
 *  	  RSTR is superstring of KSTR	yields		MISMATCH	*
 *  	  RSTR not equal to KSTR	yields		MISMATCH	*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
int
muc_min_streq(char *kstr, char *rstr, char *mstr)
#else
static
int
muc_min_streq(kstr, rstr, mstr)
char *kstr;
char *rstr;
char *mstr;
#endif	/* UsePrototypes */
{
    /* is mstr and rstr the same? */
    if( STRCASEEQ(mstr, rstr) )
	return(MATCH);
    
    /* is mstr part of rstr, and is rstr part of kstr? */
    if((muc_strstr(rstr, mstr) != (char *) NULL)
       && (muc_strstr(kstr, rstr) != (char *) NULL))
	return(MATCH);
    else
	return(MISMATCH);
}


/************************************************************************
 *									*
 *  get_minstring							*
 *									*
 *  search SRC for a substring delimited by '[]' and copy it to MIN	*
 *  while creating a NEWSRC copy of SRC without the '[]'.  if a		*
 *  substring is found, copy NEWSRC back to SRC, and return the		*
 *  substring as MIN.  if no substring exists, return (char *) NULL.	*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
char *
get_minstring(char *src)
#else
static
char *
get_minstring(src)
char *src;
#endif	/* UsePrototypes */
{
    char *newsrc, *min;
    char *sptr, *nptr, *mptr;

    newsrc = STRALLOC(src);
    min = STRALLOC(src);

    sptr = src;
    nptr = newsrc;
    mptr = min;
    
    /* we want to copy src to newsrc, but without the brackets */
    while((*sptr != '[') && (*sptr != '\0'))
	*nptr++ = *sptr++;

    /* is this the beginning of a minimum substring? */
    if(*sptr == '[')
    {
	sptr++;		/* skip the open bracket */
	
	/* copy the substring to both newsrc and min */
	while((*sptr != ']') && (*sptr != '\0'))
	    *nptr++ = *mptr++ = *sptr++;
	
	/* a close bracket indicates a true substring */
	if(*sptr == ']')
	{
	    /* skip the close bracket */
	    sptr++;

	    /* terminate min */
	    *mptr = '\0';
	    
	    /* copy the rest of src to newsrc */
	    while((*nptr++ = *sptr++) != '\0')
		;
	    /* now, copy newsrc (without '[]') to src */
	    strcpy(src, newsrc);
	}
	else	/* not a true substring */
	{
	    UFREE(newsrc);
	    UFREE(min);
	    min = (char *) NULL;
	}
    }
    else	/* no substring present */
    {
	UFREE(newsrc);
	UFREE(min);
	min = (char *) NULL;
    }
    
    return(min);
}


/************************************************************************
 *									*
 *  make_clean_string							*
 *									*
 *  returns a new version of SRC, without postmodifiers, premodifiers,	*
 *  and corporate designators.						*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
char *
make_clean_string(char *src)
#else
static
char *
make_clean_string(src)
char *src;
#endif	/* UsePrototypes */
{
    char *new;
    
    new = STRALLOC(src);
    
    remove_postmodifiers(new);
    remove_premodifiers(new);
    remove_multi_word_designators(new);
    remove_corp_designators(new);

    return(new);
}


/************************************************************************
 *									*
 *  remove_postmodifiers						*
 *									*
 *  a destructive operation on the SRC argument.			*
 *  removes the last character of SRC if it is a member of the		*
 *  list of string-postmodifiers.					*
 *									*
 *  calls is_postmodifier().						*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
remove_postmodifiers(char *src)
#else
static
void
remove_postmodifiers(src)
char *src;
#endif	/* UsePrototypes */
{
    int last = strlen(src)-1;

    /* check if the last character is a postmodifier */
    if (is_postmodifier(src[last]))
	{
	    src[last] = EOS;
	    trim_whitespace(src);
	}

    return;
}


/************************************************************************
 *									*
 *  is_postmodifier							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
Bool
is_postmodifier(char c)
#else
static
Bool
is_postmodifier(c)
char c;
#endif	/* UsePrototypes */
{
    register int i;
    int num = Postmodifiers->num_vals;
    char **postmods = (char **) Postmodifiers->value_list;
    
    for(i=0; i<num; i++)
	if(c == *postmods[i])
	    return(True);
    
    return(False);
}


/************************************************************************
 *									*
 *  remove_premodifiers							*
 *									*
 *  a destructive operation on the SRC argument.			*
 *  removes string-premodifiers from SRC string by first extracting	*
 *  tokens from SRC and creates a copy of SRC containing tokens that	*
 *  are not string-premodifiers.					*
 *									*
 *  calls is_premodifier().						*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
remove_premodifiers(char *src)
#else
static
void
remove_premodifiers(src)
char *src;
#endif	/* UsePrototypes */
{
    char *tokensrc, *token;
    
    tokensrc = STRALLOC(src);
    src[0] = EOS;

    /* grab the first token */
    if(token = strtok(tokensrc, delims))
    {
	/* append to src if not a premodifier */
	if(!is_premodifier(token))
	    strcat(src, token);
	/* extract tokens until there are no more */
	while(token = strtok((char *)NULL, delims))
	{
	    /* append to src, but include a space separator */
	    if(!is_premodifier(token))
		strcat(strcat(src, space), token);
	}
    }

    UFREE(tokensrc);
    return;
}


/************************************************************************
 *									*
 *  is_premodifier							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
Bool
is_premodifier(char *s)
#else
static
Bool
is_premodifier(s)
char *s;
#endif	/* UsePrototypes */
{
    register int i;
    int num = Premodifiers->num_vals;
    char **premods = (char **) Premodifiers->value_list;
    
    for(i=0; i<num; i++)
	if(STRCASEEQ(s, premods[i]))
	    return(True);
    
    return(False);
}


/************************************************************************
 *									*
 *  remove_multi_word_designators					*
 *									*
 *  a destructive operation on the SRC argument.			*
 *  removes multi-word corporate designators from SRC string.		*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
remove_multi_word_designators(char *src)
#else
static
void
remove_multi_word_designators(src)
char *src;
#endif	/* UsePrototypes */
{
    register int i;
    int num = Corp_designators->num_vals;
    char **designators = (char **) Corp_designators->value_list;
    char *designator;
    
    for(i=0; i<num; i++)
    {
	designator = designators[i];

	/* identify multi-word strings by searching for whitespace */
	if(strpbrk(designator, delims))
	{
	    /* remove all occurrences */
	    while(muc_strstr(src, designator))
		remove_word_from_string(src, designator);
	}
	
    }

    return;
}


/************************************************************************
 *									*
 *  remove_corp_designators						*
 *									*
 *  a destructive operation on the SRC argument.			*
 *  removes corporate designators from SRC string.			*
 *									*
 *  calls is_corp_designator().						*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
remove_corp_designators(char *src)
#else
static
void
remove_corp_designators(src)
char *src;
#endif	/* UsePrototypes */
{
    char *tokensrc, *token;
    
    tokensrc = STRALLOC(src);
    src[0] = EOS;

    /* grab the first token */
    if(token = strtok(tokensrc, delims))
    {
	/* append to src if not a corporate designator */
	if(!is_corp_designator(token))
	    strcat(src, token);
	/* extract tokens until there are no more */
	while(token = strtok((char *)NULL, delims))
	{
	    /* append to src, but include a space separator */
	    if(!is_corp_designator(token))
		strcat(strcat(src, space), token);
	}
    }

    UFREE(tokensrc);
    return;
}


/************************************************************************
 *									*
 *  is_corp_designator							*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
Bool
is_corp_designator(char *s)
#else
static
Bool
is_corp_designator(s)
char *s;
#endif	/* UsePrototypes */
{
    register int i;
    int num = Corp_designators->num_vals;
    char **designators = (char **) Corp_designators->value_list;
    
    for(i=0; i<num; i++)
	if(STRCASEEQ(s, designators[i]))
	    return(True);
    
    return(False);
}


/************************************************************************
 *									*
 *  remove_word_from_string						*
 *									*
 *  Destructive string operation that modifies STR.			*
 *  Removes the first instance of word in string				*
 *									*
 ***********************************************************************/

#if UsePrototypes
static
void
remove_word_from_string(char *str, char *word)
#else
static
void
remove_word_from_string(str, word)
char *str;
char *word;
#endif	/* UsePrototypes */
{
    char *wpos, *reststr;
    int wlen = strlen(word);
    
    /* does word occur in str? */
    if (wpos = muc_strstr(str, word))
    {
	/* set a pointer to the rest of STR that follows WORD */
	reststr = wpos + wlen;
	
	/* trim any leading whitespace reststr */
	trim_whitespace(reststr);
	
	/* now, copy the reststr to the position of word. */
	strcpy(wpos, reststr);

	/* this is to remove trailing whitespace in the even that
	   WORD was at the end of STR */
	trim_whitespace(str);
    }
    
    return;
}


/************************************************************************
 *									*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes

#else

#endif	/* UsePrototypes */


/************************************************************************
 *									*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes

#else

#endif	/* UsePrototypes */


/************************************************************************
 *									*
 *									*
 *									*
 ***********************************************************************/

#if UsePrototypes

#else

#endif	/* UsePrototypes */


