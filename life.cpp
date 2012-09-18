#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "life.hpp"

Board::Board(uint w, uint h) :
				mWidth(w),
				mHeight(h),
				mPopulation(0),
				mData(w, std::vector<bool>(h, false))
{

}

Board::~Board()
{
}

void Board::readPictoral(std::string filename, uint width, uint height)
{
	/* Open file, check for error */
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: file " << filename << " could not be opened\n";
		exit(1);
	}

	/* If values given for h and w, initialize board with them */
	if (height != 0 && width != 0) {
		mWidth = width;
		mHeight = height;
	} else {
		/* Else get values from file */
		std::pair<int, int> size = fAnalyze(file);
		mWidth = size.first;
		mHeight = size.second;
	}
	mData.resize(mWidth, std::vector<bool>(mHeight, false));
	mPopulation = 0;

	/* Read in seed board status from pictorial file */
	std::string line;
	for (int i = 0; file.good(); i++) {
		getline(file, line);
		uint len = line.size();
		for (uint j = 0; j < len; j++) {
			if (line[j] != '.') {
				mData[(j + 2) % mWidth][(i + 2) % mHeight] = true;
				++mPopulation;
			} else {
				mData[(j + 2) % mWidth][(i + 2) % mHeight] = false;
			}
		}
	}
}

std::pair<int, int> Board::fAnalyze(std::ifstream& file)
{
	std::pair<int, int> size;
	size.first = 0;
	size.second = 0;

	file.seekg(0, std::ios::end);
	uint mass = file.tellg();
	file.seekg(0, std::ios::beg);

	char *buf = new char[mass];
	file.read(buf, mass);

	uint i = 0;
	while (buf[i++] != '\n')
		size.first++;
	size.second++;

	while (i++ < mass) {
		if (buf[i] == '\n')
			size.second++;
	}

	size.first += 4;
	size.second += 4;
	file.seekg(0, std::ios::beg);
	delete[] buf;
	return size;
}

void Board::readCoordinate(std::string filename, uint width, uint height)
{
	if (width == 0 || height == 0) {
		std::cerr << "Constructing board from coordinate file"
				<< "requires nonzero height and width input\n";
		exit(1);
	}

	/* Open file, check for errors */
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: file " << filename << " could not be opened\n";
		exit(1);
	}

	/* Initialize zeroed out array */
	mWidth = width;
	mHeight = height;
	mData.resize(mWidth, std::vector<bool>(mHeight, false));
	mPopulation = 0;

	int x, y;
	std::string line;
	while (!file.eof()) {
		getline(file, line);
		std::stringstream(line) >> x >> y;
		mData[x % mWidth][y % mHeight] = true;
		++mPopulation;
	}
}

bool Board::cellCheck(int x, int y, bool wrapx, bool wrapy)
{
	int minx, miny, plusx, plusy;
	minx = x - 1;
	miny = y - 1;
	plusx = x + 1;
	plusy = y + 1;
	int w = this->mWidth;
	int h = this->mHeight;
	uint popCount = 0;

	/* Wraps around if wrap; */
	if (wrapx) {
		minx = subFix(minx, w);
		plusx = addFix(plusx, w);
	}
	if (wrapy) {
		miny = subFix(miny, h);
		plusy = addFix(plusy, h);
	}

	if (minx >= 0 && plusy < h) {
		if (this->mData[minx][plusy])
			++popCount; // down left
	}
	if (minx >= 0) {
		if (this->mData[minx][y])
			++popCount; // left
	}
	if (miny >= 0 && minx >= 0) {
		if (this->mData[minx][miny])
			++popCount; // up left
	}
	if (miny >= 0) {
		if (this->mData[x][miny])
			++popCount; // up
	}
	if (plusx < w && miny >= 0) {
		if (this->mData[plusx][miny])
			++popCount; // right up
	}
	if (plusx < w) {
		if (this->mData[plusx][y])
			++popCount; // right
	}
	if (plusx < w && plusy < h) {
		if (this->mData[plusx][plusy])
			++popCount; // down right
	}
	if (plusy < h) {
		if (this->mData[x][plusy])
			++popCount; // down
	}

	if (this->mData[x][y]) {
		if (1 < popCount && popCount < 4)
			return true;
		--mPopulation;
		return false;
	} else {
		if (popCount == 3) {
			++mPopulation;
			return true;
		}
	}

	return false;
}

void Board::compute(bool wrapx, bool wrapy, Board& scratch)
{
	/* Computes results for next turn in scratch board */
	for (uint i = 0; i < mWidth; i++) {
		for (uint j = 0; j < mHeight; j++) {
			if (cellCheck(i, j, wrapx, wrapy))
				scratch.mData[i][j] = 1;
		}
	}

	/* Copies over values, zero out scratch */
	mData = scratch.mData;
	scratch.zero();
}

void Board::drawText(bool expand)
{
	using std::cout;
	using std::endl;

	uint w = mWidth;
	uint h = mHeight;

	/* Expanded printing */
	if (expand) {
		cout << "\n";
		cout << "Population: " << mPopulation << endl;
		for (uint j = 0; j < h; j++) {
			for (uint i = 0; i < w; i++) {
				if (mData[i][j]) {
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
			if (mData[i][j]) {
				cout << "O";
			} else {
				cout << ".";
			}
		}
		cout << endl;
	}
}

void Board::playText(uint turns, bool wrapx, bool wrapy, bool expand)
{
	using std::cout;

	/* display initial board, create scratch */
	drawText(expand);
	Board scratch(mWidth, mHeight);

	/* Run game */
	for (uint i = 0; i < turns; i++) {
		if (mPopulation == 0) {
			cout << "========= SIMULATION OVER: EVERYONE DIED =======\n";
			return;
		}
		compute(wrapx, wrapy, scratch);
		drawText(expand);
		usleep(100000);
	}
	cout << "============= SIMULATION COMPLETE ==============\n";
	sleep(1);
}

void Board::drawGraphical()
{
	using sf::Color;
	using sf::Shape;

	const Color GREEN(0, 0xff, 0);
	const Color WHITE(0xff, 0xff, 0xff);
	mWindow.Clear(WHITE);

	/* Go through board, filling in cells according
	 * to cell status
	 * */
	uint h = mHeight;
	uint w = mWidth;

	Shape cell;
	int xcord = 0;
	for (uint i = 0; i < w; i++) {
		int ycord = 0;
		for (uint j = 0; j < h; j++) {
			if (mData[i][j]) {
				cell = Shape::Rectangle(xcord, ycord, xcord + 10, ycord + 19,
						GREEN);
				mWindow.Draw(cell);
			} else {
				cell = Shape::Rectangle(xcord, ycord, xcord + 10, ycord + 19,
						WHITE, -1.f, GREEN);
				mWindow.Draw(cell);
			}
			ycord += 10;
		}
		xcord += 10;
	}

	mWindow.Display();
}

void Board::playGraphical(uint turns, bool wrapx, bool wrapy)
{
	/* Initialize allegro, initialize buffer bitmap
	 * */
	const int WIDTH = mWidth * 10, HEIGHT = mHeight * 10;

	mWindow.Create(sf::VideoMode(WIDTH, HEIGHT, 32), "Game of Life");

	/* display initial board, create scratch */
	drawGraphical();
	Board scratch(mWidth, mHeight);

	/* Run game */
	for (uint i = 0; i < turns && mWindow.IsOpened(); i++) {
		if (mPopulation == 0) {
			obituary();
		}
		compute(wrapx, wrapy, scratch);
		drawGraphical();
		checkClosing();
		usleep(100000);
	}

	sleep(1);
	mWindow.Close();
	return;
}

void Board::zero()
{
	for (uint i = 0; i < mWidth; i++) {
		for (uint j = 0; j < mHeight; j++) {
			mData[i][j] = 0;
		}
	}
}

void Board::obituary() {
	sf::String message ("Everyone is dead.");
	message.SetPosition(0,mWindow.GetHeight()/2);
	message.SetColor(sf::Color(0xff,0,0));
	mWindow.Draw(message);
	mWindow.Display();
	sleep(2);
	mWindow.Close();
}

void Board::checkClosing() {
	using sf::Event;

	Event e;

	while (mWindow.GetEvent(e)) {
		switch (e.Type) {
		case Event::EventType::Closed:
			mWindow.Close();
			break;
		default:
			break;
		}
	}
}
