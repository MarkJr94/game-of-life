main: life.cpp life.hpp main.cpp
	g++ -Wall -g main.cpp life.cpp -o main

life: life.cpp life.hpp
	g++ -Wall -g -o life life.cpp

clean: 
	rm life main