#ifndef ERROR_HPP
#define ERROR_HPP

//  `Error`
/*
  Containing all necessary structs, classes and functions for declaring Error Class.
*/
#include <iostream>

// # Header Declarations

/*
  ### `class Error{...};`
  *Accessible Class*

  Class Error contains functions and members for containing error details.
*/
class Error {
private:
  std::string error;
  bool is_error;
public:
  // # Public Class Functions
  /*
    ### `std::ostream &operator<<(std::ostream, const Error)`
    *Overloaded Class Function*
      
    Overloaded functions allowing it to be printed in a C++ standard output stream object. 
  */
  friend std::ostream& operator<<(std::ostream& os, const Error& m) {
    if (m.is_error) {
      return os << "Error : " << m.error;
    }
    return os << "" ;
  }

  /*
    ### `*void* Error::Error(std::string);`
    *Public Constructor*
      
    Returns nothing. Takes in a string, describing ther error.
  */
  Error(std::string p_error) {
    error = p_error;
    is_error = true;
  }

  /*
    ### `*void* Error::Error();`
    *Public Constructor*
      
    Returns nothing. Takes in nothing.
  */
  Error() {
    is_error = false;
  }

  /*
    ### `bool Error::IsError();`
    *Public Class Function*
      
    Returns boolean whether the error is nil or not. Takes in nothing.
  */
  bool IsError() {
    return is_error;
  }
};
#endif
