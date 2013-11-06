#ifndef devnoteH
#define devnoteH
//---------------------------------------------------------------------------
#include <dbt.h>
#include <vector>
//---------------------------------------------------------------------------
/*
	VALUES OF ((PDEV_BROADCAST_HDR)lParam)->dbch_devicetype)
DBT_DEVTYP_OEM 		OEM-defined device type
DBT_DEVTYP_DEVNODE 	devnode number (specific to Windows 95)
DBT_DEVTYP_VOLUME 	logical volume (drive)
DBT_DEVTYP_PORT 	serial or parallel port
DBT_DEVTYP_NET 		network resource (UNC)
*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
USAGE REQUIRES FOLLOWING CODE IN TFORM CLASS HEADER
	BEGIN_MESSAGE_MAP
	 MESSAGE_HANDLER(WM_DEVICECHANGE,TMessage,OnDeviceChange)
	END_MESSAGE_MAP(TForm)
PLUS EVENT AS FOLLOWS
	void __fastcall TSomeForm::OnDeviceChange(TMessage &Msg)
	{
          	x.processMessage( Msg );	
	}
*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef	struct
	{
	unsigned int	dev_type;
	bool	(*fn)( const long lp );
	}
	DEVICE_NOTIFY_FILTER;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class DEVICE_NOTIFY
{
private:
	std::vector<DEVICE_NOTIFY_FILTER>	arrive;
	std::vector<DEVICE_NOTIFY_FILTER>	remove;
	bool 	deviceChange( std::vector<DEVICE_NOTIFY_FILTER> *cb,
			const DEV_BROADCAST_HDR* hdr );
	void 	setCallback( std::vector<DEVICE_NOTIFY_FILTER> *cb,
			const unsigned int device_type,
			bool (*callback)(const long lp));
public:
	DEVICE_NOTIFY( void );
	~DEVICE_NOTIFY( void );
	bool	processMessage( TMessage& msg );
	void 	setCallbackArrive( const unsigned int device_type,
			bool (*callback)(const long lp) );
	void	setCallbackRemove( const unsigned int device_type,
			bool (*callback)(const long lp) );
	static	const char DEVICE_NOTIFY::getDriveLetter( const long hdr );
	static	const int  DEVICE_NOTIFY::getDriveNumber( const long hdr );
};
//---------------------------------------------------------------------------
#endif

