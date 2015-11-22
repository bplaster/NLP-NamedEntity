#include <stdio.h>
# define U(x) x
# define NLSTATE slotconfigprevious=YYNEWLINE
# define BEGIN slotconfigbgin = slotconfigsvec + 1 +
# define INITIAL 0
# define YYLERR slotconfigsvec
# define YYSTATE (slotconfigestate-slotconfigsvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,slotconfigout)
#else
# define lex_output(c) (void)putc(c,slotconfigout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int slotconfigback(int *, int);
	int slotconfiginput(void);
	int slotconfiglook(void);
	void slotconfigoutput(int);
	int slotconfigracc(int);
	int slotconfigreject(void);
	void slotconfigunput(int);
	int slotconfiglex(void);
#ifdef YYLEX_E
	void slotconfigwoutput(wchar_t);
	wchar_t slotconfigwinput(void);
#endif
#ifndef slotconfigless
	int slotconfigless(int);
#endif
#ifndef slotconfigwrap
	int slotconfigwrap(void);
#endif
#ifdef LEXDEBUG
	void allprint(char);
	void sprint(char *);
#endif
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void exit(int);
#ifdef __cplusplus
}
#endif

#endif
# define unput(c) {slotconfigtchar= (c);if(slotconfigtchar=='\n')slotconfiglineno--;*slotconfigsptr++=slotconfigtchar;}
# define slotconfigmore() (slotconfigmorfg=1)
#ifndef __cplusplus
# define input() (((slotconfigtchar=slotconfigsptr>slotconfigsbuf?U(*--slotconfigsptr):getc(slotconfigin))==10?(slotconfiglineno++,slotconfigtchar):slotconfigtchar)==EOF?0:slotconfigtchar)
#else
# define lex_input() (((slotconfigtchar=slotconfigsptr>slotconfigsbuf?U(*--slotconfigsptr):getc(slotconfigin))==10?(slotconfiglineno++,slotconfigtchar):slotconfigtchar)==EOF?0:slotconfigtchar)
#endif
#define ECHO fprintf(slotconfigout, "%s",slotconfigtext)
# define REJECT { nstr = slotconfigreject(); goto slotconfigfussy;}
int slotconfigleng;
char slotconfigtext[YYLMAX];
int slotconfigmorfg;
extern char *slotconfigsptr, slotconfigsbuf[];
int slotconfigtchar;
FILE *slotconfigin = {stdin}, *slotconfigout = {stdout};
extern int slotconfiglineno;
struct slotconfigsvf { 
	struct slotconfigwork *slotconfigstoff;
	struct slotconfigsvf *slotconfigother;
	int *slotconfigstops;};
struct slotconfigsvf *slotconfigestate;
extern struct slotconfigsvf slotconfigsvec[], *slotconfigbgin;

# line 3 "slotconfig.l"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */


# line 7 "slotconfig.l"
/*
 * NAME
 *
 * 
 * FILE 
 *	slotconfig.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of the MUC slot configuration file.
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
static	char	SccsId[] = "@(#)slotconfig.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "y.slotconfig.h"

#ifndef FLEX_SCANNER
#undef slotconfigwrap
#endif


# line 49 "slotconfig.l"
/*
 *  lex definitions to parse MUC configuration file.
 */

Proto    (extern void, slotconfigerror, (const char *msg));

int  slotlineno;

# define YYNEWLINE 10
slotconfiglex(){
int nstr; extern int slotconfigprevious;

# line 62 "slotconfig.l"
/* comments */
#ifdef __cplusplus
/* to avoid CC and lint complaining slotconfigfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto slotconfigfussy;
#endif
while((nstr = slotconfiglook()) >= 0)
slotconfigfussy: switch(nstr){
case 0:
if(slotconfigwrap()) return(0); break;
case 1:

# line 64 "slotconfig.l"
		{ ; }
break;

# line 68 "slotconfig.l"
/* open or close parenthesis */
case 2:

# line 70 "slotconfig.l"
		{ return(slotconfigtext[0]); }
break;

# line 74 "slotconfig.l"
/* text */
case 3:

# line 76 "slotconfig.l"
	{ slotconfiglval.sval = (char *) strdup(slotconfigtext);
			  return STRING; }
break;

# line 80 "slotconfig.l"
/* single-quoted string */
case 4:

# line 82 "slotconfig.l"
	{ slotconfiglval.sval = (char *) strdup(slotconfigtext);
			  return QSTRING; }
break;

# line 86 "slotconfig.l"
/* double-quoted string */
case 5:

# line 88 "slotconfig.l"
	{ slotconfiglval.sval = (char *) strdup(slotconfigtext);
			  return QSTRING; }
break;

# line 93 "slotconfig.l"
/* whitespace */
case 6:

# line 95 "slotconfig.l"
		{ ; }
break;

# line 99 "slotconfig.l"
/* eol */
case 7:

# line 101 "slotconfig.l"
		{ slotlineno++; }
break;

# line 105 "slotconfig.l"
/* other */
case 8:

# line 107 "slotconfig.l"
		{ slotconfigerror("invalid character"); }
break;
case -1:
break;
default:
(void)fprintf(slotconfigout,"bad switch slotconfiglook %d",nstr);
} return(0); }
/* end of slotconfiglex */

#if UsePrototypes
void
slotconfigerror(const char *msg)
#else
void
slotconfigerror(msg)
char *msg;
#endif	/* UsePrototypes */
{
    printf("(slotconfig) %d: %s at '%s'\n", slotlineno, msg, slotconfigtext);
}

#if UsePrototypes
int
slotconfigwrap(void)
#else
int
slotconfigwrap()
#endif  /* UsePrototypes */
{
    return(1);
}

int slotconfigvstop[] = {
0,

3,
8,
0,

6,
8,
0,

7,
0,

3,
8,
0,

3,
8,
0,

2,
8,
0,

1,
3,
8,
0,

3,
0,

3,
0,

3,
5,
0,

3,
0,

3,
4,
0,

1,
3,
0,

1,
0,

5,
0,

4,
0,
0};
# define YYTYPE unsigned char
struct slotconfigwork { YYTYPE verify, advance; } slotconfigcrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	3,10,	0,0,	
0,0,	6,11,	1,4,	1,5,	
11,0,	14,0,	3,0,	3,0,	
7,14,	6,12,	6,0,	9,17,	
17,0,	0,0,	0,0,	0,0,	
7,15,	7,0,	0,0,	9,18,	
9,0,	0,0,	10,0,	10,0,	
0,0,	0,0,	12,12,	1,6,	
2,9,	3,0,	0,0,	3,10,	
1,7,	1,8,	6,13,	12,0,	
3,10,	3,0,	3,0,	6,11,	
6,12,	7,14,	13,0,	13,0,	
9,17,	10,0,	7,16,	7,15,	
15,15,	9,17,	9,18,	0,0,	
0,0,	10,0,	10,0,	0,0,	
0,0,	15,0,	0,0,	12,19,	
16,0,	16,0,	18,18,	0,0,	
12,12,	13,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	18,0,	
0,0,	13,0,	13,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	15,15,	0,0,	16,0,	
0,0,	0,0,	15,20,	0,0,	
0,0,	0,0,	0,0,	16,0,	
16,0,	0,0,	0,0,	18,18,	
0,0,	0,0,	0,0,	0,0,	
18,18,	0,0,	0,0,	0,0,	
0,0};
struct slotconfigsvf slotconfigsvec[] = {
0,	0,	0,
slotconfigcrank+-1,	0,		0,	
slotconfigcrank+-1,	slotconfigsvec+1,	0,	
slotconfigcrank+-5,	0,		slotconfigvstop+1,
slotconfigcrank+0,	0,		slotconfigvstop+4,
slotconfigcrank+0,	0,		slotconfigvstop+7,
slotconfigcrank+-8,	0,		slotconfigvstop+9,
slotconfigcrank+-15,	0,		slotconfigvstop+12,
slotconfigcrank+0,	0,		slotconfigvstop+15,
slotconfigcrank+-18,	0,		slotconfigvstop+18,
slotconfigcrank+-21,	slotconfigsvec+3,	slotconfigvstop+22,
slotconfigcrank+-2,	slotconfigsvec+6,	slotconfigvstop+24,
slotconfigcrank+-33,	slotconfigsvec+6,	0,	
slotconfigcrank+-41,	slotconfigsvec+3,	slotconfigvstop+26,
slotconfigcrank+-3,	slotconfigsvec+7,	slotconfigvstop+29,
slotconfigcrank+-55,	slotconfigsvec+7,	0,	
slotconfigcrank+-59,	slotconfigsvec+3,	slotconfigvstop+31,
slotconfigcrank+-10,	slotconfigsvec+9,	slotconfigvstop+34,
slotconfigcrank+-69,	slotconfigsvec+9,	slotconfigvstop+37,
slotconfigcrank+0,	0,		slotconfigvstop+39,
slotconfigcrank+0,	0,		slotconfigvstop+41,
0,	0,	0};
struct slotconfigwork *slotconfigtop = slotconfigcrank+108;
struct slotconfigsvf *slotconfigbgin = slotconfigsvec+1;
char slotconfigmatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,  34,   1,   1,   1,   1,  39, 
 40,  40,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
0};
char slotconfigextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int slotconfiglineno =1;
# define YYU(x) x
# define NLSTATE slotconfigprevious=YYNEWLINE
struct slotconfigsvf *slotconfiglstate [YYLMAX], **slotconfiglsp, **slotconfigolsp;
char slotconfigsbuf[YYLMAX];
char *slotconfigsptr = slotconfigsbuf;
int *slotconfigfnd;
extern struct slotconfigsvf *slotconfigestate;
int slotconfigprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int slotconfiglook(void)
#else
slotconfiglook()
#endif
{
	register struct slotconfigsvf *slotconfigstate, **lsp;
	register struct slotconfigwork *slotconfigt;
	struct slotconfigsvf *slotconfigz;
	int slotconfigch, slotconfigfirst;
	struct slotconfigwork *slotconfigr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *slotconfiglastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	slotconfigfirst=1;
	if (!slotconfigmorfg)
		slotconfiglastch = slotconfigtext;
	else {
		slotconfigmorfg=0;
		slotconfiglastch = slotconfigtext+slotconfigleng;
		}
	for(;;){
		lsp = slotconfiglstate;
		slotconfigestate = slotconfigstate = slotconfigbgin;
		if (slotconfigprevious==YYNEWLINE) slotconfigstate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(slotconfigout,"state %d\n",slotconfigstate-slotconfigsvec-1);
# endif
			slotconfigt = slotconfigstate->slotconfigstoff;
			if(slotconfigt == slotconfigcrank && !slotconfigfirst){  /* may not be any transitions */
				slotconfigz = slotconfigstate->slotconfigother;
				if(slotconfigz == 0)break;
				if(slotconfigz->slotconfigstoff == slotconfigcrank)break;
				}
#ifndef __cplusplus
			*slotconfiglastch++ = slotconfigch = input();
#else
			*slotconfiglastch++ = slotconfigch = lex_input();
#endif
			if(slotconfiglastch > &slotconfigtext[YYLMAX]) {
				fprintf(slotconfigout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			slotconfigfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(slotconfigout,"char ");
				allprint(slotconfigch);
				putchar('\n');
				}
# endif
			slotconfigr = slotconfigt;
			if ( (int)slotconfigt > (int)slotconfigcrank){
				slotconfigt = slotconfigr + slotconfigch;
				if (slotconfigt <= slotconfigtop && slotconfigt->verify+slotconfigsvec == slotconfigstate){
					if(slotconfigt->advance+slotconfigsvec == YYLERR)	/* error transitions */
						{unput(*--slotconfiglastch);break;}
					*lsp++ = slotconfigstate = slotconfigt->advance+slotconfigsvec;
					if(lsp > &slotconfiglstate[YYLMAX]) {
						fprintf(slotconfigout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)slotconfigt < (int)slotconfigcrank) {		/* r < slotconfigcrank */
				slotconfigt = slotconfigr = slotconfigcrank+(slotconfigcrank-slotconfigt);
# ifdef LEXDEBUG
				if(debug)fprintf(slotconfigout,"compressed state\n");
# endif
				slotconfigt = slotconfigt + slotconfigch;
				if(slotconfigt <= slotconfigtop && slotconfigt->verify+slotconfigsvec == slotconfigstate){
					if(slotconfigt->advance+slotconfigsvec == YYLERR)	/* error transitions */
						{unput(*--slotconfiglastch);break;}
					*lsp++ = slotconfigstate = slotconfigt->advance+slotconfigsvec;
					if(lsp > &slotconfiglstate[YYLMAX]) {
						fprintf(slotconfigout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				slotconfigt = slotconfigr + YYU(slotconfigmatch[slotconfigch]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(slotconfigout,"try fall back character ");
					allprint(YYU(slotconfigmatch[slotconfigch]));
					putchar('\n');
					}
# endif
				if(slotconfigt <= slotconfigtop && slotconfigt->verify+slotconfigsvec == slotconfigstate){
					if(slotconfigt->advance+slotconfigsvec == YYLERR)	/* error transition */
						{unput(*--slotconfiglastch);break;}
					*lsp++ = slotconfigstate = slotconfigt->advance+slotconfigsvec;
					if(lsp > &slotconfiglstate[YYLMAX]) {
						fprintf(slotconfigout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((slotconfigstate = slotconfigstate->slotconfigother) && (slotconfigt= slotconfigstate->slotconfigstoff) != slotconfigcrank){
# ifdef LEXDEBUG
				if(debug)fprintf(slotconfigout,"fall back to state %d\n",slotconfigstate-slotconfigsvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--slotconfiglastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(slotconfigout,"state %d char ",slotconfigstate-slotconfigsvec-1);
				allprint(slotconfigch);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(slotconfigout,"stopped at %d with ",*(lsp-1)-slotconfigsvec-1);
			allprint(slotconfigch);
			putchar('\n');
			}
# endif
		while (lsp-- > slotconfiglstate){
			*slotconfiglastch-- = 0;
			if (*lsp != 0 && (slotconfigfnd= (*lsp)->slotconfigstops) && *slotconfigfnd > 0){
				slotconfigolsp = lsp;
				if(slotconfigextra[*slotconfigfnd]){		/* must backup */
					while(slotconfigback((*lsp)->slotconfigstops,-*slotconfigfnd) != 1 && lsp > slotconfiglstate){
						lsp--;
						unput(*slotconfiglastch--);
						}
					}
				slotconfigprevious = YYU(*slotconfiglastch);
				slotconfiglsp = lsp;
				slotconfigleng = slotconfiglastch-slotconfigtext+1;
				slotconfigtext[slotconfigleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(slotconfigout,"\nmatch ");
					sprint(slotconfigtext);
					fprintf(slotconfigout," action %d\n",*slotconfigfnd);
					}
# endif
				return(*slotconfigfnd++);
				}
			unput(*slotconfiglastch);
			}
		if (slotconfigtext[0] == 0  /* && feof(slotconfigin) */)
			{
			slotconfigsptr=slotconfigsbuf;
			return(0);
			}
#ifndef __cplusplus
		slotconfigprevious = slotconfigtext[0] = input();
		if (slotconfigprevious>0)
			output(slotconfigprevious);
#else
		slotconfigprevious = slotconfigtext[0] = lex_input();
		if (slotconfigprevious>0)
			lex_output(slotconfigprevious);
#endif
		slotconfiglastch=slotconfigtext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int slotconfigback(int *p, int m)
#else
slotconfigback(p, m)
	int *p;
#endif
{
	if (p==0) return(0);
	while (*p) {
		if (*p++ == m)
			return(1);
	}
	return(0);
}
	/* the following are only used in the lex library */
#if defined(__cplusplus) || defined(__STDC__)
int slotconfiginput(void)
#else
slotconfiginput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void slotconfigoutput(int c)
#else
slotconfigoutput(c)
  int c; 
#endif
{
#ifndef __cplusplus
	output(c);
#else
	lex_output(c);
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void slotconfigunput(int c)
#else
slotconfigunput(c)
   int c; 
#endif
{
	unput(c);
	}
