#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

//  `Clipboard`

/*
  Containing all the classes, structs and functions for cross platform clipboard functionality
  This is a basic C++ wrapper for cross platform [C library `libclipboard`](https://github.com/jtanx/libclipboard).
*/

// # Header Declarations
#include <iostream>
#include "Option.hpp"
#include "libclipboard.h"


/*
  ### `class Clipboard{...};`
  *Accessible Class*
  
  Class Clipboard contains functions and members to implement clipboard for the editor.
*/
class Clipboard {
  clipboard_c *m_cb;
public:
  Clipboard();
  Option<std::string> getText();
  void putText(std::string);
  void clear();
  ~Clipboard();
};

#endif
