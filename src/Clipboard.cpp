#include "Clipboard.hpp"
#include <iostream>
extern "C" {
#include "libclipboard.h"
}

// # Public Class Functions
/*
  ### `*void* Clipboard::Clipboard();`
  *Public Constructor*
  
  Returns nothing. Takes in nothing.
  A simple constructor for the Clipboard class.
*/
Clipboard::Clipboard(){
    m_cb = clipboard_new(NULL);
}

/*
  ### `std::string Clipboard::getText();`
  *Public Class Function*

  Returns `std::string` denoting the current text in the clipboard. Takes in nothing.
*/
Option<std::string> Clipboard::getText(){
    char* cstr = clipboard_text(m_cb);
    Option<std::string> opt;
    if ( cstr == nullptr )
	return opt;

    std::string text(clipboard_text(m_cb));
    putText(text);
    opt.Set(text);
    return opt;
}

/*
  ### `void Clipboard::putText(std::string);`
  *Public Class Function*

  Returns nothing. Takes in the text to be placed in the clipboard.
*/
void Clipboard::putText(std::string text){
    clipboard_set_text(m_cb, text.c_str());
}

/*
  ### `void Clipboard::clear();`
  *Public Class Function*

  Returns nothing. Takes in nothing.
  Clears the clipboard.
*/
void Clipboard::clear(){
    clipboard_clear(m_cb, LCB_CLIPBOARD);
}

/*
  ### `*void* Clipboard::~Clipboard();`
  *Public Destructor*

  Returns nothing. Takes in nothing.
  Frees the C based variables and resources before deletion from memory.
*/
Clipboard::~Clipboard(){
    clipboard_free(m_cb);
}
