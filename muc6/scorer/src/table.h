/*
 *  Copyright 1990 Science Applications International Corporation.
 */

/*
 *  table.h include file.
 */

#ifndef TABLE_H
#define TABLE_H

#ifdef	SCCSID
static char	SccsId_table_h[] = "@(#)table.h	101.1	04 Oct 1994";
#endif


/* Return values from table functions. */
#define	TABLE_ERR	0
#define	TABLE_OK	1

#ifdef __STDC__

typedef   int  (*TABLE_COMPARE)(char *, char *); 
typedef   long (*TABLE_HASH)(char *); 
typedef   void (*TABLE_RELEASE)(char *);

/*
 *  As a temporary fix to accomodate mismatch in the TABLE_* typedef's
 *  and the system prototypes in <search.h> set up the following
 *  to use in a cast.  All app's SHOULD set up their compare functions
 *  according the to following prototype.
 */

typedef   int  (*SEARCH_COMPARE)(const void *, const void *); 

typedef struct table
{
	int	(*add)(char *datum, char *table,
		       TABLE_COMPARE compar);
	int	(*append)(char *datum, char *table,
			  TABLE_COMPARE compar);
	int	(*prepend)(char *datum, char *table,
			   TABLE_COMPARE compar);
	int	(*delet)(char *datum, char *table,
			 TABLE_COMPARE compar,
			 TABLE_RELEASE release);
	char	*(*find)(char *datum, char *table,
			 TABLE_COMPARE compar);
	void	(*first)(char *table);
	char	*(*next)(char *table);
	void	(*private_destroy)(char *table);
        char	*privat;
	int	(*count)(char *table);
				/* user-supplied functions: */
	TABLE_COMPARE compar;
	TABLE_RELEASE release;
	void	(*destroy)(struct table *);
} TABLE;

typedef   void  (*TABLE_DESTROY)(TABLE *); 

#else

typedef   int  (*TABLE_COMPARE)(); 
typedef   long  (*TABLE_HASH)(); 
typedef   void (*TABLE_RELEASE)(); 

typedef   int  (*SEARCH_COMPARE)(); 

typedef struct table
{
	int	(*add)();
	int	(*prepend)();
	int	(*append)();
	int	(*delet)();
	char	*(*find)();
	void	(*first)();
	char	*(*next)();
	void	(*private_destroy)();
	char	*privat;
	int	(*count)();
				/* user-supplied functions */
	int	(*compar)();
	void	(*release)();
	void	(*destroy)();
} TABLE;

typedef   void (*TABLE_DESTROY)(); 

#endif

#ifdef __STDC__

extern int	default_compar (char *k1, char *k2);
extern void	default_release (char *datum);
extern void	default_destroy (TABLE *table);
extern int	table_add (char *datum, TABLE *table);
extern int	table_prepend (char *datum, TABLE *table);
extern int	table_append (char *datum, TABLE *table);
extern int	table_delete (char *datum, TABLE *table);
extern char	*table_unlink (char *datum, TABLE *table);
extern char	*table_find (char *datum, TABLE *table);
extern void	table_first (TABLE *table);
extern char	*table_next (TABLE *table);
extern int	table_count (TABLE *table);
extern void	table_destroy (TABLE *table);
extern TABLE	*table_list_create (TABLE_COMPARE,
				    TABLE_RELEASE,
				    TABLE_DESTROY);
extern TABLE	*table_tree_create (int(*)(char *, char *), 
				    void (*)(char *),
				    void (*)(TABLE *));
extern TABLE	*table_hash_create (TABLE_COMPARE,
				    TABLE_RELEASE,
				    TABLE_DESTROY, 
				    TABLE_HASH,
				    int count);
extern void	table_hash_stats (TABLE *table);


/*  variables.c prototypes */ 

extern void     var_init  (void);
extern void     var_set   (char *name, char *value);
extern char    *var_get   (char *name);
extern int      var_array (char ***, char ***);
extern void     var_print (int dummy);

#else /* !__STDC__ */

extern int	default_compar ();
extern void	default_release ();
extern void	default_destroy ();
extern int	table_add ();
extern int	table_append ();
extern int	table_prepend ();
extern int	table_delete ();
extern char	*table_unlink ();
extern char	*table_find ();
extern void	table_first ();
extern char	*table_next ();
extern char	*table_next ();
extern int	table_count ();
extern void	table_destroy ();
extern TABLE	*table_list_create ();
extern TABLE	*table_tree_create ();
extern TABLE	*table_hash_create ();
extern void	table_hash_stats ();

extern void     var_init  ();
extern void     var_set   ();
extern char    *var_get   ();
extern int      var_array ();
extern void     var_print ();

#endif /* __STDC__ */
#endif /* TABLE_H */
