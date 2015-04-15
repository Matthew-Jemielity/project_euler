#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint32_t reverse( uint32_t const n )
{
    ldiv_t division;
    uint32_t result = 0;
    uint32_t workspace = n;

    do
    {
        division = ldiv( workspace, 10 );
        result = 10 * result + ( uint32_t ) division.rem;
    }
    while( 0 < ( workspace = division.quot ) );
    return result;
}

static inline bool is_palindrome( uint32_t const n ) { return reverse( n ) == n; }

static inline int32_t largest_palindrome_product( uint32_t const n )
{
    register uint32_t const limit = n / 10;
    uint32_t max = limit * limit;
    uint32_t x, y;

    for( x = n; x > limit; --x )
    {
        for( y = x; y > limit; --y )
        {
            uint32_t const product = x * y;
            if( is_palindrome( product ) ) { max = ( max < product ) ? product : max; }
        }
    }
    return max;
}

int main( int argc, char** args )
{
    if( 2 > argc ) { return 1; }
    printf
        (
            "largest palindrome from product is: %" PRIu32 "\n",
            largest_palindrome_product( atol( args[ 1 ] ) )
        );

    return 0;
}

