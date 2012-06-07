//~ #include <gtkmm/window.h>
//~ #include <gtkmm/button.h>
//~ #include <gtkmm/box.h>
//~ #include <gtkmm/stock.h>
//~ #include <gtkmm/stockitem.h>
//~ #include <gtkmm/stockid.h>
//~ #include <gtkmm/image.h>
#include <gtkmm.h>
#include <iostream>
//~ #include <fstream>
//~ #include <string>
//~ #include <sstream>
//~ #include <unistd.h>
//~ #include <cstdlib>

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
	
	/* Child thing */
	Gtk::Button *go_button;
	Gtk::Button *quit_bu;
	Gtk::Box hbox;
	
private:
	Gtk::Button *set_but(const char *lab, const Gtk::StockID& stock);
};

Gtk::Button *MyWin::set_but(const char *lab, const Gtk::StockID& stock)
{
	using namespace Gtk;
	Button *button = manage(new Button(lab));
	Image *img = manage(new Gtk::Image(stock,Gtk::ICON_SIZE_BUTTON));
	button->set_image(*img);
	button->set_label(lab);
	
	return button;
}

MyWin::MyWin()
{
	using namespace Gtk;
	set_title("Conway's Game of Life");
	set_border_width(10);
	set_default_size(200,200);
	
	go_button = set_but("GO!!!",Stock::APPLY);
	go_button->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_go_clicked));

	quit_bu = set_but("Exit",Stock::QUIT);
	quit_bu->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_quit_clicked));
			
	hbox.pack_start(*go_button);
	hbox.pack_start(*quit_bu);
	add(hbox);
	
	
	show_all_children();
};

void MyWin::on_go_clicked() {
	process("samples/weekender.txt",20,20,20,true,true,true,false);
}

void MyWin::on_quit_clicked() {
	hide();
}

MyWin::~MyWin()
{
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
