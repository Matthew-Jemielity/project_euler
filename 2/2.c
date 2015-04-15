#include <stdbool.h>
#include <stdio.h>

#define THRESHOLD 4000000
#define USE_SEQUENCE USE_PLAIN_FIBONACCI || USE_ONLY_EVENS

#if USE_SEQUENCE
typedef struct sequence_t sequence_t;

typedef unsigned long long int ( * sequence_operator_func_t )
	(
		unsigned long long int const previous,
		unsigned long long int const current
	);

typedef struct
{
	unsigned long long int previous;
	unsigned long long int current;
	sequence_operator_func_t operation;
}
sequence_ctx_t;

typedef void ( * sequence_constructor_func_t )
	(
		sequence_t * const self,
		unsigned long long int const previous,
		unsigned long long int const current,
		sequence_operator_func_t const operation
	);

typedef unsigned long long int ( * sequence_get_func_t )
    (
        sequence_t * const self
    );

typedef unsigned long long int ( * sequence_next_func_t )
	(
		sequence_t * const self
	);

struct sequence_t
{
	sequence_ctx_t ctx;
	sequence_constructor_func_t constructor;
    sequence_get_func_t get_previous;
    sequence_get_func_t get_current;
	sequence_next_func_t next;
};

typedef bool ( * sum_validator_func_t )
	(
		unsigned long long int const value
	);
#endif /* USE_SEQUENCE */

#if USE_PLAIN_FIBONACCI
static inline unsigned long long
fibonacci_operator
(
	unsigned long long int const previous,
	unsigned long long int const current
)
{
	return previous + current;
}

static inline bool
validate_is_fibonacci_even
(
	unsigned long long int const value
)
{
	return 0 == ( value & 1 );
}
#endif /* USE_PLAIN_FIBONACCI */

#if USE_ONLY_EVENS
/*
 * it can be proven that all even values in Fibonacci sequence
 * can be written as sequence:
 * F(n) = 4 * F(n-3) + F(n-6)
 * so we can construct a sequence:
 * E(n) = 4 * E(n-1) + E(n-2)
 * which, with proper initialization
 * will yield the values we want
 */
static inline unsigned long long int
fibonacci_evens_operator
(
	unsigned long long int const previous,
	unsigned long long int const current
)
{
	return 4 * current + previous;
}

static inline bool
validate_always
(
	unsigned long long int const value
)
{
	return value == value;
}
#endif /* USE_ONLY_EVENS */

#if USE_SEQUENCE
static void
fibonacci_like_constructor
(
	sequence_t * const self,
	unsigned long long int const previous,
	unsigned long long int const current,
	sequence_operator_func_t const operation
)
{
	self->ctx.previous = previous;
	self->ctx.current = current;
	self->ctx.operation = operation;
}

static unsigned long long int
fibonacci_like_get_previous
(
    sequence_t * const self
)
{
    return self->ctx.previous;
}

static unsigned long long int
fibonacci_like_get_current
(
    sequence_t * const self
)
{
    return self->ctx.current;
}

static unsigned long long int
fibonacci_like_next
(
	sequence_t * const self
)
{
	unsigned long long int const result =
		self->ctx.operation( self->ctx.previous, self->ctx.current );

	self->ctx.previous = self->ctx.current;
	self->ctx.current = result;

	return result;
}

static unsigned long long int
sequence_sum
(
	sequence_t * sequence,
	unsigned long long int const threshold,
	sum_validator_func_t const validator
)
{
	unsigned long long int sum = 0;
	unsigned long long int value;

    if( true == validator( sequence->get_previous( sequence )))
    {
        sum += sequence->get_previous( sequence );
    }
    if( true == validator( sequence->get_current( sequence )))
    {
        sum += sequence->get_current( sequence );
    }

	while( threshold > ( value = sequence->next( sequence )))
	{
#if SEQUENCE_SUM_DEBUG
		printf("%llu ", value);
#endif /* SEQUENCE_SUM_DEBUG */
		if( true == validator( value ))
		{
			sum += value;
		}
	}

#if SEQUENCE_SUM_DEBUG
	puts( "" );
#endif /* SEQUENCE_SUM_DEBUG */
	return sum;
}
#endif /* USE_SEQUENCE */

int main
(
	int argc,
	char ** args
)
{
	unsigned long long int sum;
#if USE_SEQUENCE
	sequence_t fibonacci_like =
	{
		{
			0,
			0,
			NULL
		},
		fibonacci_like_constructor,
        fibonacci_like_get_previous,
        fibonacci_like_get_current,
		fibonacci_like_next
	};
#endif /* USE_SEQUENCE */

	( void ) argc;
	( void ) args;

#if USE_PLAIN_FIBONACCI
	/* we can ignore this return value */
	fibonacci_like.constructor( &fibonacci_like, 0, 1, fibonacci_operator );
	sum = sequence_sum( &fibonacci_like, THRESHOLD, validate_is_fibonacci_even );
#endif /* USE_PLAIN_FIBONACCI */

#if USE_ONLY_EVENS
	fibonacci_like.constructor( &fibonacci_like, 0, 2, fibonacci_evens_operator );
	sum = sequence_sum( &fibonacci_like, THRESHOLD, validate_always );
#endif /* USE_ONLY_EVENS */

	printf( "%llu\n", sum );
	return 0;
}
