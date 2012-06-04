#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstdlib>

#include "life.hpp"

using namespace std;

Board::Board(uint w, uint h)
{
	data = new char*[w];
	
	for (uint i = 0; i < w; i++) {
		data[i] = new char[h];
		for (uint j = 0; j < h; j++) {
			data[i][j] = 0;
		}
	}
	wide = w;
	high = h;
}

Board::Board(const Board& other)
{
	uint w = other.wide;
	uint h = other.high;
	data = new char*[w];
	for (uint i = 0; i < w; i++) {
		data[i] = new char[h];
		for (uint j = 0; j < h; j++) {
			data[i][j] = other.data[i][j];
		}
	}
	wide = w;
	high = h;
	//~ cout << "Board-Clone created, w = " << w << ", h = " << h << "\n";
}

Board::Board(string fname, uint h, uint w)
{
	ifstream file (fname.c_str());
	if (!file.is_open()) {
		cerr << "Error: file " << fname << " could not be opened\n";
		exit(1);
	}
	
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
	
	string line;
	for (int i = 0; file.good(); i++) {
		getline(file,line);
		uint len = line.size();
		//~ cout << line << endl;
		for (uint j = 0; j < len; j++) {
			if (line[j] != '.') {
				data[j+2][i+2] = 1;
			} else {
				data[j+2][i+2] = 0;
			}
		}
	}
	file.close();
}

uint *Board::fAnalyze(std::ifstream& file)
{
	uint *size = new uint[2];
	size[0] = 0; size[1] = 0;
		
	file.seekg(0,ios::end);
	uint mass = file.tellg();
	file.seekg (0, ios::beg);
	
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
	file.seekg (0, ios::beg);
	delete [] buf;
	return size;
}

Board::Board(uint h, uint w, string fname)
{
	if (h == 0 || w == 0) {
		cerr << "Constructing board from coordinate file"
				<< "requires nonzero height and width input\n";
		exit(1);
	}
	
	ifstream file (fname.c_str());
	if (!file.is_open()) {
		cerr << "Error: file " << fname << " could not be opened\n";
		exit(1);
	}
	
	wide = w; high = h;
	data = new char*[wide];
	for (uint i = 0; i < wide; i++) {
		data[i] = new char[high];
		for (uint j = 0; j < high; j++) {
				data[i][j] = 0;
		}
	}
	
	int x,y;
	string line;
	while (!file.eof()) {
		getline(file,line);
		stringstream(line) >> x >> y;
		data[x][y] = 1;
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
	uint w = wide;
	uint h = high;
	
	if (expand) {
		cout << "\n";
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
	
	//~ Wraps around if wrap;
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
	} else {
		if (popCount == 3) return true;
	}
	
	return false;
}

void Board::compute(bool wrapx, bool wrapy)
{
	Board scratch(wide,high);
	for (uint i= 0;i <wide;i++) {
		for(uint j = 0; j <high;j++) {
			if (cellCheck(i,j,wrapx,wrapy))
				scratch.data[i][j] = 1;
		}
	}
	
	for (uint i = 0;i < wide; i++) {
		for (uint j = 0; j < high; j++) {
			this->data[i][j] = scratch.data[i][j];
		}
	}
}

void Board::play(uint turns,bool wrapx, bool wrapy,bool expand)
{
	display(expand);
	for (uint i = 0; i < turns; i++) {
		compute(wrapx,wrapy);
		display(expand);
		usleep(100000);
	}
	cout << "============= SIMULATION COMPLETE ==============\n";
	sleep(1);
}
	

Board::~Board()
{
	delete [] data;
}
