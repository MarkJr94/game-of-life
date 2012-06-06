all: life.cpp life.hpp main.cpp main.hpp cmd.cpp gui.cpp
	g++ -Wall -g cmd.cpp main.cpp life.cpp -o life-cmd
	g++ -Wall -g gui.cpp main.cpp life.cpp -o life-gui `pkg-config --cflags --libs gtkmm-3.0`


main: life.cpp life.hpp main.cpp main.hpp cmd.cpp
	g++ -Wall -g cmd.cpp main.cpp life.cpp -o life-cmd

gui: life.cpp life.hpp main.cpp main.hpp gui.cpp
	g++ -Wall -g gui.cpp main.cpp life.cpp -o life-gui `pkg-config --cflags --libs gtkmm-3.0`

clean: 
	rm main gui
