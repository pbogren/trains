#include "time_point.h"
#include <cassert>
#include <cmath>  // round
#include <iomanip>
#include <iostream>
#include <stdexcept>  // invalid_argument
#include <string>
#include <utility>  // swap

namespace pabo::time {

using TP = TimeOfDay;

constexpr auto HrsPrDay{24};
constexpr auto minsPrHr{60};
constexpr auto minsPrDay{HrsPrDay * minsPrHr};
constexpr auto fieldWidth{2};

bool validHours(double hrs);
bool validMinutes(double mins);
auto findDelimiterPos(const std::string& str) -> std::string::size_type;


TP::TimeOfDay(const std::string& s)
{
    try {
        initializeFromString(s);
    }
    catch (std::invalid_argument&) {
        throw std::invalid_argument("Bad time format: " + s);
    }
}

void TP::initializeFromString(const std::string& s)
{
    const auto delimPos = findDelimiterPos(s);
    const auto hrs = std::stoi(s.substr(0, delimPos));
    const auto mins = std::stoi(s.substr(delimPos + 1));
    if (!validHours(hrs) || !validMinutes(mins)) {
        throw std::invalid_argument("Invalid time fields!");
    }
    addMinutes(hrs * minsPrHr + mins);
}

bool validHours(const double hrs)
{
    return hrs >= 0 && hrs < HrsPrDay;
}

bool validMinutes(const double mins)
{
    return mins >= 0 && mins < minsPrHr;
}

auto findDelimiterPos(const std::string& str) -> std::string::size_type
{
    const auto delim_pos = str.find(delim);
    if (delim_pos == std::string::npos) {
        throw std::invalid_argument("String not in format hh:mm");
    }
    return delim_pos;
}

TP::TimeOfDay(const int minutes)
{
    setTime(minutes);
}


TP::TimeOfDay(const double minutes)
    : TimeOfDay{static_cast<int>(round(minutes))}
{
}

void TP::setTime(const int minutes)
{
    if (minutes >= 0) {
        m_minutes = minutes % minsPrDay;
    }
    else {
        m_minutes = minsPrDay + (minutes % minsPrDay);
    }
    m_days = minutes / minsPrDay;
}

int TP::rawTime() const noexcept
{
    return m_days * minsPrDay + m_minutes;
}

int TP::inMinutes() const noexcept
{
    return m_minutes;
}

double TP::inHours() const noexcept
{
    return hour() + (static_cast<double>(minute()) / minsPrHr);
}

std::string TP::asString() const
{
    const auto hr = hour();
    assert(hr >= 0);
    const auto min = minute();
    assert(min >= 0);

    auto hrs = std::to_string(hr);
    if (hrs.length() < fieldWidth) {
        hrs.insert(0, 1, '0');
    }
    auto mins = std::to_string(min);
    if (mins.length() < fieldWidth) {
        mins.insert(0, 1, '0');
    }
    return hrs + delim + mins;
}

int TP::hour() const noexcept
{
    return m_minutes / minsPrHr;
}

int TP::minute() const noexcept
{
    return m_minutes % minsPrHr;
}

void TP::addMinutes(const int minutesToAdd) noexcept
{
    setTime(rawTime() + minutesToAdd);
}

TimeOfDay operator+(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    return TimeOfDay{lhs.rawTime() + rhs.rawTime()};
}

TimeOfDay& operator+=(TimeOfDay& lhs, const TimeOfDay& rhs)
{
    lhs.addMinutes(rhs.rawTime());
    return lhs;
}

TimeOfDay operator-(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    const auto correctedLhs = lhs.rawTime() % (HrsPrDay * minsPrHr);
    const auto correctedRhs = rhs.rawTime() % (HrsPrDay * minsPrHr);
    return TimeOfDay{correctedLhs - correctedRhs};
}

TimeOfDay& operator-=(TimeOfDay& lhs, const TimeOfDay& rhs)
{
    const auto minuteDiff = lhs.rawTime() - rhs.rawTime();
    auto tmp = TimeOfDay{minuteDiff};
    using std::swap;
    swap(tmp, lhs);
    return lhs;
}

bool operator==(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    return lhs.rawTime() == rhs.rawTime();
}

bool operator!=(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    return lhs.rawTime() < rhs.rawTime();
}

bool operator<=(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    return lhs.rawTime() <= rhs.rawTime();
}

bool operator>(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    return lhs.rawTime() > rhs.rawTime();
}

bool operator>=(const TimeOfDay& lhs, const TimeOfDay& rhs)
{
    return lhs.rawTime() >= rhs.rawTime();
}

std::ostream& operator<<(std::ostream& os, const TimeOfDay& tp)
{
    return os << std::setfill('0')
              << std::setw(fieldWidth) << tp.hour()
              << delim
              << std::setw(fieldWidth) << tp.minute();
}

std::istream& operator>>(std::istream& is, TimeOfDay& tp)
{
    try {
        auto time = std::string{};
        is >> time;
        auto tmp = TimeOfDay{time};
        using std::swap;
        swap(tp, tmp);
    }
    catch (const std::invalid_argument&) {
        is.setstate(std::ios::failbit);
    }

    return is;
}

}  // namespace pabo::time
