#include <iostream>

#include "life.hpp"
#include "main.hpp"
#include "gui.hpp"

MyWin::MyWin()
{
	using namespace Gtk;
	set_title("Conway's Game of Life");
	set_border_width(10);
	set_resizable(false);
	
	add(grid);
	grid.set_row_spacing(15);
	grid.set_column_spacing(20);
	grid.set_row_homogeneous(false);
	
	entry.set_text("/home/markjr/game-of-life/samples/puffsup");
	grid.attach(entry,1,1,2,1);
		
	check_wrapx = manage(new CheckButton("Wrap Horizontally"));
	check_wrapx->set_active();
	check_wrapy = manage(new CheckButton("Wrap Vertically"));
	check_wrapy->set_active();
	wrapbox.pack_start(*check_wrapx,true,true);
	wrapbox.pack_start(*check_wrapy,true,true);
	wrapframe.add(wrapbox);
	wrapframe.set_label("Wrapping Options");
	wrapframe.set_shadow_type(SHADOW_ETCHED_IN);
	
	//~ grid.attach(*check_wrapx,1,2,1,1);
	//~ grid.attach(*check_wrapy,2,2,1,1);
	grid.attach(wrapframe,1,2,2,2);
	
	height_label.set_text("Height:");
	height_label.set_halign(ALIGN_START);
	enter_height.set_max_length(3);
	enter_height.set_text("0");
	width_label.set_text("Width:");
	width_label.set_halign(ALIGN_START);
	enter_width.set_max_length(3);
	enter_width.set_text("0");
	grid.attach(height_label,1,3,1,1);
	grid.attach(enter_height,2,3,1,1);
	grid.attach(width_label,1,4,1,1);
	grid.attach(enter_width,2,4,1,1);
	
	auto_check = manage(new CheckButton("Get Size Automatically"));
	auto_check->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_auto_toggled));
	auto_check->set_halign(ALIGN_CENTER);
	
	grid.attach(*auto_check,1,5,2,1);
	
	turn_label.set_text("Number of Turns:");
	turn_label.set_halign(ALIGN_START);
	enter_turns.set_text("100");
	
	grid.attach(turn_label,1,6,1,1);
	grid.attach(enter_turns,2,6,1,1);
	
	printing = manage(new CheckButton("Expanded Printing"));
	filetype = manage(new CheckButton("Coordinate File"));
	
	grid.attach(*printing,1,7,1,1);
	grid.attach(*filetype,2,7,1,1);
	
	go_button = set_but("GO!!!",Stock::APPLY);
	go_button->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_go_clicked));
	go_button->set_can_default(true);
	go_button->grab_default();
	
	go_button->set_vexpand(true);
	go_button->set_valign(ALIGN_FILL);

	quit_bu = set_but("Exit",Stock::QUIT);
	quit_bu->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_quit_clicked));

	quit_bu->set_vexpand(true);
	quit_bu->set_valign(ALIGN_FILL);
	
	grid.attach(*go_button,1,8,1,3);
	grid.attach(*quit_bu,2,8,1,3);	
	
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
