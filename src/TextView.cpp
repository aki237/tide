#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <cmath>
#include <iostream>
#include <string>
#include <regex>
#include "TextView.hpp"
#include "Option.hpp"
#include "Buffer.hpp"
#include "Node.hpp"
#include "Color.hpp"
#include "Const.hpp"
#include "Tokenizer.hpp"

// # Private Common Functions

/*
  ### `void setupSol(sol::state*)`
  *Inaccessible Common Function*
  
  Returns nothing. Takes in a pointer to the class `sol::state`.
  This function is used to setup the current sol::state engine with the user defined classes.
  Right now only the `Buffer` class and some of it's public functions are defined in the `sol::state`.
*/
void setupSol(sol::state* lua) {
  lua->new_usertype<Buffer>( "Buffer",
                            sol::constructors<Buffer(), Buffer(std::string)>(),
                            "insertChar", &Buffer::insertChar,
                            "insertText", &Buffer::insertText,
                            "insertStdString", &Buffer::insertStdString,
                            "bkspcChar", &Buffer::bkspcChar,
                            "deleteChar", &Buffer::deleteChar,
                            "cursorUp", &Buffer::cursorUp,
                            "cursorDown", &Buffer::cursorDown,
                            "cursorLeft", &Buffer::cursorLeft,
                            "cursorRight", &Buffer::cursorRight,
                            "cursorBeginLine", &Buffer::cursorBeginLine,
                            "cursorEndLine", &Buffer::cursorEndLine,
                            "cursorBeginBuffer", &Buffer::cursorBeginBuffer,
                            "cursorEndBuffer", &Buffer::cursorEndBuffer,
                            "getCursorPosX", &Buffer::getCursorPosXActual,
                            "getCursorPosY", &Buffer::getCursorPosY,
                            "getTop", &Buffer::getTop,
                            "getCurrentLine", &Buffer::getCurrentLine,
                            "selectionStart", &Buffer::selectionStart,
                            "selectionCancel", &Buffer::selectionCancel,
                            "getIsSelected", &Buffer::getIsSelected,
                            "getSelectionStartX", &Buffer::getSelectionStartXActual,
                            "getSelectionStartY", &Buffer::getSelectionStartYActual
                            );
}

// # Private Class Functions

/*
  ### `cairo_surface_t* TextView::set_surface(int, int)`
  *Private Class Function*
  
  Returns `cairo_surface_t` struct (C struct). Takes in width and 
  height of the surface to be made. This function creates a cairo 
  surface for the given FLTK parameters (`fl_display`,`fl_window`,`fl_visual->visual`).
  and sets if to the FLTK window and display context. Currently it 
  is generated from the Xlib cairo binding, meaning that it will work on linux only
  (unices running X Server). Bindings from the GDI (windows) and Quartz (macOS) 
  bindings can be made too.
*/
cairo_surface_t*  TextView::set_surface(int wo, int ho) {
  return cairo_xlib_surface_create(fl_display, fl_window, fl_visual->visual, wo, ho);
}

/*
  ### `void TextView::callbackDraw()`
  *Private Class Function*

  Returns nothing. Takes in nothing. This member functions is where all the drawing takes place.
*/
void TextView::callbackDraw() {
  Color::SetCairoColor(cr,"#272822");
  cairo_paint(cr);
  cairo_text_extents_t ex;
  cairo_font_options_t* fot = cairo_font_options_create();
  cairo_font_options_set_antialias(fot,CAIRO_ANTIALIAS_SUBPIXEL);
  cairo_font_options_set_subpixel_order(fot,CAIRO_SUBPIXEL_ORDER_RGB);
  cairo_font_options_set_hint_style(fot,CAIRO_HINT_STYLE_SLIGHT);
  cairo_font_options_set_hint_metrics(fot,CAIRO_HINT_METRICS_ON);
    
  cairo_set_font_size(cr, font_size);
  cairo_set_font_options(cr, fot);
  cairo_select_font_face(cr, "Inconsolata", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_text_extents(cr, "O", &ex);
  std::vector<std::string> lines = m.getLines(h(),1.5*font_size);

  int cx = m.getCursorPosX(), cy = m.getCursorPosY();
  int ln = m.getTop();
  int y = 0;
  Color::SetCairoColor(cr,"#56594A");
  cairo_rectangle(cr, 0, y * font_size, GUTTER_SIZE, h());
  cairo_fill(cr);
  Color::SetCairoColor(cr,"#747764");
  cairo_rectangle(cr,
                  GUTTER_SIZE + 5.0,
                  ((cy-ln) * (font_size + (font_size/2.0))) + (5.0*font_size/16.0),
                  w(),
                  ((1.7*font_size)));
  cairo_fill(cr);
  int sx = m.getSelectionStartX(h(),1.5*font_size), sy = m.getSelectionStartY(h(),1.5*font_size);
  if ( m.getIsSelected() ) {
    int iter = sy + 1, end = cy;
    if ( sy > cy ) {
      iter = cy + 1;
      end = sy;
    }
    Color::SetCairoColor(cr,"#ADB292");
    while (iter < end) {
      cairo_rectangle(cr,
                      GUTTER_SIZE + 5,
                      (iter-ln) * (font_size + (font_size/2.0))  + (5.0*font_size/16.0) ,
                      w() - GUTTER_SIZE,
                      (1.7*font_size));
      iter++;
    }
    if ( sy < cy ) {
      cairo_rectangle(cr,
                      GUTTER_SIZE + 5 +(sx*ex.x_advance),
                      (sy-ln) * (font_size + (font_size/2.0))  + (5.0*font_size/16.0) ,
                      w() - GUTTER_SIZE,
                      (1.7*font_size));
      cairo_rectangle(cr,
                      GUTTER_SIZE + 5,
                      (cy-ln) * (font_size + (font_size/2.0))  + (5.0*font_size/16.0) ,
                      cx*ex.x_advance,
                      (1.7*font_size));
    }
    if ( sy > cy ) {
      cairo_rectangle(cr,
                      GUTTER_SIZE + 5 +(cx*ex.x_advance),
                      (cy-ln) * (font_size + (font_size/2.0))  + (5.0*font_size/16.0) ,
                      w() - GUTTER_SIZE,
                      (1.7*font_size));
      cairo_rectangle(cr,
                      GUTTER_SIZE + 5,
                      (sy-ln) * (font_size + (font_size/2.0))  + (5.0*font_size/16.0) ,
                      sx*ex.x_advance,
                      (1.7*font_size));
    }
    if ( sy == cy ) {
      int iter = sx, end = cx;
      if ( sx > cx ) {
        iter = cx;
        end = sx;
      }
      cairo_rectangle(cr,
                      GUTTER_SIZE + 5 +(iter*ex.x_advance),
                      (cy-ln) * (font_size + (font_size/2.0))  + (5.0*font_size/16.0) ,
                      (end - iter)*ex.x_advance,
                      (1.7*font_size));
    }
  }
  cairo_fill(cr);
  std::string tabs("••••••••");
  for(auto const& a: lines) {
    cairo_select_font_face(cr, "Inconsolata", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    if (ln + y == cy) {
      cairo_select_font_face(cr, "Inconsolata", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    }
    cairo_move_to(cr, 10.0, (y + 1) * (font_size + (font_size/2.0)));
    Color::SetCairoColor(cr, "#FFFFFF");
    cairo_show_text(cr, std::to_string(ln + y + 1).c_str());	

    std::string an = std::regex_replace(a, std::regex("\\t"), tabs);
    cairo_select_font_face(cr, "Inconsolata", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_move_to(cr, GUTTER_SIZE + 5, (y + 1) * (font_size + (font_size/2.0)));
    Tokenizer tz(an," ({[]})*+-=/;\n");
    tz.inQuotes(true);
    Token t = setColor(tz.Next());
    std::string tmpt = "";
    while (t.notNull) {
      std::string tmp = t.token;
      tmpt += tmp;
      Color::SetCairoColor(cr, t.color);
      cairo_show_text(cr, tmp.c_str());
      cairo_text_extents(cr, tmpt.c_str(), &ex);
      cairo_move_to(cr, GUTTER_SIZE + 5 + ex.x_advance, (y + 1) * (font_size + (font_size/2.0)));
      tmp = t.delim;
      tmpt += tmp;
      Color::SetCairoColor(cr, "#FFFFFFFF");
      cairo_show_text(cr, tmp.c_str());
      cairo_text_extents(cr, tmpt.c_str(), &ex);
      cairo_move_to(cr, GUTTER_SIZE + 5 + ex.x_advance, (y + 1) * (font_size + (font_size/2.0)));
      t = setColor(tz.Next());
    }
    y++;
  }
  cairo_text_extents(cr, "O", &ex);
  cairo_rectangle(cr,
                  GUTTER_SIZE + 5 + (cx*ex.x_advance),
                  (cy-ln) * (font_size + (font_size/2.0)) + (5.0*font_size/16.0),
                  0.5,
                  ((1.7*font_size)));
  Color::SetCairoColor(cr, "#000000");
  cairo_rectangle(cr,
                  0,
                  h() - (1.7 * font_size),
                  w(),
                  ((1.7*font_size)));
  cairo_fill(cr);
  Color::SetCairoColor(cr, "#FFFFFF");
  cairo_set_font_size(cr, 0.8*font_size);
  cairo_move_to(cr,GUTTER_SIZE + 5, h() - (0.5 * font_size));
  cairo_show_text(cr, m.getFilename().c_str());
}


/*
  ### `void TextView::draw()`
  *Private Class Function*

  Returns nothing. Takes in nothing. Initializes the drawing area.
*/
void TextView::draw() {
  fl_push_no_clip();
  fl_push_clip(x(), y(), w(), h());
  fl_color(FL_WHITE);
  fl_rectf(x(), y(), w(), h());
  fl_color(FL_WHITE);
  fl_rect(x(), y(), w(), h());
  surface = set_surface(w(), h());
  cr = cairo_create(surface);
  callbackDraw();
  cairo_fill(cr);
  cairo_restore(cr);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  fl_pop_clip();
  fl_pop_clip();
}

/*
  ### `bool TextView::setShortcutKeyState(int, int)`
  *Private Class Function*
  
  Returns boolean, `true` if one of the shortcut keys is pressed. Takes in the codes for event and key pressed.
  Sets what states are the shortcut keys (Control, Alt, Shift) are at (pressed or released).
*/
bool TextView::setShortcutKeyState(int event, int key) {
  bool isShortcut = false;
  if (event == FL_SHORTCUT &&(key == FL_Control_L || key == FL_Control_R)) {
    ctrl_pressed = true;
    isShortcut = true;
  }
  if (event == FL_KEYUP && (key == FL_Control_L || key == FL_Control_R)) {
    ctrl_pressed = false;
    isShortcut = true;
  }
  if (event == FL_SHORTCUT &&(key == FL_Alt_L || key == FL_Alt_R)) {
    alt_pressed = true;
    isShortcut = true;
  }
  if (event == FL_KEYUP && (key == FL_Alt_L || key == FL_Alt_R)) {
    alt_pressed = false;
    isShortcut = true;
  }
  if (event == FL_SHORTCUT &&(key == FL_Shift_L || key == FL_Shift_R)) {
    shift_pressed = true;
    isShortcut = true;
  }
  if (event == FL_KEYUP && (key == FL_Shift_L || key == FL_Shift_R)) {
    shift_pressed = false;
    isShortcut = true;
  }
  if (event == FL_SHORTCUT &&(key == FL_Meta_L || key == FL_Meta_R)) {
    super_pressed = true;
    isShortcut = true;
  }
  if (event == FL_KEYUP && (key == FL_Meta_L || key == FL_Meta_R)) {
    super_pressed = false;
    isShortcut = true;
  }
  return isShortcut;
}

// # Private Virtual Class Functions

/*
  ### `int TextView::handle(int)`
  *Private Virtual Class Function*
  
  Returns an integer denoting whether the event passed in can be passed to other 
  elements or widgets in the window. Takes in an integer denoting an even type. (like FL_MOVE, FL_ENTER, etc.,)
*/
int TextView::handle(int event) {
  if (event != FL_KEYDOWN && event != FL_KEYUP && event != FL_SHORTCUT) {
    if ( event == FL_FOCUS ) {
      ctrl_pressed = false;
      alt_pressed = false;
      super_pressed = false;
    }
    return 0;
  }
    
  int key = Fl::event_key();
  const char* text = Fl::event_text();
  auto handleSelection = [this](bool sp) {
    if (sp) {
      m.selectionStart();
    } else {
      m.selectionCancel();
    }
  };
  if (setShortcutKeyState(event, key)) {
    return 1;
  }
  if (ctrl_pressed && event == FL_SHORTCUT) {
    Option<std::string> optsel,optpaste;
    switch (key) {
    case '=':
      font_size += 1.0;
      break;
    case '-':
      font_size -= 1.0;
      break;
    case 'c':
      std::cout << "Copy pressed" << std::endl;
      optsel = m.getSelectionText();
      if ( optsel.IsSome() ) {
        m_clipboard.putText(optsel.Get());
      }
      m.selectionCancel();
      break;
    case 'v':
      std::cout << "Paste pressed" << std::endl;
      optpaste = m_clipboard.getText();
      if ( optpaste.IsSome() ) {
        m.insertText(optpaste.Get().c_str());
      }
      break;
    case 's':
      m.saveFile();
      break;
    case 'q':
      exit(0);
    case 'a': // Emacs Baby.....
      handleSelection(shift_pressed);
      m.cursorBeginLine();
      break;
    case 'e': // Emacs Baby.....
      handleSelection(shift_pressed);
      m.cursorEndLine();
      break;
    case FL_Home:
      handleSelection(shift_pressed);
      m.cursorBeginBuffer();
      break;
    case FL_End:
      handleSelection(shift_pressed);
      m.cursorEndBuffer();
      break;
    case FL_BackSpace:
      if ( shift_pressed ) {
        m.deleteLine();
      }
      break;
    }
    draw();
    return 1;
  } else {
    if (!ctrl_pressed && !alt_pressed && !super_pressed && (event == FL_SHORTCUT)) {
      switch (key) {
      case FL_Tab:
        m.insertChar('\t');
        m.selectionCancel();
        break;
      case FL_Enter:
        m.insertChar('\n');
        m.selectionCancel();
        break;
      case FL_BackSpace:
        m.bkspcChar();
        m.selectionCancel();
        break;
      case FL_Delete:
        m.deleteChar();
        break;
      case FL_Left:
        handleSelection(shift_pressed);
        m.cursorLeft();
        break;
      case FL_Down:
        handleSelection(shift_pressed);
        m.cursorDown();
        break;
      case FL_Up:
        handleSelection(shift_pressed);
        m.cursorUp();
        break;
      case FL_Right:
        handleSelection(shift_pressed);
        m.cursorRight();
        break;
      case FL_Home:
        handleSelection(shift_pressed);
        m.cursorBeginLine();
        break;
      case FL_End:
        handleSelection(shift_pressed);
        m.cursorEndLine();
        break;
      default:
        if (key >= 32 && key <= 127) {
          std::string txt = std::string(text);
          m.insertText(txt.c_str());
          plugin["key"] = txt;
          auto result = plugin.do_file("plugin/keypress.lua");
          if(!result.valid()) {
            sol::error err = result;
            std::string what = err.what();
            std::cout << "call failed, sol::error::what() is " << what << std::endl;
          }
          m.selectionCancel();
        }
      }
      draw();
    }
  }
  return 1;
}

// # Public Class Functions

/*
  ### `void TextView::SetBuffer(Buffer&);`
  *Public Class Function*
  
  Returns nothing. Takes in a Reference to the Buffer Object.
  Sets the current Buffer to the the one passed.
*/
void TextView::SetBuffer(Buffer& p_m) {
  m = p_m;
}


/*
  ### `*void* TextView::TextView(int, int, int, int)`
  *Public Constructor*
  
  Returns nothing. Takes in the x, y co-ordinate positions and width
  and height of the box. As this is a inherited class, it calls the 
  parent constructor of the class `Fl_Box` (*FLTK Box Widget*) and 
  then initializes other members.
*/
TextView::TextView(int x, int y, int w, int h, Buffer& p_m) : Fl_Box(x,y,w,h) {
  align(FL_ALIGN_INSIDE|FL_ALIGN_LEFT|FL_ALIGN_TOP);
  surface  = NULL;
  cr       = NULL;
  font_size = 16.0;
  SetBuffer(p_m);
  plugin.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::coroutine, sol::lib::io);
  setupSol(&plugin);
  plugin["m"] = &m;
}
