#ifndef OPTION_HPP
#define OPTION_HPP

//  `Option`
/*
  Containing all necessary structs, classes and functions for declaring Optional members.
*/
#include <iostream>
#include <stdexcept>

// # Header Declarations

/*
  ### `template <class T> class Option{...};`
  *Accessible Template Class*

  Template Class Option contains functions and members to declare optional variables.
*/
template <class T> class Option {
 private:
    T some;
    bool is_some;
 public:
    Option(T);
    Option();
    void Set(T);
    bool IsSome();
    T Get();
};


// # Public Class Functions

/*
  ### `template <class T> *void* Option<T>::Option(T);`
  *Public Constructor*

  Returns nothing. Takes in a Template Parameter Class. Initializes a `Option` instance.
*/
template <class T>
Option<T>::Option(T t) {
    Set(t);
}

/*
  ### `template <class T> *void* Option<T>::Option();`
  *Public Constructor*

  Returns nothing. Takes in nothing. Initializes a `Option` instance.
*/
template <class T>
Option<T>::Option() {
    is_some = false;
}

/*
  ### `void Option<T>::Set(T);`
  *Public Class Function*

  Returns nothing. Takes in a Template Parameter Class. Sets the option value to the passed value
*/
template <class T>
void Option<T>::Set(T t) {
    some = t;
    is_some = true;    
}

/*
  ### `template <class T> bool Option<T>::IsSome();`
  *Public Class Function*

  Returns a boolean denoting that the option's value has been set. Takes in nothing.
*/
template <class T>
bool Option<T>::IsSome() {
    return is_some;
}

/*
  ### `template <class T> T Option<T>::Get();`
  *Public Class Function*

  Returns a Template Class Parameter Object. Takes in nothing.

  **Note** : This function should be called only after validating template <class T> with Option<T>::IsSome(). 
  Else this may throw an exception.
*/
template <class T>
T Option<T>::Get() {
    if (!is_some) {
	throw std::invalid_argument("Option<T>::Get() is called before Option value is set");
    }
    return some;
}

#endif
