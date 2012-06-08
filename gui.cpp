#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/settings.h>
#include <gtkmm/stock.h>
//~ #include <gtkmm/stockitem.h>
#include <gtkmm/stockid.h>
#include <gtkmm/image.h>
//~ #include <gtkmm.h>
#include <iostream>

#include "life.hpp"
#include "main.hpp"

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
	
	/* Eventually gonna be file choosing thing */
	Gtk::Entry entry;
	
	/* Box and CheckButtons for wrapping horizontally
	 * an vertically
	 * */
	Gtk::Box *wrapbox;
	Gtk::CheckButton *check_wrapx, *check_wrapy;
	Gtk::Box *vbox;
	
	/* Boxes for entering width, height, an choosing to find it
	 * automatically
	 * */
	Gtk::Box *size_hbox;
	Gtk::Box *size_ent_box;
	Gtk::Box *size_auto_box;
	
	Gtk::CheckButton *auto_check;
	Gtk::Entry enter_width, enter_height;
	Gtk::Label width_label, height_label;
	
	/* Box for number of turns 
	 * as well as entry */
	 Gtk::Box *turn_box;
	 Gtk::Label turn_label;
	 Gtk::Entry enter_turns;
	 
	 /* Box for choice of printing
	  * as well as pictorial or coordinate
	  * file
	  * */
	Gtk::Box *choose_box;
	Gtk::CheckButton *printing, *filetype;
	
	/* Primary buttons, go and quit 
	 * and accompanying box
	 * */
	Gtk::Box *hbox;
	Gtk::Button *go_button, *quit_bu;
	
	/* Helper function for putting stock labels on buttons */
	Gtk::Button *set_but(const char *lab, const Gtk::StockID& stock);
};

MyWin::MyWin()
{
	using namespace Gtk;
	set_title("Conway's Game of Life");
	set_border_width(10);
	set_default_size(200,200);
	vbox = manage(new Box(Gtk::ORIENTATION_VERTICAL));
	
	entry.set_text("/home/markjr/game-of-life/samples/puffsup");
	vbox->pack_start(entry);
	
	check_wrapx = manage(new CheckButton("Wrap Horizontally"));
	check_wrapx->set_active();
	check_wrapy = manage(new CheckButton("Wrap Vertically"));
	check_wrapy->set_active();
	wrapbox = manage(new Box());
	wrapbox->pack_start(*check_wrapx);
	wrapbox->pack_start(*check_wrapy);
	vbox->pack_start(*wrapbox);
	
	height_label.set_text("Height:");
	enter_height.set_max_length(3);
	enter_height.set_text("0");
	width_label.set_text("Width:");
	enter_width.set_max_length(3);
	enter_width.set_text("0");
	size_ent_box = manage(new Box(Gtk::ORIENTATION_VERTICAL));
	size_ent_box->pack_start(height_label);
	size_ent_box->pack_start(enter_height);
	size_ent_box->pack_start(width_label);
	size_ent_box->pack_start(enter_width);
	
	auto_check = manage(new CheckButton("Get Size Automatically"));
	auto_check->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_auto_toggled));
	size_auto_box = manage(new Box());
	size_auto_box->pack_start(*auto_check);
	
	size_hbox = manage(new Box());
	size_hbox->pack_start(*size_ent_box);
	size_hbox->pack_start(*size_auto_box);
	vbox->pack_start(*size_hbox);
	
	turn_label.set_text("Number of Turns: ");
	enter_turns.set_text("100");
	
	turn_box = manage(new Box());
	turn_box->pack_start(turn_label);
	turn_box->pack_end(enter_turns);
	vbox->pack_start(*turn_box);
	
	printing = manage(new CheckButton("Expanded Printing"));
	filetype = manage(new CheckButton("Coordinate File"));
	
	choose_box = manage(new Box());
	choose_box->pack_start(*printing);
	choose_box->pack_start(*filetype);
	vbox->pack_start(*choose_box);
	
	go_button = set_but("GO!!!",Stock::APPLY);
	go_button->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_go_clicked));
	go_button->set_can_default(true);
	go_button->grab_default();

	quit_bu = set_but("Exit",Stock::QUIT);
	quit_bu->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_quit_clicked));
	
	hbox = manage(new Box());		
	hbox->pack_start(*go_button);
	hbox->pack_start(*quit_bu);
				
	vbox->pack_start(*hbox);
	add(*vbox);
	
	show_all_children();
};

void MyWin::on_go_clicked() 
{
	std::string fname = entry.get_text();
	uint width = Atoi(enter_width.get_text().c_str());
	uint height = Atoi(enter_height.get_text().c_str());
	uint turns = Atoi(enter_turns.get_text().c_str());
	bool wrapx = check_wrapx->get_active();
	bool wrapy = check_wrapy->get_active();
	bool expand = printing->get_active();
	bool pict = !filetype->get_active();
	process(fname,width,height,turns,wrapx,wrapy,expand,pict);
	/* std::cout << "wrapx  = " << wrapx << " wrapy = " << wrapy << " expand = " 
			<< expand << " pict = " << pict << " turns = " << turns 
			<< " width = " << width	<< " height = " << height 
			<< " fname = " << fname << std::endl; */
}

void MyWin::on_quit_clicked()
{
	hide();
}

void MyWin::on_auto_toggled()
{
	bool status = auto_check->get_active();
	enter_height.set_sensitive(!status);
	enter_height.set_text("0");
	enter_width.set_sensitive(!status);
	enter_width.set_text("0");
}

MyWin::~MyWin()
{
}

Gtk::Button *MyWin::set_but(const char *lab, const Gtk::StockID& stock)
{
	using namespace Gtk;
	Button *button = manage(new Button(lab));
	Image *img = manage(new Gtk::Image(stock,Gtk::ICON_SIZE_BUTTON));
	button->set_image(*img);
	button->set_label(lab);
	
	return button;
}

int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app =
	Gtk::Application::create(argc, argv,
		"org.gtkmm.examples.base");
	Gtk::Settings::get_default()->property_gtk_button_images() = true;

	MyWin window;

	return app->run(window);
}
