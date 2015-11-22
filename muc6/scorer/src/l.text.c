#include <stdio.h>
# define U(x) x
# define NLSTATE textsprevious=YYNEWLINE
# define BEGIN textsbgin = textssvec + 1 +
# define INITIAL 0
# define YYLERR textssvec
# define YYSTATE (textsestate-textssvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,textsout)
#else
# define lex_output(c) (void)putc(c,textsout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int textsback(int *, int);
	int textsinput(void);
	int textslook(void);
	void textsoutput(int);
	int textsracc(int);
	int textsreject(void);
	void textsunput(int);
	int textslex(void);
#ifdef YYLEX_E
	void textswoutput(wchar_t);
	wchar_t textswinput(void);
#endif
#ifndef textsless
	int textsless(int);
#endif
#ifndef textswrap
	int textswrap(void);
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
# define unput(c) {textstchar= (c);if(textstchar=='\n')textslineno--;*textssptr++=textstchar;}
# define textsmore() (textsmorfg=1)
#ifndef __cplusplus
# define input() (((textstchar=textssptr>textssbuf?U(*--textssptr):getc(textsin))==10?(textslineno++,textstchar):textstchar)==EOF?0:textstchar)
#else
# define lex_input() (((textstchar=textssptr>textssbuf?U(*--textssptr):getc(textsin))==10?(textslineno++,textstchar):textstchar)==EOF?0:textstchar)
#endif
#define ECHO fprintf(textsout, "%s",textstext)
# define REJECT { nstr = textsreject(); goto textsfussy;}
int textsleng;
char textstext[YYLMAX];
int textsmorfg;
extern char *textssptr, textssbuf[];
int textstchar;
FILE *textsin = {stdin}, *textsout = {stdout};
extern int textslineno;
struct textssvf { 
	struct textswork *textsstoff;
	struct textssvf *textsother;
	int *textsstops;};
struct textssvf *textsestate;
extern struct textssvf textssvec[], *textsbgin;

# line 3 "text.l"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */


# line 7 "text.l"
/*
 * NAME
 *
 * 
 * FILE 
 *	text.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of MUC text files.
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
static	char	SccsId[] = "@(#)text.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "y.text.h"

#ifndef FLEX_SCANNER
#undef textswrap
#endif


# line 49 "text.l"
/*
 *  lex definitions to parse MUC text file.
 */

Proto    (extern void, textserror, (const char *msg));

long  textlineno, textcharno;
long  textbegin, textend;

# define YYNEWLINE 10
textslex(){
int nstr; extern int textsprevious;

# line 63 "text.l"
/*  start of document */
#ifdef __cplusplus
/* to avoid CC and lint complaining textsfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto textsfussy;
#endif
while((nstr = textslook()) >= 0)
textsfussy: switch(nstr){
case 0:
if(textswrap()) return(0); break;
case 1:

# line 65 "text.l"
	{ textbegin = textcharno;
				  textcharno += textsleng;
				  return DOC_START; }
break;

# line 71 "text.l"
/*  end of document */
case 2:

# line 73 "text.l"
	{ textcharno += textsleng;
				  textend = textcharno;
				  return DOC_END; }
break;

# line 79 "text.l"
/*  start of document number sgml tag */
case 3:

# line 81 "text.l"
{ textcharno += textsleng;
				  return DOCNO_START; }
break;

# line 86 "text.l"
/*  end of document number */
case 4:

# line 88 "text.l"
{ textcharno += textsleng;
				  return DOCNO_END; }
break;

# line 93 "text.l"
/*  text */
case 5:

# line 95 "text.l"
		{ textslval.sval = (char *) strdup(textstext);
				  textcharno += textsleng;
				  return STRING; }
break;

# line 101 "text.l"
/*  whitespace */
case 6:

# line 103 "text.l"
			{ textcharno += textsleng; }
break;

# line 107 "text.l"
/*  eol */
case 7:

# line 109 "text.l"
			{ textlineno++;
				  textcharno++; }
break;

# line 114 "text.l"
/*  other */
case 8:

# line 116 "text.l"
			{ textserror("invalid character"); }
break;
case -1:
break;
default:
(void)fprintf(textsout,"bad switch textslook %d",nstr);
} return(0); }
/* end of textslex */

#if UsePrototypes
void
textserror(const char *msg)
#else
void
textserror(msg)
char *msg;
#endif	/* UsePrototypes */
{
    printf("(text) %d: %s at '%s'\n", textlineno, msg, textstext);
}

#if UsePrototypes
int
textswrap(void)
#else
int
textswrap()
#endif  /* UsePrototypes */
{
    return(1);
}

int textsvstop[] = {
0,

5,
8,
0,

6,
8,
0,

7,
0,

5,
8,
0,

5,
0,

5,
0,

5,
0,

5,
0,

5,
0,

5,
0,

5,
0,

5,
0,

1,
5,
0,

5,
0,

2,
5,
0,

5,
0,

5,
0,

5,
0,

3,
5,
0,

4,
5,
0,
0};
# define YYTYPE unsigned char
struct textswork { YYTYPE verify, advance; } textscrank[] = {
0,0,	0,0,	1,3,	0,0,	
3,7,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
3,0,	3,0,	6,0,	6,0,	
7,0,	7,0,	8,0,	8,0,	
9,0,	9,0,	10,0,	10,0,	
0,0,	11,0,	11,0,	12,0,	
12,0,	0,0,	0,0,	13,0,	
13,0,	14,0,	14,0,	3,0,	
0,0,	6,0,	0,0,	7,0,	
0,0,	8,0,	0,0,	9,0,	
0,0,	10,0,	0,0,	0,0,	
11,0,	0,0,	12,0,	0,0,	
6,8,	0,0,	13,0,	0,0,	
14,0,	0,0,	0,0,	0,0,	
0,0,	1,6,	2,6,	0,0,	
15,0,	15,0,	16,0,	16,0,	
1,3,	1,3,	3,7,	3,7,	
0,0,	6,9,	17,0,	17,0,	
0,0,	8,10,	0,0,	1,3,	
1,3,	3,7,	3,7,	11,13,	
13,15,	12,14,	14,17,	15,0,	
0,0,	16,0,	9,11,	0,0,	
10,12,	18,0,	18,0,	19,0,	
19,0,	17,0,	20,0,	20,0,	
13,16,	0,0,	14,18,	21,0,	
21,0,	22,0,	22,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
18,0,	0,0,	19,0,	0,0,	
0,0,	20,0,	0,0,	0,0,	
0,0,	0,0,	21,0,	0,0,	
22,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
16,19,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
19,21,	0,0,	0,0,	20,22,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	18,20,	
0,0};
struct textssvf textssvec[] = {
0,	0,	0,
textscrank+-1,	0,		0,	
textscrank+-2,	textssvec+1,	0,	
textscrank+-3,	0,		textsvstop+1,
textscrank+0,	0,		textsvstop+4,
textscrank+0,	0,		textsvstop+7,
textscrank+-5,	textssvec+3,	textsvstop+9,
textscrank+-7,	textssvec+3,	textsvstop+12,
textscrank+-9,	textssvec+3,	textsvstop+14,
textscrank+-11,	textssvec+3,	textsvstop+16,
textscrank+-13,	textssvec+3,	textsvstop+18,
textscrank+-16,	textssvec+3,	textsvstop+20,
textscrank+-18,	textssvec+3,	textsvstop+22,
textscrank+-22,	textssvec+3,	textsvstop+24,
textscrank+-24,	textssvec+3,	textsvstop+26,
textscrank+-55,	textssvec+3,	textsvstop+28,
textscrank+-57,	textssvec+3,	textsvstop+31,
textscrank+-65,	textssvec+3,	textsvstop+33,
textscrank+-84,	textssvec+3,	textsvstop+36,
textscrank+-86,	textssvec+3,	textsvstop+38,
textscrank+-89,	textssvec+3,	textsvstop+40,
textscrank+-94,	textssvec+3,	textsvstop+42,
textscrank+-96,	textssvec+3,	textsvstop+45,
0,	0,	0};
struct textswork *textstop = textscrank+163;
struct textssvf *textsbgin = textssvec+1;
char textsmatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,  67,  68,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,  78,  79, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,  67,  68,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,  78,  79, 
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
char textsextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int textslineno =1;
# define YYU(x) x
# define NLSTATE textsprevious=YYNEWLINE
struct textssvf *textslstate [YYLMAX], **textslsp, **textsolsp;
char textssbuf[YYLMAX];
char *textssptr = textssbuf;
int *textsfnd;
extern struct textssvf *textsestate;
int textsprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int textslook(void)
#else
textslook()
#endif
{
	register struct textssvf *textsstate, **lsp;
	register struct textswork *textst;
	struct textssvf *textsz;
	int textsch, textsfirst;
	struct textswork *textsr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *textslastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	textsfirst=1;
	if (!textsmorfg)
		textslastch = textstext;
	else {
		textsmorfg=0;
		textslastch = textstext+textsleng;
		}
	for(;;){
		lsp = textslstate;
		textsestate = textsstate = textsbgin;
		if (textsprevious==YYNEWLINE) textsstate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(textsout,"state %d\n",textsstate-textssvec-1);
# endif
			textst = textsstate->textsstoff;
			if(textst == textscrank && !textsfirst){  /* may not be any transitions */
				textsz = textsstate->textsother;
				if(textsz == 0)break;
				if(textsz->textsstoff == textscrank)break;
				}
#ifndef __cplusplus
			*textslastch++ = textsch = input();
#else
			*textslastch++ = textsch = lex_input();
#endif
			if(textslastch > &textstext[YYLMAX]) {
				fprintf(textsout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			textsfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(textsout,"char ");
				allprint(textsch);
				putchar('\n');
				}
# endif
			textsr = textst;
			if ( (int)textst > (int)textscrank){
				textst = textsr + textsch;
				if (textst <= textstop && textst->verify+textssvec == textsstate){
					if(textst->advance+textssvec == YYLERR)	/* error transitions */
						{unput(*--textslastch);break;}
					*lsp++ = textsstate = textst->advance+textssvec;
					if(lsp > &textslstate[YYLMAX]) {
						fprintf(textsout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)textst < (int)textscrank) {		/* r < textscrank */
				textst = textsr = textscrank+(textscrank-textst);
# ifdef LEXDEBUG
				if(debug)fprintf(textsout,"compressed state\n");
# endif
				textst = textst + textsch;
				if(textst <= textstop && textst->verify+textssvec == textsstate){
					if(textst->advance+textssvec == YYLERR)	/* error transitions */
						{unput(*--textslastch);break;}
					*lsp++ = textsstate = textst->advance+textssvec;
					if(lsp > &textslstate[YYLMAX]) {
						fprintf(textsout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				textst = textsr + YYU(textsmatch[textsch]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(textsout,"try fall back character ");
					allprint(YYU(textsmatch[textsch]));
					putchar('\n');
					}
# endif
				if(textst <= textstop && textst->verify+textssvec == textsstate){
					if(textst->advance+textssvec == YYLERR)	/* error transition */
						{unput(*--textslastch);break;}
					*lsp++ = textsstate = textst->advance+textssvec;
					if(lsp > &textslstate[YYLMAX]) {
						fprintf(textsout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((textsstate = textsstate->textsother) && (textst= textsstate->textsstoff) != textscrank){
# ifdef LEXDEBUG
				if(debug)fprintf(textsout,"fall back to state %d\n",textsstate-textssvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--textslastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(textsout,"state %d char ",textsstate-textssvec-1);
				allprint(textsch);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(textsout,"stopped at %d with ",*(lsp-1)-textssvec-1);
			allprint(textsch);
			putchar('\n');
			}
# endif
		while (lsp-- > textslstate){
			*textslastch-- = 0;
			if (*lsp != 0 && (textsfnd= (*lsp)->textsstops) && *textsfnd > 0){
				textsolsp = lsp;
				if(textsextra[*textsfnd]){		/* must backup */
					while(textsback((*lsp)->textsstops,-*textsfnd) != 1 && lsp > textslstate){
						lsp--;
						unput(*textslastch--);
						}
					}
				textsprevious = YYU(*textslastch);
				textslsp = lsp;
				textsleng = textslastch-textstext+1;
				textstext[textsleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(textsout,"\nmatch ");
					sprint(textstext);
					fprintf(textsout," action %d\n",*textsfnd);
					}
# endif
				return(*textsfnd++);
				}
			unput(*textslastch);
			}
		if (textstext[0] == 0  /* && feof(textsin) */)
			{
			textssptr=textssbuf;
			return(0);
			}
#ifndef __cplusplus
		textsprevious = textstext[0] = input();
		if (textsprevious>0)
			output(textsprevious);
#else
		textsprevious = textstext[0] = lex_input();
		if (textsprevious>0)
			lex_output(textsprevious);
#endif
		textslastch=textstext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int textsback(int *p, int m)
#else
textsback(p, m)
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
int textsinput(void)
#else
textsinput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void textsoutput(int c)
#else
textsoutput(c)
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
void textsunput(int c)
#else
textsunput(c)
   int c; 
#endif
{
	unput(c);
	}
