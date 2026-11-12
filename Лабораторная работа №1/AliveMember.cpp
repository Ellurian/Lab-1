#include "AliveMember.h"

AliveMember::AliveMember() : Base() {
    std::cout << "[Конструктор] AliveMember без параметров" << std::endl;
}

AliveMember::AliveMember(int memberId, const std::string& name, const Date& birth)
    : Base(memberId, name, birth) {
    std::cout << "[Конструктор] AliveMember с параметрами (" << fullName << ")" << std::endl;
}

AliveMember::AliveMember(const AliveMember& other) : Base(other) {
    std::cout << "[Конструктор копирования] AliveMember (" << fullName << ")" << std::endl;
}

AliveMember::~AliveMember() {
    std::cout << "[Деструктор] AliveMember (" << fullName << ")" << std::endl;
}

int AliveMember::getType() const { return 1; }

int AliveMember::getAge() const {
    return calcAge(birthDate, currentDate());
}

const char* AliveMember::getTypeName() const { return "живой"; }

Base* AliveMember::clone() const {
    return new AliveMember(*this);
}

void AliveMember::print(std::ostream& os) const {
    Base::print(os);
    os << " | статус: жив(а)";
}