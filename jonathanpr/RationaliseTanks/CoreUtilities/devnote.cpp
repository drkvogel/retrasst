#include <vcl.h>
#pragma hdrstop
#include "devnote.h"
//---------------------------------------------------------------------------
/* IF DBT_DEVTYP_VOLUME == hdr->dbch_devicetype, THEN CAST LPARAM TO
	DEV_BROADCAST_VOLUME* AND USE THIS TO DETERMINE DRIVE
*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const char DEVICE_NOTIFY::getDriveLetter( const long hdr )
{
	int	n = getDriveNumber( hdr );
	return( ( n >= 0 && n < 26 )
		? (const char)('A' + n)
		: (const char) 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int DEVICE_NOTIFY::getDriveNumber( const long hdr )
{
	const 	DEV_BROADCAST_VOLUME* dbv =
			reinterpret_cast<const DEV_BROADCAST_VOLUME*>(hdr);
//	if ( ! ( dbv->dbcv_flags & DBTF_MEDIA ) )
//		{return( -1 );	// MEDIA BIT-FLAG NOT SET (APPARENTLY A PROBLEM?)
//		}
	DWORD   volume_bits = dbv->dbcv_unitmask;
	DWORD   bit_mask    = 1;
	int	drive;
	for ( drive = 0; drive < 26; drive++ )
		{if( bit_mask & volume_bits )
			{return( drive );
			}
		bit_mask <<= 1;
		}
	return( -1 );					// FAILURE
}
//===========================================================================
DEVICE_NOTIFY::DEVICE_NOTIFY( void )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DEVICE_NOTIFY::~DEVICE_NOTIFY( void )
{
}
//===========================================================================
bool DEVICE_NOTIFY::processMessage( TMessage& msg )
{
	bool	handled = false;
	switch(msg.WParam)
		{
		case DBT_DEVICEARRIVAL:		// DEVICE HAS ARRIVED
			handled = deviceChange( &arrive,
				reinterpret_cast<const
					DEV_BROADCAST_HDR*>(msg.LParam) );
			break;
		case DBT_DEVICEREMOVECOMPLETE:	// DEVICE HAS GONE AWAY
			handled = deviceChange( &remove,
				reinterpret_cast<const
					DEV_BROADCAST_HDR*>(msg.LParam) );
			break;
		case DBT_DEVICEQUERYREMOVE:	// ASK PERMISSION TO GO AWAY
		case DBT_DEVICEQUERYREMOVEFAILED: // QUERY-REMOVE REFUSED
		case DBT_DEVICETYPESPECIFIC:
		default:   	// OTHER CODES EXIST ALSO
			break;
		}
	return( handled );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool DEVICE_NOTIFY::deviceChange( std::vector<DEVICE_NOTIFY_FILTER> *cb,
	const DEV_BROADCAST_HDR* hdr )
{
	std::vector<DEVICE_NOTIFY_FILTER>::iterator di = cb->begin();
	while ( di != cb->end() )
		{
		if ( hdr->dbch_devicetype == di->dev_type || 0 == di->dev_type )
			{if ( di->fn( (const long) hdr ) )
				{return( true );	// HANDLED
				}
			}
		di++;
		}

	return( false );
}
//----------------------------------------------------------------------------
void DEVICE_NOTIFY::setCallback( std::vector<DEVICE_NOTIFY_FILTER> *cb,
		const unsigned int device_type,
		bool (*callback)( const long lp ) )
{
	DEVICE_NOTIFY_FILTER	df;
	df.dev_type = device_type;
	df.fn = callback;
	cb->push_back( df );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DEVICE_NOTIFY::setCallbackArrive( const unsigned int device_type,
		bool (*callback)( const long lp ) )
{
	setCallback( &arrive, device_type, callback );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DEVICE_NOTIFY::setCallbackRemove( const unsigned int device_type,
		bool (*callback)( const long lp ) )
{
	setCallback( &remove, device_type, callback );
}
//===========================================================================
#pragma package(smart_init)

