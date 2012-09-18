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
void process(std::string fname, uint width, uint height, uint turns, bool wrapx,
		bool wrapy, bool expand, bool pict)
{
	if (pict) {
		std::lock_guard < std::mutex > lock(life_lock);
		life_is_running = true;
		Board gameBoard;
		gameBoard.readPictoral(fname, width, height);
		gameBoard.playGraphical(turns, wrapx, wrapy);
		life_is_running = false;
	} else {
		std::lock_guard < std::mutex > lock(life_lock);
		life_is_running = true;
		height == 0 ? height = 20 : 0;
		width == 0 ? width = 20 : 0;
		Board gameBoard;
		gameBoard.readCoordinate(fname, width, height);
		gameBoard.playGraphical(turns, wrapx, wrapy);
		life_is_running = false;
	}
}

MyWin::MyWin() :
				mCheckWrapx("Wrap Horizontally"),
				mCheckWrapy("Wrap Vertically"),
				mCheckAuto("Get Size Automatically"),
				mCheckPrinting("Expanded Printing"),
				mEntryFiletype("Coordinate File")
{
//	using namespace Gtk;
	set_title("Conway's Game of Life");
	set_border_width(10);
	set_resizable(false);
	set_default_size(300, 200);

	add(mGrid);
	mGrid.set_row_spacing(15);
	mGrid.set_column_spacing(20);
	mGrid.set_row_homogeneous(false);

	mEntryFile.set_text("samples/puffsup");
	setStock(mFileButton, "Choose File", Gtk::Stock::DIRECTORY);
	mFileButton.signal_clicked().connect(
			sigc::mem_fun(*this, &MyWin::onFileClicked));

	mBoxFile.pack_start(mEntryFile, true, true);
	mBoxFile.pack_end(mFileButton, false, false);

	mGrid.attach(mBoxFile, 1, 1, 2, 1);

	mCheckWrapx.set_active();
	mCheckWrapy.set_active();

	mGrid.attach(mCheckWrapx, 1, 2, 1, 1);
	mGrid.attach(mCheckWrapy, 2, 2, 1, 1);

	mLabelHeight.set_text("Height:");
	mLabelHeight.set_halign(Gtk::ALIGN_START);

	mEntryHeight.set_max_length(3);
	mEntryHeight.set_text("0");

	mLabelWidth.set_text("Width:");
	mLabelWidth.set_halign(Gtk::ALIGN_START);

	mEntryWidth.set_max_length(3);
	mEntryWidth.set_text("0");

	mGrid.attach(mLabelHeight, 1, 4, 1, 1);
	mGrid.attach(mEntryHeight, 2, 4, 1, 1);
	mGrid.attach(mLabelWidth, 1, 5, 1, 1);
	mGrid.attach(mEntryWidth, 2, 5, 1, 1);

	mCheckAuto.signal_clicked().connect(
			sigc::mem_fun(*this, &MyWin::onAutoToggled));
	mCheckAuto.set_halign(Gtk::ALIGN_CENTER);

	mGrid.attach(mCheckAuto, 1, 6, 2, 1);

	mLabelTurn.set_text("Number of Turns:");
	mLabelTurn.set_halign(Gtk::ALIGN_START);
	mEntryTurn.set_text("50");

	mGrid.attach(mLabelTurn, 1, 7, 1, 1);
	mGrid.attach(mEntryTurn, 2, 7, 1, 1);

	mGrid.attach(mCheckPrinting, 1, 8, 1, 1);
	mGrid.attach(mEntryFiletype, 2, 8, 1, 1);

	setStock(mButtonGo, "GO!!!", Gtk::Stock::APPLY);
	mButtonGo.signal_clicked().connect(
			sigc::mem_fun(*this, &MyWin::onGoClicked));
	mButtonGo.set_can_default(true);
	mButtonGo.grab_default();

	mButtonGo.set_vexpand(true);
	mButtonGo.set_valign(Gtk::ALIGN_FILL);

	setStock(mButtonQuit, "Exit", Gtk::Stock::QUIT);
	mButtonQuit.signal_clicked().connect(
			sigc::mem_fun(*this, &MyWin::onQuitClicked));

	mButtonQuit.set_vexpand(true);
	mButtonQuit.set_valign(Gtk::ALIGN_FILL);

	mGrid.attach(mButtonGo, 1, 9, 1, 3);
	mGrid.attach(mButtonQuit, 2, 9, 1, 3);

	show_all_children();
}

void MyWin::onGoClicked()
{
	if (life_is_running)
		return;
	std::string fname = mEntryFile.get_text();
	uint width = Atoi(mEntryWidth.get_text().c_str());
	uint height = Atoi(mEntryHeight.get_text().c_str());
	uint turns = Atoi(mEntryTurn.get_text().c_str());
	bool wrapx = mCheckWrapx.get_active();
	bool wrapy = mCheckWrapy.get_active();
	bool expand = mCheckPrinting.get_active();
	bool pict = !mEntryFiletype.get_active();

	std::thread thread(process, fname, width, height, turns, wrapx, wrapy,
			expand, pict);
	thread.detach();
	/* std::cout << "wrapx  = " << wrapx << " wrapy = " << wrapy << " expand = " 
	 << expand << " pict = " << pict << " turns = " << turns
	 << " width = " << width	<< " height = " << height
	 << " fname = " << fname << std::endl; */
}

void MyWin::onQuitClicked()
{
	hide();
}

void MyWin::onAutoToggled()
{
	bool status = mCheckAuto.get_active();
	mEntryHeight.set_sensitive(!status);
	mEntryHeight.set_text("0");
	mEntryWidth.set_sensitive(!status);
	mEntryWidth.set_text("0");
}

void MyWin::onFileClicked()
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

	switch (result) {
	case (Gtk::RESPONSE_OK): {
		fname = dialog.get_filename();
		std::cout << "File selected: " << fname << std::endl;
		mEntryFile.set_text(fname);
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

void MyWin::setStock(Gtk::Button& button, const std::string& label,
		const Gtk::StockID& stock)
{
	Gtk::Image *img = manage(new Gtk::Image(stock, Gtk::ICON_SIZE_BUTTON));
	button.set_image(*img);
	button.set_label(label);
}

int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
			"org.gtkmm.examples.base");
	Gtk::Settings::get_default()->property_gtk_button_images() = true;

	MyWin window;
	return app->run(window);
}
