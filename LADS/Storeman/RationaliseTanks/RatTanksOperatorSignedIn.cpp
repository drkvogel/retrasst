//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RatTanksOperatorSignedIn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace RationaliseTanks;

operatorSignedIn::operatorSignedIn()
{
	m_hasSignedIn = false;
}

operatorSignedIn::~operatorSignedIn()
{

}

/*
	This one checks they are on a machine which is allowed and they have permission to
	use our page.
*/
bool operatorSignedIn::hasPermissionToSignIn()
{
	m_hasSignedIn = false;

	if (m_operatorPremissions.size() == 0)
	{
		//this user has no premissions, this means they can run with no restrictions
		//but they can't rationalise.
		m_hasSignedIn = true;
		return true;
	}
	//now lets check the premissions of this user.
	for (std::map<std::string, std::map<std::string,std::string> >::iterator itPresmssion=m_operatorPremissions.begin(); itPresmssion!=m_operatorPremissions.end(); ++itPresmssion)
	{
		std::map<std::string,std::string> prem = itPresmssion->second;

		std::string machine_cid = prem["machine_cid"];
		if (machine_cid != "0")
		{
			if (m_machine_cid != machine_cid)
				continue; //for a different machine
		}
		std::string page_number  = prem["page_number"];
		if (page_number  != "0")
		{
			if (page_number  != "5")
				continue; //for a different page
		}
		m_hasSignedIn = true;
		//good enough
		return true;
	}
	return false;
}

bool operatorSignedIn::hasPermissionToRationalise(const std::string &targetProject_cid)
{
	if (!m_hasSignedIn)
		return false;

	if (m_operatorPremissions.size() == 0)
	{
		#ifdef _DEBUG
		String displayString = String("You do not have permission to update the database, but as this is in debug mode, would you like to anyway?");
		int result = Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_YESNO);
		if (result == IDYES)
			return true;
		#else
		return false;
		#endif
	}
	//now lets check the premissions of this user.
	for (std::map<std::string, std::map<std::string,std::string> >::iterator itPresmssion=m_operatorPremissions.begin(); itPresmssion!=m_operatorPremissions.end(); ++itPresmssion)
	{
		std::map<std::string,std::string> prem = itPresmssion->second;

//See if user is allowed to save data from StoreMan
		if (prem["program_cid"] == "100") //StoreMan in c_objectname.object_cid [external_name = StoreMan]
		{
			std::string machine_cid = prem["machine_cid"];
			if (machine_cid != "0")
			{
				if (m_machine_cid != machine_cid)
					continue; //for a different machine
			}

			std::string page_number  = prem["page_number"];
			if (page_number  != "0")
			{
				if (page_number  != "5")
					continue; //for a different page
			}

//for the project,
			std::string project_cid  = prem["project_cid"];
			if (project_cid  != "0")
			{
				if (targetProject_cid  != project_cid)
					continue; //for a different project
			}
			return true;
		}
	}
	return false;
}




