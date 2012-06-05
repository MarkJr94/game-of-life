main: life.cpp life.hpp main.cpp main.hpp
	g++ -Wall -g main.cpp life.cpp -o main

gui: life.cpp life.hpp main.cpp main.hpp gui.cpp
	g++ -Wall -g main.cpp life.cpp gui.cpp -o gui

clean: 
	rm main
