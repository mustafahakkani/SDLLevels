#include "Unit.h"
#include <iostream>

// Constructor
Unit::Unit() = default;

// Destructor
Unit::~Unit() = default;

// Assign non-default values to the unit
void Unit::AssignNonDefaultValues() {
    Resource::AssignNonDefaultValues();
}

// Serialize the unit state
void Unit::Serialize(std::ostream& stream) {
    Resource::Serialize(stream);
}

// Deserialize the unit state
void Unit::Deserialize(std::istream& stream) {
    Resource::Deserialize(stream);
}

// Convert the unit to a string representation
void Unit::ToString() {
    std::cout << "UNIT" << std::endl;
    Resource::ToString();
}
