#include <FMX.Dialogs.hpp>
#include <memory>
#include "TWaitDlg.h"
#include "ValCDialogs.h"

namespace valcui
{


void reportException( const Exception& e )
{
    MessageDlg( e.Message, TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK, 0 );
}

unsigned long showWaitDialog( HANDLE signal, const std::string& msg, unsigned long waitMillis )
{
    std::unique_ptr<TWaitDlg> dlg( new TWaitDlg(NULL, signal, 2000, waitMillis, msg ) );

    dlg->ShowModal();
    
    return dlg->getWaitResult();
}

}

