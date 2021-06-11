/**
    @file include/map.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Definition of the Path class.

    A path is a connection between two points. The points each have
    a name and a distance between them.
*/
#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include "capacity.h"
#include <iosfwd>
#include <string>

namespace pabo::train {

class Path {
public:
    Path() = default;
    Path(std::string a, std::string b, double distance = 0.0);

    [[nodiscard]] std::string pointA() const;
    [[nodiscard]] std::string pointB() const;
    [[nodiscard]] Capacity<double> distance() const;

private:
    std::string m_pointA;
    std::string m_pointB;
    Capacity<double> m_distance;
};

// Returns true of lhs and rhs contains the same points
// regardless of order.
bool operator==(const Path& lhs, const Path& rhs);

// Stream operations
std::ostream& operator<<(std::ostream& os, const Path& p);
std::istream& operator>>(std::istream&, Path&);

}  // namespace pabo::train

#endif
