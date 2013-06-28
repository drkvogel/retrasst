//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ProfileReport1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TProfileReport *ProfileReport;

//---------------------------------------------------------------------------

__fastcall TProfileReport::TProfileReport(TComponent* Owner)
	: TForm(Owner)
{
	ddbQuery -> SQL -> Text = "select test_profile_name, test_profile_description, t.test_profile_id,"
			" ct.external_name, ct.test_cid, t.lower_test_limit, t.upper_test_limit, t.trigger_id,"
			" cm.external_name as machine_name, t.machine_cid as machine_cid_1"
		" from test_profile_description p, test_profile t, c_test ct, c_machine cm"
		" where p.test_profile_id = t.test_profile_id"
			" and t.test_cid = ct.test_cid and t.machine_cid = cm.machine_cid"

		" union (select test_profile_name, test_profile_description, t.test_profile_id,"
			" ct.external_name, ct.test_cid, t.lower_test_limit, t.upper_test_limit, t.trigger_id,"
			" cl.external_name as machine_name, t.machine_cid as machine_cid_1"
		" from test_profile_description p, test_profile t, c_test ct, c_object_name cl"
		" where p.test_profile_id = t.test_profile_id"
			" and t.test_cid = ct.test_cid and t.machine_cid = cl.object_cid)"

		" order by test_profile_name, external_name, machine_name";
}

//---------------------------------------------------------------------------
