#pragma once
#include <iostream>
#include <string>
#include "Base.h"

// Контейнер: хранит членов семьи (фамильное дерево)
class Keeper {
private:
    Base** items;     // динамический массив указателей на Base
    int count;
    int capacity;
    int nextId;

    void grow();
    void pushBack(Base* member);
    int indexOfId(int id) const;
    std::string describeId(int id) const;

public:
    Keeper();
    explicit Keeper(int initialCapacity);
    Keeper(const Keeper& other);
    ~Keeper();
    Keeper& operator=(const Keeper& other);

    int getCount() const;
    Base* getByIndex(int index) const;
    Base* operator[](int index) const;
    Base* findById(int id) const;

    int add(Base* member);               // контейнер забирает объект себе, возвращает ID
    void removeById(int id);
    void clear();

    void setParent(int childId, int parentId);
    void setSpouse(int firstId, int secondId);
    void removeSpouse(int id);
    void markDeceased(int id, const Date& deathDate);

    void printAll(std::ostream& os) const;
    void printDetails(int id, std::ostream& os) const;

    void saveToFile(const std::string& fileName) const;
    void loadFromFile(const std::string& fileName);
};