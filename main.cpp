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
	Board coord (20,20,"samples/weekender.txt");
	coord.play(100,true,true,true);
	
	Board pict ("samples/puffsup");
	pict.play(100,true,true);
	
	return 1;
}

/* Processes arguments, actually begins simulation
 * for ease of use with gui plugin
 * */
void process(std::string fname, uint width, uint height, uint turns,
		bool wrapx, bool wrapy, bool expand, bool pict)
{
	if (pict) {
		Board gameBoard (fname,width,height);
		gameBoard.play(turns, wrapx, wrapy);
	} else {
		height == 0 ? height = 20 : 0;
		width == 0 ? width = 20 : 0;
		Board gameBoard (width,height,fname);
		gameBoard.play(turns, wrapx, wrapy);
	}
}
