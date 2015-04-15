#! /usr/bin/env python2.7

from itertools import ifilter

class Sequence:
    def __init__( self, previous, current, operation ):
        self._previous, self._current, self._operation = previous, current, operation
        self._threshold = 0
    def __iter__( self ):
        return self
    def __call__(self, threshold ):
        self._threshold = threshold
        return self
    def previous( self ):
        return self._previous
    def current( self ):
        return self._current
    def next( self ):
        result = self._operation( self._previous, self._current )
        if result > self._threshold: raise StopIteration
        self._previous, self._current = self._current, result
        return self._current
    __next__ = next

if __name__ == "__main__":
    plain_fibonacci = Sequence( 0, 1, lambda previous, current: previous + current )
    evens_only_fibonacci = Sequence( 0, 2, lambda previous, current: previous + 4 * current )

    sum = lambda validator, sequence, threshold = 4000000, sumator = lambda x, y: x + y: reduce( sumator, map( lambda iterable: reduce( sumator, ifilter( validator, iterable )), [[ sequence.previous(), sequence.current() ], sequence( threshold ) ] ) )

    print( "plain fibonacci: " + str( sum( lambda x: 0 == x % 2, plain_fibonacci ) ))
    print( "evens only: " + str( sum( lambda x: True, evens_only_fibonacci ) ))

