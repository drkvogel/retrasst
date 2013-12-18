local sqlType = { integer = 0, string = 1, real = 2 }
local connectionHandle = openConnection('dsn=paulst_brat_64;db=paulst_test')
local queryHandle = executeQuery( 'select test_cid, external_name from c_test order by 1', {sqlType.integer, sqlType.string}, connectionHandle )
local row = fetchNextRow( queryHandle )
while row do
    print( row[1], row[2] );
row = fetchNextRow( queryHandle )
end
closeQuery( queryHandle )
closeConnection( connectionHandle )
