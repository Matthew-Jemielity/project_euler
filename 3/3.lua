function max_prime_factor( i )
    local divide = function( i, div ) while 0 ~= i % div do div = div + 1 end return div, i / div end
    local factorize = function( i ) while 1 < i do div, i = divide( i, 2 ); coroutine.yield( div ) end end
    local generate = coroutine.create( factorize )
    local iterate = function( _, __ ) --arguments unused
        return "dead" ~= coroutine.status( generate ) and select( 2, coroutine.resume( generate, i )) or nil
    end
    local factory = function( i ) return iterate, nil, nil end
    local max = 1; for it in factory( i ) do max = max < it and it or max end; return max
end

print( max_prime_factor( tonumber( arg[1] )))

