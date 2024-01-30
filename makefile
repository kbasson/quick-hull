quickHull: quickHull.o helpers.o
	clang -Wall -pedantic -std=c99 quickHull.o helpers.o -o quickHull

quickHull.o: quickHull.c quickHull.h
	clang -Wall -pedantic -std=c99 -c quickHull.c -o quickHull.o

helpers.o: helpers.c quickHull.h
	clang -Wall -pedantic -std=c99 -c helpers.c -o helpers.o

clean:
	rm *.o
