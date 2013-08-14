#include "MockConfig.h"

namespace valc
{

const std::string MockConfig::config =
"LoadWorklistEntries:\n"
"SELECT wl.record_no, machine_cid, barcode, test_cid, group_id, \\n"
"category_id, sample_id, project_cid,                           \\n"
"profile_id, profile_name, time_stamp, ts_sequence, status,     \\n"
"diluent, buddy_result_id                                       \\n"
"from valc_worklist wl                                          \\n"
"WHERE                                                          \\n"
"wl.machine_cid in (-1019349, -1019430)                         \\n"
"and wl.status in ('Q','P','C','S','W', 'X')                    \\n"
"ORDER BY wl.record_no                                          \n"
"\n"
"\n"
"\n"
"LoadWorklistRelations:\n"
"SELECT wl.record_no, wr.rerun_id, wr.parent_id, wr.motivation  \\n"
"FROM valc_worklist wl, c_worklist_relation wr                  \\n"
"WHERE wl.record_no in ( wr.rerun_id, wr.parent_id )            \\n"
"and                                                            \\n"
"wl.machine_cid in (-1019349, -1019430)                         \\n"
"and wl.status in ('Q','P','C','S','W', 'X')                    \\n"
"ORDER BY wl.record_no                                          \n"
"\n"
"\n"
"\n"
"RefTempTableName:\n"
"Q109Temp\n"
"\n"
"\n"
"\n"
"# The following queries both reference a session temporary table.  Results from each are processed IN PARALLEL.                \n"
"# A possible problem is that one set of results is not obtained, with the complaint that the session                           \n"
"# temporary table is not visible or does not exist. A cause of such a problem is that, behind the scenes, the ODBC             \n"
"# driver has created a distinct connection/session.                                                                            \n"
"#                                                                                                                              \n"
"# Ways of resolving this:                                                                                                      \n"
"# 1) Use SysWOW64/odbcad32 to Configure the User Data Source. On the Advanced tab, select 'Cursors' rather than 'Select Loops'.\n"
"#                                                                                                                              \n"
"# 2) Alternatively, don't use a session temporary table. Use an ordinary table.                                                \n"
"\n"
"\n"
"LoadReferencedWorklistEntries:\n"
"SELECT wl.record_no, machine_cid, barcode, test_cid, group_id, \\n"
"category_id, sample_id, project_cid,                           \\n"
"profile_id, profile_name, time_stamp, ts_sequence, status,     \\n"
"diluent, buddy_result_id                                       \\n"
"FROM valc_worklist wl, Q109Temp tt                             \\n"
"WHERE                                                          \\n"
"wl.record_no == tt.record_no                                  \\n"
"ORDER BY wl.record_no                                          \n"
"\n"
"\n"
"\n"
"LoadReferencedWorklistRelations:\n"
"SELECT wl.record_no, wr.rerun_id, wr.parent_id, wr.motivation  \\n"
"FROM c_worklist_relation wr, Q109Temp wl                       \\n"
"WHERE wl.record_no in ( wr.rerun_id, wr.parent_id )            \\n"
"ORDER BY wl.record_no                                          \n"
"\n"
"\n"
"\n"
"LoadNonLocalResults:\n"
"SELECT bd.barcode, bd.machine_cid, bd.alpha_sample_id,         \\n"
"bd.database_name, brf.buddy_result_id, brf.test_id,            \\n"
"brf.res_value, brf.action_flag, brf.date_analysed,             \\n"
"brf.res_text, brf.update_when, cbw.record_no                   \\n"
"FROM valc_worklist cbw, buddy_database bd,                     \\n"
"buddy_result_float_valc brf                                    \\n"
"WHERE                                                          \\n"
"( (brf.cbw_record_no = cbw.record_no)                          \\n"
"  or                                                           \\n"
"  (brf.buddy_result_id = cbw.buddy_result_id)                  \\n"
")                                                              \\n"
"and brf.buddy_sample_id = bd.buddy_sample_id                   \\n"
"and bd.machine_cid != -1019349                                 \\n"
"and cbw.machine_cid in (-1019349, -1019430)                    \\n"
"and cbw.status in ('Q','P','C','S','W', 'X')                   \\n"
"\n"
"\n"
"\n"
"LoadBuddyDatabase:\n"
"SELECT bd.buddy_sample_id, bd.barcode, bd.date_analysed,       \\n"
"bd.database_name, bd.alpha_sample_id, bd.machine_cid,          \\n"
"brf.buddy_result_id, brf.test_id, brf.res_value,               \\n"
"brf.action_flag, brf.date_analysed, brf.res_text,              \\n"
"brf.update_when, brf.cbw_record_no, sr.run_id, sr.is_open,     \\n"
"sr.created_when, sr.closed_when, sr.sequence_position          \\n"
"FROM                                                           \\n"
"(                                                              \\n"
"    ( buddy_database bd left join buddy_result_float_valc brf  \\n"
"      on bd.buddy_sample_id = brf.buddy_sample_id )            \\n"
"    left join sample_run sr on bd.sample_run_id = sr.run_id    \\n"
")                                                              \\n"
"WHERE                                                          \\n"
"bd.machine_cid = -1019349                                      \\n"
"AND                                                            \\n"
"( ( bd.barcode like 'QC%' ) OR ( bd.alpha_sample_id != 0 ) )   \\n"
"AND                                                            \\n"
"NVL( sr.fao_level_one, 'y' ) = 'y'                             \\n"
"order by NVL( sr.sequence_position, bd.buddy_sample_id )       \n"
"\n"
"\n"
;

}

