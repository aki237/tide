#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Double_Window.H>
#include <FL/x.H>
#include <FL/Fl_Button.H>
#include <fstream>
#include "Const.hpp"
#include "TextView.hpp"
#include "Buffer.hpp"

void msgout(std::string &a) {
  std::cout << a << std::endl;
}

std::string readScript(std::string filename) {
  std::string line, text;
  std::ifstream f(filename);
  if (!f.is_open()) {
    std::cout << "Unable to open the file" << std::endl;
    return "";
  }
  while ( std::getline(f, line) ) {
    text += line + '\n';
  }
  f.close();
  return text;
}

int main(int argc, char **argv) {
  if ( argc < 2 ) {
    return 2;
  }
  Fl_Double_Window* win = (Fl_Double_Window *) 0;
  TextView* canvas = (TextView *) 0;
  Buffer m(argv[1]);
  win = new Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT); 
  win->begin();
  canvas  = new TextView(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m);
  win->end();
    
  canvas->box(FL_FLAT_BOX);

  win->resizable(canvas);
  win->label("Tide Editor");
  win->show();

  return Fl::run();
}
