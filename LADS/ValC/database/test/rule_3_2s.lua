local result = { resultCode = 1, rule = '3:2s', msg = tostring( context.qc.resultID ) } 
local tooHigh = function ( result, mean, stddev ) return ( result > ( mean + ( 2 * stddev ) ) ) end 
local tooLow  = function ( result, mean, stddev ) return ( result < ( mean - ( 2 * stddev ) ) ) end 
local isFailure = nil                                                                              

if ( tooHigh( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) ) then isFailure = tooHigh end 

if ( tooLow ( context.qc.resultValue, context.gate.mean, context.gate.stdDev ) ) then assert( not(isFailure) ) isFailure = tooLow end 
                                                                                                          
if ( isFailure ) then                                                                                     
   local beyondFirst = false
   local failures = 1
   local passes   = 0
   local tested   = 1

   for i, elem in ipairs( context.sequence ) do
      if (  beyondFirst ) then
          tested = tested + 1
          result.msg = result.msg .. ', ' .. elem.resultID
          
          if ( isFailure( elem.resultValue, elem.mean, elem.stdDev ) ) then
              failures = failures + 1
              if ( failures == 3 ) then
                  break
              end
          else
              passes = passes + 1
              break -- No need to continue if a result has passed the test
          end
      else 
          beyondFirst = ( elem.resultID == context.qc.resultID )
      end
   end 

   assert( beyondFirst, 'Bad sequence' )

   if ( failures == 3 ) then
      result.resultCode = FAIL 
   elseif ( ( tested < 3 ) and ( passes == 0 ) ) then
      result.resultCode = ERROR
   end 

end 

local errorText = { [0] = 'Fail', [1] = 'Pass', [2] = 'Bord', [3] = 'Err', [-1] = 'None' } 

result.msg = errorText[ result.resultCode ] .. ' ' .. result.msg

return result

