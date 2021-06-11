/**
    @file include/sleeping_car.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The definition of the SleepingCar class.
*/
#ifndef INCLUDE_SLEEPING_CAR_H
#define INCLUDE_SLEEPING_CAR_H

#include "train_car.h"
#include "vehicle_type.h"

namespace pabo::train {

class SleepingCar : public TrainCar {
public:
    SleepingCar(int id, int noBeds);

private:
    [[nodiscard]] Type type_() const noexcept override;
    [[nodiscard]] int id_() const noexcept override;
    [[nodiscard]] bool carriesPassengers_() const noexcept override;
    [[nodiscard]] int numberOfPassengers_() const noexcept override;

    int m_id;
    int m_noBeds;
};

}  // namespace pabo::train

#endif
