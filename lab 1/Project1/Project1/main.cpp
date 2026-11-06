#include <iostream>
#include <string>
#include <vector>
#include <locale>

using namespace std;

// Класс Vehicle (Часть) 
class Vehicle {
private:
    string model;   // модель
    int year;       // год выпуска
    string number;  // номер

    // Статическое поле — счётчик всех созданных транспортных средств
    static int vehicleCount;

public:
    // Конструктор
    Vehicle(const string& m, int y, const string& n)
        : model(m), year(y), number(n) {
        vehicleCount++;
        cout << "Создано ТС: \"" << model << "\" (" << year << "), номер \"" << number << "\"" << endl;
    }

    // Деструктор
    ~Vehicle() {
        vehicleCount--;
        cout << "Удалено ТС: \"" << model << "\" (" << number << ")" << endl;
    }

    // Геттеры
    string getModel() const { return model; }
    int getYear() const { return year; }
    string getNumber() const { return number; }

    // Статический метод
    static int getVehicleCount() {
        return vehicleCount;
    }

    // Вывод информации
    void print() const {
        cout << "  - " << model << ", " << year << " г.в., номер: " << number << endl;
    }
};

// Инициализация статического поля
int Vehicle::vehicleCount = 0;

//  Класс Fleet (Целое) 
class Fleet {
private:
    string name;
    vector<Vehicle*> vehicles; // агрегация: указатели

public:
    Fleet(const string& n) : name(n) {
        cout << "Создан автопарк \"" << name << "\"" << endl;
    }

    // Деструктор — ТС НЕ удаляются!
    ~Fleet() {
        cout << "Закрыт автопарк \"" << name << "\"" << endl;
    }

    // Добавление ТС (проверка на nullptr)
    void addVehicle(Vehicle* v) {
        if (v == nullptr) {
            cout << "Ошибка: попытка добавить nullptr!" << endl;
            return;
        }
        vehicles.push_back(v);
        cout << "ТС \"" << v->getModel() << "\" добавлено в автопарк" << endl;
    }

    // Вывод списка ТС
    void print() const {
        cout << "Автопарк \"" << name << "\" содержит " << vehicles.size() << " ТС:" << endl;
        for (const auto* v : vehicles) {
            if (v != nullptr) {
                v->print();
            }
        }
    }
};

//  main 
int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== Создание транспортных средств ===" << endl;
    Vehicle* v1 = new Vehicle("KAMAZ 5490", 2020, "A123BC77");
    Vehicle* v2 = new Vehicle("GAZelle Next", 2019, "B456EK50");
    Vehicle* v3 = new Vehicle("MAN TGX", 2021, "C789MH99");

    cout << "\nВсего ТС: " << Vehicle::getVehicleCount() << endl;

    cout << "\n=== Работа автопарка ===" << endl;
    {
        Fleet fleet("Северный");
        fleet.addVehicle(v1);
        fleet.addVehicle(v2);
        fleet.addVehicle(v3);
        fleet.print();
        fleet.addVehicle(nullptr);

        cout << "\n--- После закрытия автопарка ---" << endl;
        cout << "ТС всё ещё существует: " << Vehicle::getVehicleCount() << endl;
        cout << "Транспортные средства продолжают существовать:" << endl;
        v1->print();
        v2->print();
        v3->print();
    }

    cout << "\n=== Конец работы (ТС удалятся автоматически) ===" << endl;
    delete v1;
    delete v2;
    delete v3;

    cout << "\nНажмите Enter для выхода..." << endl;
    cin.get();
    return 0;
}