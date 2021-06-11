#include "vehicle_factory.h"
#include <stdexcept>
#include "coach.h"                // for Coach
#include "covered_freight_car.h"  // for CoveredFreightCar
#include "diesel_locomotive.h"    // for DieselLocomotive
#include "electric_locomotive.h"  // for ElectricLocomotive
#include "open_freight_car.h"     // for OpenFreightCar
#include "sleeping_car.h"         // for SleepingCar

namespace pabo::train {

std::unique_ptr<Coach> makeCoach(const Params& p);
std::unique_ptr<SleepingCar> makeSleepingCar(const Params& p);
std::unique_ptr<OpenFreightCar> makeOpenFreightCar(const Params& p);
std::unique_ptr<CoveredFreightCar> makeCoveredFreightCar(const Params& p);
std::unique_ptr<ElectricLocomotive> makeElectricLocomotive(const Params& p);
std::unique_ptr<DieselLocomotive> makeDieselLocomotive(const Params& p);

std::unique_ptr<Vehicle> makeVehicle(Vehicle::Type t, Params p)
{
    switch(t) {
    case Vehicle::Type::coach :
        return makeCoach(p);
    case Vehicle::Type::sleepingCar :
        return makeSleepingCar(p);
    case Vehicle::Type::openFreightCar :
        return makeOpenFreightCar(p);
    case Vehicle::Type::coveredFreightCar :
        return makeCoveredFreightCar(p);
    case Vehicle::Type::electricLocomotive :
        return makeElectricLocomotive(p);
    case Vehicle::Type::dieselLocomotive :
        return makeDieselLocomotive(p);
    default:
        throw std::runtime_error{"Invalid vehicle type!"};
    }
}

std::unique_ptr<Coach> makeCoach(const Params& p)
{
        const auto& [id, seats, internet] = p;
        return std::make_unique<Coach>(id, seats, internet);
}

std::unique_ptr<SleepingCar> makeSleepingCar(const Params& p)
{
        const auto& [id, beds, discard] = p;
        return std::make_unique<SleepingCar>(id, beds);
}

std::unique_ptr<OpenFreightCar> makeOpenFreightCar(const Params& p)
{
        const auto& [id, cargoCap, floorArea] = p;
        return std::make_unique<OpenFreightCar>(id, cargoCap, floorArea);
}

std::unique_ptr<CoveredFreightCar> makeCoveredFreightCar(const Params& p)
{
        const auto& [id, cargoCap, discard] = p;
        return std::make_unique<CoveredFreightCar>(id, cargoCap);
}

std::unique_ptr<ElectricLocomotive> makeElectricLocomotive(const Params& p)
{
        const auto& [id, maxSpeed, power] = p;
        return std::make_unique<ElectricLocomotive>(id, maxSpeed, power);
}

std::unique_ptr<DieselLocomotive> makeDieselLocomotive(const Params& p)
{
    const auto& [id, maxSpeed, fuelcons] = p;
    return std::make_unique<DieselLocomotive> (id, maxSpeed, fuelcons);
}

}  // namespace pabo::train
