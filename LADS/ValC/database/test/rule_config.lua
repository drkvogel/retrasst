 rule = function () end
 context = {}
 sequenceSQL = [[
select brf.buddy_result_id, bd.barcode, brf.res_value, brf.res_text, brf.action_flag, bd.database_name,
qc_gate.record_cid, qc_gate.mean, qc_gate.stddev, qc_gate.source
from
buddy_result_float brf, buddy_database bd, qc_lot, qc_gate
where
brf.test_id = %d
and
brf.buddy_sample_id = bd.buddy_sample_id
and
bd.machine_cid = %d
and
left( bd.barcode, 8 ) = left( qc_lot.barcode_pattern, 8 )
and
qc_lot.qc_status in ( 0, 1, 2 )
and
qc_gate.qc_id = qc_lot.qc_id
and
qc_gate.status = 1
and 
qc_gate.source > 0
and 
qc_gate.test_cid = brf.test_id
and
qc_gate.machine_cid = bd.machine_cid
order by 
brf.date_analysed desc
 ]]
 sqlType = { integer = 0, string = 1, real = 2 }
 sqlTypes = { sqlType.integer, sqlType.string, sqlType.real, sqlType.string,
               sqlType.string,  sqlType.string, sqlType.integer, sqlType.real, sqlType.real, sqlType.integer }

 function onLoad(loadFunc)
      local script = loadFunc('3:2s')
      rule, errorMsg = load(script)
      if ( errorMsg ) then error( errorMsg ) end
      local connectionHandle = openConnection( config('QCResultSequenceConnectionString') )
      local sql = string.format( sequenceSQL, TEST_ID, MACHINE_ID )
      local queryHandle = executeQuery( sql, sqlTypes, connectionHandle )
      local row = fetchNextRow( queryHandle )
      context.sequence = {}
      while row do
          local elem = {}
          elem.resultID    = row[1]
          elem.barcode     = row[2]
          elem.resultValue = row[3]
          elem.resultText  = row[4]
          elem.actionFlag  = row[5]
          elem.dbName      = row[6]
          elem.gateID      = row[7]
          elem.mean        = row[8]
          elem.stdDev      = row[9]
          elem.source      = row[10]
          elem.testID      = TEST_ID
          elem.machineID   = MACHINE_ID
          table.insert( context.sequence, elem )
          row = fetchNextRow( queryHandle )
      end
      
      closeQuery( queryHandle )
      closeConnection( connectionHandle )
 end

 function applyRules( qc, currentGateIfAny )
   assert( qc.testID == TEST_ID, 'Wrong test. Configuration error' )
   assert( currentGateIfAny )
   local results = {}
   context.qc = qc
   context.gate = currentGateIfAny
   local result = rule()
   table.insert( results, result )
   return { results, result.msg, result.resultCode, context.gate.id }
 end 

