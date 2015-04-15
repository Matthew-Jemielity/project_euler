#include <stdio.h>

#define LIMIT 1000
#define UNUSED( arg ) (( void )( arg ))

#define indirection( obj ) ( * ( obj ))
#define pointer( type ) type *
#define reference( obj ) ( & ( obj ))

typedef long ( indirection( operator_func_t ))
(
	unsigned int const x,
	unsigned int const y
);

typedef struct
{
	unsigned int multiply;
	unsigned int limit;
	operator_func_t operator;
}
operation_t;

static inline long
summator
(
	unsigned int const x,
	unsigned int const y
)
{
	return x + y;
}

static inline long
substractor
(
	unsigned int const x,
	unsigned int const y
)
{
	return x - y;
}

static inline long
operate_on_multiplies
(
	unsigned int const multiply,
	unsigned int const limit,
	operator_func_t operator
)
{
	unsigned int iterator;
	long result;

	for
	(
		iterator = multiply,
		result = 0
		;
		iterator < limit
		;
		iterator += multiply
	)
	{
		/*
		 * don't rely on order of operations
		 * in for loop's afterthought
		 */
		result = operator( result, iterator );
	}

	return result;
}

int
main
(
	int argc,
	char** args
)
{
	long sum;
	operation_t operations[] =
	{
		{ 3, LIMIT, summator },
		{ 15, LIMIT, substractor },
		{ 5, LIMIT, summator }
	};
	size_t count = sizeof( operations ) / sizeof( operations[ 0 ] );
	size_t iterator;

	UNUSED
	(
		argc
	);
	UNUSED
	(
		args
	);

	for
	(
		iterator = 0,
		sum = 0
		;
		iterator < count
		;
		++iterator
	)
	{
		pointer( operation_t ) current = 
			reference( operations[ iterator ] );
		sum += operate_on_multiplies
			(
				current->multiply,
				current->limit,
				current->operator
			);
	}

	printf
	(
		"%ld\n",
		sum
	);

	return 0;
}

