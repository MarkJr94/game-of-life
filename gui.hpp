#ifndef GUI_HPP_
#define GUI_HPP_

#include <gtkmm.h>

class MyWin : public Gtk::Window
{
public:
	MyWin();
	virtual ~MyWin();
	
private:
	/* Run program */
	void onGoClicked();
	void onQuitClicked();
	void onAutoToggled();
	void onFileClicked();
	
	/* Top level Grid */
	Gtk::Grid mGrid;
	
	/* Eventually gonna be file choosing thing */
	Gtk::Entry mEntryFile;
	Gtk::Button mFileButton;
	Gtk::Box mBoxFile;
	
	/* CheckButtons for wrapping horizontally
	 * an vertically
	 * */
	Gtk::CheckButton mCheckWrapx, mCheckWrapy;
	
	/* Boxes for entering width, height, an choosing to find it
	 * automatically
	 * */
	
	Gtk::CheckButton mCheckAuto;
	Gtk::Entry mEntryWidth, mEntryHeight;
	Gtk::Label mLabelWidth, mLabelHeight;
	
	/* Box for number of turns 
	 * as well as entry */
	 Gtk::Label mLabelTurn;
	 Gtk::Entry mEntryTurn;
	 
	 /* Box for choice of printing
	  * as well as pictorial or coordinate
	  * file
	  * */
	Gtk::CheckButton mCheckPrinting, mEntryFiletype;
	
	/* Primary buttons, go and quit 
	 * and accompanying box
	 * */
	Gtk::Button mButtonGo, mButtonQuit;
	
	/* Helper function for putting stock labels on buttons */
	void setStock(Gtk::Button& button, const std::string& lab, const Gtk::StockID& stock);

	typedef Gtk::Window super;
};

void process(std::string fname, uint width, uint height, uint turns,
		bool wrapx, bool wrapy, bool expand, bool pict);
#endif /* GUI_HPP_ */
