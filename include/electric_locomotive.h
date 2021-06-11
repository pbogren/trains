/**
    @file include/electric_locomotive.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the ElectricLocomotive class.
*/

#ifndef INCLUDE_ELECTRIC_LOCOMOTIVE_H
#define INCLUDE_ELECTRIC_LOCOMOTIVE_H

#include "locomotive.h"
#include "vehicle_type.h"

namespace pabo::train {


class ElectricLocomotive : public Locomotive {
public:
    ElectricLocomotive(int id, double maxSpd, double maxPwr);

private:
    [[nodiscard]] Type type_() const noexcept override;
    [[nodiscard]] int id_() const noexcept override;
    [[nodiscard]] Speed maxSpeed_() const noexcept override;
    [[nodiscard]] Power maxPower_() const noexcept override;

    int m_id;
    Speed m_maxSpd;
    Power m_maxPwr;
};

}  // namespace pabo::train

#endif
