all: life.cpp life.hpp main.cpp main.hpp cmd.cpp gui.cpp gui.hpp
	g++ -Wall -g cmd.cpp main.cpp life.cpp -o life-cmd `allegro-config --libs`
	g++ -Wall -g gui.cpp main.cpp life.cpp -o life-gui `pkg-config --cflags --libs gtkmm-3.0` `allegro-config --libs`


cmd: life.cpp life.hpp main.cpp main.hpp cmd.cpp
	g++ -Wall -g cmd.cpp main.cpp life.cpp -o life-cmd `allegro-config --libs`

gui: life.cpp life.hpp main.cpp main.hpp gui.cpp gui.hpp
	g++ -Wall -g gui.cpp main.cpp life.cpp -o life-gui `pkg-config --cflags --libs gtkmm-3.0` `allegro-config --libs`

clean: 
	rm life-cmd life-gui
