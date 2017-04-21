#ifndef CORE_HPP
#define CORE_HPP
//  `Core`

/*
  Containing all necessary structs classes and functions controlling the whole applications including buffers and plugins.
*/
#include <vector>
#include "Buffer.hpp"

class Core {
private:
    std::vector<Buffer> buffers;
};

#endif
