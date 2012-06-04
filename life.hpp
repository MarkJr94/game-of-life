#ifndef LIFE_HPP_
#define LIFE_HPP_

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
		
		/* prints board 
		 * board can be tightly packed or expanded
		 * */
		void display(bool expand = false);

		
		/* plays for a given number of turns, 
		 * with given wrapping options
		 * and whether to display tightly packed or expanded
		 * ends simulation if population gets to 0
		 * */
		void play(uint turns = 100, bool wrapx = true, bool wrapy = true, bool expand = false);
		
		/* destructor */
		~Board();
		
	private:
		/* Data members */
		uint wide;
		uint high;
		uint population;
		char **data;
		
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
