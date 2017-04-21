#ifndef TEXTVIEW_HPP
#define TEXTVIEW_HPP
//  `TextView`

/*
  Containing all necessary structs classes and functions for implementing a custom textview in a FLTK Window.
*/

// # Header Declarations
#include <iostream>
#include <FL/Fl_Box.H>
#include <cairo.h>
#include <cairo-xlib.h>
#include "Buffer.hpp"
#include "Clipboard.hpp"
#include "sol.hpp"

/*
  ### `typedef struct {...} font_dimension;`
  *Accessible Type*
  
  This type contains the dimensions of a glyph for a selected font and size.
*/
typedef struct {
    double w,h;
} font_dimension;

/*
  ### `class TextView : public Fl_Box{...};`
  *Accessible Class*

  Class TextView contains functions and members to implement a custom text view in a FLTK Window.
*/
class TextView : public Fl_Box {
 private:
    void draw(); 
    cairo_t* cr;
    cairo_surface_t*  surface;
    cairo_surface_t*  set_surface(int wo, int ho);
    void callbackDraw();
    virtual int handle(int);
    bool setShortcutKeyState(int, int);
    bool focused;
    bool ctrl_pressed, shift_pressed, alt_pressed, super_pressed;
    double font_size ;
    font_dimension fdim;
    Buffer m;
    Clipboard m_clipboard;
    sol::state plugin;
 public:
    void SetBuffer(Buffer&);
    TextView(int x, int y, int w, int h, Buffer&);
};
#endif
