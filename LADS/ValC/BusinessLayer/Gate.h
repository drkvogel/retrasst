#ifndef GATEH
#define GATEH

class lua_State;

namespace valc
{

class Gate
{
public:
    Gate();
    Gate( double mean, double stddev, int ID, int source );
    Gate( const Gate& g );
    Gate& operator=( const Gate& g );
    double getMean() const;
    int    getSource() const;
    double getStdDev() const;
    int getID() const;
private:
    double m_mean, m_stdDev;
    int m_id, m_source;
};

void lua_pushGate( lua_State* L, const Gate& g );

}

#endif

