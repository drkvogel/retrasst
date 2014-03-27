SHELL=/bin/sh
INC=$(II_SYSTEM)/ingres/files
CXX=g++
LDLIB=-linterp.1 -liiapi
CXXFLAGS=-Dint_lnx -DRANDTEST -g -Wall -Wextra -I$(INC)
OBJS=rosebase.o rosetta.o xaes.o xbinob.o xcgi.o xdb.o xencode.o xerror.o xexec.o xmd5.o xmz.o xquery.o xrand.o xsql.o xtime.o libaes.a
AUX=dicom.o model.o page0.o page1.o randutil.o enckey.o session.o view.o

status_biocore_test.cgi : main.o $(OBJS) $(AUX)
        $(CXX) $(CXXFLAGS) -o status_biocore_test.cgi main.cpp $(OBJS) $(AUX) -L$(II_SYSTEM)/ingres/lib -lingres $(LDLIB)

clean :
        rm *.o

all : status_biocore_test.cgi

