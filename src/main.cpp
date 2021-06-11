#include "console_IO.h"
#include "user_interface.h"
#include <string>
#include <iostream>
#include <stdexcept>

int main()
{
    using namespace std::string_literals;
    // When eof is read from stdin the stream can not be used
    // any more (at least on Linux / Unix). Exceptions, unlike
    // std::abort and std::exit, will destroy objects with
    // automatic storage duration when the stack is unwounded.
    std::cin.exceptions(std::iostream::eofbit);

    auto UI = pabo::app::UserInterface{};
    try {
        UI.run();
    }
    catch (std::runtime_error& e) {
        using pabo::IO::println;
        println("\nError: "s + e.what());
        println("Exiting...");
        return 1;
    }
}
