#include "vessel.hpp"
#include <string>

Vessel::Vessel(std::string vessel_name)
{
    name = vessel_name;
}

Vessel::~Vessel()
{
}

size_t Vessel::add(std::string item, size_t rate)
{
    return 0;
}
