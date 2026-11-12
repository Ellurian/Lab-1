#include "Date.h"
#include <ctime>
#include <iomanip>

Date makeDate(int day, int month, int year) {
    Date d;
    d.day = day;
    d.month = month;
    d.year = year;
    return d;
}

Date currentDate() {
    std::time_t t = std::time(nullptr);
    std::tm local;
    localtime_s(&local, &t);
    return makeDate(local.tm_mday, local.tm_mon + 1, local.tm_year + 1900);
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int daysInMonth(int month, int year) {
    static const int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

bool isValidDate(const Date& d) {
    if (d.year < 1 || d.year > 9999) return false;
    if (d.month < 1 || d.month > 12) return false;
    return d.day >= 1 && d.day <= daysInMonth(d.month, d.year);
}

bool isEarlier(const Date& a, const Date& b) {
    if (a.year != b.year) return a.year < b.year;
    if (a.month != b.month) return a.month < b.month;
    return a.day < b.day;
}

bool operator==(const Date& a, const Date& b) {
    return a.day == b.day && a.month == b.month && a.year == b.year;
}

int calcAge(const Date& birth, const Date& onDate) {
    int age = onDate.year - birth.year;
    if (onDate.month < birth.month ||
        (onDate.month == birth.month && onDate.day < birth.day)) {
        age--;
    }
    return age;
}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << std::setfill('0') << std::setw(2) << d.day << '.'
        << std::setw(2) << d.month << '.'
        << std::setw(4) << d.year << std::setfill(' ');
    return os;
}

std::istream& operator>>(std::istream& is, Date& d) {
    is >> d.day >> d.month >> d.year;
    return is;
}