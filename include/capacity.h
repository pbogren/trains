/**
    @file include/capacity.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the Capacity class.

    The class is meant to handle different units of measurement by holding
    a value and a corresponding unit of measurement as a string.
*/
#ifndef INCLUDE_CAPACITY_H
#define INCLUDE_CAPACITY_H

#include <iostream>
#include <memory>
#include <string>

template<typename T>
struct Capacity {
public:
    Capacity() = default;
    Capacity(T val, std::string ut)
        : value{std::move(val)}
        , unit{std::move(ut)}
    {
    }

    T value{0};
    std::string unit;
};

template<typename T>
bool operator==(const Capacity<T>& lhs, const T& rhs)
{
    return lhs.value == rhs;
}

template<typename T>
bool operator==(const T& lhs, const Capacity<T>& rhs)
{
    return lhs == rhs.value;
}

template<typename T>
bool operator==(const Capacity<T>& lhs, const Capacity<T>& rhs)
{
    return rhs.value == lhs.value;
}

template<typename T>
bool operator<(const Capacity<T>& lhs, const Capacity<T>& rhs)
{
    return lhs.value < rhs.value;
}

template<typename T>
bool operator>(const Capacity<T>& lhs, const Capacity<T>& rhs)
{
    return lhs.value > rhs.value;
}

template<typename T>
bool operator>(const Capacity<T>& lhs, const T& rhs)
{
    return lhs.value > rhs;
}


template<typename T>
bool operator>(const T& lhs, const Capacity<T>& rhs)
{
    return lhs > rhs.value;
}

template<typename T>
bool operator<(const Capacity<T>& lhs, const T& rhs)
{
    return lhs.value < rhs;
}


template<typename T>
bool operator<(const T& lhs, const Capacity<T>& rhs)
{
    return lhs < rhs.value;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Capacity<T>& cap)
{
    return os << cap.value << ' ' << cap.unit;
}

template<typename T>
std::istream& operator>>(std::istream& is, Capacity<T>& cap)
{
    T value;
    is >> value;
    cap = Capacity<T>(value);
    return is;
}

#endif
