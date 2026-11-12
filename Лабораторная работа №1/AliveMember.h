#pragma once
#include "Base.h"

// Живой член семьи: возраст считается до сегодняшнего дня
class AliveMember : public Base {
public:
    AliveMember();
    AliveMember(int memberId, const std::string& name, const Date& birth);
    AliveMember(const AliveMember& other);
    ~AliveMember() override;

    int getType() const override;
    int getAge() const override;
    const char* getTypeName() const override;
    Base* clone() const override;
    void print(std::ostream& os) const override;
};