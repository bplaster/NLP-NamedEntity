#include <stdio.h>
# define U(x) x
# define NLSTATE mapconfigprevious=YYNEWLINE
# define BEGIN mapconfigbgin = mapconfigsvec + 1 +
# define INITIAL 0
# define YYLERR mapconfigsvec
# define YYSTATE (mapconfigestate-mapconfigsvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,mapconfigout)
#else
# define lex_output(c) (void)putc(c,mapconfigout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int mapconfigback(int *, int);
	int mapconfiginput(void);
	int mapconfiglook(void);
	void mapconfigoutput(int);
	int mapconfigracc(int);
	int mapconfigreject(void);
	void mapconfigunput(int);
	int mapconfiglex(void);
#ifdef YYLEX_E
	void mapconfigwoutput(wchar_t);
	wchar_t mapconfigwinput(void);
#endif
#ifndef mapconfigless
	int mapconfigless(int);
#endif
#ifndef mapconfigwrap
	int mapconfigwrap(void);
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
# define unput(c) {mapconfigtchar= (c);if(mapconfigtchar=='\n')mapconfiglineno--;*mapconfigsptr++=mapconfigtchar;}
# define mapconfigmore() (mapconfigmorfg=1)
#ifndef __cplusplus
# define input() (((mapconfigtchar=mapconfigsptr>mapconfigsbuf?U(*--mapconfigsptr):getc(mapconfigin))==10?(mapconfiglineno++,mapconfigtchar):mapconfigtchar)==EOF?0:mapconfigtchar)
#else
# define lex_input() (((mapconfigtchar=mapconfigsptr>mapconfigsbuf?U(*--mapconfigsptr):getc(mapconfigin))==10?(mapconfiglineno++,mapconfigtchar):mapconfigtchar)==EOF?0:mapconfigtchar)
#endif
#define ECHO fprintf(mapconfigout, "%s",mapconfigtext)
# define REJECT { nstr = mapconfigreject(); goto mapconfigfussy;}
int mapconfigleng;
char mapconfigtext[YYLMAX];
int mapconfigmorfg;
extern char *mapconfigsptr, mapconfigsbuf[];
int mapconfigtchar;
FILE *mapconfigin = {stdin}, *mapconfigout = {stdout};
extern int mapconfiglineno;
struct mapconfigsvf { 
	struct mapconfigwork *mapconfigstoff;
	struct mapconfigsvf *mapconfigother;
	int *mapconfigstops;};
struct mapconfigsvf *mapconfigestate;
extern struct mapconfigsvf mapconfigsvec[], *mapconfigbgin;

# line 3 "mapconfig.l"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */


# line 7 "mapconfig.l"
/*
 * NAME
 *
 * 
 * FILE 
 *	mapconfig.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of the MUC map configuration file.
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
static	char	SccsId[] = "@(#)mapconfig.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "muc_defs.h"
#include  "y.mapconfig.h"

#ifndef FLEX_SCANNER
#undef mapconfigwrap
#endif


# line 50 "mapconfig.l"
/*
 *  lex definitions to parse MUC map configuration file.
 */

Proto    (extern void, mapconfigerror, (const char *msg));

int  maplineno;

# define YYNEWLINE 10
mapconfiglex(){
int nstr; extern int mapconfigprevious;

# line 63 "mapconfig.l"
/* comments */
#ifdef __cplusplus
/* to avoid CC and lint complaining mapconfigfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto mapconfigfussy;
#endif
while((nstr = mapconfiglook()) >= 0)
mapconfigfussy: switch(nstr){
case 0:
if(mapconfigwrap()) return(0); break;
case 1:

# line 65 "mapconfig.l"
			{ ; }
break;

# line 69 "mapconfig.l"
/* open or close parenthesis */
case 2:

# line 71 "mapconfig.l"
			{ return(mapconfigtext[0]); }
break;

# line 75 "mapconfig.l"
/* option name */
case 3:

# line 77 "mapconfig.l"
{ if STRCASEEQ(mapconfigtext, ":scored_slots")
				      return SCORED_SLOTS;
				  else if STRCASEEQ(mapconfigtext, ":unscored_slots")
				      return UNSCORED_SLOTS;
				  else if STRCASEEQ(mapconfigtext, ":scored_templates")
				      return SCORED_TEMPLATES;
				  else if STRCASEEQ(mapconfigtext, ":map_order")
				      return MAP_ORDER;
				  else if STRCASEEQ(mapconfigtext, ":map_weights")
				      return MAP_WEIGHTS;
				  else
				      mapconfigerror("unrecognized option"); }
break;

# line 92 "mapconfig.l"
/* literal 'all' */
case 4:

# line 94 "mapconfig.l"
			{ return SCORE_ALL; }
break;

# line 98 "mapconfig.l"
/* text */
case 5:

# line 100 "mapconfig.l"
		{ mapconfiglval.sval = (char *) strdup(mapconfigtext);
				  return STRING; }
break;

# line 104 "mapconfig.l"
/* single-quoted string */
case 6:

# line 106 "mapconfig.l"
		{ mapconfiglval.sval = (char *) strdup(mapconfigtext);
				  return QSTRING; }
break;

# line 110 "mapconfig.l"
/* double-quoted string */
case 7:

# line 112 "mapconfig.l"
		{ mapconfiglval.sval = (char *) strdup(mapconfigtext);
				  return QSTRING; }
break;

# line 117 "mapconfig.l"
/* eol */
case 8:

# line 119 "mapconfig.l"
			{ maplineno++; }
break;

# line 123 "mapconfig.l"
/* whitespace */
case 9:

# line 125 "mapconfig.l"
			{ ; }
break;

# line 129 "mapconfig.l"
/* other */
case 10:

# line 131 "mapconfig.l"
			{ mapconfigerror("invalid character"); }
break;
case -1:
break;
default:
(void)fprintf(mapconfigout,"bad switch mapconfiglook %d",nstr);
} return(0); }
/* end of mapconfiglex */

#if UsePrototypes
void
mapconfigerror(const char *msg)
#else
void
mapconfigerror(msg)
char *msg;
#endif	/* UsePrototypes */
{
    printf("(mapconfig) %d: %s at '%s'\n", maplineno, msg, mapconfigtext);
}

#if UsePrototypes
int
mapconfigwrap(void)
#else
int
mapconfigwrap()
#endif  /* UsePrototypes */
{
    return(1);
}

int mapconfigvstop[] = {
0,

5,
10,
0,

9,
10,
0,

8,
0,

5,
10,
0,

5,
10,
0,

2,
10,
0,

5,
10,
0,

1,
5,
10,
0,

5,
10,
0,

5,
0,

5,
0,

5,
7,
0,

5,
0,

5,
6,
0,

5,
0,

1,
5,
0,

1,
0,

3,
5,
0,

7,
0,

6,
0,

4,
5,
0,
0};
# define YYTYPE unsigned char
struct mapconfigwork { YYTYPE verify, advance; } mapconfigcrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	3,12,	0,0,	0,0,	
0,0,	6,13,	1,4,	1,5,	
13,0,	3,0,	3,0,	16,0,	
20,0,	6,14,	6,0,	7,16,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	7,17,	
7,0,	0,0,	0,0,	0,0,	
0,0,	9,0,	9,0,	1,6,	
3,0,	2,10,	3,12,	0,0,	
1,7,	1,8,	6,15,	3,12,	
3,0,	3,0,	1,3,	6,13,	
6,14,	3,12,	0,0,	0,0,	
7,16,	6,13,	11,0,	11,0,	
9,0,	7,18,	7,17,	0,0,	
2,11,	0,0,	10,20,	7,16,	
9,0,	9,0,	1,3,	0,0,	
0,0,	3,12,	10,21,	10,0,	
0,0,	6,13,	0,0,	14,14,	
0,0,	11,0,	0,0,	0,0,	
12,0,	12,0,	0,0,	7,16,	
14,0,	11,0,	11,0,	17,17,	
0,0,	0,0,	0,0,	15,0,	
15,0,	0,0,	0,0,	10,20,	
17,0,	0,0,	1,9,	2,9,	
10,20,	10,21,	0,0,	12,0,	
18,0,	18,0,	10,20,	0,0,	
14,23,	0,0,	11,22,	12,0,	
12,0,	14,14,	15,0,	19,0,	
19,0,	22,0,	22,0,	14,14,	
17,17,	21,21,	15,0,	15,0,	
0,0,	17,24,	10,20,	18,0,	
0,0,	0,0,	21,0,	17,17,	
9,19,	0,0,	0,0,	18,0,	
18,0,	0,0,	19,0,	14,14,	
22,0,	0,0,	0,0,	25,0,	
25,0,	0,0,	19,0,	19,0,	
22,0,	22,0,	0,0,	17,17,	
0,0,	22,22,	21,21,	0,0,	
0,0,	0,0,	0,0,	21,21,	
0,0,	0,0,	0,0,	0,0,	
0,0,	21,21,	25,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	22,22,	25,0,	25,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	21,21,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	19,25,	0,0,	
0,0};
struct mapconfigsvf mapconfigsvec[] = {
0,	0,	0,
mapconfigcrank+-1,	0,		0,	
mapconfigcrank+-2,	mapconfigsvec+1,	0,	
mapconfigcrank+-4,	0,		mapconfigvstop+1,
mapconfigcrank+0,	0,		mapconfigvstop+4,
mapconfigcrank+0,	0,		mapconfigvstop+7,
mapconfigcrank+-8,	0,		mapconfigvstop+9,
mapconfigcrank+-18,	0,		mapconfigvstop+12,
mapconfigcrank+0,	0,		mapconfigvstop+15,
mapconfigcrank+-24,	mapconfigsvec+3,	mapconfigvstop+18,
mapconfigcrank+-61,	0,		mapconfigvstop+21,
mapconfigcrank+-45,	mapconfigsvec+3,	mapconfigvstop+25,
mapconfigcrank+-71,	mapconfigsvec+3,	mapconfigvstop+28,
mapconfigcrank+-2,	mapconfigsvec+6,	mapconfigvstop+30,
mapconfigcrank+-74,	mapconfigsvec+6,	0,	
mapconfigcrank+-82,	mapconfigsvec+3,	mapconfigvstop+32,
mapconfigcrank+-5,	mapconfigsvec+7,	mapconfigvstop+35,
mapconfigcrank+-86,	mapconfigsvec+7,	0,	
mapconfigcrank+-95,	mapconfigsvec+3,	mapconfigvstop+37,
mapconfigcrank+-106,	mapconfigsvec+3,	mapconfigvstop+40,
mapconfigcrank+-6,	mapconfigsvec+10,	mapconfigvstop+42,
mapconfigcrank+-120,	mapconfigsvec+10,	mapconfigvstop+45,
mapconfigcrank+-108,	mapconfigsvec+3,	mapconfigvstop+47,
mapconfigcrank+0,	0,		mapconfigvstop+50,
mapconfigcrank+0,	0,		mapconfigvstop+52,
mapconfigcrank+-134,	mapconfigsvec+3,	mapconfigvstop+54,
0,	0,	0};
struct mapconfigwork *mapconfigtop = mapconfigcrank+214;
struct mapconfigsvf *mapconfigbgin = mapconfigsvec+1;
char mapconfigmatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,  34,   1,   1,   1,   1,  39, 
 40,  40,   1,   1,   1,  45,   1,   1, 
 45,  45,  45,  45,  45,  45,  45,  45, 
 45,  45,   1,   1,   1,   1,   1,   1, 
  1,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,   1,   1,   1,   1,  45, 
  1,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,   1,   1,   1,   1,   1, 
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
char mapconfigextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int mapconfiglineno =1;
# define YYU(x) x
# define NLSTATE mapconfigprevious=YYNEWLINE
struct mapconfigsvf *mapconfiglstate [YYLMAX], **mapconfiglsp, **mapconfigolsp;
char mapconfigsbuf[YYLMAX];
char *mapconfigsptr = mapconfigsbuf;
int *mapconfigfnd;
extern struct mapconfigsvf *mapconfigestate;
int mapconfigprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int mapconfiglook(void)
#else
mapconfiglook()
#endif
{
	register struct mapconfigsvf *mapconfigstate, **lsp;
	register struct mapconfigwork *mapconfigt;
	struct mapconfigsvf *mapconfigz;
	int mapconfigch, mapconfigfirst;
	struct mapconfigwork *mapconfigr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *mapconfiglastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	mapconfigfirst=1;
	if (!mapconfigmorfg)
		mapconfiglastch = mapconfigtext;
	else {
		mapconfigmorfg=0;
		mapconfiglastch = mapconfigtext+mapconfigleng;
		}
	for(;;){
		lsp = mapconfiglstate;
		mapconfigestate = mapconfigstate = mapconfigbgin;
		if (mapconfigprevious==YYNEWLINE) mapconfigstate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(mapconfigout,"state %d\n",mapconfigstate-mapconfigsvec-1);
# endif
			mapconfigt = mapconfigstate->mapconfigstoff;
			if(mapconfigt == mapconfigcrank && !mapconfigfirst){  /* may not be any transitions */
				mapconfigz = mapconfigstate->mapconfigother;
				if(mapconfigz == 0)break;
				if(mapconfigz->mapconfigstoff == mapconfigcrank)break;
				}
#ifndef __cplusplus
			*mapconfiglastch++ = mapconfigch = input();
#else
			*mapconfiglastch++ = mapconfigch = lex_input();
#endif
			if(mapconfiglastch > &mapconfigtext[YYLMAX]) {
				fprintf(mapconfigout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			mapconfigfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(mapconfigout,"char ");
				allprint(mapconfigch);
				putchar('\n');
				}
# endif
			mapconfigr = mapconfigt;
			if ( (int)mapconfigt > (int)mapconfigcrank){
				mapconfigt = mapconfigr + mapconfigch;
				if (mapconfigt <= mapconfigtop && mapconfigt->verify+mapconfigsvec == mapconfigstate){
					if(mapconfigt->advance+mapconfigsvec == YYLERR)	/* error transitions */
						{unput(*--mapconfiglastch);break;}
					*lsp++ = mapconfigstate = mapconfigt->advance+mapconfigsvec;
					if(lsp > &mapconfiglstate[YYLMAX]) {
						fprintf(mapconfigout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)mapconfigt < (int)mapconfigcrank) {		/* r < mapconfigcrank */
				mapconfigt = mapconfigr = mapconfigcrank+(mapconfigcrank-mapconfigt);
# ifdef LEXDEBUG
				if(debug)fprintf(mapconfigout,"compressed state\n");
# endif
				mapconfigt = mapconfigt + mapconfigch;
				if(mapconfigt <= mapconfigtop && mapconfigt->verify+mapconfigsvec == mapconfigstate){
					if(mapconfigt->advance+mapconfigsvec == YYLERR)	/* error transitions */
						{unput(*--mapconfiglastch);break;}
					*lsp++ = mapconfigstate = mapconfigt->advance+mapconfigsvec;
					if(lsp > &mapconfiglstate[YYLMAX]) {
						fprintf(mapconfigout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				mapconfigt = mapconfigr + YYU(mapconfigmatch[mapconfigch]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(mapconfigout,"try fall back character ");
					allprint(YYU(mapconfigmatch[mapconfigch]));
					putchar('\n');
					}
# endif
				if(mapconfigt <= mapconfigtop && mapconfigt->verify+mapconfigsvec == mapconfigstate){
					if(mapconfigt->advance+mapconfigsvec == YYLERR)	/* error transition */
						{unput(*--mapconfiglastch);break;}
					*lsp++ = mapconfigstate = mapconfigt->advance+mapconfigsvec;
					if(lsp > &mapconfiglstate[YYLMAX]) {
						fprintf(mapconfigout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((mapconfigstate = mapconfigstate->mapconfigother) && (mapconfigt= mapconfigstate->mapconfigstoff) != mapconfigcrank){
# ifdef LEXDEBUG
				if(debug)fprintf(mapconfigout,"fall back to state %d\n",mapconfigstate-mapconfigsvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--mapconfiglastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(mapconfigout,"state %d char ",mapconfigstate-mapconfigsvec-1);
				allprint(mapconfigch);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(mapconfigout,"stopped at %d with ",*(lsp-1)-mapconfigsvec-1);
			allprint(mapconfigch);
			putchar('\n');
			}
# endif
		while (lsp-- > mapconfiglstate){
			*mapconfiglastch-- = 0;
			if (*lsp != 0 && (mapconfigfnd= (*lsp)->mapconfigstops) && *mapconfigfnd > 0){
				mapconfigolsp = lsp;
				if(mapconfigextra[*mapconfigfnd]){		/* must backup */
					while(mapconfigback((*lsp)->mapconfigstops,-*mapconfigfnd) != 1 && lsp > mapconfiglstate){
						lsp--;
						unput(*mapconfiglastch--);
						}
					}
				mapconfigprevious = YYU(*mapconfiglastch);
				mapconfiglsp = lsp;
				mapconfigleng = mapconfiglastch-mapconfigtext+1;
				mapconfigtext[mapconfigleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(mapconfigout,"\nmatch ");
					sprint(mapconfigtext);
					fprintf(mapconfigout," action %d\n",*mapconfigfnd);
					}
# endif
				return(*mapconfigfnd++);
				}
			unput(*mapconfiglastch);
			}
		if (mapconfigtext[0] == 0  /* && feof(mapconfigin) */)
			{
			mapconfigsptr=mapconfigsbuf;
			return(0);
			}
#ifndef __cplusplus
		mapconfigprevious = mapconfigtext[0] = input();
		if (mapconfigprevious>0)
			output(mapconfigprevious);
#else
		mapconfigprevious = mapconfigtext[0] = lex_input();
		if (mapconfigprevious>0)
			lex_output(mapconfigprevious);
#endif
		mapconfiglastch=mapconfigtext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int mapconfigback(int *p, int m)
#else
mapconfigback(p, m)
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
int mapconfiginput(void)
#else
mapconfiginput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void mapconfigoutput(int c)
#else
mapconfigoutput(c)
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
void mapconfigunput(int c)
#else
mapconfigunput(c)
   int c; 
#endif
{
	unput(c);
	}
