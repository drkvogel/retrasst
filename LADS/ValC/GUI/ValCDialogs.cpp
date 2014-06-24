#include <FMX.Dialogs.hpp>
#include <memory>
#include "ValCDialogs.h"

namespace valcui
{

void showMsg( const std::string& msg, TMsgDlgType t )
{
    const UnicodeString uMsg( msg.c_str() );

    MessageDlg( uMsg, t, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0 );
}

void reportException( const Exception& e )
{
    showMsg( AnsiString( e.Message.c_str() ).c_str(), TMsgDlgType::mtError );
}

void showErrorMsg( const std::string& msg )
{
    showMsg( msg, TMsgDlgType::mtError );
}

void showWarningMsg( const std::string& msg )
{
    showMsg( msg, TMsgDlgType::mtWarning );
}

}

