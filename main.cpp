#include <iostream>
#include <string>
#include <cstdlib>

#include "life.hpp"
#include "main.hpp"

int Atoi(const char *str)
{
	int retval;
	
	if ((retval = atoi(str)) < 0) {
		std::cerr << "Error: Argument value must be positive integer\n";
		exit(1);
	}
	
	return retval;
}

/* Test routine
 * constructs one board from a coordinate file
 * runs it with with all wrapping on, expanded display
 * constructs another board from pictoral file
 * runs it with vertical wrapping off, condensed display
 * */
int testing()
{
	Board coord;
	coord.readCoordinate("samples/weekender.txt",20,20);
	coord.playText(100,true,true,true);
	
	Board pict;
	pict.readPictoral("samples/puffsup");
	pict.playGraphical(100,true,true);
	
	return 1;
}
