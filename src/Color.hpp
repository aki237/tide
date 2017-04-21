#ifndef COLOR_HPP
#define COLOR_HPP

//  `Color`
/*
  Containing all necessary structs, classes and functions for different color convertions.
*/

#include <iostream>
#include <cairo.h>
// # Header Declarations

/*
  ### `typedef unsigned int uint;`
  *Accessible Type*

  Unsigned Integer type.
 */
typedef unsigned int uint;

/*
  ### `typedef struct{...} color_struct;`
  *Accessible Type*

  Struct containing Red, Green, Blue, Alpha (uint) and ColorString (std::string) as members.
  The uint typed members are supposed to range from 0 to 255.
 */
typedef struct {
    uint Red, Blue, Green, Alpha;
    std::string ColorString;
} color_struct;

/*
  ### `typedef struct{...} color_cairo_struct;`
  *Accessible Type*

  Struct containing Red, Green, Blue, Alpha (float) as members.
  The float typed members are supposed to range from 0 to 1.
  This struct is used to contain the color in r,g,b,a for use in the cairo functions.
*/
typedef struct {
    float Red, Blue, Green, Alpha;
} color_cairo_struct;

/*
  ### `class Color{...};`
  *Accessible Class*

  Class Color contains functions and members to convert among different color formats like HTML, RGB, RGB::Cairo.
*/
class Color {
 private:
    uint red, green, blue, alpha;
    std::string color_string;
    void set_color(color_struct);
 public:
    static void SetCairoColor(cairo_t*,std::string);
    Color(std::string);
    Color(uint,uint,uint,uint);
    void Print();
    color_cairo_struct ToCairoColor();
    std::string ToHTMLColor();
    color_struct ToRGBColor();
};

#endif
