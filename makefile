CXX = g++

CXXFLAGS = -Wall -g 

lutris-tvmode.out: ./obj/main.o ./obj/sql.o ./obj/game.o ./obj/settings.o ./obj/process.o ./obj/category.o
	$(CXX) $(CXXFLAGS) -o lutris-tvmode.out ./obj/main.o ./obj/sql.o ./obj/game.o ./obj/settings.o ./obj/process.o ./obj/category.o -lSDL3 -lSDL3_ttf -lSDL3_image -lsqlite3

./obj/main.o: ./src/main.cpp ./obj/gui.o
	$(CXX) $(CXXFLAGS) -c ./src/main.cpp -o ./obj/main.o

./obj/game.o: ./src/game.cpp ./src/game.h
	$(CXX) $(CXXFLAGS) -c ./src/game.cpp -o ./obj/game.o

./obj/sql.o: ./src/sql.cpp ./src/sql.h ./obj/game.o ./obj/category.o
	$(CXX) $(CXXFLAGS) -c ./src/sql.cpp -o ./obj/sql.o

./obj/settings.o: ./src/settings.h ./src/settings.cpp
	$(CXX) $(CXXFLAGS) -c ./src/settings.cpp -o ./obj/settings.o

./obj/process.o: ./src/process.h ./src/process.cpp
	$(CXX) $(CXXFLAGS) -c ./src/process.cpp -o ./obj/process.o

./obj/category.o: ./src/category.h ./src/category.cpp
	$(CXX) $(CXXFLAGS) -c ./src/category.cpp -o ./obj/category.o

./obj/node_renderer.o: ./src/node_renderer.cpp ./src/node_renderer.h
	$(CXX) $(CXXFLAGS) -c ./src/node_renderer.cpp -o ./obj/node_renderer.o

./obj/node_games_grid.o: ./src/node_games_grid.cpp ./src/node_games_grid.h
	$(CXX) $(CXXFLAGS) -c ./src/node_games_grid.cpp -o ./obj/node_games_grid.o

./obj/node_category_menu.o: ./src/node_category_menu.cpp ./src/node_category_menu.h
	$(CXX) $(CXXFLAGS) -c ./src/node_category_menu.cpp -o ./obj/node_category_menu.o

.PHONY: clean doc 

clean:
	rm -f ./obj/*.o

doc:
	doxygen lutris-tv-mode.dox
