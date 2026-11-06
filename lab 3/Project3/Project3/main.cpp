#include <iostream>
#include <stdexcept>
#include <utility>
#include <algorithm>

template <typename T>
class Vector {
private:
    T* data;
    int sz;
    int cap;

public:
    // ==========================================
    // 1. Конструкторы
    // ==========================================

    // По умолчанию
    Vector() : data(nullptr), sz(0), cap(0) {}

    // С параметром (размер)
    explicit Vector(int n) : sz(n), cap(n) {
        if (n < 0) throw std::invalid_argument("Size cannot be negative");
        data = new T[cap];
        for (int i = 0; i < sz; ++i) {
            data[i] = T();
        }
    }

    // С параметром (размер, значение)
    Vector(int n, const T& val) : sz(n), cap(n) {
        if (n < 0) throw std::invalid_argument("Size cannot be negative");
        data = new T[cap];
        for (int i = 0; i < sz; ++i) {
            data[i] = val;
        }
    }

    // ==========================================
    // 2. Правило пяти
    // ==========================================

    // Деструктор
    ~Vector() {
        delete[] data;
    }

    // Конструктор копирования (глубокое копирование)
    Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (int i = 0; i < sz; ++i) {
            data[i] = other.data[i];
        }
    }

    // Оператор присваивания копированием (copy-and-swap idiom)
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other); // копируем во временный объект
            swap(temp);         // обмениваемся
        }
        return *this;
    }

    // Конструктор перемещения
    Vector(Vector&& other) noexcept
        : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Оператор присваивания перемещением
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;

            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    // ==========================================
    // 3. Методы доступа
    // ==========================================
    int size() const { return sz; }
    int capacity() const { return cap; }
    bool empty() const { return sz == 0; }

    T& operator[](int index) {
        if (index < 0 || index >= sz) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= sz) throw std::out_of_range("Index out of range");
        return data[index];
    }

    // ==========================================
    // 4. Методы изменения
    // ==========================================

    void reserve(int newCap) {
        if (newCap <= cap) return;
        T* newData = new T[newCap];
        for (int i = 0; i < sz; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

    void push_back(const T& value) {
        if (sz >= cap) {
            int newCap = (cap == 0) ? 1 : cap * 2;
            reserve(newCap);
        }
        data[sz++] = value;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Vector is empty");
        sz--;
    }

    void clear() {
        sz = 0;
    }

    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

    // ==========================================
    // 5. Итераторы (для range-based for)
    // ==========================================
    T* begin() { return data; }
    T* end() { return data + sz; }
    const T* begin() const { return data; }
    const T* end() const { return data + sz; }

    // ==========================================
    // 6. Дополнительная функциональность (Вариант 6)
    // ==========================================

    // Удаление всех вхождений значения
    void removeAll(const T& value) {
        int writeIndex = 0;
        for (int readIndex = 0; readIndex < sz; ++readIndex) {
            if (data[readIndex] != value) {
                data[writeIndex] = data[readIndex];
                writeIndex++;
            }
        }
        sz = writeIndex; // Обновляем размер
    }

    // Вспомогательный метод для вывода
    void print() const {
        std::cout << "[";
        for (int i = 0; i < sz; ++i) {
            std::cout << data[i];
            if (i < sz - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

// ==========================================
// Демонстрация работы
// ==========================================
int main() {
    setlocale(LC_ALL, "ru");

    std::cout << "=== Демонстрация работы Vector (Вариант 6) ===\n\n";

    Vector<int> v;
    std::cout << "1. Добавление элементов (push_back):\n";
    for (int i = 1; i <= 8; ++i) {
        v.push_back(i % 3 + 1); // Добавляем: 2, 3, 1, 2, 3, 1, 2, 3
    }
    std::cout << "Исходный вектор: ";
    v.print();

    std::cout << "\n2. Тест removeAll (удаляем все '2'):\n";
    v.removeAll(2);
    std::cout << "Вектор после removeAll(2): ";
    v.print();

    std::cout << "\n3. Тест правила пяти:\n";
    Vector<int> v_copy(v); // Конструктор копирования
    std::cout << "Копия: ";
    v_copy.print();

    Vector<int> v_move(std::move(v)); // Конструктор перемещения
    std::cout << "Перемещенный вектор: ";
    v_move.print();
    std::cout << "Исходный вектор после перемещения (пуст): size = " << v.size() << "\n";

    std::cout << "\n4. Обработка выхода за границы:\n";
    try {
        std::cout << v_move[100] << "\n";
    }
    catch (const std::out_of_range& e) {
        std::cout << "Перехвачено исключение: " << e.what() << "\n";
    }

    return 0;
}