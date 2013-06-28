/*---------------------------------------------------------------------------
 *
 *      Shared code for QC_Material, QC_Level and QC_test_machine
 *
 *      22 March 2010, NG:	Added dilution code to be copied into worklist
 *
 ---------------------------------------------------------------------------*/

#include <vcl.h>
#include "LCDbQCLevel.h"
#include "LCDbAnalyser.h"
#include "LCDbTest.h"
#include "LQuery.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Constructor: read fields from combined tables; generate description
//---------------------------------------------------------------------------

LCDbQCLevel::LCDbQCLevel( const LQuery & qCentral )
 : LCDbID( qCentral.readInt( "record_cid" ) ),
   material( qCentral.readString( "Material" ) ),
   level( qCentral.readString( "QC_Level" ) ),
   status( qCentral.readShort( "status" ) ),
   description( qCentral.readString( "Material_Descrip" )
		+ " " + qCentral.readString( "Level_Descrip" ) )
{}

//---------------------------------------------------------------------------

LCDbQCLevel::Test::Test( const LQuery & qCentral )
 : LCDbID( qCentral.readInt( "record_cid" ) ),
   machineID( qCentral.readInt( "machine_cid" ) ),
   testID( qCentral.readInt( "test_cid" ) ),
   diluent( qCentral.readDouble( "diluent" ) ),
   LDbValid( qCentral.readDateTime( "valid_from" ),
			 qCentral.readDateTime( "valid_to" ),
			 qCentral.readShort( "status" ) )
{}

//---------------------------------------------------------------------------
//	Read (currently configured) QCs tests from central database
//---------------------------------------------------------------------------

bool LCDbQCLevels::read( LQuery qCentral, bool readAll )
{
	if( readAll )
	{
		qCentral.setSQL( "select l.*, Material_Descrip"
						" from QC_Material m, QC_Level l where m.Material = l.Material"
						" order by record_cid" );
	}
	else
	{	qCentral.setSQL( "select l.*, Material_Descrip"
						" from QC_Material m, QC_Level l where m.Material = l.Material"
						" and status <> :old"
						" order by l.record_cid" );
		qCentral.setParam( 0, LDbValid::DELETED );
	}

	if( !readData( qCentral ) )
		return false;

	if( readAll )
	{
		qCentral.setSQL( "select * from qc_test_machine"
					  " order by level_cid, record_cid" );
	}
	else
	{	qCentral.setSQL( "select * from qc_test_machine"
					  " where status <> :sts and 'now' between valid_from and valid_to"
					  " order by level_cid, record_cid" );
		qCentral.setParam( 0, LDbValid::DELETED );
	}

	iterator ci = begin();
	for( qCentral.open(); !qCentral.eof(); qCentral.next() )
	{
		int did = qCentral.readInt( "level_cid" );
		while( ci != end() && ci -> getID() < did )
			ci ++;

		if( ci != end() && ci -> getID() == did )
			ci -> addTest( qCentral );
	}
	return true;
}

//---------------------------------------------------------------------------
//	Find QC level record from the material and level (e.g. RL 1)
//---------------------------------------------------------------------------

class /* LCDbQCLevels:: */ Matcher : public std::unary_function< LCDbQCLevel, bool >
{
	String qc, level;

public:

	Matcher( const String & material, const String & qcLevel )
	 : qc( material ), level( qcLevel )
	{}

	operator String() const { return "QC" + qc + level; }

	bool operator() ( const LCDbQCLevel & other ) const
	{
		return level == other.getLevel() && qc == other.getMaterial();
	}
};

//---------------------------------------------------------------------------

const LCDbQCLevel * LCDbQCLevels::find( const String & material, const String & qcLevel ) const
{
	return findMatch( Matcher( material, qcLevel ) );
}

//---------------------------------------------------------------------------

