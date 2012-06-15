#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>

#include "life.hpp"
#include "main.hpp"
#include "gui.hpp"

std::mutex life_lock;
bool life_is_running;

/* Processes arguments, actually begins simulation
 * for ease of use with gui plugin
 * */
void process(std::string fname, uint width, uint height, uint turns,
		bool wrapx, bool wrapy, bool expand, bool pict)
{
	if (pict) {
		std::lock_guard<std::mutex> lock(life_lock);
		life_is_running = true;
		Board gameBoard (fname,width,height);
		gameBoard.play(turns, wrapx, wrapy);
		life_is_running = false;
	} else {
		std::lock_guard<std::mutex> lock(life_lock);
		life_is_running = true;
		height == 0 ? height = 20 : 0;
		width == 0 ? width = 20 : 0;
		Board gameBoard (width,height,fname);
		gameBoard.play(turns, wrapx, wrapy);
		life_is_running = false;
	}
}

MyWin::MyWin()
{
	using namespace Gtk;
	set_title("Conway's Game of Life");
	set_border_width(10);
	set_resizable(false);
	set_default_size(300,200);
	
	add(grid);
	grid.set_row_spacing(15);
	grid.set_column_spacing(20);
	grid.set_row_homogeneous(false);
	
	entry.set_text("/home/markjr/game-of-life/samples/puffsup");
	file_button = set_but("Choose File",Stock::DIRECTORY);
	file_button->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_file_chosen));
	
	ent_box.pack_start(entry,true,true);
	ent_box.pack_end(*file_button,false,false);
	
	grid.attach(ent_box,1,1,2,1);
	
	check_wrapx = manage(new CheckButton("Wrap Horizontally"));
	check_wrapx->set_active();
	check_wrapy = manage(new CheckButton("Wrap Vertically"));
	check_wrapy->set_active();
	
	grid.attach(*check_wrapx,1,2,1,1);
	grid.attach(*check_wrapy,2,2,1,1);
	
	height_label.set_text("Height:");
	height_label.set_halign(ALIGN_START);
	enter_height.set_max_length(3);
	enter_height.set_text("0");
	width_label.set_text("Width:");
	width_label.set_halign(ALIGN_START);
	enter_width.set_max_length(3);
	enter_width.set_text("0");
	grid.attach(height_label,1,4,1,1);
	grid.attach(enter_height,2,4,1,1);
	grid.attach(width_label,1,5,1,1);
	grid.attach(enter_width,2,5,1,1);
	
	auto_check = manage(new CheckButton("Get Size Automatically"));
	auto_check->signal_clicked().connect(sigc::mem_fun(*this,
			&MyWin::on_auto_toggled));
	auto_check->set_halign(ALIGN_CENTER);
	
	grid.attach(*auto_check,1,6,2,1);
	
	turn_label.set_text("Number of Turns:");
	turn_label.set_halign(ALIGN_START);
	enter_turns.set_text("50");
	
	grid.attach(turn_label,1,7,1,1);
	grid.attach(enter_turns,2,7,1,1);
	
	printing = manage(new CheckButton("Expanded Printing"));
	filetype = manage(new CheckButton("Coordinate File"));
	
	grid.attach(*printing,1,8,1,1);
	grid.attach(*filetype,2,8,1,1);
	
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
	
	grid.attach(*go_button,1,9,1,3);
	grid.attach(*quit_bu,2,9,1,3);	
	
	show_all_children();
};

void MyWin::on_go_clicked() 
{
	if (life_is_running)return;
	std::string fname = entry.get_text();
	uint width = Atoi(enter_width.get_text().c_str());
	uint height = Atoi(enter_height.get_text().c_str());
	uint turns = Atoi(enter_turns.get_text().c_str());
	bool wrapx = check_wrapx->get_active();
	bool wrapy = check_wrapy->get_active();
	bool expand = printing->get_active();
	bool pict = !filetype->get_active();
	
	std::thread thread(process,fname,width,height,turns,wrapx,wrapy,expand,pict);
	thread.detach();
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

void MyWin::on_file_chosen()
{
	Gtk::FileChooserDialog dialog("Please choose a file",
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	
	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	
	Glib::RefPtr<Gtk::FileFilter> filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog.add_filter(filter_text);
	
	Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog.add_filter(filter_any);
	
	int result = dialog.run();
	std::string fname;
	
	switch(result) {
		case (Gtk::RESPONSE_OK): {
			fname = dialog.get_filename();
			std::cout << "File selected: " << fname << std::endl;
			entry.set_text(fname);
			break;
		}
		case (Gtk::RESPONSE_CANCEL): {
			std::cout << "Cancel clicked\n";
			break;
		}
		default: {
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	}
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
