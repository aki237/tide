#include "Color.hpp"
#include <iostream>

const std::string allowed = "0123456789ABCDEF";


// # Private Common Functions
/*
  ### `bool check_color (std::string)`
  *Inaccessible Common Function*

  Returns boolean if the passed color (HTML format) contains valid charaters.
  The valid charaters are in the allowed variable above ("0123456789ABCDEF").
 
  ```c++
  check_color("#344556");
  // => true
  check_color("#344556FG");
  // => false // Invalid because G is a invalid hexadecimal number.
  check_color("#344556677889");
  // => true // (even though that is a invalid notation of HTML Formatted color)
  ```
*/
bool check_color (std::string color) {
    for (uint i = 1 ; i < color.length(); i++) {
	if (std::string::npos == allowed.find(color.at(i),0)) {
	    std::cerr << "Color.cpp <check_color> :: Invalid character found - \"" << color.at(i) << "\"" << std::endl;
	    return false;
	}
    }
    return true;
}

/*
 ### `std::string processColorHTML (uint, uint, uint, uint)`
 *Inaccessible Common Function*

 Returns the HTML format color string (like #FF0000FF), for the passed red, green, and blue values.
 Note that the values passed must be from 0 to 255 else this returns a empty string.
 
 ```c++
 processColorHTML(52, 69, 86, 255);
 // => #344556FF
 processColorHTML(100,12000,388,399);
 // => "" (and prints "Color.cpp <Color::Color(uint,uint,uint,uint) :: Invalid value (greater than 255)" in stderr)
 ```
*/
std::string processColorHTML(uint r, uint g, uint b, uint a) {
    if (r > 255 || g > 255 || b > 255 || a > 255) {
	std::cerr << "Color.cpp <Color::Color(uint,uint,uint,uint) :: Invalid value (greater than 255)." <<  std::endl;
	return "";
    }
    std::string return_color = "#";
    return_color += allowed.at(r/16);
    return_color += allowed.at(r%16);
    return_color += allowed.at(g/16);
    return_color += allowed.at(g%16);
    return_color += allowed.at(b/16);
    return_color += allowed.at(b%16);
    return_color += allowed.at(a/16);
    return_color += allowed.at(a%16);
    return return_color;
}

/*
  ### `color_struct processColor (std::string)`
  *Inaccessible Common Function*

  Returns a color_struct for the passed HTML formatted color string.
  See color_struct documentation in the Color.h header file where it is defined.

  ```c++
  processColor("#344556");
  // => color_struct<Red:52, Green:69, Blue:86, Alpha:255>
  // for any invalid color it returns a uninitialised `color_struct`
  ```
*/
color_struct processColor(std::string c) {
    color_struct r;
    if (c.length() == 9 && c[0] == '#' && check_color(c)) {
	r.Red = (uint(allowed.find(c.at(1),0))) * 16;
	r.Red += uint(allowed.find(c.at(2),0));
	r.Green = (uint(allowed.find(c.at(3),0))) * 16;
	r.Green += uint(allowed.find(c.at(4),0));
	r.Blue = (uint(allowed.find(c.at(5),0))) * 16;
	r.Blue += uint(allowed.find(c.at(6),0));
	r.Alpha = (uint(allowed.find(c.at(7),0))) * 16;
	r.Alpha += uint(allowed.find(c.at(8),0));
	r.ColorString = c;
    }
    return r;
}

// # Private Class Functions

/*
  ### `void Color::set_color (color_struct)`
  *Private Class Function*

  Returns nothing. It sets the corresponding values of the struct members to the class members (red, green, blue, alpha).
*/
void Color::set_color(color_struct r) {
    red = r.Red;
    green = r.Green;
    blue = r.Blue;
    alpha = r.Alpha;
    color_string = r.ColorString;
}


// # Public Class Functions

/*
  ### `*void* Color::Color (std::string)`
  *Public Constructor*

  Returns nothing. Takes in a properly formatted HTML Color String.
  Valid Strings can be:
  + #345 - #RGB
  + #344556 - #RRGGBB
  + #344556FE - #RRGGBBAA
*/
Color::Color(std::string color) {
    if ((color.length() == 4) && (color[0] == '#')){
	if (check_color(color)) {
	    color += "F";
	    std::string newcolor = "#";
	    for (uint i = 1 ; i < color.length(); i++) {
		newcolor += color.at(i);
		newcolor += color.at(i);
	    }
	    set_color(processColor(newcolor));
	}
	return;
    }
    if ((color.length() == 7) && (color[0] == '#')){
	if (check_color(color)){
	    color += "FF";
	    set_color(processColor(color));
	}
	return;
    }
    if ((color.length() == 9) && (color[0] == '#')){
	if (check_color(color)){
	    set_color(processColor(color));
	}
	return;
    } else {
	std::cerr << "Color.cpp <Color::Color(std::string)> :: Invalid color format - \"" << color << "\"." << std ::endl;
    }
}

/*
  ### `*void* Color::Color (uint, uint, uint, uint)`
  *Public Constructor*

  Returns nothing. Takes in the unsigned int values of red, green, blue and alpha.
  Valid values for the parameters ranges from 0 to 255.
*/
Color::Color(uint r, uint g, uint b, uint a) {
    if (r > 255 || g > 255 || b > 255 || a > 255) {
	std::cerr << "Color.cpp <Color::Color(uint,uint,uint,uint) :: Invalid value (greater than 255)." <<  std::endl;
	return;
    }
    red = r;
    green = g;
    blue = b;
    alpha = a;
    color_string = processColorHTML(r,g,b,a);
}

/*
  ### `void Color::Print ()`
  *Public Class Function*

  Returns Nothing. Takes in nothing. Prints the Color in 2 formats namely HTML Format and RGBA format.
  ```c++
  Color c("#344556");
  c.Print();
  // => Color : #344556FF - rgba(52, 69, 86, 255)
  ```
*/
void Color::Print() {
    std::cout << "Color : " <<
	color_string <<
	" - rgba(" << red << ", " << green << ", " << blue << ", " << alpha << ")" <<
	std::endl;
}

/*
  ### `color_cairo struct Color::ToCairoColor();`
  *Public Class Function*

  Returns a color_cairo_struct (Refer the Color.h header for the struct Documentation).

  ```c++
  Color c("#344556");
  c.ToCairoColor();
  // => color_cairo_struct<red:0.20392156862745098, green:0.27058823529411763, blue:0.33725490196078434, alpha:1.0>
  ```
*/
color_cairo_struct Color::ToCairoColor() {
    return color_cairo_struct{float(red)/float(255.0), float(green)/float(255.0), float(blue)/float(255.0), float(alpha)/float(255.0)};
}

/*
  ### `std::string Color::ToHTMLColor();`
  *Public Class Function*

  Returns a Formatted HTML color string.
  ```c++
  Color c(52, 69, 86, 255);
  c.ToHTMLColor();
  // => #344556FF
  ```
*/
std::string Color::ToHTMLColor() {
    return color_string;
}

/*
  ### `color_struct Color::ToRGBColor();`
  *Public Class Function*

  Returns a color_struct (Refer the Color.h header for the struct Documentation).
  ```c++
  Color c("#344556");
  c.ToRGBColor();
  // => color_struct<red:52, green:69, blue:86, alpha:255>
  ```
*/
color_struct Color::ToRGBColor() {
    return color_struct{red, green, blue, alpha};
}

// # Public Static Functions
/*
  ### `static void Color::SetCairoColor(cairo_t*, std::string);`
  *Public Static Class Function*

  Returns nothing. Takes in a pointer to `cairo_context_t` and `std::string` denoting the color (HTML Format).
  This function sets drawing color to cairo context passed. 
*/
void Color::SetCairoColor(cairo_t* cr, std::string p_color) {
    Color l_c(p_color);
    color_cairo_struct l_cr_c = l_c.ToCairoColor();
    cairo_set_source_rgb(cr, l_cr_c.Red, l_cr_c.Blue, l_cr_c.Green);
}
