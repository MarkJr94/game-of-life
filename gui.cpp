#include <gtkmm/window.h>
#include <gtkmm/button.h>
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
	
	/* Child thing */
	Gtk::Button go_button;
};

MyWin::MyWin()
{
	go_button.add_pixlabel("info.xpm","Go!");
	
	set_title("Conway's Game of Life");
	set_border_width(10);
	
	go_button.signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_go_clicked));
			
	add(go_button);
	
	show_all_children();
};

void MyWin::on_go_clicked() {
	process("samples/weekender.txt",20,20,20,true,true,true,false);
}

MyWin::~MyWin()
{
}

int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app =
	Gtk::Application::create(argc, argv,
		"org.gtkmm.examples.base");

	//~ Gtk::Window window;
	MyWin window;

	return app->run(window);
}
