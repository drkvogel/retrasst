SHELL=/bin/sh
INC=$(II_SYSTEM)/ingres/files
CXX=g++
LDLIB=-linterp.1 -liiapi
CXXFLAGS=-Dint_lnx -DRANDTEST -g -Wall -Wextra -I$(INC)
OBJS=rosebase.o rosetta.o xbinob.o xcgi.o xdb.o xencode.o xerror.o xexec.o xmd5.o xmz.o xquery.o xrand.o xsql.o xtime.o

pacs_test.cgi :main.o $(OBJS)
        $(CXX) $(CXXFLAGS) -o pacs_test.cgi main.cpp $(OBJS) -L$(II_SYSTEM)/ingres/lib -lingres $(LDLIB)

clean :
        rm *.o

all : pacs_test.cgi
