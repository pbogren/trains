/**
    @file include/open_freight_car.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the OpenFreightCar class.
*/
#ifndef INCLUDE_OPEN_FREIGHT_CAR_H
#define INCLUDE_OPEN_FREIGHT_CAR_H

#include "train_car.h"
#include "vehicle_type.h"

namespace pabo::train {

class OpenFreightCar : public TrainCar {
public:
    OpenFreightCar(int id, double cargoCapacity, double floorArea);

private:
    [[nodiscard]] Type type_() const noexcept override;
    [[nodiscard]] int id_() const noexcept override;
    [[nodiscard]] bool carriesCargo_() const noexcept override;
    [[nodiscard]] Weight cargoCapacity_() const noexcept override;
    [[nodiscard]] Area floorArea_() const noexcept override;

    int m_id;
    Weight m_cargoCapacity;
    Area m_floorArea;
};

}  // namespace pabo::train

#endif
