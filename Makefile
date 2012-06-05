main: life.cpp life.hpp main.cpp main.hpp cmd.cpp
	g++ -Wall -g cmd.cpp main.cpp life.cpp -o main

gui: life.cpp life.hpp main.cpp main.hpp gui.cpp
	g++ -Wall -g main.cpp life.cpp gui.cpp -o gui `pkg-config --cflags --libs gtkmm-3.0`

clean: 
	rm main gui
