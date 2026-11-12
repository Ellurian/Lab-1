#pragma once
#include <iostream>

// Простая структура даты (вспомогательная, не входит в иерархию классов)
struct Date {
    int day;
    int month;
    int year;
};

Date makeDate(int day, int month, int year);
Date currentDate();
bool isLeapYear(int year);
bool isValidDate(const Date& d);
bool isEarlier(const Date& a, const Date& b);   // a раньше b
int calcAge(const Date& birth, const Date& onDate);

bool operator==(const Date& a, const Date& b);
std::ostream& operator<<(std::ostream& os, const Date& d);
std::istream& operator>>(std::istream& is, Date& d);  // читает три числа: день месяц год