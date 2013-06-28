//---------------------------------------------------------------------------

#include "LDbStage.h"

#pragma hdrstop
#pragma package(smart_init)


//---------------------------------------------------------------------------
//	Return the status of a record as a string for the user
//---------------------------------------------------------------------------

String LDbStage::getStatus() const
{
	switch( getStage() )
	{
		case EXPECTED:
			return "Expected";

		case ARRIVED:
			return "In progress";

		case TRANSMITTED:
			return "New results";

		case ALIQUOTED:
			return "New aliquots";

		case PROGRAMMED:
			return "Programmed";

		case ANALYSED:
			return "Analysed";

		case INVALIDATED:
			return "Invalidated";

		case L2_VALIDATED:
			return "Validated";

		case COLLECTED:
			return "Collected";

		case NOTIFIED:
			return "Notified";

		case DELETED:
			return "Deleted";

		default:
			return "Unknown";
	}
}

//---------------------------------------------------------------------------
