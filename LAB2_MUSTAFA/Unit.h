#ifndef UNIT_H
#define UNIT_H

#include "Resource.h"
#include <memory>

class Unit : public Resource {
public:
    // Constructor and Destructor
    Unit();
    virtual ~Unit();

    // Serialization Methods
    void Serialize(std::ostream& stream) override;
    void Deserialize(std::istream& stream) override;

    // Utility Methods
    void ToString() override;
    void AssignNonDefaultValues() override;

    // Static Object Pool for Unit management
    static std::unique_ptr<ObjectPool<Unit>> Pool;

private:
    
};

#endif // UNIT_H
