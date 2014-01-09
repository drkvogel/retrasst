#include "LuaInclude.h"
#include "LuaUtil.h"
#include "UncontrolledResult.h"

namespace valc
{

UncontrolledResult::UncontrolledResult()
    :
    testID(0),
    resultID(0),
    machineID(0),
    resultValue(0),
    projectID(0),
    dateAnalysed(1977, 1, 1, 1, 1, 1, 1),
    actionFlag('?')
{
}

UncontrolledResult popUncontrolledResult( lua_State* L, int stackPosition )
{
    UncontrolledResult r;

    require( lua_istable( L, stackPosition ) );

    r.testID        = retrieveTableValue<int>         ( L, stackPosition, "testID"     , lua_isnumber, lua_toInteger );
    r.resultID      = retrieveTableValue<int>         ( L, stackPosition, "resultID"   , lua_isnumber, lua_toInteger );
    r.machineID     = retrieveTableValue<int>         ( L, stackPosition, "machineID"  , lua_isnumber, lua_toInteger );
    r.projectID     = retrieveTableValue<int>         ( L, stackPosition, "projectID"  , lua_isnumber, lua_toInteger );
    r.resultValue   = retrieveTableValue<double>      ( L, stackPosition, "resultValue", lua_isnumber, lua_toNumber  );
    r.resultText    = retrieveTableValue<std::string> ( L, stackPosition, "resultText" , lua_isstring, lua_toString  ); 
    r.barcode       = retrieveTableValue<std::string> ( L, stackPosition, "barcode"    , lua_isstring, lua_toString  ); 
    r.runID         = retrieveTableValue<std::string> ( L, stackPosition, "runID  "    , lua_isstring, lua_toString  ); 

    lua_getfield( L, stackPosition, "actionFlag" );
    require( lua_isstring( L, -1 ) );
    const char* actionFlagBuffer = lua_tostring( L, -1 );
    r.actionFlag = actionFlagBuffer[0];
    lua_pop( L, 1 );

    unsigned short year, month, day, hour, min, sec, msec;

    lua_getfield( L, stackPosition, "dateAnalysed" );

    require( lua_istable( L, -1 ) );

    year  = retrieveTableValue<int>( L, -1, "year"  , lua_isnumber, lua_toInteger );
    month = retrieveTableValue<int>( L, -1, "month" , lua_isnumber, lua_toInteger );
    day   = retrieveTableValue<int>( L, -1, "day"   , lua_isnumber, lua_toInteger );
    hour  = retrieveTableValue<int>( L, -1, "hour"  , lua_isnumber, lua_toInteger );
    min   = retrieveTableValue<int>( L, -1, "min"   , lua_isnumber, lua_toInteger );
    sec   = retrieveTableValue<int>( L, -1, "sec"   , lua_isnumber, lua_toInteger );
    msec  = retrieveTableValue<int>( L, -1, "msec"  , lua_isnumber, lua_toInteger );

    lua_pop( L, 1 );

    r.dateAnalysed = TDateTime( year, month, day, hour, min, sec, msec );

    return r;
}

void lua_pushUncontrolledResult( lua_State* L, const UncontrolledResult& r )
{
    char actionFlag[2];
    actionFlag[0] = r.actionFlag;
    actionFlag[1] = '\0';

    lua_newtable( L );
    lua_pushinteger( L,    r.testID  );
    lua_setfield   ( L, -2, "testID" );
    lua_pushinteger( L,    r.resultID  );
    lua_setfield   ( L, -2, "resultID" );
    lua_pushinteger( L,    r.machineID  );
    lua_setfield   ( L, -2, "machineID" );
    lua_pushinteger( L,    r.projectID  );
    lua_setfield   ( L, -2, "projectID" );
    lua_pushnumber ( L,    r.resultValue );
    lua_setfield   ( L, -2, "resultValue" );
    lua_pushstring ( L,    r.resultText.c_str() );
    lua_setfield   ( L, -2, "resultText" );
    lua_pushstring ( L,    r.barcode.c_str() );
    lua_setfield   ( L, -2, "barcode" );
    lua_pushstring ( L,    r.runID.c_str() );
    lua_setfield   ( L, -2, "runID" );
    lua_pushstring ( L,    actionFlag );
    lua_setfield   ( L, -2, "actionFlag" );
    
    unsigned short year, month, day, hour, min, sec, msec;

    r.dateAnalysed.DecodeDate( &year, &month, &day );
    r.dateAnalysed.DecodeTime( &hour, &min, &sec, &msec );

    lua_newtable( L );
    lua_pushinteger( L,      year );
    lua_setfield   ( L, -2, "year" );
    lua_pushinteger( L,      month );
    lua_setfield   ( L, -2, "month" );
    lua_pushinteger( L,      day );
    lua_setfield   ( L, -2, "day" );
    lua_pushinteger( L,      hour );
    lua_setfield   ( L, -2, "hour" );
    lua_pushinteger( L,      min );
    lua_setfield   ( L, -2, "min" );
    lua_pushinteger( L,      sec );
    lua_setfield   ( L, -2, "sec" );
    lua_pushinteger( L,      msec );
    lua_setfield   ( L, -2, "msec" );
    lua_setfield   ( L, -2, "dateAnalysed" );
}



}


