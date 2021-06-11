/**
    @file include/coach.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The definition of the Coach class.
*/
#ifndef INCLUDE_COACH_H
#define INCLUDE_COACH_H

#include "train_car.h"
#include "vehicle_type.h"

namespace pabo::train {

class Coach : public TrainCar {
public:
    Coach(int id, int noSeats, bool hasInternet);

private:
    [[nodiscard]] Type type_() const noexcept override;
    [[nodiscard]] int id_() const noexcept override;

    [[nodiscard]] bool carriesPassengers_() const noexcept override;
    [[nodiscard]] int numberOfPassengers_() const noexcept override;
    [[nodiscard]] bool hasInternet_() const noexcept override;

    int m_id;
    int m_noSeats;
    bool m_hasInternet;
};

}  // namespace pabo::train

#endif
