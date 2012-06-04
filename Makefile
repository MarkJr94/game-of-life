main: life.cpp life.hpp main.cpp
	g++ -Wall -g main.cpp life.cpp -o main

clean: 
	rm main