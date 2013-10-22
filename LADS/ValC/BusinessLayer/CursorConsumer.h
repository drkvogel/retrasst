#ifndef CURSORCONSUMERH
#define CURSORCONSUMERH

#include <string>

namespace paulstdb
{
    class Cursor;
}

namespace valc
{

class CursorConsumer
{
public:
    CursorConsumer();
    virtual ~CursorConsumer();
    virtual void consume( paulstdb::Cursor* c ) = 0;
    virtual std::string getSQL() = 0;
private:
    CursorConsumer( const CursorConsumer& );
    CursorConsumer& operator=( const CursorConsumer& );
};

}

#endif

