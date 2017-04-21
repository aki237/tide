#ifndef BUFFER_HPP
#define BUFFER_HPP

//  `Buffer`
/*
  Containing all necessary structs, classes and functions for managing text.
*/
#include <iostream>
#include <vector>
#include "Option.hpp"
#include "Error.hpp"
#include "Node.hpp"
// # Header Declarations


/*
  ### `typedef Node<std::string> Text;`
  *Accessible Type*

  This type is a doubly linked list representation of a text document.
*/
typedef Node<std::string> Text;

/*
  ### `typedef struct {...} Selection;`
  *Accessible Type*

  This type contains 3 members namely start_x, start_y denoting the start position of the selection, and pointer `Text`
  object pointing to the line where selection started. 
*/
typedef struct {
    int start_x;
    int start_y;
    Text* start_line;
} Selection;

/*
  ### `typedef struct {...} Cursor;`
  *Accessible Type*

  This type is used to describe a cursor.
  Contains members namely 
    + `x`, `y` - `int,int` denoting the cursor position in the file), 
    + `top` - `int` denoting the scroll top,
    + `is_selection` - `bool` denoting whether text is selected,
    + `sel` - `Selection` struct denoting the selection bounds [actually only the start. end is always the cursor.]), 
    + `current` - `Text` denoting the current line.
*/
typedef struct {
    int x;
    int y;
    int top;
    bool is_selection;
    Selection sel;
    Text* current;
} TCursor;


/*
  ### `class Buffer{...};`
  *Accessible Class*

  Class Buffer contains functions and members to manage text for the TextView.
*/
class Buffer {
 private:
    Text* text;
    Option<std::string> filename;
    Error readFile();
    TCursor m_cursor;
    int sl_lines;
 public:
    static int count_tabs(std::string, int);
    Buffer();
    Buffer(std::string);
    void insertChar(char);
    void insertText(const char*);
    void insertStdString(std::string);
    void bkspcChar();
    void deleteChar();
    void deleteLine();
    void cursorUp();
    void cursorDown();
    void cursorLeft();
    void cursorRight();
    void cursorBeginLine();
    void cursorEndLine();
    void cursorBeginBuffer();
    void cursorEndBuffer();
    int getCursorPosX();
    int getCursorPosXActual();
    int getCursorPosY();
    int getTop();
    std::string getCurrentLine();
    void setTop(double, double);
    void selectionStart();
    void selectionCancel();
    bool getIsSelected();
    int getSelectionStartX(double,double);
    int getSelectionStartY(double,double);
    int getSelectionStartXActual();
    int getSelectionStartYActual();
    Option<std::string> getSelectionText();
    std::vector<std::string> getLines(double,double);
    Error saveFile();
};

#endif
