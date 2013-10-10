//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <boost/scoped_ptr.hpp>
#include "LoggingService.h"
#include "MainFrm.h"
#include "MockConnection.h"
#include "MockConnectionFactory.h"
#include "MockConfig.h"
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TMainForm *MainForm;

class ExceptionHandlingPolicy : public valc::DBUpdateExceptionHandlingPolicy
{
public:

	void handleException( const std::string& msg )
	{
		ShowMessage( msg.c_str() );
	}
};

class TMemoLogger : public paulst::Writer
{
public:
	TMemoLogger( TMemo* memo ) : m_memo( memo ) {}

	void write( const std::string& msg )
	{
		m_memo->Lines->Add( msg.c_str() );
	}
private:
	TMemo* m_memo;
};

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner), m_warningsListener( this )
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::onShow(TObject *Sender)
{
	try
	{
		using namespace valc;

		static const int LOCAL_MACHINE_ID = -1019349;
		static const int USER_ID          = 1234;
		MockConnectionFactory::reset();

		MockConnectionFactory::clusters = "-1019430,\n";
		MockConnectionFactory::projects = "-832455,reveal,ldb25,\n";
		MockConnectionFactory::worklist =
 //rec  machine  barcode   test     group      c sample project p prof                  timestamp           seq s dil  result
 "-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
 "-36846,-1019430,118507091,-1031391,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,0,\n"
			 ;

		std::string tests[2] = {
 //bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,res ,a,date analysed      ,restx,update when      ,cbw
 "882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.8 ,0,27-06-2013 11:57:47,1.8,27-06-2013 10:57:49,-36845,",
 "882291,118507091,27-06-2013 11:55:36,REVEAL,432560,-1019349,882432,-1031391,1.3 ,0,27-06-2013 11:57:47,1.3,27-06-2013 10:57:49,-36846,"
			 };

		//                                runID, isOpen, when created       , when closed, sequence position
		std::string sampleRunData[2] = { "   12,      1,27-06-2013 11:42:36,,882290,y,",
								  ",,,,,," };

		MockConnectionFactory::buddyDB =
			tests[0] + sampleRunData[0] + "\n" +
			tests[1] + sampleRunData[1] + "\n"
		;

		boost::scoped_ptr<paulst::LoggingService> log(
			new paulst::LoggingService( new TMemoLogger(output) ) );

		MockConfig config;

        InitialiseApplicationContext( LOCAL_MACHINE_ID, USER_ID, config.toString(), log.get() );
        SnapshotPtr s = Load( &m_warningsListener );

		const bool blockTillNoPendingUpdates = true;
		ExceptionHandlingPolicy exceptionListener;

		s->runPendingDatabaseUpdates( &exceptionListener, blockTillNoPendingUpdates );
	}
	catch( const Exception& e )
	{
		ShowMessage( e.Message );
	}

    valc::DeleteApplicationContext();
}
//---------------------------------------------------------------------------
void TMainForm::addWarning( const std::string& warning )
{
	warnings->Lines->Add( warning.c_str() );
}

