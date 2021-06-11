/**
    @file include/time_point.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The definition of the TimeOfDay class.
*/

#ifndef INCLUDE_TIME_POINT_H
#define INCLUDE_TIME_POINT_H

#include <string>
#include <iosfwd>

namespace pabo::time {

const auto delim{':'};

class TimeOfDay {
public:

    TimeOfDay() = default;
    // Throws invalid_argument if string is not a valid time (hh:mm)
    explicit TimeOfDay(const std::string&);
    explicit TimeOfDay(int minutes);
    explicit TimeOfDay(double minutes);

    // Returns the total raw time in minutes. This value can be
    // negative.
    [[nodiscard]] int rawTime() const noexcept;

    // Returns the time of day as minutes or hours from 00:00
    // This values will never be negative.
    [[nodiscard]] int inMinutes() const noexcept;
    [[nodiscard]] double inHours() const noexcept;

    // Returns the current hour
    [[nodiscard]] int hour() const noexcept;
    // returns the current minute
    [[nodiscard]] int minute() const noexcept;
    // Returns the current time as a string in the format hh:mm
    [[nodiscard]] std::string asString() const;

    // Adds more minutes to the current time
    void addMinutes(int) noexcept;

private:
    void initializeFromString(const std::string&);
    void setTime(int minutes);

    int m_minutes{0};
    int m_days{0};  // used to keep track of "wraparounds"
};

// Comparisons
TimeOfDay operator+(const TimeOfDay& lhs, const TimeOfDay& rhs);
TimeOfDay& operator+=(TimeOfDay& lhs, const TimeOfDay& rhs);
TimeOfDay operator-(const TimeOfDay& lhs, const TimeOfDay& rhs);
TimeOfDay& operator-=(TimeOfDay& lhs, const TimeOfDay& rhs);
bool operator==(const TimeOfDay& lhs, const TimeOfDay& rhs);
bool operator!=(const TimeOfDay& lhs, const TimeOfDay& rhs);
bool operator<(const TimeOfDay& lhs, const TimeOfDay& rhs);
bool operator<=(const TimeOfDay& lhs, const TimeOfDay& rhs);
bool operator>(const TimeOfDay& lhs, const TimeOfDay& rhs);
bool operator>=(const TimeOfDay& lhs, const TimeOfDay& rhs);

// Stream operations
std::ostream& operator<<(std::ostream&, const TimeOfDay&);
std::istream& operator>>(std::istream&, TimeOfDay&);

}  // namespace pabo::time

#endif
