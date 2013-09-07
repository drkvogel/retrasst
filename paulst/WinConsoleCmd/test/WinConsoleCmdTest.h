#ifndef WINCONSOLECMDTESTH
#define WINCONSOLECMDTESTH

#include <tut.h>

#include <algorithm>
#include <boost/algorithm/string/erase.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "AcquireCriticalSection.h"
#include "CritSec.h"
#include "CSVIterator.h"
#include "FixedCapacityBuffer.h"
#include "StrUtil.h"
#include "Util.h"
#include "WinConsoleCmd.h"

std::auto_ptr<paulst::CritSec> logCritSec;
std::auto_ptr<std::ostringstream> logOutput;
std::auto_ptr<std::ostringstream> consoleOutput;

bool LOG_TO_STDOUT = false;

struct Contains
{
    std::string target;

    Contains( const std::string& t )
        : target(t)
    {
    }

    bool operator()( const std::string& s )
    {
        return std::string::npos != s.find( target );
    }
};

Contains contains( const std::string& s )
{
    return Contains(s);
}

void log( const char* msg )
{
    paulst::AcquireCriticalSection a(*logCritSec);
    (*logOutput) << msg << "\n";
    if ( LOG_TO_STDOUT )
    {
        std::cout << msg << "\n";
    }
}

bool processOutputFromCmd( const char* output, char* nextCmd, char* userData )
{
    (*consoleOutput) << output;

    std::string out(output);
    
    if ( out.size() && (*out.rbegin()) == '>' && *userData )
    {
        removeLastCommand( nextCmd, userData );
        return true;
    }
    else
    {
        return false;
    }     
}

paulst::FixedCapacityBuffer buffer(1024);

bool isIngresWaitingForInput( const char* output )
{
    const char* begin = output;
    const char* end   = output + std::strlen( output );

    std::string lastLine = paulst::last_line( begin, end );
    char        lastChar = paulst::lastNonWhiteSpaceChar( begin, end );

    return ( ( lastLine == "continue" ) || ( lastLine =="* go" ) ) && ( lastChar == '*' );
}

bool processOutputFromSql( const char* output, char* nextCmd, char* userData )
{
    (*consoleOutput) << output;

    boost::erase_all_copy( std::back_inserter( buffer ), output, "\r" );

    std::string bufferCopy( buffer.copy() );

    if ( isIngresWaitingForInput( bufferCopy.c_str() ) )
    {
        removeLastCommand( nextCmd, userData );
        return true;
    }
    else
    {
        return false;
    }
}

bool isPSCPWaitingForInput( const char* pscpOutput )
{
    return paulst::endsWith( pscpOutput, "password: " ) || paulst::endsWith( pscpOutput, "Store key in cache? (y/n) " );
}

bool processOutputFromPscp( const char* output, char* nextCmd, char* userData )
{
    (*consoleOutput) << output;

    boost::erase_all_copy( std::back_inserter( buffer ), output, "\r" );

    std::string bufferCopy( buffer.copy() );

    if ( isPSCPWaitingForInput( bufferCopy.c_str() ) )
    {
        removeLastCommand( nextCmd, userData );
        return true;
    }
    else
    {
        return false;
    }
}

namespace tut
{
	class WinConsoleCmdTestFixture
    {
    };

    typedef test_group<WinConsoleCmdTestFixture, 6> WinConsoleCmdTestGroup;
	WinConsoleCmdTestGroup testGroupWinConsoleCmd( "WinConsoleCmd tests");
	typedef WinConsoleCmdTestGroup::object testWinConsoleCmd;

    void ensureThreadsLogTermination( const std::string& log )
    {
        std::vector< std::string > lines;
        std::copy( paulst::CSVIterator<'\n'>( log ), paulst::CSVIterator<'\n'>(), std::back_inserter( lines ) );

        ensure_equals( std::count_if( lines.begin(), lines.end(), contains( "terminating" ) ), 2 );
    }

    bool contains( const std::string& s, const char* targetStr )
    {
        return std::string::npos != s.find( targetStr );
    }

    template<>
    template<>
	void testWinConsoleCmd::test<1>()
    {
        set_test_name("removeLastCommand");

        char stack[1024];
        std::memset( stack, 0, 1024 );

        std::string cmds( "one\ntwenty\nfour\n" );

        std::copy( cmds.begin(), cmds.end(), stack );

        char cmd[1024];

        std::string expected[] = { "four\n", "twenty\n", "one\n" };

        for ( int i  = 0; i < 3; ++i )
        {
            removeLastCommand( cmd, stack );

            ensure( std::string( cmd ) == expected[i] );
        }

        ensure( stack[0] == '\0' );

        ensure( cmd[0] != '\0' );

        removeLastCommand( cmd, stack );

        ensure( "blah", cmd[0] == '\0' );
    }

    template<>
    template<>
	void testWinConsoleCmd::test<2>()
    {
        set_test_name("runWinConsoleCmd");
        
        logCritSec.reset( new paulst::CritSec() );
        logOutput.reset(     new std::ostringstream() );
        consoleOutput.reset( new std::ostringstream() );

        WinConsoleCmdArgs args;
        args.launchString = "cmd";
        args.dialogLogic = processOutputFromCmd;
        args.log = log;
        args.userData = "exit\ndir\n";
        runWinConsoleCmd( &args );
        ensure( args.success() );
        ensure_equals( '\0',  args.exceptionMsg[0] );
        ensure_equals( false, args.timedOut );
        ensure_equals( 0,     args.exitCode );

        ensureThreadsLogTermination( logOutput->str() );
        std::string consoleOut = consoleOutput->str();
        ensure( std::string::npos != consoleOut.find( "main.cpp" ) );
    }

    template<>
    template<>
	void testWinConsoleCmd::test<3>()
    {
        set_test_name("runWinConsoleCmd - running Ingres sql terminal monitor");
        
        logCritSec.reset( new paulst::CritSec() );
        logOutput.reset(     new std::ostringstream() );
        consoleOutput.reset( new std::ostringstream() );

        WinConsoleCmdArgs args;
        args.launchString = "sql brat_cp::paulst_test";
        args.dialogLogic = processOutputFromSql;
        args.log = log;
        args.userData = "\\q\nhelp sql\\g\n";
        runWinConsoleCmd( &args );
        ensure( args.success() );
        ensure_equals( '\0',  args.exceptionMsg[0] );
        ensure_equals( false, args.timedOut );
        ensure_equals( 0,     args.exitCode );
        
        ensureThreadsLogTermination( logOutput->str() );
        ensure( contains( consoleOutput->str(), "HELP is available on the following SQL topics" ) );
    }

    template<>
    template<>
	void testWinConsoleCmd::test<4>()
    {
        set_test_name("timeout");
        
        logCritSec.reset( new paulst::CritSec() );
        logOutput.reset(     new std::ostringstream() );
        consoleOutput.reset( new std::ostringstream() );

        WinConsoleCmdArgs args;
        args.launchString = "cmd";
        args.dialogLogic = processOutputFromCmd;
        args.log = log;
        args.userData = "dir\n";
        args.timeoutMillis = 1000;
        runWinConsoleCmd( &args );
        ensure_not( args.success() );
        ensure_equals( '\0', args.exceptionMsg[0] );
        ensure_equals( true, args.timedOut );
        ensure_equals( 259,  args.exitCode );

        ensureThreadsLogTermination( logOutput->str() );
        ensure( contains( logOutput->str(), "Timeout" ) );
        ensure( contains( consoleOutput->str(), "main.cpp" ) );
    }

    template<>
    template<>
	void testWinConsoleCmd::test<5>()
    {
        set_test_name("pscp");
        
        logCritSec   .reset( new paulst::CritSec()    );
        logOutput    .reset( new std::ostringstream() );
        consoleOutput.reset( new std::ostringstream() );

        WinConsoleCmdArgs args;
        args.launchString = "pscp cp@brat:paulst/part.txt ./";
        args.dialogLogic = processOutputFromPscp;
        args.log = log;
        args.userData = "Fl0gg3m!\r\n";
        args.timeoutMillis = 5000;
        runWinConsoleCmd( &args );
        ensure( args.success() );
        ensure_equals( '\0',  args.exceptionMsg[0] );
        ensure_equals( false, args.timedOut );
        ensure_equals( 0,     args.exitCode );

        ensure( contains( consoleOutput->str(), "100%" ) );
        ensureThreadsLogTermination( logOutput->str() );
        ensure_not( contains( logOutput->str(), "Timeout" ) );
    }

    template<>
    template<>
	void testWinConsoleCmd::test<6>()
    {
        set_test_name("pscp fail");
        
        logCritSec   .reset( new paulst::CritSec()    );
        logOutput    .reset( new std::ostringstream() );
        consoleOutput.reset( new std::ostringstream() );

        WinConsoleCmdArgs args;
        args.launchString = "pscp cp@brat:paulst/par.txt ./";
        args.dialogLogic = processOutputFromPscp;
        args.log = log;
        args.userData = "Fl0gg3m!\r\n";
        args.timeoutMillis = 5000;
        runWinConsoleCmd( &args );
        ensure_not( args.success() );
        ensure_equals( '\0',  args.exceptionMsg[0] );
        ensure_equals( false, args.timedOut );
        ensure       ( 0 != args.exitCode );

        ensureThreadsLogTermination( logOutput->str() );
        ensure_not( contains( logOutput->str(), "Timeout" ) );
    }

};
 
#endif

