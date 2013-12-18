#pragma hdrstop
#pragma argsused

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "xdb.h"
#include "xquery.h"
#include "xexec.h"
#include "xaes.h"
#include "xtime.h"
#include "enckey.h"

//---------------------------------------------------------------------------
int     lines_done = 0;
int     error_count = 0;
XDB     *db = NULL;

//---------------------------------------------------------------------------

std::string decrypt( std::string encrypted )
{
	std::string enckey = enckey::getDBEncKey();
	XAES myAES = XAES(enckey);
	if (!myAES.isValid())
	{ //some problem with the encrpytion
		std::string error = myAES.getLastError();
		printf((std::string("Error with AES:") + error).c_str());
		return "";
	}
	int length = 0;
	unsigned char *clr;
	if (!myAES.decryptHex(&clr, &length,encrypted.c_str()))
	{
		std::string error = myAES.getLastError();
		printf((std::string("Error with AES:") + error).c_str());
		return "";
	}
	std::string s((const char*)clr,length);
	return s;
}

//---------------------------------------------------------------------------
								/* READ AND ENCRYPT TABLE LINE-BY-LINE */
bool decryptTable( void )
{
	XDATE testDOB;
	XQUERY  q( db, "SELECT person_id,name_enc,nhsnumber_enc,carersname_enc"
			" FROM person WHERE status_enc = 1");

	if ( ! q.isValid()  )
	{
		printf( "\nInternal error creating database access objects" );
		error_count++;
		return( false );
	}
	bool found_data = false;
	int row_ident;

	std::string name_clear, nhsnumber_clear,carersname_clear;
	if ( ! q.open() )
	{
		printf( "\nFailed to open query on person" );
		error_count++;
		return( false );
	}

	ROSETTA R;
	while(q.fetch() )
	{
		row_ident = q.result.getInt( 0 );
		name_clear = q.result.getString( 1 );
		nhsnumber_clear = q.result.getString( 2 );
		carersname_clear = q.result.getString(3);

		if (name_clear.empty() && nhsnumber_clear.empty() && carersname_clear.empty())
			continue;

		if (!name_clear.empty())
			name_clear = decrypt(name_clear);
		if (!nhsnumber_clear.empty())
			nhsnumber_clear = decrypt(nhsnumber_clear);
		if (!carersname_clear.empty())
			carersname_clear = decrypt(carersname_clear);

		ROSETTA Rdetails;
		Rdetails.setString("name",name_clear);
		Rdetails.setString("nhsnumber",nhsnumber_clear);
		Rdetails.setString("carersname",carersname_clear);
		Rdetails.setInt("status_enc",0);
		ROSETTA Rwhere;
		Rwhere.setInt("person_id",row_ident);
		ROSETTA Rdata;
		Rdata.setRosetta("Rdetails",&Rdetails);
		Rdata.setRosetta("Rwhere",&Rwhere);

		std::stringstream ss;
		ss << row_ident;
		R.setRosetta(ss.str(),&Rdata);
	}
	q.close();

	XEXEC exe(db);
	for (int i=0;i<R.count();i++)
	{
		ROSETTA Rdata = R.getRosetta(i);
		ROSETTA Rwhere = Rdata.getRosetta("Rwhere");
		ROSETTA Rdetails = Rdata.getRosetta("Rdetails");

		if (!exe.update("person",&Rdetails,&Rwhere))
		{
			printf( "\nFailed to insert on person" );
			error_count++;
		}
		else
			lines_done++;
	}
	return( true );
}
//---------------------------------------------------------------------------
		/* XDB ROUTINES CALL THIS FUNCTION IF THE ENCOUNTER AN ERROR */
bool dbErrCB( const std::string object, const int instance,
		const int ecount, const int ecode, const std::string error_txt )
{
	printf( "\nDatabase Error callback %s\n%s", object.c_str(),error_txt.c_str() );
	error_count++;
	return( true );
}
//---------------------------------------------------------------------------
int main(int argc, char **argv )
{
#ifdef RANDLIVE
	std::string	name = std::string("erg_attila_live");
#else
	std::string	name = std::string("erg_attila_test");
#endif

	#ifndef __BORLANDC__
	db = new XDB( name );
	#else
	db = new XDB( std::string("dice_erg::") + name );
	#endif
	if ( NULL == db )
	{
		printf( "\nFailed to create database object" );
		exit( EXIT_FAILURE );
	}
	db->setErrorCallBack( dbErrCB );
	if ( ! db->isValid() || ! db->open() )
	{
		printf( "\nUnable to open database");
		exit( EXIT_FAILURE );
	}
	bool ok = decryptTable();
	db->close();
	delete db;
	printf( "\nDone %d lines, with %d errors", lines_done, error_count );
	return( ok ? EXIT_SUCCESS : EXIT_FAILURE );
}
//---------------------------------------------------------------------------

