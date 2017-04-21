#include <Option.h>
#include <iostream>

int main(int argc, char *argv[]) {
    Option<std::string> io("initialised option");
    Option<std::string> uo;
    // Right usage
    if (io.IsSome()){
	std::cout << io.Get() << std::endl;
    }
    // Perfect Usage : Continue the program even after an exception
    try {
	uo.Get();
    } catch( const std::invalid_argument& e ) {
	std::cout << "Exception : " << e.what() << std::endl;
    }
    // Wrong Usage : wishes for getting your core dumped
    std::cout << uo.Get() << std::endl;
    return 0;
}

