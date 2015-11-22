#include <stdio.h>
# define U(x) x
# define NLSTATE templatesprevious=YYNEWLINE
# define BEGIN templatesbgin = templatessvec + 1 +
# define INITIAL 0
# define YYLERR templatessvec
# define YYSTATE (templatesestate-templatessvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,templatesout)
#else
# define lex_output(c) (void)putc(c,templatesout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int templatesback(int *, int);
	int templatesinput(void);
	int templateslook(void);
	void templatesoutput(int);
	int templatesracc(int);
	int templatesreject(void);
	void templatesunput(int);
	int templateslex(void);
#ifdef YYLEX_E
	void templateswoutput(wchar_t);
	wchar_t templateswinput(void);
#endif
#ifndef templatesless
	int templatesless(int);
#endif
#ifndef templateswrap
	int templateswrap(void);
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
# define unput(c) {templatestchar= (c);if(templatestchar=='\n')templateslineno--;*templatessptr++=templatestchar;}
# define templatesmore() (templatesmorfg=1)
#ifndef __cplusplus
# define input() (((templatestchar=templatessptr>templatessbuf?U(*--templatessptr):getc(templatesin))==10?(templateslineno++,templatestchar):templatestchar)==EOF?0:templatestchar)
#else
# define lex_input() (((templatestchar=templatessptr>templatessbuf?U(*--templatessptr):getc(templatesin))==10?(templateslineno++,templatestchar):templatestchar)==EOF?0:templatestchar)
#endif
#define ECHO fprintf(templatesout, "%s",templatestext)
# define REJECT { nstr = templatesreject(); goto templatesfussy;}
int templatesleng;
char templatestext[YYLMAX];
int templatesmorfg;
extern char *templatessptr, templatessbuf[];
int templatestchar;
FILE *templatesin = {stdin}, *templatesout = {stdout};
extern int templateslineno;
struct templatessvf { 
	struct templateswork *templatesstoff;
	struct templatessvf *templatesother;
	int *templatesstops;};
struct templatessvf *templatesestate;
extern struct templatessvf templatessvec[], *templatesbgin;

# line 3 "template.l"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */


# line 7 "template.l"
/*
 * NAME
 *
 * 
 * FILE 
 *	template.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of MUC template files.
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
static	char	SccsId[] = "@(#)template.l	1.3 31 Aug 1995  Copyright 1995 Science Applications International Corporation.";
#endif


#include "cdefs.h"
#include "y.template.h"

#ifndef FLEX_SCANNER
#undef templateswrap
#endif


# line 49 "template.l"
/*
 *  lex definitions to parse MUC template file.
 */

Proto    (extern void, templateserror, (const char *msg));
Proto    (extern int, templateswrap, (void));
Proto    (extern void, finish_template, (void));

long  templlineno, templcharno;
long  templbegin;

# define SLOT 2
# define YYNEWLINE 10
templateslex(){
int nstr; extern int templatesprevious;

# line 67 "template.l"
/* comments */
#ifdef __cplusplus
/* to avoid CC and lint complaining templatesfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto templatesfussy;
#endif
while((nstr = templateslook()) >= 0)
templatesfussy: switch(nstr){
case 0:
if(templateswrap()) return(0); break;
case 1:

# line 69 "template.l"
			{ templcharno += templatesleng; }
break;

# line 73 "template.l"
/* instance header */
case 2:

# line 75 "template.l"
{ templbegin = templcharno;
				  templcharno += templatesleng;
				  templateslval.sval = (char *) strdup(templatestext);
				  return INST_START; }
break;

# line 82 "template.l"
/* slot name */
case 3:

# line 84 "template.l"
	{ templcharno += templatesleng;
				  templateslval.sval = (char *) strdup(templatestext);
				  BEGIN SLOT;
				  return SLOTNAME; }
break;

# line 91 "template.l"
/* eol while parsing slot */
case 4:

# line 93 "template.l"
		{ templlineno++;
				  templcharno++;
				  BEGIN INITIAL; }
break;

# line 99 "template.l"
/* optional marker while parsing slot */
case 5:

# line 101 "template.l"
		{ templcharno += templatesleng;
				  BEGIN INITIAL;
				  return OPTMARK; }
break;

# line 107 "template.l"
/* alternate marker */
case 6:

# line 109 "template.l"
		{ templcharno += templatesleng;
				  return ALTMARK; }
break;

# line 114 "template.l"
/* instance pointer */
case 7:

# line 116 "template.l"
		{ templcharno += templatesleng;
				  templateslval.sval = (char *) strdup(templatestext);
				  return POINTER; }
break;

# line 122 "template.l"
/* double-quoted string */
case 8:

# line 124 "template.l"
		{ templcharno += templatesleng;
				  templateslval.sval = (char *) strdup(templatestext);
				  return QSTRING; }
break;

# line 130 "template.l"
/* single-quoted string */
case 9:

# line 132 "template.l"
		{ templcharno += templatesleng;
				  templateslval.sval = (char *) strdup(templatestext);
				  return QSTRING; }
break;

# line 138 "template.l"
/* slot filler */
case 10:

# line 140 "template.l"
	{ templcharno += templatesleng;
				  templateslval.sval = (char *) strdup(templatestext);
				  return SLOT_FILL; }
break;

# line 146 "template.l"
/* whitespace */
case 11:

# line 148 "template.l"
			{ templcharno += templatesleng; }
break;

# line 152 "template.l"
/* eol */
case 12:

# line 154 "template.l"
			{ templlineno++;
				  templcharno++; }
break;

# line 159 "template.l"
/* other */
case 13:

# line 161 "template.l"
			{ templateserror("invalid character"); }
break;
case -1:
break;
default:
(void)fprintf(templatesout,"bad switch templateslook %d",nstr);
} return(0); }
/* end of templateslex */

#if UsePrototypes
void
templateserror(const char *msg)
#else
void
templateserror(msg)
char *msg;
#endif  /* UsePrototypes */
{
    printf("(template) %d: %s at '%s'\n", templlineno, msg, templatestext);
}


#if UsePrototypes
int
templateswrap(void)
#else
int
templateswrap()
#endif  /* UsePrototypes */
{
    finish_template();
    return(1);
}
int templatesvstop[] = {
0,

13,
0,

11,
13,
0,

12,
0,

13,
0,

13,
0,

13,
0,

13,
0,

13,
0,

11,
13,
0,

12,
0,

13,
0,

1,
13,
0,

13,
0,

6,
13,
0,

13,
0,

11,
13,
0,

4,
12,
0,

5,
13,
0,

11,
13,
0,

4,
12,
0,

5,
6,
13,
0,

10,
0,

10,
0,

8,
10,
0,

10,
0,

9,
10,
0,

10,
0,

10,
0,

3,
10,
0,

6,
0,

3,
0,

10,
0,

8,
10,
0,

3,
10,
0,

1,
10,
0,

1,
0,

1,
3,
10,
0,

10,
0,

9,
10,
0,

3,
10,
0,

10,
0,

3,
10,
0,

5,
0,

5,
6,
0,

7,
10,
0,

7,
0,

1,
10,
0,

7,
10,
0,

3,
0,

7,
0,

3,
10,
0,

3,
0,

10,
0,

2,
10,
0,

2,
0,

2,
10,
0,

2,
0,
0};
# define YYTYPE unsigned char
struct templateswork { YYTYPE verify, advance; } templatescrank[] = {
0,0,	0,0,	1,5,	0,0,	
0,0,	2,12,	0,0,	0,0,	
0,0,	0,0,	1,6,	1,7,	
5,26,	2,13,	2,14,	26,0,	
3,20,	3,21,	4,23,	4,24,	
5,27,	5,0,	27,0,	28,0,	
29,0,	30,0,	31,0,	32,0,	
33,0,	58,0,	23,56,	60,0,	
72,0,	0,0,	14,0,	1,8,	
1,5,	20,54,	2,15,	2,16,	
1,9,	0,0,	61,64,	2,17,	
8,28,	5,26,	5,26,	0,0,	
1,10,	0,0,	5,26,	2,18,	
8,29,	8,0,	3,22,	68,0,	
4,25,	14,0,	5,26,	36,38,	
20,54,	1,11,	9,31,	1,5,	
2,19,	11,34,	2,12,	3,11,	
23,57,	4,19,	9,32,	9,0,	
14,37,	5,26,	11,35,	20,55,	
12,36,	8,30,	8,28,	13,37,	
38,38,	43,38,	8,28,	14,41,	
12,27,	12,37,	15,42,	13,39,	
13,37,	49,38,	8,28,	61,65,	
18,0,	16,45,	15,29,	9,31,	
9,31,	69,38,	11,34,	11,34,	
9,33,	16,46,	70,73,	11,34,	
0,0,	8,28,	68,72,	56,56,	
9,31,	12,36,	12,36,	11,34,	
13,37,	13,37,	12,36,	18,0,	
0,0,	13,37,	65,38,	15,43,	
15,42,	65,69,	12,36,	9,31,	
15,42,	13,40,	16,45,	16,45,	
17,48,	24,0,	18,37,	16,45,	
15,42,	12,38,	25,0,	19,51,	
17,32,	12,36,	0,0,	16,45,	
13,37,	18,41,	34,34,	15,44,	
19,52,	56,57,	0,0,	15,42,	
37,0,	0,0,	16,47,	34,35,	
24,0,	0,0,	16,45,	0,0,	
40,0,	25,0,	0,0,	0,0,	
0,0,	17,48,	17,48,	63,66,	
0,0,	0,0,	17,49,	24,37,	
19,51,	19,51,	41,0,	37,0,	
25,37,	19,51,	17,48,	34,34,	
34,34,	0,0,	24,41,	40,0,	
34,34,	19,51,	35,35,	25,41,	
62,0,	17,50,	37,37,	0,0,	
34,34,	17,48,	35,0,	35,35,	
19,53,	41,0,	40,37,	0,0,	
42,42,	37,41,	44,42,	0,0,	
57,0,	63,37,	66,66,	34,58,	
42,29,	40,41,	44,29,	62,0,	
41,37,	64,64,	64,0,	0,0,	
63,67,	35,0,	45,45,	35,35,	
35,35,	0,0,	0,0,	41,41,	
35,35,	0,0,	45,46,	57,0,	
0,0,	66,66,	0,0,	0,0,	
35,35,	42,43,	42,42,	44,43,	
44,42,	62,62,	42,42,	0,0,	
44,42,	0,0,	57,37,	0,0,	
0,0,	0,0,	42,42,	35,59,	
44,42,	0,0,	71,0,	45,45,	
45,45,	57,41,	0,0,	66,70,	
45,45,	42,44,	46,60,	44,44,	
47,45,	42,42,	64,68,	44,42,	
45,45,	0,0,	46,46,	46,0,	
47,46,	48,48,	0,0,	0,0,	
0,0,	71,0,	0,0,	45,47,	
50,48,	48,32,	51,51,	45,45,	
0,0,	0,0,	0,0,	0,0,	
50,32,	0,0,	0,0,	51,52,	
71,37,	0,0,	0,0,	46,60,	
46,60,	47,45,	47,45,	0,0,	
46,60,	0,0,	47,45,	71,41,	
0,0,	0,0,	48,48,	48,48,	
46,60,	0,0,	47,45,	48,49,	
0,0,	50,48,	50,48,	51,51,	
51,51,	0,0,	50,49,	48,48,	
51,51,	47,47,	0,0,	46,60,	
52,52,	47,45,	50,48,	53,51,	
51,51,	0,0,	48,50,	67,0,	
52,0,	52,52,	48,48,	0,0,	
53,52,	50,50,	0,0,	51,53,	
0,0,	50,48,	0,0,	51,61,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	67,0,	52,0,	
0,0,	52,52,	52,52,	0,0,	
53,51,	53,51,	52,52,	0,0,	
0,0,	53,51,	0,0,	0,0,	
0,0,	67,37,	52,52,	0,0,	
0,0,	53,51,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
67,41,	52,62,	0,0,	67,71,	
53,53,	52,63,	0,0,	0,0,	
53,61,	0,0,	0,0,	0,0,	
0,0};
struct templatessvf templatessvec[] = {
0,	0,	0,
templatescrank+-1,	0,		0,	
templatescrank+-4,	0,		0,	
templatescrank+-7,	templatessvec+1,	0,	
templatescrank+-9,	templatessvec+2,	0,	
templatescrank+-11,	0,		templatesvstop+1,
templatescrank+0,	0,		templatesvstop+3,
templatescrank+0,	0,		templatesvstop+6,
templatescrank+-43,	0,		templatesvstop+8,
templatescrank+-61,	0,		templatesvstop+10,
templatescrank+0,	0,		templatesvstop+12,
templatescrank+-64,	templatessvec+5,	templatesvstop+14,
templatescrank+-75,	0,		templatesvstop+16,
templatescrank+-78,	0,		templatesvstop+18,
templatescrank+-25,	templatessvec+13,	templatesvstop+21,
templatescrank+-85,	templatessvec+12,	templatesvstop+23,
templatescrank+-92,	templatessvec+12,	templatesvstop+25,
templatescrank+-127,	templatessvec+12,	templatesvstop+28,
templatescrank+-83,	templatessvec+13,	templatesvstop+30,
templatescrank+-134,	templatessvec+12,	templatesvstop+33,
templatescrank+28,	0,		templatesvstop+35,
templatescrank+0,	0,		templatesvstop+38,
templatescrank+0,	0,		templatesvstop+41,
templatescrank+-21,	templatessvec+13,	templatesvstop+44,
templatescrank+-120,	templatessvec+13,	templatesvstop+47,
templatescrank+-125,	templatessvec+13,	templatesvstop+50,
templatescrank+-5,	templatessvec+5,	templatesvstop+54,
templatescrank+-12,	templatessvec+5,	0,	
templatescrank+-13,	templatessvec+8,	templatesvstop+56,
templatescrank+-14,	templatessvec+8,	0,	
templatescrank+-15,	templatessvec+5,	templatesvstop+58,
templatescrank+-16,	templatessvec+9,	templatesvstop+61,
templatescrank+-17,	templatessvec+9,	0,	
templatescrank+-18,	templatessvec+5,	templatesvstop+63,
templatescrank+-141,	templatessvec+5,	templatesvstop+66,
templatescrank+-181,	0,		0,	
templatescrank+-1,	templatessvec+12,	templatesvstop+68,
templatescrank+-139,	templatessvec+13,	0,	
templatescrank+-22,	templatessvec+12,	templatesvstop+70,
templatescrank+0,	templatessvec+13,	0,	
templatescrank+-147,	templatessvec+13,	templatesvstop+73,
templatescrank+-161,	templatessvec+13,	templatesvstop+75,
templatescrank+-195,	templatessvec+12,	templatesvstop+77,
templatescrank+-23,	templatessvec+12,	templatesvstop+79,
templatescrank+-197,	templatessvec+12,	templatesvstop+82,
templatescrank+-213,	templatessvec+12,	templatesvstop+85,
templatescrank+-253,	0,		templatesvstop+88,
templatescrank+-255,	templatessvec+12,	templatesvstop+90,
templatescrank+-264,	templatessvec+12,	templatesvstop+94,
templatescrank+-31,	templatessvec+12,	templatesvstop+96,
templatescrank+-271,	templatessvec+12,	templatesvstop+99,
templatescrank+-273,	templatessvec+12,	templatesvstop+102,
templatescrank+-315,	0,		0,	
templatescrank+-318,	templatessvec+12,	templatesvstop+104,
templatescrank+0,	templatessvec+20,	0,	
templatescrank+0,	0,		templatesvstop+107,
templatescrank+-98,	templatessvec+13,	0,	
templatescrank+-191,	templatessvec+13,	templatesvstop+109,
templatescrank+-19,	templatessvec+5,	templatesvstop+112,
templatescrank+0,	0,		templatesvstop+115,
templatescrank+-21,	templatessvec+46,	templatesvstop+117,
templatescrank+-33,	templatessvec+12,	templatesvstop+120,
templatescrank+-175,	templatessvec+52,	templatesvstop+123,
templatescrank+-154,	templatessvec+13,	templatesvstop+125,
templatescrank+-200,	templatessvec+5,	0,	
templatescrank+-60,	templatessvec+12,	templatesvstop+127,
templatescrank+193,	0,		0,	
templatescrank+-314,	templatessvec+13,	templatesvstop+130,
templatescrank+-45,	templatessvec+5,	templatesvstop+132,
templatescrank+-39,	templatessvec+12,	templatesvstop+134,
templatescrank+41,	0,		0,	
templatescrank+-237,	templatessvec+13,	templatesvstop+137,
templatescrank+-22,	templatessvec+5,	templatesvstop+139,
templatescrank+0,	0,		templatesvstop+142,
0,	0,	0};
struct templateswork *templatestop = templatescrank+380;
struct templatessvf *templatesbgin = templatessvec+1;
char templatesmatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,  34,  35,   1,   1,   1,  39, 
  1,   1,   1,   1,   1,   1,   1,  47, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,  35,   1,   1,  62,   1, 
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
char templatesextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int templateslineno =1;
# define YYU(x) x
# define NLSTATE templatesprevious=YYNEWLINE
struct templatessvf *templateslstate [YYLMAX], **templateslsp, **templatesolsp;
char templatessbuf[YYLMAX];
char *templatessptr = templatessbuf;
int *templatesfnd;
extern struct templatessvf *templatesestate;
int templatesprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int templateslook(void)
#else
templateslook()
#endif
{
	register struct templatessvf *templatesstate, **lsp;
	register struct templateswork *templatest;
	struct templatessvf *templatesz;
	int templatesch, templatesfirst;
	struct templateswork *templatesr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *templateslastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	templatesfirst=1;
	if (!templatesmorfg)
		templateslastch = templatestext;
	else {
		templatesmorfg=0;
		templateslastch = templatestext+templatesleng;
		}
	for(;;){
		lsp = templateslstate;
		templatesestate = templatesstate = templatesbgin;
		if (templatesprevious==YYNEWLINE) templatesstate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(templatesout,"state %d\n",templatesstate-templatessvec-1);
# endif
			templatest = templatesstate->templatesstoff;
			if(templatest == templatescrank && !templatesfirst){  /* may not be any transitions */
				templatesz = templatesstate->templatesother;
				if(templatesz == 0)break;
				if(templatesz->templatesstoff == templatescrank)break;
				}
#ifndef __cplusplus
			*templateslastch++ = templatesch = input();
#else
			*templateslastch++ = templatesch = lex_input();
#endif
			if(templateslastch > &templatestext[YYLMAX]) {
				fprintf(templatesout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			templatesfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(templatesout,"char ");
				allprint(templatesch);
				putchar('\n');
				}
# endif
			templatesr = templatest;
			if ( (int)templatest > (int)templatescrank){
				templatest = templatesr + templatesch;
				if (templatest <= templatestop && templatest->verify+templatessvec == templatesstate){
					if(templatest->advance+templatessvec == YYLERR)	/* error transitions */
						{unput(*--templateslastch);break;}
					*lsp++ = templatesstate = templatest->advance+templatessvec;
					if(lsp > &templateslstate[YYLMAX]) {
						fprintf(templatesout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)templatest < (int)templatescrank) {		/* r < templatescrank */
				templatest = templatesr = templatescrank+(templatescrank-templatest);
# ifdef LEXDEBUG
				if(debug)fprintf(templatesout,"compressed state\n");
# endif
				templatest = templatest + templatesch;
				if(templatest <= templatestop && templatest->verify+templatessvec == templatesstate){
					if(templatest->advance+templatessvec == YYLERR)	/* error transitions */
						{unput(*--templateslastch);break;}
					*lsp++ = templatesstate = templatest->advance+templatessvec;
					if(lsp > &templateslstate[YYLMAX]) {
						fprintf(templatesout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				templatest = templatesr + YYU(templatesmatch[templatesch]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(templatesout,"try fall back character ");
					allprint(YYU(templatesmatch[templatesch]));
					putchar('\n');
					}
# endif
				if(templatest <= templatestop && templatest->verify+templatessvec == templatesstate){
					if(templatest->advance+templatessvec == YYLERR)	/* error transition */
						{unput(*--templateslastch);break;}
					*lsp++ = templatesstate = templatest->advance+templatessvec;
					if(lsp > &templateslstate[YYLMAX]) {
						fprintf(templatesout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((templatesstate = templatesstate->templatesother) && (templatest= templatesstate->templatesstoff) != templatescrank){
# ifdef LEXDEBUG
				if(debug)fprintf(templatesout,"fall back to state %d\n",templatesstate-templatessvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--templateslastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(templatesout,"state %d char ",templatesstate-templatessvec-1);
				allprint(templatesch);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(templatesout,"stopped at %d with ",*(lsp-1)-templatessvec-1);
			allprint(templatesch);
			putchar('\n');
			}
# endif
		while (lsp-- > templateslstate){
			*templateslastch-- = 0;
			if (*lsp != 0 && (templatesfnd= (*lsp)->templatesstops) && *templatesfnd > 0){
				templatesolsp = lsp;
				if(templatesextra[*templatesfnd]){		/* must backup */
					while(templatesback((*lsp)->templatesstops,-*templatesfnd) != 1 && lsp > templateslstate){
						lsp--;
						unput(*templateslastch--);
						}
					}
				templatesprevious = YYU(*templateslastch);
				templateslsp = lsp;
				templatesleng = templateslastch-templatestext+1;
				templatestext[templatesleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(templatesout,"\nmatch ");
					sprint(templatestext);
					fprintf(templatesout," action %d\n",*templatesfnd);
					}
# endif
				return(*templatesfnd++);
				}
			unput(*templateslastch);
			}
		if (templatestext[0] == 0  /* && feof(templatesin) */)
			{
			templatessptr=templatessbuf;
			return(0);
			}
#ifndef __cplusplus
		templatesprevious = templatestext[0] = input();
		if (templatesprevious>0)
			output(templatesprevious);
#else
		templatesprevious = templatestext[0] = lex_input();
		if (templatesprevious>0)
			lex_output(templatesprevious);
#endif
		templateslastch=templatestext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int templatesback(int *p, int m)
#else
templatesback(p, m)
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
int templatesinput(void)
#else
templatesinput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void templatesoutput(int c)
#else
templatesoutput(c)
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
void templatesunput(int c)
#else
templatesunput(c)
   int c; 
#endif
{
	unput(c);
	}
