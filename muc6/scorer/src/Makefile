#########################################################################
#									#
#  Copyright 1996 Science Applications International Corporation.	#
#									#
#########################################################################

SHELL		= /bin/sh
BINDIR          = ../bin

# C header files

HEADERS         = a_list.h cdefs.h hash.h muc.h muc_defs.h \
                  muc_globals.h muc_protos.h sysdefs.h table.h

# C source files

CSRCS		= a_list.c alt_fill_set.c catalog_entry.c \
                  class_def.c class_inst.c class_instlst.c class_score.c \
                  connect.c file_section.c formulae.c \
                  hash.c in_and_out.c init_muc.c \
                  inst_pair.c inst_pairs.c list.c muc.c \
                  map.c map_history.c muc_globals.c muc_strings.c \
                  option.c parsers.c pointer.c \
                  rm_opt_insts.c root.c \
                  score.c score_report.c \
                  slot_def.c slot_fill.c slot_score.c string_fill.c \
                  table.c tallies.c tree.c \
                  variables.c 

#  yacc source files

YSRCS		= config.y slotconfig.y mapconfig.y text.y \
		  template.y 

#  lex source files

LSRCS		= config.l slotconfig.l mapconfig.l text.l \
		  template.l markup.l

SRCS		= $(CSRCS) $(YSRCS) $(LSRCS)
OBJS		= $(CSRCS:.c=.o) \
		  y.config.o l.config.o \
		  y.slotconfig.o l.slotconfig.o \
		  y.mapconfig.o l.mapconfig.o \
		  y.text.o l.text.o \
		  y.template.o l.template.o \
		  l.markup.o

DEBUG		= -g
CFLAGS		= 

CC		= cc
LD		= $(CC)

# Yacc or bison tool and flags

YACC		= yacc
YFLAGS		= -d


# Lex or flex tool and flags

LEX:sh		= VER=`uname -r | sed -e "1,1s/^\(.\).*/\1/"`;  \
		    if [ $VER -gt 4 ]; then echo "lex" ; else \
		    echo "flex"; fi
LEXFLAGS	=


# C preprocessor flags 

OS:sh		= VER=`uname -r | sed -e "1,1s/^\(.\).*/\1/"`;  \
		    if [ $VER -gt 4 ]; then echo "__svr4__" ; else \
		    echo "__bsd__"; fi

CPPFLAGS	= -D__bsd__ 
#$(OS)


# suffix rule(s)

.SUFFIXES:	.o .c .c~ .y .y~ .l .l~ .h .h~

.c.o:
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $<



# The main file depends on all object files

install:	$(OBJS)
		$(LD) $(DEBUG) -o muc $(OBJS)
		mv muc $(BINDIR)/MUC_scorer


# Rules for .y and .l files to avoid name conflicts 
 
y.config.o:	config.y
		$(YACC) $(YFLAGS) config.y
		sed -e 's/yy/config/g' y.tab.h > y.config.h
		sed -e 's/yy/config/g' y.tab.c > y.config.c
		$(CC) $(CFLAGS) -c y.config.c
		rm y.tab.c

l.config.o:	config.l y.config.o
		$(LEX) $(LEXFLAGS) config.l
		sed -e 's/yy/config/g' lex.yy.c > l.config.c
		$(CC) $(CFLAGS) -c l.config.c
		rm lex.yy.c 

y.slotconfig.o:	slotconfig.y
		$(YACC) $(YFLAGS) slotconfig.y
		sed -e 's/yy/slotconfig/g' y.tab.h > y.slotconfig.h
		sed -e 's/yy/slotconfig/g' y.tab.c > y.slotconfig.c
		$(CC) $(CFLAGS) -c y.slotconfig.c
		rm y.tab.c

l.slotconfig.o:	slotconfig.l y.slotconfig.o
		$(LEX) $(LEXFLAGS) slotconfig.l
		sed -e 's/yy/slotconfig/g' lex.yy.c > l.slotconfig.c
		$(CC) $(CFLAGS) -c l.slotconfig.c
		rm lex.yy.c

y.mapconfig.o:	mapconfig.y
		$(YACC) $(YFLAGS) mapconfig.y
		sed -e 's/yy/mapconfig/g' y.tab.h > y.mapconfig.h
		sed -e 's/yy/mapconfig/g' y.tab.c > y.mapconfig.c
		$(CC) $(CFLAGS) -c y.mapconfig.c
		rm y.tab.c

l.mapconfig.o:	mapconfig.l y.mapconfig.o
		$(LEX) $(LEXFLAGS) mapconfig.l
		sed -e 's/yy/mapconfig/g' lex.yy.c > l.mapconfig.c
		$(CC) $(CFLAGS) -c l.mapconfig.c
		rm lex.yy.c

y.text.o:	text.y
		$(YACC) $(YFLAGS) text.y
		sed -e 's/yy/texts/g' y.tab.h > y.text.h
		sed -e 's/yy/texts/g' y.tab.c > y.text.c
		$(CC) $(CFLAGS) -c y.text.c
		rm y.tab.c

l.text.o:	text.l y.text.o
		$(LEX) $(LEXFLAGS) text.l
		sed -e 's/yy/texts/g' lex.yy.c > l.text.c
		$(CC) $(CFLAGS) -c l.text.c
		rm lex.yy.c

y.template.o:	template.y
		$(YACC) $(YFLAGS) template.y
		sed -e 's/yy/templates/g' y.tab.h > y.template.h
		sed -e 's/yy/templates/g' y.tab.c > y.template.c
		$(CC) $(CFLAGS) -c y.template.c
		rm y.tab.c

l.template.o:	template.l y.template.o
		$(LEX) $(LEXFLAGS) template.l
		sed -e 's/yy/templates/g' lex.yy.c > l.template.c
		$(CC) $(CFLAGS) -c l.template.c
		rm lex.yy.c

l.markup.o:	markup.l
		$(LEX) $(LEXFLAGS) markup.l
		sed -e 's/yy/markup_/g' lex.yy.c > l.markup.c
		$(CC) $(CFLAGS) -c l.markup.c
		rm lex.yy.c

#y.ne.o:		ne.y
#		$(YACC) $(YFLAGS) ne.y
#		sed -e 's/yy/named_entity_/g' y.tab.h > y.ne.h
#		sed -e 's/yy/named_entity_/g' y.tab.c > y.ne.c
#		$(CC) $(CFLAGS) -c y.ne.c
#		rm y.tab.c
#
#l.ne.o:		ne.l y.ne.o
#		$(LEX) $(LEXFLAGS) -v ne.l
#		sed -e 's/yy/named_entity_/g' lex.yy.c > l.ne.c
#		$(CC) $(CFLAGS) -c l.ne.c
#		rm lex.yy.c

#  #include dependencies 

alt_fill_set.o:		muc.h	muc_protos.h	muc_globals.h
catalog_entry.o:	muc.h	muc_protos.h	muc_globals.h
class_def.o:		muc.h	muc_protos.h	muc_globals.h
class_inst.o:		muc.h	muc_protos.h	muc_globals.h
class_instlst.o:	muc.h	muc_protos.h	muc_globals.h
class_score.o:		muc.h	muc_protos.h	muc_globals.h
file_section.o:		muc.h	muc_protos.h	muc_globals.h
init_muc.o:		muc.h	muc_protos.h	muc_globals.h
inst_pairs.o:		muc.h	muc_protos.h	muc_globals.h
muc.o:			muc.h	muc_protos.h	muc_globals.h
mapconfig.y:		muc.h	muc_protos.h	muc_globals.h
muc_strings.o:		muc.h	muc_protos.h	muc_globals.h
pointer.o:		muc.h	muc_protos.h	muc_globals.h
rm_opt_insts.o:		muc.h	muc_protos.h	muc_globals.h
root.o:			muc.h	muc_protos.h	muc_globals.h
slot_def.o:		muc.h	muc_protos.h	muc_globals.h
slot_fill.o:		muc.h	muc_protos.h	muc_globals.h
string_fill.o:		muc.h	muc_protos.h	muc_globals.h

connect.o:		table.h	muc.h	muc_protos.h	muc_globals.h
map.o:			table.h	muc.h	muc_protos.h	muc_globals.h
map_history.o:		table.h	muc.h	muc_protos.h	muc_globals.h
in_and_out.o:		table.h	muc.h	muc_protos.h	muc_globals.h
score.o:		table.h	muc.h	muc_protos.h	muc_globals.h
score_report.o:		table.h	muc.h	muc_protos.h	muc_globals.h

formulae.o:		muc.h	muc_protos.h
inst_pair.o:		muc.h	muc_protos.h
slot_score.o:		muc.h	muc_protos.h
tallies.o:		muc.h	muc_protos.h

option.o:		muc.h	muc_globals.h

parsers.o:		muc_protos.h muc_globals.h

y.config.o:		muc_protos.h
y.slotconfig.o:		muc_protos.h
y.template.o:		muc_protos.h
y.text.o:		muc_protos.h
y.template.o:		muc_globals.h

# each lex file FILE.l also includes y.FILE.h, but that
# dependency is taken care of in the name conflict
# dodging above (markup.l has no yacc parser, so it
# doesn't call y.markup.h anywhere).

l.config.o:		cdefs.h	
l.markup.o:		cdefs.h	muc_defs.h muc_protos.h	muc_globals.h
l.mapconfig.o:		cdefs.h	muc_defs.h
l.slotconfig.o:		cdefs.h	
l.template.o:		cdefs.h	
l.text.o:		cdefs.h	

muc_globals.o:		muc.h	
muc_protos.h:		muc.h

muc.h:			cdefs.h	muc_defs.h	table.h
muc_defs.h:		cdefs.h
sysdefs.h:		cdefs.h

a_list.o:		sysdefs.h	a_list.h
hash.o:			sysdefs.h	table.h	hash.h	muc.h
list.o:			table.h		a_list.h
table.o:		sysdefs.h	table.h	muc.h
tree.o:			table.h
variables.o:		sysdefs.h	table.h	muc.h

# utilities

clean:
		-$(RM) $(OBJS) core
		-$(RM) *~
