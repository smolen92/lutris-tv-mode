CXX = g++

CXXFLAGS = -Wall -g 

lutris-tvmode.out: ./obj/category.o ./obj/game.o ./obj/gui_manager.o ./obj/node_category_menu.o ./obj/node_games_grid.o ./obj/process.o ./obj/renderer.o ./obj/settings.o ./obj/sql.o ./obj/main.o ./obj/node_start_menu.o
	$(CXX) $(CXXFLAGS) -o lutris-tvmode.out ./obj/category.o ./obj/game.o ./obj/gui_manager.o ./obj/node_category_menu.o ./obj/node_games_grid.o ./obj/process.o ./obj/renderer.o ./obj/settings.o ./obj/sql.o ./obj/main.o ./obj/node_start_menu.o -lSDL3 -lSDL3_ttf -lSDL3_image -lsqlite3

./obj/category.o: ./src/category.h ./src/category.cpp
	$(CXX) $(CXXFLAGS) -c ./src/category.cpp -o ./obj/category.o

./obj/game.o: ./src/game.cpp ./src/game.h
	$(CXX) $(CXXFLAGS) -c ./src/game.cpp -o ./obj/game.o

./obj/gui_manager.o: ./src/gui_manager.cpp ./src/gui_manager.h ./obj/sql.o ./obj/node_category_menu.o ./obj/node_games_grid.o ./obj/node_start_menu.o ./obj/process.o
	$(CXX) $(CXXFLAGS) -c ./src/gui_manager.cpp -o ./obj/gui_manager.o

./obj/node_category_menu.o: ./src/node_category_menu.cpp ./src/node_category_menu.h ./obj/category.o ./obj/renderer.o
	$(CXX) $(CXXFLAGS) -c ./src/node_category_menu.cpp -o ./obj/node_category_menu.o

./obj/node_games_grid.o: ./src/node_games_grid.cpp ./src/node_games_grid.h ./obj/renderer.o ./obj/game.o
	$(CXX) $(CXXFLAGS) -c ./src/node_games_grid.cpp -o ./obj/node_games_grid.o

./obj/process.o: ./src/process.h ./src/process.cpp
	$(CXX) $(CXXFLAGS) -c ./src/process.cpp -o ./obj/process.o

./obj/renderer.o: ./src/renderer.cpp ./src/renderer.h ./obj/settings.o ./src/font.h ./src/node.h
	$(CXX) $(CXXFLAGS) -c ./src/renderer.cpp -o ./obj/renderer.o

./obj/settings.o: ./src/settings.h ./src/settings.cpp
	$(CXX) $(CXXFLAGS) -c ./src/settings.cpp -o ./obj/settings.o

./obj/sql.o: ./src/sql.cpp ./src/sql.h ./obj/game.o ./obj/category.o
	$(CXX) $(CXXFLAGS) -c ./src/sql.cpp -o ./obj/sql.o

./obj/main.o: ./src/main.cpp ./obj/gui_manager.o
	$(CXX) $(CXXFLAGS) -c ./src/main.cpp -o ./obj/main.o

./obj/node_start_menu.o: ./src/node_start_menu.h ./src/node_start_menu.cpp ./obj/renderer.o
	$(CXX) $(CXXFLAGS) -c ./src/node_start_menu.cpp -o ./obj/node_start_menu.o

.PHONY: clean doc 

clean:
	rm -f ./obj/*.o

doc:
	doxygen lutris-tv-mode.dox
