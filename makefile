CXX = g++

CXXFLAGS = -Wall -g -lSDL3 -lsqlite3

lutris-tvmode.out: ./obj/main.o 
	$(CXX) $(CXXFLAGS) -o lutris-tvmode.out ./obj/main.o 

./obj/main.o: ./src/main.cpp
	$(CXX) $(CXXFLAGS) -c ./src/main.cpp -o ./obj/main.o

.PHONY: clean 

clean:
	rm -f ./obj/*.o
