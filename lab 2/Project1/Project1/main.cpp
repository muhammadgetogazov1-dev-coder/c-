#include <iostream>
#include <chrono>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// ==========================================
// БАЗОВЫЙ УРОВЕНЬ: Шаблонная функция
// Вариант 6: T average(T arr[], int n)
// Сложность: O(n), так как происходит один цикл по массиву
// ==========================================
template <typename T>
T average(T arr[], int n) {
    if (n <= 0) throw invalid_argument("Array size must be > 0");
    T sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum / n;
}

// ==========================================
// СРЕДНИЙ УРОВЕНЬ: Шаблонный класс Nullable<T>
// Вариант 6
// ==========================================
template <typename T>
class Nullable {
private:
    T value;
    bool hasVal;

public:
    // Конструктор по умолчанию (без значения) - O(1)
    Nullable() : hasVal(false) {}

    // Конструктор со значением - O(1)
    Nullable(T val) : value(val), hasVal(true) {}

    // Проверка наличия значения - O(1)
    bool hasValue() const {
        return hasVal;
    }

    // Получение значения - O(1)
    T getValue() const {
        if (!hasVal) {
            throw runtime_error("Value is not set");
        }
        return value;
    }

    // Сброс значения - O(1)
    void reset() {
        hasVal = false;
    }
};


template <typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

// Линейный поиск O(n)
template <typename T>
int linearSearch(const vector<T>& arr, const T& value) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == value) return i;
    }
    return -1;
}

// Бинарный поиск O(log n)
template <typename T>
int binarySearch(const vector<T>& arr, const T& value) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == value) return mid;
        if (arr[mid] < value) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main() {
    setlocale(LC_ALL, "ru");

    // --- Тестирование шаблонной функции ---
    cout << "=== Тестирование шаблонной функции average ===\n";
    int intArr[] = { 1, 2, 3, 4, 5 };
    double doubleArr[] = { 1.5, 2.5, 3.5 };

    cout << "Среднее (int): " << average(intArr, 5) << endl;
    cout << "Среднее (double): " << average(doubleArr, 3) << endl;

    // --- Тестирование шаблонного класса ---
    cout << "\n=== Тестирование шаблонного класса Nullable ===\n";
    Nullable<int> n1;
    cout << "n1.hasValue(): " << (n1.hasValue() ? "true" : "false") << endl;

    Nullable<int> n2(42);
    cout << "n2.hasValue(): " << (n2.hasValue() ? "true" : "false") << endl;
    cout << "n2.getValue(): " << n2.getValue() << endl;

    n2.reset();
    cout << "После reset, n2.hasValue(): " << (n2.hasValue() ? "true" : "false") << endl;

    // --- Экспериментальное сравнение (Линейный vs Бинарный поиск) ---
    cout << "\n=== Сравнение линейного и бинарного поиска ===\n";
    cout << "Размер массива\tЛинейный (мкс)\tБинарный (мкс)\n";

    vector<int> sizes = { 10000, 100000, 500000, 1000000 };

    for (int n : sizes) {
        vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = i;
        int target = n - 1; // Ищем последний элемент (худший случай для линейного)

        long long linearTime = measureTime([&]() {
            linearSearch(arr, target);
            });

        long long binaryTime = measureTime([&]() {
            binarySearch(arr, target);
            });

        cout << n << "\t\t" << linearTime << "\t\t" << binaryTime << "\n";
    }

    return 0;
}