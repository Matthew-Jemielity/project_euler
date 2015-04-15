from sys import argv

class PrimeFactor(object):
    def __init__(self, integer): self.factor, self.integer = 2, integer
    def __iter__(self): return self
    def next(self): return self.__next__()
    def __next__(self):
        if self.integer < self.factor: raise StopIteration
	# I actually don't have to check for factor's primarity
	# because I know it will always be in this algorithm
	# C version feels stupid now
        while 0 != (self.integer % self.factor): self.factor += 1
        result = self.factor
        self.__init__(self.integer / self.factor)
        return result


if __name__ == "__main__":
    print(str(max(PrimeFactor(int(argv[1])))))

