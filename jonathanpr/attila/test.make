SHELL=/bin/sh
INC=$(II_SYSTEM)/ingres/files
CXX=g++
LDLIB=-linterp.1 -liiapi
CXXFLAGS=-Dint_lnx -DRANDTEST -g -Wall -Wextra -I$(INC)
OBJS=rosebase.o rosetta.o xaes.o xbinob.o xcgi.o xdb.o xencode.o xerror.o xexec.o xmd5.o xmz.o xquery.o xrand.o xsql.o xtime.o libaes.a
AUX=centre.o checklist.o clinician.o enckey.o model.o page0.o page1.o page2.o page3.o page4.o page5.o page6.o pageIneligible.o patient.o randutil.o session.o view.o

attila_rand_test.cgi : attila.o $(OBJS) $(AUX)
        $(CXX) $(CXXFLAGS) -o attila_rand_test.cgi attila.cpp $(OBJS) $(AUX) -L$(II_SYSTEM)/ingres/lib -lingres $(LDLIB)

clean :
        rm *.o

all : attila_rand_test.cgi
