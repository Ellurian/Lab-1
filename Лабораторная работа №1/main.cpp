#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include "Keeper.h"
#include "AliveMember.h"
#include "DeceasedMember.h"
#include "Exceptions.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// ---------- ввод с клавиатуры ----------
int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) {
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return v;
        }
        if (cin.eof()) std::exit(0);
        cin.clear();
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        cout << "Ошибка: нужно ввести целое число. Попробуйте ещё раз.\n";
    }
}

string readLine(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (!std::getline(cin, s)) std::exit(0);
        if (!s.empty()) return s;
        cout << "Ошибка: строка не должна быть пустой.\n";
    }
}

Date readDate(const string& title) {
    while (true) {
        cout << title << endl;
        int d = readInt("  День: ");
        int m = readInt("  Месяц: ");
        int y = readInt("  Год: ");
        Date date = makeDate(d, m, y);
        if (isValidDate(date)) return date;
        cout << "Ошибка: такой даты не существует.\n";
    }
}

// ---------- действия меню ----------
void addMember(Keeper& keeper, bool deceased) {
    string name = readLine("ФИО: ");
    Date birth = readDate("Дата рождения:");
    Base* m = nullptr;
    if (deceased) {
        Date death = readDate("Дата смерти:");
        m = new DeceasedMember(0, name, birth, death);
    }
    else {
        m = new AliveMember(0, name, birth);
    }
    int id = keeper.add(m);
    cout << "Добавлено. Присвоен ID " << id << endl;
}

void removeMember(Keeper& keeper) {
    int id = readInt("ID удаляемого: ");
    keeper.removeById(id);
    cout << "Удалено." << endl;
}

void showMemberDetails(Keeper& keeper) {
    int id = readInt("ID: ");
    keeper.printDetails(id, cout);
}

void editName(Keeper& keeper) {
    int id = readInt("ID: ");
    Base* m = keeper.findById(id);
    m->setFullName(readLine("Новое ФИО: "));
    cout << "Изменено." << endl;
}

void editBirth(Keeper& keeper) {
    int id = readInt("ID: ");
    Base* m = keeper.findById(id);
    m->setBirthDate(readDate("Новая дата рождения:"));
    cout << "Изменено." << endl;
}

void editDeath(Keeper& keeper) {
    int id = readInt("ID: ");
    DeceasedMember* d = dynamic_cast<DeceasedMember*>(keeper.findById(id));
    if (d == nullptr)
        throw InvalidDataException("Этот человек не отмечен как умерший (используйте пункт 9)");
    d->setDeathDate(readDate("Новая дата смерти:"));
    cout << "Изменено." << endl;
}

void markDeceased(Keeper& keeper) {
    int id = readInt("ID: ");
    keeper.markDeceased(id, readDate("Дата смерти:"));
    cout << "Человек отмечен как умерший." << endl;
}

void setParent(Keeper& keeper) {
    int child = readInt("ID ребёнка: ");
    int parent = readInt("ID родителя: ");
    keeper.setParent(child, parent);
    cout << "Связь родитель-ребёнок установлена." << endl;
}

void setSpouse(Keeper& keeper) {
    int a = readInt("ID первого супруга: ");
    int b = readInt("ID второго супруга: ");
    keeper.setSpouse(a, b);
    cout << "Брак зарегистрирован." << endl;
}

void removeSpouse(Keeper& keeper) {
    int id = readInt("ID человека: ");
    keeper.removeSpouse(id);
    cout << "Связь супругов удалена." << endl;
}

void saveFile(Keeper& keeper) {
    string name = readLine("Имя файла для сохранения (например tree.txt): ");
    keeper.saveToFile(name);
    cout << "Сохранено в файл " << name << endl;
}

void loadFile(Keeper& keeper) {
    string name = readLine("Имя файла для загрузки: ");
    keeper.loadFromFile(name);
    cout << "Загружено. Членов семьи: " << keeper.getCount() << endl;
}

void copyDemo(Keeper& keeper) {
    int id = readInt("ID человека для создания копии: ");
    Base* copy = keeper.findById(id)->clone();   // вызов конструктора копирования
    cout << "Копия: " << *copy << endl;
    delete copy;                                  // вызов деструктора
}

void printMenu() {
    cout << "\n========= ФАМИЛЬНОЕ ДЕРЕВО =========\n"
        << " 1. Добавить живого человека\n"
        << " 2. Добавить умершего человека\n"
        << " 3. Удалить человека\n"
        << " 4. Показать всех\n"
        << " 5. Подробно о человеке (родители, супруг, дети)\n"
        << " 6. Изменить ФИО\n"
        << " 7. Изменить дату рождения\n"
        << " 8. Изменить дату смерти (умершему)\n"
        << " 9. Отметить человека как умершего\n"
        << "10. Указать родителя\n"
        << "11. Указать супругов\n"
        << "12. Убрать супруга\n"
        << "13. Сохранить дерево в файл\n"
        << "14. Загрузить дерево из файла\n"
        << "15. Демонстрация копирования объекта\n"
        << " 0. Выход\n"
        << "====================================\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Keeper keeper;
    int choice = -1;

    while (choice != 0) {
        printMenu();
        choice = readInt("Ваш выбор: ");
        try {
            switch (choice) {
            case 1:  addMember(keeper, false); break;
            case 2:  addMember(keeper, true); break;
            case 3:  removeMember(keeper); break;
            case 4:  keeper.printAll(cout); break;
            case 5:  showMemberDetails(keeper); break;
            case 6:  editName(keeper); break;
            case 7:  editBirth(keeper); break;
            case 8:  editDeath(keeper); break;
            case 9:  markDeceased(keeper); break;
            case 10: setParent(keeper); break;
            case 11: setSpouse(keeper); break;
            case 12: removeSpouse(keeper); break;
            case 13: saveFile(keeper); break;
            case 14: loadFile(keeper); break;
            case 15: copyDemo(keeper); break;
            case 0:  cout << "Выход из программы." << endl; break;
            default: cout << "Нет такого пункта меню." << endl; break;
            }
        }
        catch (const FamilyException& e) {
            cout << "\n[ОШИБКА] " << e.what() << endl;
        }
        catch (const std::exception& e) {
            cout << "\n[СИСТЕМНАЯ ОШИБКА] " << e.what() << endl;
        }
    }
    return 0;
}