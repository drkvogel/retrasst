//---------------------------------------------------------------------------

#ifndef LeaseMod1H
#define LeaseMod1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TLeaseManager;
class TLeaseMod : public TDataModule
{
__published:	// IDE-managed Components
private:	// User declarations
	TLeaseManager	*LeaseManager;
	int	ProjectLeasePid;			// PROJECT ID
	int	CentralLeasePid;			// PROJECT ID
	int	LeaseTid;			// TASK ID
	int	LeaseLevel;			// LEASE TYPE
	String	LeaseDescrip;
public:		// User declarations
    void SetUpLeaseManager( TDatabase* database );
    bool ActivateCentralLease();
    bool ActivateProjectLease();
    void DeactivateCentralLease();
    void DeactivateProjectLease();
    void DeleteLeaseManager();
    __fastcall TLeaseMod(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLeaseMod *LeaseMod;
//---------------------------------------------------------------------------
#define	LEASEE_BUDDY2ALPHA		10      // BLOCKS WRAPPERS
#define	LEASEE_SAMPLERECEPTION		20	// BLOCKS ITSELF
#define	LEASEE_EXTERNALWRAPPER		30	// BLOCKS ITSELF, BUDDY2ALPHA
#define	LEASEE_WIZARD			40	// BLOCKS ALL AND VICE VERSA

#endif
 