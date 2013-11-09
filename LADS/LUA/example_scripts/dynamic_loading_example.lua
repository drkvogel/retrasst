

function someFunction( someArg )
    print( 'someFunction', someArg )
end

local pieces = { ' someFunction( someArg ) ' }

function getPieces()
    local piece = nil
    if #pieces > 0 then
       piece = table.remove( pieces, 1 )
    end
    return piece
end 


local newFunction = load( getPieces )

newFunction( 'Hi Paul' )


local functionTable = {}

functionTable.one = loadstring( [[
    sequence = args.sequence
    mean     = args.mean
    stdDev   = args.stdDev
    print( sequence, mean, stdDev )
    ]] )

functionTable.two = loadstring( [[
    sequence = args.sequence
    mean     = args.mean
    stdDev   = args.stdDev
    print( stdDev, mean, sequence )
    ]] )
    

function callLoadedFunctions( sequence, mean, stdDev )

    args.sequence = sequence
    args.mean     = mean
    args.stdDev   = stdDev
    for functionName, functionCode in pairs( functionTable ) do
        functionCode()
    end
end

args = {}

callLoadedFunctions( 'bingo', 'howzat?!', 'Goal!' )
callLoadedFunctions( 'one', 'two', 'three' )



