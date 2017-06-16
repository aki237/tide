#include "Buffer.hpp"
#include "Error.hpp"
#include <iostream>
#include <fstream>
#include <string>

// # Public Static Class Functions

/*
  ### `int Buffer::count_tabs(std::string,int);`
  *Public Static Class Function*

  Returns an `int`. Takes in a `std::string` and an `int`.
  This function counts the number of tab character(\t) in the given string `a`
  till the index `x`
*/
int Buffer::count_tabs(std::string a, int x) {
  int count = 0, i = 0;
  while (i < x) {
    if ( a[i] == '\t' )
      count++;
    i++;
  }
  return count;
}

// # Private Class Functions

/*
  ### `Error Buffer::readFile();`
  *Private Class Function*

  Returns a Error object. Takes in nothing.
  Reads the file (`filename`). Returns an Error object if any error.
*/
Error Buffer::readFile() {
  std::string line;
  Text* lineiter = text;
  try {
    std::ifstream f(filename.Get());
    if (!f.is_open()) {
      Error e("Unable to open the file.");
      return e;
    }
    while ( !std::getline(f, line).eof() ) {
      lineiter->data = line;
      lineiter = lineiter->NewAfter();
    }
    f.close();
    Error e;
    return e;
  } catch ( const std::invalid_argument& ex ) {
    Error e(ex.what());
    return e;
  }
  Error e;
  return e;
}

// # Public Class Functions

/*
  ### `*void* Buffer::Buffer();`
  *Public Constructor*

  Returns nothing. Takes in nothing. Initializes a `Buffer` instance.
*/
Buffer::Buffer() {
  text = new Text();
  m_cursor = TCursor{0,0,0,false,Selection{0,0,new Text()},text};
}

/*
  ### `*void* Buffer::Buffer(std::string);`
  *Public Constructor*

  Returns nothing. Takes in `filename` (`std::string`).
  Initializes a `Buffer` instance with the text read from the `filename` passed.
*/
Buffer::Buffer(std::string p_filename) : filename(p_filename) {
  text = new Text();
  Error e = readFile();
  if (e.IsError()) {
    std::cout << e << std::endl;
  }
  m_cursor = TCursor{0,0,0,false,Selection{0,0, new Text()},text};
}

/*
  ### `void Buffer::insertChar(char);`
  *Public Class Function*

  Returns nothing. Takes in a single `char`.
  This is used to insert the character into the text.
*/
void Buffer::insertChar(char c) {
  if (c == '\n') {
    std::string linedata = m_cursor.current->data.substr(m_cursor.x,-1);
    m_cursor.current->data = m_cursor.current->data.substr(0,m_cursor.x);
    m_cursor.x = 0;
	
    Text* temp = m_cursor.current->NewAfter();
    temp->data = linedata;
    cursorDown();
    return;
  }
  m_cursor.current->data.insert(m_cursor.x,1,c);
  m_cursor.x++;
}

/*
  ### `void Buffer::insertText(const char*);`
  *Public Class Function*

  Returns nothing. Takes in a `char*`.
  This is used to insert the string into the text.
*/
void Buffer::insertText(const char* text) {
  std::string tmp(text);
  for ( uint i = 0; i < tmp.length(); i++ ) {
    insertChar(tmp[i]);
  }
}

/*
  ### `void Buffer::insertStdString(std::string);`
  *Public Class Function*

  Returns nothing. Takes in a `std::string`.
  This is used to insert the `std::string` into the text.
*/
void Buffer::insertStdString(std::string tc) {
  for ( uint i = 0; i < tc.length(); i++ ) {
    if (tc[i] == '\0')
      break;
    insertChar(tc[i]);
  }
}

/*
  ### `void Buffer::bkspcChar();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function deletes a character to the left.
*/
void Buffer::bkspcChar() {
  if ( m_cursor.x == 0 ) {
    if ( m_cursor.y != 0 ) {
      m_cursor.x = m_cursor.current->prev->data.length();
      m_cursor.y--;
      if (m_cursor.current->data != "")
        m_cursor.current->prev->data += m_cursor.current->data;
      Text* temp = m_cursor.current;
      m_cursor.current = m_cursor.current->Delete();
      delete temp;
    }
    return;
  }
  m_cursor.x--;
  m_cursor.current->data = m_cursor.current->data.erase(m_cursor.x,1);
}

/*
  ### `void Buffer::deleteChar();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function deletes a character to the right.
*/
void Buffer::deleteChar() {
  cursorRight();
  bkspcChar();
}

/*
  ### `void Buffer::deleteLine();`
  *Public Class Function`

  Returns nothing. Takes in nothing.
  This function deletes the current line.
*/
void Buffer::deleteLine() {
  m_cursor.x = 0;
  Text* temp = m_cursor.current;
  m_cursor.current = m_cursor.current->Delete();
  m_cursor.current = m_cursor.current->next;
  delete temp;
}

/*
  ### `void Buffer::cursorUp();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor up a line.
*/
void Buffer::cursorUp() {
  if (m_cursor.y != 0 || m_cursor.current->prev != nullptr) {
    m_cursor.y--;
    int tempx = m_cursor.x;
    m_cursor.current = m_cursor.current->prev;
    if ( int(m_cursor.current->data.length()) < tempx )
      m_cursor.x = m_cursor.current->data.length();
  }
}

/*
  ### `void Buffer::cursorDown();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor down a line.
*/
void Buffer::cursorDown() {
  if (m_cursor.current->next != nullptr) {
    m_cursor.y++;
    int tempx = m_cursor.x;
    m_cursor.current = m_cursor.current->next;
    if ( int(m_cursor.current->data.length()) < tempx )
      m_cursor.x = m_cursor.current->data.length();
  }
}

/*
  ### `void Buffer::cursorLeft();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor left a character.
*/
void Buffer::cursorLeft() {
  if ( m_cursor.x == 0 ) {
    if ( (m_cursor.y != 0) && (m_cursor.current->prev != nullptr) ) {
      cursorUp();
      m_cursor.x = m_cursor.current->data.length();
    }
  } else {
    m_cursor.x--;
  }
}

/*
  ### `void Buffer::cursorRight();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor right a character.
*/
void Buffer::cursorRight() {
  if (m_cursor.x == int(m_cursor.current->data.length())) {
    if ( m_cursor.current->next != nullptr ) {
      m_cursor.x = 0;
      cursorDown();
    }
  } else {
    m_cursor.x++;
  }
}

/*
  ### `void Buffer::cursorBeginLine();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor to the beginning of the line.
*/
void Buffer::cursorBeginLine() {
  m_cursor.x = 0;
}

/*
  ### `void Buffer::cursorEndLine();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor to the end of the line.
*/
void Buffer::cursorEndLine() {
  m_cursor.x = m_cursor.current->data.length();
}

/*
  ### `void Buffer::cursorBeginBuffer();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor to the beginning of the buffer.
*/
void Buffer::cursorBeginBuffer() {
  while (m_cursor.current->prev != nullptr) {
    cursorUp();
  }
  m_cursor.top = 0;
  cursorBeginLine();
}

/*
  ### `void Buffer::cursorEndBuffer();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function moves the cursor to the end of the buffer.
*/
void Buffer::cursorEndBuffer() {
  while (m_cursor.current->next != nullptr) {
    cursorDown();
  }
  m_cursor.top = m_cursor.y - sl_lines + 1;
  cursorEndLine();
}

/*
  ### `std::vector<std::string> Buffer::getLines(double,double);`
  *Public Class Function*

  Returns a `std::vector<std::string>`. Takes in window height `double` and font height `double`.
  This function returns only the lines needed to be printed on the viewport.
*/
std::vector<std::string> Buffer::getLines(double p_window_height,double p_font_height) {
  setTop(p_window_height,p_font_height);
  Text* init = m_cursor.current;
  std::vector<std::string> ret;
  int c = 0;
  while (c < m_cursor.y - m_cursor.top) {
    if (init->prev == nullptr) {
      break;
    }
    init = init->prev;
    c++;
  }
  c = 0;
  while (c < sl_lines) {
    ret.push_back(init->data);
    if (init->next == nullptr) {
      break;
    }
    init = init->next;
    c++;
  }
  return ret;
}

/*
  ### `int Buffer::getCursorPosX();`
  *Public Class Function*

  Returns an `int`. Takes in nothing.
  This function returns the cursor's x co-ordinate in the viewport.
*/
int Buffer::getCursorPosX() {
  return m_cursor.x + 7*count_tabs(m_cursor.current->data, m_cursor.x);
}

/*
  ### `int Buffer::getCursorPosX();`
  *Public Class Function*

  Returns an `int`. Takes in nothing.
  This function returns the cursor's x co-ordinate in the text (not counting multi width charaters).
*/
int Buffer::getCursorPosXActual() {
  return m_cursor.x;
}

/*
  ### `int Buffer::getCursorPosY();`
  *Public Class Function*

  Returns an `int`. Takes in nothing.
  This function returns the cursor's y co-ordinate in the viewport.
*/
int Buffer::getCursorPosY() {
  return m_cursor.y;
}

/*
  ### `int Buffer::getTop();`
  *Public Class Function*

  Returns an `int`. Takes in nothing.
  This function returns the line number of the top most line in the viewport.
*/
int Buffer::getTop() {
  return m_cursor.top;
}


/*
  ### `std::string Buffer::getCurrentLine();`
  *Public Class Function*

  Returns an `std::string`. Takes in nothing.
  This function returns current line contents as a std::string.
*/
std::string Buffer::getCurrentLine() {
  return m_cursor.current->data;
}

/*
  ### `void Buffer::setTop();`
  *Public Class Function*

  Returns nothing. Takes the window size and font height.
  Used to set the top most co-ordinate of the viewport.
*/
void Buffer::setTop(double p_window_height,double p_font_height) {
  sl_lines = int(p_window_height/p_font_height) + 1;
  if (sl_lines-(m_cursor.y-m_cursor.top) <= 4){
    m_cursor.top += 1;
  } else if (m_cursor.y-m_cursor.top < 1){
    m_cursor.top -= 1;
    if (m_cursor.top < 0) {
      m_cursor.top = 0;
    }
  }
  if ( sl_lines < m_cursor.y - m_cursor.top ) {
    m_cursor.top = m_cursor.y - sl_lines + 2;
  }
}

/*
  ### `void Buffer::selectionStart();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function is called when a selection keystroke is triggered. (Shift + <Arrows>)
*/
void Buffer::selectionStart(){
  if ( !m_cursor.is_selection ) {
    m_cursor.is_selection = true;
    m_cursor.sel.start_x = m_cursor.x;
    m_cursor.sel.start_y = m_cursor.y;
    m_cursor.sel.start_line = m_cursor.current;
  }
}

/*
  ### `void Buffer::selectionCancel();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  This function is called to cancel a selection.
*/
void Buffer::selectionCancel(){
  m_cursor.is_selection = false;
}

/*
  ### `bool Buffer::getIsSelected();`
  *Public Class Function*

  Returns a `bool`. Takes in nothing.
  This returns a `bool` denoting whether a selection is going on currently.
*/
bool Buffer::getIsSelected() {
  return m_cursor.is_selection;
}

/*
  ### `int Buffer::getSelectionStartX();`
  *Public Class Function*

  Returns a `int`. Takes in nothing.
  This returns the x co-ordinate of the start point of the selection.
*/
int Buffer::getSelectionStartX(double p_window_height,double p_font_height){
  sl_lines = int(p_window_height/p_font_height) + 1;
  if ( m_cursor.top > m_cursor.sel.start_y ) {
    return 0;
  }
  if ( m_cursor.sel.start_y > m_cursor.top + sl_lines ) {
    return 0;
  }
  return m_cursor.sel.start_x + 7*count_tabs(m_cursor.sel.start_line->data, m_cursor.sel.start_x);
}

/*
  ### `int Buffer::getSelectionStartX();`
  *Public Class Function*

  Returns a `int`. Takes in nothing.
  This returns the y co-ordinate of the start point of the selection.
*/
int Buffer::getSelectionStartY(double p_window_height,double p_font_height) {
  sl_lines = int(p_window_height/p_font_height) + 1;
  if ( m_cursor.top > m_cursor.sel.start_y ) {
    return m_cursor.top;
  }
  if ( m_cursor.sel.start_y > m_cursor.top + sl_lines ) {
    return m_cursor.top + sl_lines;
  }
  return m_cursor.sel.start_y;
}


int Buffer::getSelectionStartXActual(){
  return m_cursor.sel.start_x;
}

int Buffer::getSelectionStartYActual(){
  return m_cursor.sel.start_y;
}

/*
  ### `Option<std::string> Buffer::getSelectionText();`
  *Public Class Function*

  Returns a `Option<std::string>`. Takes in nothing.
  This returns the text under selection.
*/
Option<std::string> Buffer::getSelectionText() {
  Option<std::string> selection;
  if ( !m_cursor.is_selection ) {
    return selection;
  }
  std::string sel_text = "";
  Text* iter = m_cursor.sel.start_line;
  Text* end = m_cursor.current;
  int l_sx = m_cursor.sel.start_x, l_ex = m_cursor.x;
  if ( m_cursor.y < m_cursor.sel.start_y ) {
    iter = m_cursor.current;
    end = m_cursor.sel.start_line;
  }
  if ( m_cursor.x < m_cursor.sel.start_x ) {
    l_ex = m_cursor.sel.start_x;
    l_sx = m_cursor.x;
  }
  while ( iter != end ) {
    sel_text += iter->data.substr(l_sx,-1) + "\n";
    iter = iter->next;
    l_sx = 0;
  }
  if ( iter == end ) {
    sel_text += iter->data.substr(l_sx, l_ex - l_sx);
  }
  selection.Set(sel_text);
  return selection;
}

/*
  ### `Error Buffer::saveFile();`
  *Public Class Function*

  Returns a Error object. Takes in nothing.
  Writes contents to the file (`filename`). Returns an Error object if any error.
*/
Error Buffer::saveFile() {
  std::string line;
  Text* lineiter = text;
  try {
    std::ofstream outfile(filename.Get());
    if (!outfile.is_open()) {
      Error e("Unable to open the file.");
      return e;
    }
    while ( true ) {
      line = lineiter->data;
      outfile << line << '\n';
      lineiter = lineiter->next;
      if ( lineiter == nullptr ) {
        break;
      }
    }
    outfile.close();
    Error e;
    return e;
  } catch ( const std::invalid_argument& ex ) {
    Error e(ex.what());
    return e;
  }
  Error e;
  return e;
}

/*
  ### `std::string Buffer::getFilename();`
  *Public Class Function*

  Returns a `std::string`. Takes in nothing.
  Returns the file name of the current buffer.
*/
std::string Buffer::getFilename() {
  try {
    return filename.Get();
  } catch ( const std::invalid_argument& ex ) {
    return "Untitled";
  }
  return "Untitled";
}
