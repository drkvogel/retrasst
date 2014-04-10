SHELL=/bin/sh
INC=$(II_SYSTEM)/ingres/files
CXX=g++
LDLIB=-linterp.1 -liiapi
CXXFLAGS=-Dint_lnx -DRANDLIVE -g -Wall -Wextra -I$(INC)
OBJS=rosebase.o rosetta.o xaes.o xbinob.o xcgi.o xdb.o xencode.o xerror.o xexec.o xmd5.o xmz.o xquery.o xrand.o xsql.o xtime.o libaes.a
AUX=randutil.o 

pacswebsitecoms.cgi : main.o $(OBJS) $(AUX)
        $(CXX) $(CXXFLAGS) -o pacswebsitecoms.cgi main.cpp $(OBJS) $(AUX) -L$(II_SYSTEM)/ingres/lib -lingres $(LDLIB)

clean :
        rm *.o

all : pacswebsitecoms.cgi

