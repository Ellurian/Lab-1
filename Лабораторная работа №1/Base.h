#pragma once
#include <iostream>
#include <string>
#include "Date.h"

// Абстрактный базовый класс: член семьи
class Base {
protected:
    int id;                 // уникальный номер члена семьи
    std::string fullName;   // ФИО
    Date birthDate;         // дата рождения
    int parents[2];         // ID родителей (-1 = не указан)
    int spouseId;           // ID супруга (-1 = нет)
    int* children;          // динамический массив ID детей
    int childCount;
    int childCapacity;

public:
    Base();
    Base(int memberId, const std::string& name, const Date& birth);
    Base(const Base& other);
    virtual ~Base();
    Base& operator=(const Base& other);

    // get
    int getId() const;
    std::string getFullName() const;
    Date getBirthDate() const;
    int getParent(int index) const;
    int getSpouseId() const;
    int getChildCount() const;
    int getChildId(int index) const;

    // set
    void setId(int memberId);
    void setFullName(const std::string& name);
    virtual void setBirthDate(const Date& d);
    void setParent(int index, int parentId);
    void setSpouseId(int id);

    // дети
    void addChild(int childId);
    bool removeChild(int childId);
    bool hasChild(int childId) const;

    // чисто виртуальные методы (делают класс абстрактным)
    virtual int getType() const = 0;            // 1 = живой, 2 = умерший
    virtual int getAge() const = 0;
    virtual const char* getTypeName() const = 0;
    virtual Base* clone() const = 0;

    // виртуальные методы с реализацией по умолчанию
    virtual void print(std::ostream& os) const;
    virtual void saveTo(std::ostream& os) const;
    virtual void loadFrom(std::istream& is);

    bool operator==(const Base& other) const;   // один и тот же человек: ФИО + дата рождения
};

std::ostream& operator<<(std::ostream& os, const Base& b);