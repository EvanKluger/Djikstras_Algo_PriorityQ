usedijkstras.exe: usedijkstras.o heap.o hash.o graph.o
	g++ -o usedijkstras.exe usedijkstras.o heap.o hash.o graph.o

usedijkstras.o: usedijkstras.cpp
	g++ -c usedijkstras.cpp

graph.o: graph.cpp graph.h
	g++ -c graph.cpp

heap.o: heap.cpp heap.h
	g++ -c heap.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

debug:
	g++ -g -o usedijkstrasDebug.exe usedijkstras.cpp heap.cpp hash.cpp graph.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups