#! /usr/bin/env python

if __name__ == "__main__":
	limit = 1000

	# utility functors
	reductor = lambda x, y: reduce( x, y, 0 )
	substractor = lambda x, y: x - y
	summator = lambda x, y: x + y

	# data
	multiplies = \
	[
		3,
		15,		# doubled sum for multipliers of 15
		5
	]
	limits = [ limit ] * len( multiplies )
	operators = \
	[
		summator,
		substractor,	# doubled sum for multipliers of 15
		summator
	]

	# get sequences of numbers to add for all multipliers
	#	xrange creates next in seq as it's needed
	#	3rd argument for xrange is step,
	#	since it's the same as start value
	#	reuse multipliers for that
	iterators = map( xrange, multiplies, limits, multiplies )

	# for each sequence add/substract the numbers produced
	sums = map( reductor, operators, iterators )

	sum = reductor( summator, sums )

	print( sum )

