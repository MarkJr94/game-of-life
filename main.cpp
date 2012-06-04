#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstdlib>

#include "life.hpp"

int Atoi(const char *str)
{
	int retval;
	
	if ((retval = atoi(str)) <= 0) {
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

/* Main routine */
int main(int argc, char **argv)
{
	using namespace std;
	
	bool wrapx = true ,wrapy = true, expand = false, pict = true;
	uint width = 0, height = 0;
	uint turns = 100;
	string fname;
	int c;
	
	opterr = 1;
	
	while (( c= getopt(argc,argv,"w:h:t:xycef:")) != -1) {
		switch (c) {
			/* Options with numerical arguments */
			case 'w':
				width = Atoi(optarg);
				break;
			case 'h':
				height = Atoi(optarg);
				break;
			case 't':
				turns = Atoi(optarg);
				break;
			/* options that are simple bool flags */
			case 'x':
				wrapx = false;
				break;
			case 'y':
				wrapy = false;
				break;
			case 'c':
				pict = false;
				break;
			case 'e':
				expand = true;
				break;
			/* Option for filename (C style string)  */
			case 'f':
				//~ strcpy(fname,optarg);
				fname = optarg;
				break;
			default:
				cout << "WWTTFF BBRROO??????\n";
		}
	}
	
	cout << "wrapx  = " << wrapx << " wrapy = " << wrapy << " expand = " 
			<< expand << " pict = " << pict << " turns = " << turns 
			<< " width = " << width	<< " height = " << height 
			<< " fname = " << fname << endl;
	
	for (int index = optind; index < argc; index++)
         cout <<"Non-option argument " << argv[index] << endl;
	/* For testing purposes */
	//~ testing(); 
	
	/* Actual command processing/execution */
	if (pict) {
		Board gameBoard (fname,width,height);
		gameBoard.play(turns, wrapx, wrapy, expand);
	} else {
		height == 0 ? height = 20 : 0;
		width == 0 ? width = 20 : 0;
		Board gameBoard (width,height,fname);
		gameBoard.play(turns, wrapx, wrapy, expand);
	}
	
	return 0;
}
