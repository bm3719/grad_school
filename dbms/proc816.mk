include $(ORACLE_HOME)/ord/ts/lib/env_ordts.mk

# This Makefile builds an executable, the target below (small) 
# from a C program containing embedded SQL statements. It is
# entitled small.pc.
#
# The target 'small' causes the creation of an executable $(EXE) 
# from the .o files in the $(OBJS) and the libraries in $(PROLDLIBS).
# These, in turn, are used to build the c program once the PRO*C/C++
# pre-compiler creates a c program from small.pc. 
# The rules to make .o files from .c and .pc files are given below:
# ($(PROLDLIBS) includes the client shared library, and $(STATICPROLDLIBS) does
# not.)
#

#***********************************************************************
# This makefile, proc816.mk, is executed by issuing 
#   make -f proc816.mk OBJS=filename.o EXE=filename INAME=filename.pc filename
# The last 'filename' serves as the target.

#****** To compile, change 'filename' to the name of the file you are
#       compiling in the following line *******

small.pc: $(OBJS)
	$(CC) -w -o $(EXE) $(OBJS) -L$(LIBHOME) $(PROLDLIBS)
clean: 	
	$(RM) -f *.c *.o *.lis
#***********************************************************************

#
# Here are some rules for converting .pc -> .c -> .o and for .typ -> .h.
#
# If proc needs to find .h files, it should find the same .h files that the 
# c compiler finds.  We use a macro named INCLUDE to hadle that.  The general 
# format of the INCLUDE macro is 
# INCLUDE= $(I_SYM)dir1 $(I_SYM)dir2 ...
#
# Normally, I_SYM=-I, for the c compiler.  However, we have a special target,
# pc1, which calls $(PROC) with various arguments, include $(INCLUDE).  It
# is used like this:
#	$(MAKE) -f $(MAKEFILE) <more args to make> I_SYM=include= pc1
# This is used for some of $(SAMPLES) and for $(OBJECT_SAMPLE).
.SUFFIXES: .pc .c .o .typ .h

pc1:
	$(PROC) $(PROCFLAGS) iname=$(PCCSRC) $(INCLUDE)

.pc.c:
	$(PROC) $(PROCFLAGS) $(PROCINCLUDE) iname=$*

.pc.o:
	$(PROC) $(PROCFLAGS) $(PROCINCLUDE) iname=$*
	$(CC) -w $(CFLAGS) -c $*.c

.c.o:
	$(CC) -w $(CFLAGS) -c $*.c

.typ.h:
	$(OTT) intype=$*.typ hfile=$*.h outtype=$*o.typ $(OTTFLAGS) code=c user=ordsys/oracle

#
# The macro definition fill in some details or override some defaults from 
# other files.
#
PROC=proc
OTT=ott
OTTFLAGS=$(PCCFLAGS)
CLIBS= $(TTLIBS_QA) $(LDLIBS)
PRODUCT_LIBHOME=
PROCPLSFLAGS= sqlcheck=full userid=$(USERID) 
PROCPPFLAGS= code=cpp $(CCPSYSINCLUDE)
NETWORKHOME=$(ORACLE_HOME)/network/
PLSQLHOME=$(ORACLE_HOME)/plsql/
INCLUDE=$(I_SYM). $(I_SYM)$(PRECOMPHOME)public $(I_SYM)$(RDBMSHOME)public $(I_SYM)$(RDBMSHOME)demo $(I_SYM)$(PLSQLHOME)public $(I_SYM)$(NETWORKHOME)public
I_SYM=-I
STATICPROLDLIBS=$(SCOREPT) $(SSCOREED) $(DEF_ON) $(LLIBCLIENT) $(LLIBSQL) $(TTLIBS)
PROLDLIBS=$(LLIBCLNTSH) $(STATICPROLDLIBS)
STATICCPPLDLIBS=$(SCOREPT) $(SSCOREED) $(DEF_ON) $(LLIBCLIENT) $(LLIBSQL) $(DEVTTLIBS)
CPPLDLIBS=$(LLIBCLNTSH) $(STATICCPPLDLIBS)
PROCINCLUDE=include=$(ORACLE_HOME)/rdbms/demo include=$(ORACLE_HOME)/rdbms/public include=$(ORACLE_HOME)/network/public include=$(ORACLE_HOME)/plsql/public


