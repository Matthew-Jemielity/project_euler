/* C99 */
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef void* ( * iterator_t_init )
    ( void const* const data );

typedef struct
    {
        bool has_next;
    }
    iterator_t_next_result_t;

typedef iterator_t_next_result_t* ( * iterator_t_next )
    ( void* const data );

typedef struct
    {
        iterator_t_init init;
        iterator_t_next next;
    }
    iterator_t;

typedef struct
    {
        uint64_t integer;
        uint64_t factor;
    }
    factorizator_data_t;

typedef struct
    {
        bool has_next; /* must be first, iterator_t_next_result_t */
        uint64_t result;
    }
    factorizator_next_result_t;

static inline factorizator_data_t* factorizator_internal_init
( factorizator_data_t const* const data )
{
    factorizator_data_t* copy;
    if( NULL == ( copy = calloc( 1, sizeof( factorizator_data_t ))))
    {
        return NULL;
    }
    copy->integer = data->integer;
    copy->factor = data->factor;
    return copy;
}

static inline void* factorizator_init( void const* const data )
{
    return factorizator_internal_init( data );
}

static void* factorizator_internal_next
( factorizator_data_t* const data )
{
    factorizator_next_result_t* result;
    if( NULL == ( result = calloc( 1, sizeof( factorizator_next_result_t ))))
    {
        return NULL;
    }
    if( data->integer < data->factor )
    {
        result->has_next = false;
        return result;
    }
    while( 0 != ( data->integer % data->factor )) { ++data->factor; }

    result->has_next = true;
    result->result = data->factor;

    data->integer /= data->factor;
    data->factor = 2;

    return result;
}

static inline iterator_t_next_result_t* factorizator_next( void* const data )
{
    return factorizator_internal_next( data );
}

typedef void ( * for_operation_t )( void* const result, iterator_t_next_result_t const* const next_result );
typedef void* ( * for_result_init_t )( void );

static void* iterator_t_for( iterator_t const* const iterator, void const* const init_data, for_operation_t const op, for_result_init_t const result_init )
{
    void* result = result_init();
    void* data = iterator->init( init_data );
    iterator_t_next_result_t* next_result;

    if( NULL == result || NULL == data ) { return result; }
    while
    (
        NULL != ( next_result = iterator->next( data ))
        &&
        true == next_result->has_next
    )
    {
         op( result, next_result );
         free( next_result );
    }
    free( next_result );
    free( data );
    return result;
}

typedef struct
    {
        uint64_t result;
    }
    factorizator_max_result_t;

static inline void factorizator_internal_max( factorizator_max_result_t* const result, factorizator_next_result_t const* const next_result )
{
    if( result->result < next_result->result ){ result->result = next_result->result; }
}

static inline void factorizator_max( void* const result, iterator_t_next_result_t const* const next_result )
{
    factorizator_internal_max( result, ( void const* const ) next_result );
}

static inline void* factorizator_max_result_init( void )
{
    return calloc( 1, sizeof( factorizator_max_result_t ));
}

static uint64_t max_prime_factor( uint64_t const value )
{
    factorizator_data_t init_data = { 0, 2 };
    factorizator_max_result_t* max = NULL;
    iterator_t const factorizator = { factorizator_init, factorizator_next };
    if( 1 == value ) { return 1 };
    init_data.integer = value;
    max = iterator_t_for( &factorizator, &init_data, factorizator_max, factorizator_max_result_init );
    if( NULL == max ) { return 0; }
    {
        uint64_t mpf = max->result;
        free( max );
        return mpf;
    }
}

int main( int argc, char** args )
{
    if( argc < 2 ) { return 1; }
    printf( "max prime factor for %s is %" PRIu64 "\n", args[ 1 ], max_prime_factor( atoll( args[ 1 ] )));
    return 0;
}

