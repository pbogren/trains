#include "path.h"
#include <iostream>
#include <utility>

namespace pabo::train {

Path::Path(std::string a, std::string b, double distance /* = 0 */)
    : m_pointA{std::move(a)}
    , m_pointB{std::move(b)}
    , m_distance{distance, "km"}
{
}

std::string Path::pointA() const
{
    return m_pointA;
}

std::string Path::pointB() const
{
    return m_pointB;
}

Capacity<double> Path::distance() const
{
    return m_distance;
}

bool operator==(const Path& lhs, const Path& rhs)
{
    return (lhs.pointA() == rhs.pointA() && lhs.pointB() == rhs.pointB()) ||
           (lhs.pointB() == rhs.pointA() && lhs.pointA() == rhs.pointB());
}

std::ostream& operator<<(std::ostream& os, const Path& p)
{
    return os << p.pointA() << ' ' << p.pointB() << ' ' << p.distance();
}

std::istream& operator>>(std::istream& is, Path& p)
{
    auto pointA = std::string{};
    auto pointB = std::string{};
    auto distance = double{};

    is >> pointA >> pointB >> distance;
    if (is.fail()) { return is; }

    auto tmp = Path(pointA, pointB, distance);
    using std::swap;
    swap(tmp, p);

    return is;
}

}  // namespace pabo::train
