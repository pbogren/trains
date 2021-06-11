/**
    @file include/console_IO.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Functions for reading and writing to the console.
*/
#ifndef INCLUDE_CONSOLE_IO_H
#define INCLUDE_CONSOLE_IO_H

#include <iostream>
#include <stdexcept>
#include <string>

namespace pabo::IO {

void clearScreen();
void clearInput();
void waitForEnter();

// Write an element of type T to stdout
template<typename T>
void print(const T& elem) noexcept
{
    std::cout << elem;
}

// Write a line of an element of type T to stdout
template<typename T>
void println(const T& elem) noexcept
{
    std::cout << elem << '\n';
}

// Get an object of type T from stdin.
// Throws domain_error if the input could not be converted
// to type T.
template<typename T>
T get(const std::string& prompt)
{
    print(prompt);
    T input;
    std::cin >> input;
    if (std::cin.fail()) {
        clearInput();
        throw std::domain_error("Bad input!");
    }
    clearInput();
    return input;
}

}  // namespace pabo::IO

#endif

