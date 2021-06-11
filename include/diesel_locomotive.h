#ifndef INCLUDE_DIESEL_LOCOMOTIVE_H
#define INCLUDE_DIESEL_LOCOMOTIVE_H

#include "vehicle_type.h"
#include "locomotive.h"

namespace pabo::train {

class DieselLocomotive : public Locomotive {
public:
    DieselLocomotive(int id, double spd, double fuelConsumption);

private:
    [[nodiscard]] Type type_() const noexcept override;
    [[nodiscard]] int id_() const noexcept override;
    [[nodiscard]] Speed maxSpeed_() const noexcept override;
    [[nodiscard]] FlowRate fuelConsumption_() const noexcept override;

    int m_id;
    Speed m_maxSpd;
    FlowRate m_fuelConsumption;
};


}  // namespace pabo::train

#endif
