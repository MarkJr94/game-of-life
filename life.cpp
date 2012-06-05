#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>

#include "life.hpp"

Board::Board(uint w, uint h)
{
	using namespace std;

	data = new char*[w];
	
	/* Initialize zeroed out array */
	for (uint i = 0; i < w; i++) {
		data[i] = new char[h];
		for (uint j = 0; j < h; j++) {
			data[i][j] = 0;
		}
	}
	
	wide = w;
	high = h;
	population = 0;
	cout << "Clean board created\n";
}

Board::Board(const Board& other)
{
	using namespace std;

	uint w = other.wide;
	uint h = other.high;
	
	/* Copy in data from array */
	data = new char*[w];
	for (uint i = 0; i < w; i++) {
		data[i] = new char[h];
		for (uint j = 0; j < h; j++) {
			data[i][j] = other.data[i][j];
		}
	}
	wide = w;
	high = h;
	population = 0;
	cout << "Board-Clone created, w = " << w << ", h = " << h << "\n";
}

Board::Board(std::string fname, uint h, uint w)
{
	using namespace std;

	/* Open file, check for error */
	ifstream file (fname.c_str());
	if (!file.is_open()) {
		cerr << "Error: file " << fname << " could not be opened\n";
		exit(1);
	}
	
	/* If values given for h and w, initialize board with them */
	if (h != 0 && w != 0) {
		wide = w; high = h;
		data = new char*[wide];
		for (uint i = 0; i < wide; i++) {
			data[i] = new char[high];
			for (uint j = 0; j < high; j++) {
					data[i][j] = 0;
			}
		}
	} else {
		/* Else get values from file */
		uint * size = fAnalyze(file);
		wide = size[0]; high = size[1];
		data = new char*[wide];
		for (uint i = 0; i < wide; i++) {
			data[i] = new char[high];
			for (uint j = 0; j < high; j++) {
					data[i][j] = 0;
			}
		}		
	}
	population = 0;
	
	/* Read in seed board status from pictorial file */
	string line;
	for (int i = 0; file.good(); i++) {
		getline(file,line);
		uint len = line.size();
		//~ cout << line << endl;
		for (uint j = 0; j < len; j++) {
			if (line[j] != '.') {
				data[(j+2)%wide][(i+2)%high] = 1;
				++population;
			} else {
				data[(j+2)%wide][(i+2)%high] = 0;
			}
		}
	}
	file.close();
}

uint *Board::fAnalyze(std::ifstream& file)
{

	uint *size = new uint[2];
	size[0] = 0; size[1] = 0;
		
	file.seekg(0, std::ios::end);
	uint mass = file.tellg();
	file.seekg (0, std::ios::beg);
	
	char *buf = new char[mass];
	file.read(buf,mass);
	
	uint i = 0;
	while (buf[i++] != '\n') size[0]++;
	size[1]++;
	
	while (i++ < mass) {
		if (buf[i] == '\n') size[1]++;
	}
	
	size[0] += 4; 
	size[1] += 4;
	file.seekg (0, std::ios::beg);
	delete [] buf;
	return size;
}

Board::Board(uint h, uint w, std::string fname)
{
	using namespace std;

	if (h == 0 || w == 0) {
		cerr << "Constructing board from coordinate file"
				<< "requires nonzero height and width input\n";
		exit(1);
	}
	
	/* Open file, check for errors */
	ifstream file (fname.c_str());
	if (!file.is_open()) {
		cerr << "Error: file " << fname << " could not be opened\n";
		exit(1);
	}
	
	/* Initialize zeroed out array */
	wide = w; high = h;
	data = new char*[wide];
	for (uint i = 0; i < wide; i++) {
		data[i] = new char[high];
		for (uint j = 0; j < high; j++) {
				data[i][j] = 0;
		}
	}
	population = 0;
	
	int x,y;
	string line;
	while (!file.eof()) {
		getline(file,line);
		stringstream(line) >> x >> y;
		data[x%wide][y%high] = 1;
		++population;
	}
	file.close();
}

uint Board::getWide()
{
	return wide;
}

uint Board::getHigh()
{
	return high;
}

void Board::display(bool expand)
{
	using namespace std;

	uint w = wide;
	uint h = high;
	
	/* Expanded printing */
	if (expand) {
		cout << "\n";
		cout << "Population: " << population << endl;
		for (uint j = 0; j < h; j++) {
			for (uint i = 0; i < w; i++) {
				if (data[i][j]) {
					cout << "@ ";
				} else {
					cout << "- ";
				}
			}
			cout << "\n";
		}
		return;
	}
	
	/* Normal */
	cout << "\n";
	for (uint j = 0; j < h; j++) {
		for (uint i = 0; i < w; i++) {
			if (data[i][j]) {
				cout << "O";
			} else {
				cout << ".";
			}
		}
		cout << "\n";
	}	
}

inline int Board::addFix(int val, int dim)
{
	if (val >= dim) return val - dim;
	return val;
}

inline int Board::subFix(int val, int dim)
{
	if (val < 0) return val + dim;
	return val;
}

bool Board::cellCheck(int x, int y, bool wrapx, bool wrapy)
{
	int minx,miny,plusx,plusy;
	minx = x-1; miny = y-1; plusx = x +1; plusy = y+1;
	int w = this->wide;
	int h = this->high;
	uint popCount = 0;
	
	/* Wraps around if wrap; */
	if (wrapx) {
		minx = subFix(minx,w);
		plusx = addFix(plusx,w);
	}
	if (wrapy) {
		miny = subFix(miny,h);
		plusy = addFix(plusy,h);
	}
	
	if (minx >= 0 && plusy < h) {
		if (this->data[minx][plusy]) ++popCount; // down left
	}
	if (minx >= 0) {
		if (this->data[minx][y]) ++popCount; // left
	} 
	if (miny >= 0 && minx >= 0) {
		if (this->data[minx][miny]) ++popCount; // up left
	}
	if (miny >= 0) { 
		if (this->data[x][miny]) ++popCount; // up
	}
	if (plusx < w && miny >= 0) {
		if (this->data[plusx][miny]) ++popCount; // right up
	}
	if (plusx < w) {
		if (this->data[plusx][y]) ++popCount; // right
	}
	if (plusx < w && plusy < h ) {
		if (this->data[plusx][plusy]) ++popCount; // down right
	}
	if (plusy < h) {
		if (this->data[x][plusy]) ++popCount; // down
	}
	
	if (this->data[x][y]) {
		if (1 < popCount && popCount < 4) return true;
		--population;
		return false;
	} else {
		if (popCount == 3) {
			++population;
			return true;
		}
	}
	
	return false;
}

void Board::compute(bool wrapx, bool wrapy, Board& scratch)
{
	/* Computes results for next turn in scratch board */
	for (uint i= 0;i <wide;i++) {
		for(uint j = 0; j <high;j++) {
			if (cellCheck(i,j,wrapx,wrapy))
				scratch.data[i][j] = 1;
		}
	}
	
	/* Copies over values, zero out scratch */
	for (uint i = 0;i < wide; i++) {
		for (uint j = 0; j < high; j++) {
			this->data[i][j] = scratch.data[i][j];
		}
	}
	scratch.zero();
}

void Board::play(uint turns,bool wrapx, bool wrapy,bool expand)
{
	using namespace std;

	/* diplsay initial board, create scratch */
	display(expand);
	Board scratch (wide,high);
	
	/* Run game */
	for (uint i = 0; i < turns; i++) {
		if (population == 0) {
			cout << "========= SIMULATION OVER: EVERYONE DIED =======\n";
			return;
		}
		compute(wrapx,wrapy,scratch);
		display(expand);
		usleep(100000);
	}
	cout << "============= SIMULATION COMPLETE ==============\n";
	sleep(1);
}

void Board::zero()
{
	for (uint i = 0; i < wide; i++) {
		for (uint j = 0; j < high; j++) {
			data[i][j] = 0;
		}
	}
}


Board::~Board()
{
	delete [] data;
}
