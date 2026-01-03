CXX = g++

CXXFLAGS = -Wall -g 

lutris-tvmode.out: ./obj/main.o ./obj/sql.o ./obj/game.o ./obj/gui.o
	$(CXX) $(CXXFLAGS) -o lutris-tvmode.out ./obj/main.o ./obj/sql.o ./obj/game.o ./obj/gui.o -lSDL3 -lSDL3_ttf -lSDL3_image -lsqlite3

./obj/main.o: ./src/main.cpp ./obj/sql.o ./obj/gui.o
	$(CXX) $(CXXFLAGS) -c ./src/main.cpp -o ./obj/main.o

./obj/game.o: ./src/game.cpp ./src/game.h
	$(CXX) $(CXXFLAGS) -c ./src/game.cpp -o ./obj/game.o

./obj/sql.o: ./src/sql.cpp ./src/sql.h ./obj/game.o
	$(CXX) $(CXXFLAGS) -c ./src/sql.cpp -o ./obj/sql.o

./obj/gui.o: ./src/gui.h ./src/gui.cpp
	$(CXX) $(CXXFLAGS) -c ./src/gui.cpp -o ./obj/gui.o

.PHONY: clean doc 

clean:
	rm -f ./obj/*.o

doc:
	doxygen lutris-tv-mode.dox
