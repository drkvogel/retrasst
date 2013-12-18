#include "Gate.h"
#include "LuaInclude.h"

namespace valc
{

Gate::Gate()
    :
    m_mean(0.0), m_stdDev(0.0), m_id(0), m_source(0)
{
}

Gate::Gate( double mean, double stdDev, int id, int source )
    :
    m_mean( mean), m_stdDev( stdDev ), m_id( id ), m_source( source )
{
}

Gate::Gate( const Gate& g )
    :
    m_mean  ( g.m_mean ),
    m_stdDev( g.m_stdDev ),
    m_id    ( g.m_id ),
    m_source( g.m_source )
{
}

Gate& Gate::operator=( const Gate& g )
{
    m_mean   = g.m_mean;
    m_stdDev = g.m_stdDev;
    m_id     = g.m_id;
    m_source = g.m_source;
    return *this;
}

double Gate::getMean() const
{
    return m_mean;
}

int Gate::getSource() const
{
    return m_source;
}

double Gate::getStdDev() const
{
    return m_stdDev;
}

int Gate::getID() const
{
    return m_id;
}

void lua_pushGate( lua_State* L, const Gate& g )
{
    lua_newtable   ( L );
    lua_pushnumber ( L, g.getMean() );
    lua_setfield   ( L, -2, "mean" );
    lua_pushnumber ( L, g.getStdDev() );
    lua_setfield   ( L, -2, "stdDev" );
    lua_pushinteger( L, g.getSource() );
    lua_setfield   ( L, -2, "source" );
    lua_pushinteger( L, g.getID() );
    lua_setfield   ( L, -2, "id" );
}

}

