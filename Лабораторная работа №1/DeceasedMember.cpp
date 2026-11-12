#include "DeceasedMember.h"
#include "Exceptions.h"

DeceasedMember::DeceasedMember() : Base(), deathDate(makeDate(1, 1, 2000)) {
    std::cout << "[Конструктор] DeceasedMember без параметров" << std::endl;
}

DeceasedMember::DeceasedMember(int memberId, const std::string& name,
    const Date& birth, const Date& death)
    : Base(memberId, name, birth), deathDate(makeDate(1, 1, 2000)) {
    setDeathDate(death);
    std::cout << "[Конструктор] DeceasedMember с параметрами (" << fullName << ")" << std::endl;
}

DeceasedMember::DeceasedMember(const DeceasedMember& other)
    : Base(other), deathDate(other.deathDate) {
    std::cout << "[Конструктор копирования] DeceasedMember (" << fullName << ")" << std::endl;
}

DeceasedMember::~DeceasedMember() {
    std::cout << "[Деструктор] DeceasedMember (" << fullName << ")" << std::endl;
}

DeceasedMember& DeceasedMember::operator=(const DeceasedMember& other) {
    if (this == &other) return *this;
    Base::operator=(other);
    deathDate = other.deathDate;
    std::cout << "[Оператор =] DeceasedMember (" << fullName << ")" << std::endl;
    return *this;
}

Date DeceasedMember::getDeathDate() const { return deathDate; }

void DeceasedMember::setDeathDate(const Date& d) {
    if (!isValidDate(d))
        throw InvalidDataException("Некорректная дата смерти (такой даты не существует)");
    if (isEarlier(currentDate(), d))
        throw InvalidDataException("Дата смерти не может быть в будущем");
    if (isEarlier(d, birthDate))
        throw InvalidDataException("Дата смерти не может быть раньше даты рождения");
    deathDate = d;
}

void DeceasedMember::setBirthDate(const Date& d) {
    if (isValidDate(d) && isEarlier(deathDate, d))
        throw InvalidDataException("Дата рождения не может быть позже даты смерти");
    Base::setBirthDate(d);
}

int DeceasedMember::getType() const { return 2; }

int DeceasedMember::getAge() const {
    return calcAge(birthDate, deathDate);
}

const char* DeceasedMember::getTypeName() const { return "умерший"; }

Base* DeceasedMember::clone() const {
    return new DeceasedMember(*this);
}

void DeceasedMember::print(std::ostream& os) const {
    Base::print(os);
    os << " | умер(ла): " << deathDate;
}

void DeceasedMember::saveTo(std::ostream& os) const {
    Base::saveTo(os);
    os << deathDate.day << ' ' << deathDate.month << ' ' << deathDate.year << '\n';
}

void DeceasedMember::loadFrom(std::istream& is) {
    Base::loadFrom(is);
    Date d = makeDate(1, 1, 2000);
    is >> d;
    if (!is || !isValidDate(d) || isEarlier(d, birthDate))
        throw FileException("Файл повреждён: неверная дата смерти");
    deathDate = d;
}