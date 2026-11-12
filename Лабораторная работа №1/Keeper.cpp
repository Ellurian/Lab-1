#include "Keeper.h"
#include "AliveMember.h"
#include "DeceasedMember.h"
#include "Exceptions.h"
#include <fstream>
#include <utility>

Keeper::Keeper() : items(new Base* [4]), count(0), capacity(4), nextId(1) {
    std::cout << "[Конструктор] Keeper без параметров" << std::endl;
}

Keeper::Keeper(int initialCapacity) : items(nullptr), count(0), capacity(0), nextId(1) {
    if (initialCapacity <= 0)
        throw InvalidDataException("Начальная вместимость контейнера должна быть больше нуля");
    items = new Base * [initialCapacity];
    capacity = initialCapacity;
    std::cout << "[Конструктор] Keeper с параметрами (вместимость " << capacity << ")" << std::endl;
}

Keeper::Keeper(const Keeper& other)
    : items(new Base* [other.capacity]), count(0), capacity(other.capacity), nextId(other.nextId) {
    for (int i = 0; i < other.count; ++i) {
        items[i] = other.items[i]->clone();
        ++count;
    }
    std::cout << "[Конструктор копирования] Keeper" << std::endl;
}

Keeper::~Keeper() {
    clear();
    delete[] items;
    std::cout << "[Деструктор] Keeper" << std::endl;
}

Keeper& Keeper::operator=(const Keeper& other) {
    if (this == &other) return *this;
    Base** newItems = new Base * [other.capacity];
    int made = 0;
    try {
        for (; made < other.count; ++made) newItems[made] = other.items[made]->clone();
    }
    catch (...) {
        for (int i = 0; i < made; ++i) delete newItems[i];
        delete[] newItems;
        throw;
    }
    clear();
    delete[] items;
    items = newItems;
    count = other.count;
    capacity = other.capacity;
    nextId = other.nextId;
    std::cout << "[Оператор =] Keeper" << std::endl;
    return *this;
}

// ---------- служебные ----------
void Keeper::grow() {
    int newCap = capacity * 2;
    Base** bigger = new Base * [newCap];
    for (int i = 0; i < count; ++i) bigger[i] = items[i];
    delete[] items;
    items = bigger;
    capacity = newCap;
}

void Keeper::pushBack(Base* member) {
    if (count == capacity) grow();
    items[count++] = member;
}

int Keeper::indexOfId(int id) const {
    for (int i = 0; i < count; ++i)
        if (items[i]->getId() == id) return i;
    return -1;
}

std::string Keeper::describeId(int id) const {
    if (id < 0) return "не указан";
    int idx = indexOfId(id);
    if (idx < 0) return "неизвестен (ID " + std::to_string(id) + ")";
    return items[idx]->getFullName() + " (ID " + std::to_string(id) + ")";
}

// ---------- доступ ----------
int Keeper::getCount() const { return count; }

Base* Keeper::getByIndex(int index) const {
    if (index < 0 || index >= count)
        throw InvalidDataException("Индекс вне диапазона контейнера");
    return items[index];
}

Base* Keeper::operator[](int index) const { return getByIndex(index); }

Base* Keeper::findById(int id) const {
    int idx = indexOfId(id);
    if (idx < 0)
        throw NotFoundException("Член семьи с ID " + std::to_string(id) + " не найден");
    return items[idx];
}

// ---------- добавление / удаление ----------
int Keeper::add(Base* member) {
    if (member == nullptr)
        throw InvalidDataException("Нельзя добавить пустой объект");
    for (int i = 0; i < count; ++i) {
        if (*items[i] == *member) {
            delete member;
            throw InvalidDataException("Такой человек (то же ФИО и дата рождения) уже есть в дереве");
        }
    }
    member->setId(nextId++);
    pushBack(member);
    return member->getId();
}

void Keeper::removeById(int id) {
    int idx = indexOfId(id);
    if (idx < 0)
        throw NotFoundException("Член семьи с ID " + std::to_string(id) + " не найден");

    // убираем все ссылки на удаляемого человека у остальных
    for (int i = 0; i < count; ++i) {
        if (i == idx) continue;
        Base* m = items[i];
        if (m->getParent(0) == id) m->setParent(0, -1);
        if (m->getParent(1) == id) m->setParent(1, -1);
        if (m->getSpouseId() == id) m->setSpouseId(-1);
        m->removeChild(id);
    }
    delete items[idx];
    for (int i = idx; i < count - 1; ++i) items[i] = items[i + 1];
    --count;
}

void Keeper::clear() {
    for (int i = 0; i < count; ++i) delete items[i];
    count = 0;
}

// ---------- связи ----------
void Keeper::setParent(int childId, int parentId) {
    Base* child = findById(childId);
    Base* parent = findById(parentId);
    if (childId == parentId)
        throw InvalidDataException("Человек не может быть родителем самому себе");
    if (child->getParent(0) == parentId || child->getParent(1) == parentId)
        throw InvalidDataException("Этот родитель уже указан");
    if (!isEarlier(parent->getBirthDate(), child->getBirthDate()))
        throw InvalidDataException("Родитель должен быть старше ребёнка");

    int slot;
    if (child->getParent(0) == -1) slot = 0;
    else if (child->getParent(1) == -1) slot = 1;
    else throw InvalidDataException("У этого человека уже указаны два родителя");

    child->setParent(slot, parentId);
    parent->addChild(childId);
}

void Keeper::setSpouse(int firstId, int secondId) {
    Base* a = findById(firstId);
    Base* b = findById(secondId);
    if (firstId == secondId)
        throw InvalidDataException("Человек не может быть супругом самому себе");
    if (a->getSpouseId() != -1 || b->getSpouseId() != -1)
        throw InvalidDataException("У одного из людей уже указан супруг (сначала уберите его)");
    a->setSpouseId(secondId);
    b->setSpouseId(firstId);
}

void Keeper::removeSpouse(int id) {
    Base* a = findById(id);
    if (a->getSpouseId() == -1)
        throw InvalidDataException("У этого человека супруг не указан");
    int otherId = a->getSpouseId();
    int idx = indexOfId(otherId);
    if (idx >= 0) items[idx]->setSpouseId(-1);
    a->setSpouseId(-1);
}

void Keeper::markDeceased(int id, const Date& deathDate) {
    int idx = indexOfId(id);
    if (idx < 0)
        throw NotFoundException("Член семьи с ID " + std::to_string(id) + " не найден");
    if (items[idx]->getType() == 2)
        throw InvalidDataException("Этот человек уже отмечен как умерший");

    DeceasedMember* d = new DeceasedMember();
    try {
        static_cast<Base&>(*d) = *items[idx];   // копируем все общие данные и связи
        d->setDeathDate(deathDate);
    }
    catch (...) {
        delete d;
        throw;
    }
    delete items[idx];
    items[idx] = d;
}

// ---------- вывод ----------
void Keeper::printAll(std::ostream& os) const {
    if (count == 0) {
        os << "Дерево пусто.\n";
        return;
    }
    for (int i = 0; i < count; ++i) os << *items[i] << '\n';
}

void Keeper::printDetails(int id, std::ostream& os) const {
    const Base* m = findById(id);
    os << *m << '\n';
    os << "  Родитель 1: " << describeId(m->getParent(0)) << '\n';
    os << "  Родитель 2: " << describeId(m->getParent(1)) << '\n';
    os << "  Супруг(а):  " << describeId(m->getSpouseId()) << '\n';
    os << "  Дети:       ";
    if (m->getChildCount() == 0) {
        os << "нет";
    }
    else {
        for (int i = 0; i < m->getChildCount(); ++i) {
            if (i > 0) os << ", ";
            os << describeId(m->getChildId(i));
        }
    }
    os << '\n';
}

// ---------- файл ----------
void Keeper::saveToFile(const std::string& fileName) const {
    std::ofstream out(fileName);
    if (!out)
        throw FileException("Не удалось открыть файл для записи: " + fileName);
    out << "FAMILYTREE_V1\n" << count << '\n';
    for (int i = 0; i < count; ++i) {
        out << items[i]->getType() << '\n';
        items[i]->saveTo(out);
    }
    out.flush();
    if (!out)
        throw FileException("Ошибка при записи в файл: " + fileName);
}

void Keeper::loadFromFile(const std::string& fileName) {
    std::ifstream in(fileName);
    if (!in)
        throw FileException("Не удалось открыть файл для чтения: " + fileName);

    std::string header;
    std::getline(in, header);
    if (header != "FAMILYTREE_V1")
        throw FileException("Неверный формат файла (нет заголовка FAMILYTREE_V1)");

    int total = 0;
    in >> total;
    if (!in || total < 0 || total > 1000000)
        throw FileException("Файл повреждён: неверное количество членов семьи");

    Keeper tmp;   // читаем во временный контейнер, чтобы при ошибке не испортить текущие данные
    for (int i = 0; i < total; ++i) {
        int type = 0;
        in >> type;
        if (!in)
            throw FileException("Файл повреждён: не прочитан тип записи №" + std::to_string(i + 1));

        Base* m = nullptr;
        if (type == 1) m = new AliveMember();
        else if (type == 2) m = new DeceasedMember();
        else throw FileException("Файл повреждён: неизвестный тип записи " + std::to_string(type));

        try {
            m->loadFrom(in);
        }
        catch (...) {
            delete m;
            throw;
        }
        if (tmp.indexOfId(m->getId()) != -1) {
            int dupId = m->getId();
            delete m;
            throw FileException("Файл повреждён: повторяется ID " + std::to_string(dupId));
        }
        tmp.pushBack(m);
    }

    // проверяем, что все связи ведут на существующих людей
    int maxId = 0;
    for (int i = 0; i < tmp.count; ++i) {
        Base* m = tmp.items[i];
        if (m->getId() > maxId) maxId = m->getId();
        for (int k = 0; k < 2; ++k)
            if (m->getParent(k) != -1 && tmp.indexOfId(m->getParent(k)) == -1)
                throw FileException("Файл повреждён: ссылка на несуществующего родителя");
        if (m->getSpouseId() != -1 && tmp.indexOfId(m->getSpouseId()) == -1)
            throw FileException("Файл повреждён: ссылка на несуществующего супруга");
        for (int c = 0; c < m->getChildCount(); ++c)
            if (tmp.indexOfId(m->getChildId(c)) == -1)
                throw FileException("Файл повреждён: ссылка на несуществующего ребёнка");
    }
    tmp.nextId = maxId + 1;

    // всё хорошо: забираем данные временного контейнера
    std::swap(items, tmp.items);
    std::swap(count, tmp.count);
    std::swap(capacity, tmp.capacity);
    std::swap(nextId, tmp.nextId);
}