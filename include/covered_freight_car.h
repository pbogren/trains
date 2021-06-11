/**
    @file include/covered_freight_car.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the CoveredFreightCar class.
*/
#ifndef INCLUDE_COVERED_FREIGHT_CAR_H
#define INCLUDE_COVERED_FREIGHT_CAR_H

#include "train_car.h"
#include "vehicle_type.h"

namespace pabo::train {

class CoveredFreightCar : public TrainCar {
public:
    CoveredFreightCar(int id, double cargoVolume);

private:
    [[nodiscard]] Type type_() const noexcept override;
    [[nodiscard]] int id_() const noexcept override;
    [[nodiscard]] bool carriesCargo_() const noexcept override;
    [[nodiscard]] Volume cargoCapacity_() const noexcept override;

    int m_id;
    Volume m_capacity;
};

}  // namespace pabo::train

#endif

