#ifndef LIFE_HPP_
#define LIFE_HPP_

#include <vector>

typedef unsigned int uint;

class Board {
	public:
		
		/* Constructor */
		Board(uint w, uint h);
		
		/* Copy constructor */
		Board(const Board& other);
		
		/* construct from pictoral file */
		Board(std::string fname, uint h = 0, uint w = 0);
		
		/* Construct board from coordinate file */
		Board(uint h, uint w, std::string fname);			
		
		/* Wide getter */
		uint getWide();
		
		/* High getter */
		uint getHigh();
		
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
		uint wide;
		uint high;
		uint population;
		//~ char **data;
		std::vector<std::vector<bool> > data;
		
		/* Private functions for use with wrapping */
		inline int addFix(int val, int dim);
		inline int subFix(int val, int dim);
		
		/* For use with construction from pictoral file
		 * gets necessary width and height
		 * */
		uint *fAnalyze(std::ifstream& file);
				
		/* Checks if an individual cell will be alive
		 * next round */
		bool cellCheck(int x, int y, bool wrapx, bool wrapy);
		
		/* computes board for next round */
		void compute(bool wrapx,bool wrapy, Board& scratch);
		
		/* Zeroes out a board */
		void zero();
};

#endif /* LIFE_HPP_ */
