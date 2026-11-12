#include "Base.h"
#include "Exceptions.h"

Base::Base()
    : id(0), fullName(""), birthDate(makeDate(1, 1, 2000)),
    spouseId(-1), children(nullptr), childCount(0), childCapacity(0) {
    parents[0] = -1;
    parents[1] = -1;
    std::cout << "[Конструктор] Base без параметров" << std::endl;
}

Base::Base(int memberId, const std::string& name, const Date& birth)
    : id(memberId), fullName(""), birthDate(makeDate(1, 1, 2000)),
    spouseId(-1), children(nullptr), childCount(0), childCapacity(0) {
    parents[0] = -1;
    parents[1] = -1;
    setFullName(name);
    setBirthDate(birth);
    std::cout << "[Конструктор] Base с параметрами (" << fullName << ")" << std::endl;
}

Base::Base(const Base& other)
    : id(other.id), fullName(other.fullName), birthDate(other.birthDate),
    spouseId(other.spouseId), children(nullptr), childCount(0), childCapacity(0) {
    parents[0] = other.parents[0];
    parents[1] = other.parents[1];
    if (other.childCount > 0) {
        children = new int[other.childCount];
        childCapacity = other.childCount;
        for (int i = 0; i < other.childCount; ++i) children[i] = other.children[i];
        childCount = other.childCount;
    }
    std::cout << "[Конструктор копирования] Base (" << fullName << ")" << std::endl;
}

Base::~Base() {
    delete[] children;
    std::cout << "[Деструктор] Base (" << fullName << ")" << std::endl;
}

Base& Base::operator=(const Base& other) {
    if (this == &other) return *this;
    int* newChildren = nullptr;
    if (other.childCount > 0) {
        newChildren = new int[other.childCount];
        for (int i = 0; i < other.childCount; ++i) newChildren[i] = other.children[i];
    }
    delete[] children;
    children = newChildren;
    childCount = other.childCount;
    childCapacity = other.childCount;
    id = other.id;
    fullName = other.fullName;
    birthDate = other.birthDate;
    parents[0] = other.parents[0];
    parents[1] = other.parents[1];
    spouseId = other.spouseId;
    std::cout << "[Оператор =] Base (" << fullName << ")" << std::endl;
    return *this;
}

// ---------- get ----------
int Base::getId() const { return id; }
std::string Base::getFullName() const { return fullName; }
Date Base::getBirthDate() const { return birthDate; }
int Base::getSpouseId() const { return spouseId; }
int Base::getChildCount() const { return childCount; }

int Base::getParent(int index) const {
    if (index < 0 || index > 1)
        throw InvalidDataException("Номер родителя должен быть 0 или 1");
    return parents[index];
}

int Base::getChildId(int index) const {
    if (index < 0 || index >= childCount)
        throw InvalidDataException("Номер ребёнка вне диапазона");
    return children[index];
}

// ---------- set ----------
void Base::setId(int memberId) { id = memberId; }

void Base::setFullName(const std::string& name) {
    if (name.empty())
        throw InvalidDataException("ФИО не может быть пустым");
    fullName = name;
}

void Base::setBirthDate(const Date& d) {
    if (!isValidDate(d))
        throw InvalidDataException("Некорректная дата рождения (такой даты не существует)");
    if (isEarlier(currentDate(), d))
        throw InvalidDataException("Дата рождения не может быть в будущем");
    birthDate = d;
}

void Base::setParent(int index, int parentId) {
    if (index < 0 || index > 1)
        throw InvalidDataException("Номер родителя должен быть 0 или 1");
    parents[index] = parentId;
}

void Base::setSpouseId(int spouse) { spouseId = spouse; }

// ---------- дети ----------
bool Base::hasChild(int childId) const {
    for (int i = 0; i < childCount; ++i)
        if (children[i] == childId) return true;
    return false;
}

void Base::addChild(int childId) {
    if (hasChild(childId))
        throw InvalidDataException("Этот ребёнок уже добавлен");
    if (childCount == childCapacity) {
        int newCap = (childCapacity == 0) ? 4 : childCapacity * 2;
        int* bigger = new int[newCap];
        for (int i = 0; i < childCount; ++i) bigger[i] = children[i];
        delete[] children;
        children = bigger;
        childCapacity = newCap;
    }
    children[childCount++] = childId;
}

bool Base::removeChild(int childId) {
    for (int i = 0; i < childCount; ++i) {
        if (children[i] == childId) {
            for (int j = i; j < childCount - 1; ++j) children[j] = children[j + 1];
            --childCount;
            return true;
        }
    }
    return false;
}

// ---------- вывод / файл ----------
void Base::print(std::ostream& os) const {
    os << "ID " << id << " | " << fullName
        << " | родился(ась): " << birthDate
        << " | возраст: " << getAge();
}

void Base::saveTo(std::ostream& os) const {
    os << id << '\n';
    os << fullName << '\n';
    os << birthDate.day << ' ' << birthDate.month << ' ' << birthDate.year << '\n';
    os << parents[0] << ' ' << parents[1] << ' ' << spouseId << '\n';
    os << childCount;
    for (int i = 0; i < childCount; ++i) os << ' ' << children[i];
    os << '\n';
}

void Base::loadFrom(std::istream& is) {
    int newId = 0;
    std::string newName;
    Date newBirth = makeDate(1, 1, 2000);
    int newParents[2] = { -1, -1 };
    int newSpouse = -1;
    int newCount = 0;

    is >> newId;
    std::getline(is >> std::ws, newName);
    is >> newBirth >> newParents[0] >> newParents[1] >> newSpouse >> newCount;

    if (!is)
        throw FileException("Файл повреждён: не удалось прочитать данные члена семьи");
    if (newName.empty() || !isValidDate(newBirth) || newCount < 0 || newCount > 1000000)
        throw FileException("Файл повреждён: недопустимые данные члена семьи");

    int* newChildren = nullptr;
    if (newCount > 0) {
        newChildren = new int[newCount];
        for (int i = 0; i < newCount; ++i) is >> newChildren[i];
        if (!is) {
            delete[] newChildren;
            throw FileException("Файл повреждён: не удалось прочитать список детей");
        }
    }

    delete[] children;
    children = newChildren;
    childCount = newCount;
    childCapacity = newCount;
    id = newId;
    fullName = newName;
    birthDate = newBirth;
    parents[0] = newParents[0];
    parents[1] = newParents[1];
    spouseId = newSpouse;
}

bool Base::operator==(const Base& other) const {
    return fullName == other.fullName && birthDate == other.birthDate;
}

std::ostream& operator<<(std::ostream& os, const Base& b) {
    b.print(os);
    return os;
}