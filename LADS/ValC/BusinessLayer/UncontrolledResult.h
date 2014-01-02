#ifndef UNCONTROLLEDRESULTH
#define UNCONTROLLEDRESULTH

#include <string>
#include <System.hpp>

class lua_State;

namespace valc
{

struct UncontrolledResult
{
    int         testID;             /* default: 0 */
    int         resultID;           /* default: 0 */
    int         machineID;          /* default: 0 */
    double      resultValue;        /* default: 0 */
    std::string resultText;         /* default: '' */
    std::string barcode;            /* default: '' */
    int         projectID;          /* default: 0 */
    TDateTime   dateAnalysed;       /* default: 1977 */
    char        actionFlag;         /* default: '?' */

    UncontrolledResult();
};


void               lua_pushUncontrolledResult( lua_State* L, const UncontrolledResult& r );
UncontrolledResult popUncontrolledResult     ( lua_State* L, int stackPosition );

}

#endif

