#ifndef IsysDebugCommsH
#define IsysDebugCommsH

#include <System.hpp>
#include <SysUtils.hpp>

#include "DebugCommsFrm.h"
#include "InputFile.h"

class IsysDebugComms : public LIMSCommsBase
{
public:
    IsysDebugComms();
    unsigned available();
	unsigned readBytes( unsigned char * buffer, unsigned count );
	unsigned sendBytes( const unsigned char * bytes, unsigned count );
    void printMessage(String msg);
    void addMessage(String msg);
	void clear();
    static bool useDebugComms;
    static bool disableReply;
private:
    std::auto_ptr<TdebugComms> frm;

    IsysDebugComms( const IsysDebugComms& );
    IsysDebugComms& operator=( const IsysDebugComms& );
};

#endif
