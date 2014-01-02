#ifndef DBCONNECTIONH
#define DBCONNECTIONH

#include <string>

namespace paulstdb
{

class Cursor;

/*
    Abstract representation of a connection to a database that
    can be queried and updated.
*/
class DBConnection
{
public:
    DBConnection();
    virtual ~DBConnection();
    virtual void close() = 0;
    /*
        Executes the specified SQL query and returns a Cursor
        for reading the results of the query.

        Be sure to delete the Cursor when it is no longer needed.
    */
    virtual Cursor* executeQuery( const std::string& sql ) = 0;
	/*
        Executes the specified SQL.
    */
    virtual void    executeStmt ( const std::string& sql ) = 0;
private:
    DBConnection( const DBConnection& );
    DBConnection& operator=( const DBConnection& );
};

}

#endif

