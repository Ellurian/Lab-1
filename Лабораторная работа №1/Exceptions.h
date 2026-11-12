#pragma once
#include <exception>
#include <string>

// Базовое исключение программы: хранит поясняющий текст
class FamilyException : public std::exception {
protected:
    std::string message;
public:
    explicit FamilyException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

// Неверные данные (пустое ФИО, несуществующая дата и т.п.)
class InvalidDataException : public FamilyException {
public:
    explicit InvalidDataException(const std::string& msg) : FamilyException(msg) {}
};

// Объект не найден в контейнере
class NotFoundException : public FamilyException {
public:
    explicit NotFoundException(const std::string& msg) : FamilyException(msg) {}
};

// Ошибка работы с файлом
class FileException : public FamilyException {
public:
    explicit FileException(const std::string& msg) : FamilyException(msg) {}
};