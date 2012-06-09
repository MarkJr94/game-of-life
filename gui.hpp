#ifndef GUI_HPP_
#define GUI_HPP_

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>
#include <gtkmm/settings.h>
#include <gtkmm/stock.h>
#include <gtkmm/stockid.h>
#include <gtkmm/image.h>

class MyWin : public Gtk::Window
{
public:
	MyWin();
	virtual ~MyWin();
	
protected:
	/* Run program */
	void on_go_clicked();
	void on_quit_clicked();
	void on_auto_toggled();
	
	/* Top level Grid */
	Gtk::Grid grid;
	
	/* Eventually gonna be file choosing thing */
	Gtk::Entry entry;
	
	/* Box and CheckButtons for wrapping horizontally
	 * an vertically
	 * */
	Gtk::CheckButton *check_wrapx, *check_wrapy;
	
	/* Boxes for entering width, height, an choosing to find it
	 * automatically
	 * */
	
	Gtk::CheckButton *auto_check;
	Gtk::Entry enter_width, enter_height;
	Gtk::Label width_label, height_label;
	
	/* Box for number of turns 
	 * as well as entry */
	 Gtk::Label turn_label;
	 Gtk::Entry enter_turns;
	 
	 /* Box for choice of printing
	  * as well as pictorial or coordinate
	  * file
	  * */
	Gtk::CheckButton *printing, *filetype;
	
	/* Primary buttons, go and quit 
	 * and accompanying box
	 * */
	Gtk::Button *go_button, *quit_bu;
	
	/* Helper function for putting stock labels on buttons */
	Gtk::Button *set_but(const char *lab, const Gtk::StockID& stock);
};

#endif /* GUI_HPP_ */
