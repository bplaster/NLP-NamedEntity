
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 
{
    int   ival;
    char  *sval;
} YYSTYPE;
extern YYSTYPE configlval;
# define NUMBER 257
# define STRING 258
# define OPTION 259
# define VALUE 260
# define QSTRING 261
