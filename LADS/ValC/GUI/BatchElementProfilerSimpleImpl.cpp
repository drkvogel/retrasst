#include "BatchElementProfilerSimpleImpl.h"
#include "ExceptionUtil.h"
#include "SnapshotUtil.h"
#include <sstream>

namespace valcui
{
BatchElementProfilerSimpleImpl::BatchElementProfilerSimpleImpl()
{
}

std::string BatchElementProfilerSimpleImpl::characterizeGroup( bool QC )
{
    return QC ? "QCGroup" : "UnkGroup";
}

std::string BatchElementProfilerSimpleImpl::characterizeRun( bool QC, bool isOpen )
{
    std::ostringstream s;
    s << ( QC ? "QC" : "Unk" ) << "Run" << ( isOpen ? "Open" : "Closed" );
    return s.str();
}

std::string BatchElementProfilerSimpleImpl::characterizeResult(   
    const valc::TestResult* r,
    const paulst::Nullable<valc::RuleResults>& rr,
    const valc::WorklistEntry* associatedWorklistEntry )
{
    const bool QC = isQC( associatedWorklistEntry );

    std::ostringstream s;

    s << ( QC ? "QC" : "Unk" ) << "Result";

    if ( QC )
    {
        if ( rr.isNull() )
        {
            s << "NoRuleResults";
        }
        else
        {
            switch( rr.get().getSummaryResultCode() )
            {
            case valc::ResultCode::RESULT_CODE_FAIL             : s << "Fail"           ; break;
            case valc::ResultCode::RESULT_CODE_PASS             : s << "Pass"           ; break;
            case valc::ResultCode::RESULT_CODE_BORDERLINE       : s << "Borderline"     ; break;
            case valc::ResultCode::RESULT_CODE_ERROR            : s << "Error"          ; break;
            case valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED : s << "NoRulesApplied" ; break;
            case valc::ResultCode::RESULT_CODE_NULL             : s << "Null"           ; break;
            default: paulst::exception( "ResultCode value of %d not expected", rr.get().getSummaryResultCode() );
            }
        }
    }
    else
    {
        switch ( r->getControlStatus().summaryCode() )
        {
        case valc::ControlCode::CONTROL_STATUS_UNCONTROLLED             : s << "Uncontrolled"       ; break;
        case valc::ControlCode::CONTROL_STATUS_CONFIG_ERROR_NO_RULES    : s << "ConfigErrorNoRules" ; break;
        case valc::ControlCode::CONTROL_STATUS_ERROR                    : s << "Error"              ; break;
        case valc::ControlCode::CONTROL_STATUS_FAIL                     : s << "Fail"               ; break;
        case valc::ControlCode::CONTROL_STATUS_BORDERLINE               : s << "Borderline"         ; break;
        case valc::ControlCode::CONTROL_STATUS_PASS                     : s << "Pass"               ; break;
        default: paulst::exception( "ControlCode value of %d not expected", r->getControlStatus().summaryCode() );
        }
    }

    return s.str();
}

std::string BatchElementProfilerSimpleImpl::characterizePending( const valc::WorklistEntry* w )
{
    return isQC(w) ? "QCPending" : "UnkPending";
}

}

