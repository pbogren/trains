#include "console_IO.h"
#include <iostream>  // cin, cout, streamsize
#include <limits>  // numeric_limits
#include <string>  // string, getline


namespace pabo::IO {

void clearScreen()
{
    const auto lineCount{50};
    std::cout << std::string(lineCount, '\n');
}

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void waitForEnter()
{
    print("\nPress enter to continue...");
    auto dummy = std::string{};
    std::getline(std::cin, dummy);
}

}  // namespace pabo::IO
