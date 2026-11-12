#pragma once
#include "Base.h"

// Умерший член семьи: есть дата смерти, возраст считается до неё
class DeceasedMember : public Base {
private:
    Date deathDate;

public:
    DeceasedMember();
    DeceasedMember(int memberId, const std::string& name, const Date& birth, const Date& death);
    DeceasedMember(const DeceasedMember& other);
    ~DeceasedMember() override;
    DeceasedMember& operator=(const DeceasedMember& other);

    Date getDeathDate() const;
    void setDeathDate(const Date& d);
    void setBirthDate(const Date& d) override;

    int getType() const override;
    int getAge() const override;
    const char* getTypeName() const override;
    Base* clone() const override;
    void print(std::ostream& os) const override;
    void saveTo(std::ostream& os) const override;
    void loadFrom(std::istream& is) override;
};