local Sequence = {}
Sequence.__index = Sequence

setmetatable
(
    Sequence,
    {
        __call = function( class, previous, current, operation )
            local self = setmetatable( {}, class or Sequence )
            if "number" ~= type( previous ) or "number" ~= type( current ) then
                error( "must pass numbers as previous and current values" )
            end
            if "function" ~= type( operation ) then
                error( "must pass operation( previous, current ) function" )
            end
            if "number" ~= type( operation( previous, current )) then
                error( "operation must return numeric value" )
            end
            self._previous, self._current, self._operation = previous, current, operation
            return self
        end
    }
)

function Sequence:previous()
    return self._previous
end

function Sequence:current()
    return self._current
end

function Sequence:next()
    self._previous, self._current = self._current, self._operation( self._previous, self._current )
    return self._current
end

function Sequence:next_until( threshold )
    local iterator = function( threshold, state )
        local result = self:next()
        if result <= threshold then return result end
    end
    return iterator, threshold, 0
end

local validate_equal = function( value )
    if 0 == value % 2 then return value end
end

local validate_all = function( value )
    return value
end

local sum = function( sequence, threshold, validator )
    local sum = 0
    sum = sum + ( validator( sequence:previous() ) or 0 )
    sum = sum + ( validator( sequence:current() ) or 0 )
    for value in sequence:next_until( threshold ) do
        sum = sum + ( validator( value ) or 0 )
    end
    return sum
end

local fibonacci = Sequence( 0, 1, function( previous, current ) return previous + current end )
local fibonacci_evens_only = Sequence( 0, 2, function( previous, current ) return previous + 4 * current end )

local threshold = tonumber(arg[1]) or 4000000
print( "fibonacci:", sum( fibonacci, threshold, validate_equal ))
print( "fibonacci-like, evens only:", sum( fibonacci_evens_only, threshold, validate_all ))

