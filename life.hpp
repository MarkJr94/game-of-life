#ifndef LIFE_HPP_
#define LIFE_HPP_

#include <utility>
#include <vector>

// SFML Includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

typedef unsigned int uint;

class Board {
public:

	/* Constructor */
	Board(uint width = 0, uint height = 0);

	/* construct from pictoral file */
	void readPictoral(std::string filename, uint width = 0, uint height = 0);

	/* Construct board from coordinate file */
	void readCoordinate(std::string filename,  uint width, uint height);

	/* Wide getter */
	uint width()
	{
		return mWidth;
	}

	/* High getter */
	uint height()
	{
		return mHeight;
	}

	/* Draws board, not implemented, you must overload
	 * it on the basis of whatever drawing thing you're using
	 *
	 * */
	void drawGraphical();

	/* prints board
	 * board can be tightly packed or expanded
	 * */
	void drawText(bool);

	/* For play using graphical drawing
	 * */
	void playGraphical(uint turns, bool wrapx, bool wrapy);

	/* plays for a given number of turns,
	 * with given wrapping options
	 * and whether to display tightly packed or expanded
	 * ends simulation if population gets to 0
	 * */
	void playText(uint turns, bool wrapx, bool wrapy, bool expand);

	/* destructor */
	~Board();

private:
	/* Data members */
	uint mWidth;
	uint mHeight;
	uint mPopulation;
	std::vector<std::vector<bool> > mData;
	sf::RenderWindow mWindow;

	/* Private functions for use with wrapping */
	inline int addFix(int val, int dim)
	{
		return (val >= dim ? val - dim : val);
	}

	inline int subFix(int val, int dim)
	{
		return (val < 0 ? val + dim : val);
	}

	/* For use with construction from pictoral file
	 * gets necessary width and height
	 * */
	std::pair<int, int> fAnalyze(std::ifstream& file);

	/* Checks if an individual cell will be alive
	 * next round */
	bool cellCheck(int x, int y, bool wrapx, bool wrapy);

	/* computes board for next round */
	void compute(bool wrapx, bool wrapy, Board& scratch);

	/* Zeroes out a board */
	void zero();
};

#endif /* LIFE_HPP_ */
