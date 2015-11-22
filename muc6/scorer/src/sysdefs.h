#ifndef SYSDEFS_H
#define SYSDEFS_H

#ifndef	lint
static char SccsId_sysdefs_h[] = "@(#)sysdefs.h	105.1 19 May 1995 SAIC";
#endif /* lint */

#ifndef __svr4__

#ifndef _TYPES_
#include	<sys/types.h>
#endif /* _TYPES_ */

#ifndef _TIME_
#include	<sys/time.h>
#endif /* _TIME_ */

/*
 * Provide definitions of standard library functions.
 */

#include "cdefs.h"   /* For consistent definition of Proto macro. */

Proto (extern off_t,	lseek, (int fd, off_t o, int mode));
Proto (extern char,	*index, (char *s, int ch));
Proto (extern char,	*rindex, (char *s, int ch));
Proto (extern int,	getopt, (int argc, char *const *argv, 
				 const char *optstring));
Proto (extern int,	gettimeofday, (struct timeval *timep, 
				       struct timezone *tzp));
Proto (extern int,	fork, (void));
Proto (extern int,	select, (int nfds, 
				 fd_set *readfds, 
				 fd_set *writefds,
				 fd_set *exceptfds, 
				 struct timeval *timeout));
Proto (extern int,	system, (const char *string));

Proto (extern int,	bzero, (char *a, int len));
Proto (extern void,	bcopy, (char *src, char *dst, int len));
Proto (extern int,	bcmp, (char *b1, char *b2, int len));

Proto (extern time_t,	time, (time_t *t));

#ifdef FILE
Proto (extern int,	fprintf, (FILE *stream, const char *format, ...));
Proto (extern int,	fscanf, (FILE *stream, const char *format, ...));
Proto (extern int,	fseek, (FILE *stream, long offset, int ptrname));
Proto (extern int,	fflush, (FILE *stream));
Proto (extern int,	fclose, (FILE *stream));
Proto (extern int,	fread, (char *buf, int size, int count, FILE *stream));
Proto (extern int,	fwrite, (char *buf, int size, int count, FILE *stream));
Proto (extern int,	fgetc, (FILE *stream));
Proto (extern int,	ungetc, (int c, FILE *stream));
Proto (extern int,	_filbuf, ());		/* UGH!!! */
Proto (extern int,	_flsbuf, ());		/* UGH!!! */

Proto (extern int,      vfprintf, (FILE *, char *, void *));
Proto (extern int,	pclose, (FILE *stream));
	
#ifndef getc
Proto (extern int,	getc, (FILE *stream));
#endif /*getc*/

#else  /*FILE*/

#ifndef __stdio_h	/* For /usr/5include/stdio.h */
# if __bsd__
Proto (extern char *,	sprintf, (char *, const char *format, ...));
# else
Proto (extern int,	sprintf, (char *, const char *format, ...));
# endif
#endif /*!__stdio_h*/

#endif /*FILE*/

Proto (extern void,	perror, (const char *message));

Proto (extern int,	printf, (const char *format, ...));
Proto (extern int,	scanf, (const char *format, ...));
Proto (extern int,	sscanf, (const char *string, const char *format, ...));

/**
 ** This is declared as unsigned in isis.h, we had it here as void, which
 ** caused conflicts when including both includefiles and compiling
 ** with a strict compiler.  Also, the arg type is int in isis, we
 ** had it as unsigned in this file.  They're both changed to int
 ** here until we can decide who is right and fix the problem.
 **/
Proto (extern unsigned,	sleep, (unsigned seconds));

#ifdef WNOHANG
Proto (extern int,	wait, (union wait *statusp));
Proto (extern int,	wait3, (union wait *statusp, 
				int options, 
				struct rusage *ruse));
#endif /* WNOHANG */

#ifdef __STAT_HEADER__
Proto (extern int, 	stat, (char *path, struct stat *buf));
Proto (extern int,	lstat, (char *path, struct stat *buf));
Proto (extern int,	fstat, (int fd, struct stat *buf));
#endif /* __STAT_HEADER__ */

#endif /* not __svr4__ */

#endif /* SYSDEFS_H */

