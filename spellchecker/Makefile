#Kevin Zink
#mg004
speller: main.o list.o word.o tree.o
	g++ -Wall main.o list.o word.o tree.o -o speller
list.o: list.cpp list.h word.h
	g++ -Wall -c list.cpp
word.o: word.cpp word.h
	g++ -Wall -c word.cpp
tree.o: tree.cpp tree.h list.h
	g++ -Wall -c tree.cpp
main.o: main.cpp list.h word.h
	g++ -Wall -c main.cpp
clean:
	rm -f *.o speller
