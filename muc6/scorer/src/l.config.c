#include <stdio.h>
# define U(x) x
# define NLSTATE configprevious=YYNEWLINE
# define BEGIN configbgin = configsvec + 1 +
# define INITIAL 0
# define YYLERR configsvec
# define YYSTATE (configestate-configsvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,configout)
#else
# define lex_output(c) (void)putc(c,configout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int configback(int *, int);
	int configinput(void);
	int configlook(void);
	void configoutput(int);
	int configracc(int);
	int configreject(void);
	void configunput(int);
	int configlex(void);
#ifdef YYLEX_E
	void configwoutput(wchar_t);
	wchar_t configwinput(void);
#endif
#ifndef configless
	int configless(int);
#endif
#ifndef configwrap
	int configwrap(void);
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
# define unput(c) {configtchar= (c);if(configtchar=='\n')configlineno--;*configsptr++=configtchar;}
# define configmore() (configmorfg=1)
#ifndef __cplusplus
# define input() (((configtchar=configsptr>configsbuf?U(*--configsptr):getc(configin))==10?(configlineno++,configtchar):configtchar)==EOF?0:configtchar)
#else
# define lex_input() (((configtchar=configsptr>configsbuf?U(*--configsptr):getc(configin))==10?(configlineno++,configtchar):configtchar)==EOF?0:configtchar)
#endif
#define ECHO fprintf(configout, "%s",configtext)
# define REJECT { nstr = configreject(); goto configfussy;}
int configleng;
char configtext[YYLMAX];
int configmorfg;
extern char *configsptr, configsbuf[];
int configtchar;
FILE *configin = {stdin}, *configout = {stdout};
extern int configlineno;
struct configsvf { 
	struct configwork *configstoff;
	struct configsvf *configother;
	int *configstops;};
struct configsvf *configestate;
extern struct configsvf configsvec[], *configbgin;

# line 3 "config.l"
/*
 *  Copyright 1995 Science Applications International Corporation.
 */


# line 7 "config.l"
/*
 * NAME
 *
 * 
 * FILE 
 *	config.l
 *
 * SYNOPSIS
 *
 *
 * DESCRIPTION
 *	This file contains the lex specifications for the lexical
 *	analyzer of the MUC configuration file.
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
static	char	SccsId[] = "@(#)config.l	1.4 2/27/96  Copyright 1995 Science Applications International Corporation.";
#endif


#include  "cdefs.h"
#include  "y.config.h"


#ifndef FLEX_SCANNER
#undef configwrap
#endif


# line 50 "config.l"
/*********************************************************************
 *                                                                   *
 *  lex definitions to parse MUC configuration file.                 *
 *                                                                   *
 ********************************************************************/

Proto    (extern void,  configerror, (const char *msg));

int  configlineno;

# define YYNEWLINE 10
configlex(){
int nstr; extern int configprevious;

# line 65 "config.l"
/* comments */
#ifdef __cplusplus
/* to avoid CC and lint complaining configfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto configfussy;
#endif
while((nstr = configlook()) >= 0)
configfussy: switch(nstr){
case 0:
if(configwrap()) return(0); break;
case 1:

# line 67 "config.l"
			{ ; }
break;

# line 71 "config.l"
/* option name */
case 2:

# line 73 "config.l"
{ configlval.sval = (char *) strdup(configtext);
				  return OPTION; }
break;

# line 78 "config.l"
/* single-quoted string */
case 3:

# line 80 "config.l"
		{ configlval.sval = (char *) strdup(configtext);
				  return QSTRING; }
break;

# line 85 "config.l"
/* double-quoted string */
case 4:

# line 87 "config.l"
		{ configlval.sval = (char *) strdup(configtext);
				  return QSTRING; }
break;

# line 92 "config.l"
/* text */
case 5:

# line 94 "config.l"
		{ configlval.sval = (char *) strdup(configtext);
				  return VALUE; }
break;

# line 99 "config.l"
/* eol */
case 6:

# line 101 "config.l"
			{ configlineno++; }
break;

# line 105 "config.l"
/* whitespace */
case 7:

# line 107 "config.l"
			{ ; }
break;

# line 111 "config.l"
/* other */
case 8:

# line 113 "config.l"
			{ configerror("invalid character"); }
break;
case -1:
break;
default:
(void)fprintf(configout,"bad switch configlook %d",nstr);
} return(0); }
/* end of configlex */

#if UsePrototypes
void
configerror(const char *msg)
#else
void
configerror(msg)
char *msg;
#endif	/* UsePrototypes */
{
    printf("(config) %d: %s at '%s'\n", configlineno, msg, configtext);
}

#if UsePrototypes
int
configwrap(void)
#else
int
configwrap()
#endif  /* UsePrototypes */
{
    return(1);
}

int configvstop[] = {
0,

5,
8,
0,

7,
8,
0,

6,
0,

5,
8,
0,

5,
8,
0,

1,
5,
8,
0,

5,
8,
0,

5,
0,

5,
0,

4,
5,
0,

5,
0,

3,
5,
0,

1,
5,
0,

1,
0,

2,
5,
0,

4,
0,

3,
0,
0};
# define YYTYPE unsigned char
struct configwork { YYTYPE verify, advance; } configcrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	3,10,	0,0,	
0,0,	6,11,	1,4,	1,5,	
11,0,	14,0,	3,0,	3,0,	
7,14,	6,12,	6,0,	8,17,	
9,0,	9,0,	10,0,	10,0,	
7,15,	7,0,	17,0,	8,18,	
8,0,	0,0,	0,0,	0,0,	
0,0,	13,0,	13,0,	1,6,	
2,8,	3,0,	0,0,	3,10,	
1,7,	0,0,	6,13,	9,0,	
3,10,	10,0,	1,3,	6,11,	
0,0,	7,14,	3,10,	0,0,	
8,17,	6,11,	7,16,	12,12,	
13,0,	8,17,	15,15,	2,9,	
7,14,	16,0,	16,0,	8,17,	
12,0,	18,18,	1,3,	15,0,	
0,0,	0,0,	3,10,	19,0,	
19,0,	6,11,	18,0,	0,0,	
9,19,	0,0,	0,0,	0,0,	
7,14,	0,0,	0,0,	8,17,	
16,0,	0,0,	0,0,	0,0,	
12,20,	0,0,	0,0,	15,15,	
0,0,	12,12,	19,0,	0,0,	
15,21,	0,0,	18,18,	12,12,	
0,0,	0,0,	15,15,	18,18,	
0,0,	0,0,	0,0,	19,19,	
0,0,	18,18,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	12,12,	
0,0,	0,0,	15,15,	0,0,	
0,0,	0,0,	0,0,	19,19,	
0,0,	18,18,	0,0,	0,0,	
0,0};
struct configsvf configsvec[] = {
0,	0,	0,
configcrank+-1,	0,		0,	
configcrank+-1,	configsvec+1,	0,	
configcrank+-5,	0,		configvstop+1,
configcrank+0,	0,		configvstop+4,
configcrank+0,	0,		configvstop+7,
configcrank+-8,	0,		configvstop+9,
configcrank+-15,	0,		configvstop+12,
configcrank+-18,	0,		configvstop+15,
configcrank+-11,	configsvec+3,	configvstop+19,
configcrank+-13,	configsvec+3,	configvstop+22,
configcrank+-2,	configsvec+6,	configvstop+24,
configcrank+-54,	configsvec+6,	0,	
configcrank+-24,	configsvec+3,	configvstop+26,
configcrank+-3,	configsvec+7,	configvstop+29,
configcrank+-57,	configsvec+7,	0,	
configcrank+-52,	configsvec+3,	configvstop+31,
configcrank+-16,	configsvec+8,	configvstop+34,
configcrank+-64,	configsvec+8,	configvstop+37,
configcrank+-62,	configsvec+3,	configvstop+39,
configcrank+0,	0,		configvstop+42,
configcrank+0,	0,		configvstop+44,
0,	0,	0};
struct configwork *configtop = configcrank+129;
struct configsvf *configbgin = configsvec+1;
char configmatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,  34,   1,   1,   1,   1,  39, 
  1,   1,   1,   1,   1,  45,   1,   1, 
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
char configextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int configlineno =1;
# define YYU(x) x
# define NLSTATE configprevious=YYNEWLINE
struct configsvf *configlstate [YYLMAX], **configlsp, **configolsp;
char configsbuf[YYLMAX];
char *configsptr = configsbuf;
int *configfnd;
extern struct configsvf *configestate;
int configprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int configlook(void)
#else
configlook()
#endif
{
	register struct configsvf *configstate, **lsp;
	register struct configwork *configt;
	struct configsvf *configz;
	int configch, configfirst;
	struct configwork *configr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *configlastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	configfirst=1;
	if (!configmorfg)
		configlastch = configtext;
	else {
		configmorfg=0;
		configlastch = configtext+configleng;
		}
	for(;;){
		lsp = configlstate;
		configestate = configstate = configbgin;
		if (configprevious==YYNEWLINE) configstate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(configout,"state %d\n",configstate-configsvec-1);
# endif
			configt = configstate->configstoff;
			if(configt == configcrank && !configfirst){  /* may not be any transitions */
				configz = configstate->configother;
				if(configz == 0)break;
				if(configz->configstoff == configcrank)break;
				}
#ifndef __cplusplus
			*configlastch++ = configch = input();
#else
			*configlastch++ = configch = lex_input();
#endif
			if(configlastch > &configtext[YYLMAX]) {
				fprintf(configout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			configfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(configout,"char ");
				allprint(configch);
				putchar('\n');
				}
# endif
			configr = configt;
			if ( (int)configt > (int)configcrank){
				configt = configr + configch;
				if (configt <= configtop && configt->verify+configsvec == configstate){
					if(configt->advance+configsvec == YYLERR)	/* error transitions */
						{unput(*--configlastch);break;}
					*lsp++ = configstate = configt->advance+configsvec;
					if(lsp > &configlstate[YYLMAX]) {
						fprintf(configout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)configt < (int)configcrank) {		/* r < configcrank */
				configt = configr = configcrank+(configcrank-configt);
# ifdef LEXDEBUG
				if(debug)fprintf(configout,"compressed state\n");
# endif
				configt = configt + configch;
				if(configt <= configtop && configt->verify+configsvec == configstate){
					if(configt->advance+configsvec == YYLERR)	/* error transitions */
						{unput(*--configlastch);break;}
					*lsp++ = configstate = configt->advance+configsvec;
					if(lsp > &configlstate[YYLMAX]) {
						fprintf(configout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				configt = configr + YYU(configmatch[configch]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(configout,"try fall back character ");
					allprint(YYU(configmatch[configch]));
					putchar('\n');
					}
# endif
				if(configt <= configtop && configt->verify+configsvec == configstate){
					if(configt->advance+configsvec == YYLERR)	/* error transition */
						{unput(*--configlastch);break;}
					*lsp++ = configstate = configt->advance+configsvec;
					if(lsp > &configlstate[YYLMAX]) {
						fprintf(configout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((configstate = configstate->configother) && (configt= configstate->configstoff) != configcrank){
# ifdef LEXDEBUG
				if(debug)fprintf(configout,"fall back to state %d\n",configstate-configsvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--configlastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(configout,"state %d char ",configstate-configsvec-1);
				allprint(configch);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(configout,"stopped at %d with ",*(lsp-1)-configsvec-1);
			allprint(configch);
			putchar('\n');
			}
# endif
		while (lsp-- > configlstate){
			*configlastch-- = 0;
			if (*lsp != 0 && (configfnd= (*lsp)->configstops) && *configfnd > 0){
				configolsp = lsp;
				if(configextra[*configfnd]){		/* must backup */
					while(configback((*lsp)->configstops,-*configfnd) != 1 && lsp > configlstate){
						lsp--;
						unput(*configlastch--);
						}
					}
				configprevious = YYU(*configlastch);
				configlsp = lsp;
				configleng = configlastch-configtext+1;
				configtext[configleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(configout,"\nmatch ");
					sprint(configtext);
					fprintf(configout," action %d\n",*configfnd);
					}
# endif
				return(*configfnd++);
				}
			unput(*configlastch);
			}
		if (configtext[0] == 0  /* && feof(configin) */)
			{
			configsptr=configsbuf;
			return(0);
			}
#ifndef __cplusplus
		configprevious = configtext[0] = input();
		if (configprevious>0)
			output(configprevious);
#else
		configprevious = configtext[0] = lex_input();
		if (configprevious>0)
			lex_output(configprevious);
#endif
		configlastch=configtext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int configback(int *p, int m)
#else
configback(p, m)
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
int configinput(void)
#else
configinput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void configoutput(int c)
#else
configoutput(c)
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
void configunput(int c)
#else
configunput(c)
   int c; 
#endif
{
	unput(c);
	}
