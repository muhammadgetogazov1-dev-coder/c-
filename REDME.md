МИНИCTEPCTBO НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ
Федеральное государственное автономное
образовательное учреждение высшего образования
«СЕВЕРО-КАВКАЗСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ»
Институт перспективной инженерии
Департамент цифровых, робототехнических систем и электроники
Межинститутская базовая кафедра 


Отчет по лабораторной работе № 8
Вариант 5
Тема: «Двоичный поиск. Хеш-таблицы»


Выполнил: 
Студент группы ПИЖ-б-о-25-2 (1),
направление подготовки: 09.03.04 «Программная инженерия»
Гетогазов Мухаммад Умарович 
Проверил: 
Ассистент межинститутской базовой кафедры ДЦРСиЭ 
Белик.А.И





Ставрополь 2026

Условие задачи 
1)double Сравнение с погрешностью
2)хеш-таблица (Метод разрешения коллизий-линейнок пробирование)(Тип ключа - string)(Тип значения - int)

Код main.cpp

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <windows.h>

using namespace std;
using namespace chrono;

// PART 1: 

const double EPS = 1e-9;

int compareDouble(double a, double b, double eps = EPS) {
    if (fabs(a - b) < eps) return 0;
    return (a < b) ? -1 : 1;
}

int binarySearchDouble(const vector<double>& arr, double target, double eps = EPS) {
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = compareDouble(arr[mid], target, eps);
        if (cmp == 0) return mid;
        if (cmp < 0)  left = mid + 1;
        else          right = mid - 1;
    }
    return -1;
}

int binarySearchDoubleRec(const vector<double>& arr, int left, int right,
    double target, double eps = EPS) {
    if (left > right) return -1;
    int mid = left + (right - left) / 2;
    int cmp = compareDouble(arr[mid], target, eps);
    if (cmp == 0) return mid;
    if (cmp < 0)  return binarySearchDoubleRec(arr, mid + 1, right, target, eps);
    return binarySearchDoubleRec(arr, left, mid - 1, target, eps);
}

void demoBinarySearchDouble() {
    cout << "\n========== PART 1: Binary search (double, EPS=1e-9) ==========\n";
    vector<double> arr = { 0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5 };
    sort(arr.begin(), arr.end());

    cout << "Array: ";
    for (double x : arr) cout << x << " ";
    cout << "\n\n";

    double targets[] = { 0.7, 1.3, 0.5000000001, 0.7000000005, 2.0 };
    cout << left << setw(20) << "target"
        << setw(10) << "iter"
        << setw(10) << "rec" << "comment\n";
    cout << string(60, '-') << "\n";

    for (double t : targets) {
        int it = binarySearchDouble(arr, t);
        int rec = binarySearchDoubleRec(arr, 0, (int)arr.size() - 1, t);

        string comment;
        if (it == -1) comment = "not found";
        else if (fabs(t - 0.7000000005) < 1e-15) comment = "EPS saved!";
        else comment = "found";

        cout << left << setw(20) << t
            << setw(10) << it
            << setw(10) << rec
            << comment << "\n";
    }
}


// PART 2: 

class HashTableLinear {
private:
    struct Entry {
        string key;
        int    value;
        bool   occupied;
        bool   deleted;
        Entry() : value(0), occupied(false), deleted(false) {}
    };

    vector<Entry> table;
    int capacity;
    int count;
    static constexpr double LOAD_FACTOR = 0.7;

    int hash(const string& key) const {
        unsigned long long h = 0;
        for (unsigned char c : key)
            h = (h * 31 + c) % capacity;
        return (int)h;
    }

    int probe(const string& key, bool forInsert) const {
        int idx = hash(key);
        int firstDeleted = -1;
        for (int i = 0; i < capacity; ++i) {
            int pos = (idx + i) % capacity;
            const Entry& e = table[pos];
            if (!e.occupied && !e.deleted)
                return (firstDeleted != -1) ? firstDeleted : pos;
            if (e.deleted) {
                if (firstDeleted == -1) firstDeleted = pos;
            }
            else if (e.key == key) {
                return pos;
            }
        }
        return firstDeleted;
    }

    void rehash() {
        vector<Entry> old = std::move(table);
        capacity *= 2;
        table.assign(capacity, Entry());
        count = 0;
        for (auto& e : old)
            if (e.occupied && !e.deleted) insert(e.key, e.value);
    }

public:
    HashTableLinear(int cap = 11) : capacity(cap), count(0) {
        table.assign(capacity, Entry());
    }

    void insert(const string& key, int value) {
        if ((double)(count + 1) / capacity > LOAD_FACTOR) rehash();
        int pos = probe(key, true);
        if (pos == -1) { rehash(); pos = probe(key, true); }
        Entry& e = table[pos];
        if (!e.occupied || e.deleted) ++count;
        e.key = key; e.value = value;
        e.occupied = true; e.deleted = false;
    }

    bool find(const string& key, int& result) const {
        int pos = probe(key, false);
        if (pos == -1) return false;
        const Entry& e = table[pos];
        if (e.occupied && !e.deleted && e.key == key) {
            result = e.value;
            return true;
        }
        return false;
    }

    bool remove(const string& key) {
        int pos = probe(key, false);
        if (pos == -1) return false;
        Entry& e = table[pos];
        if (e.occupied && !e.deleted && e.key == key) {
            e.deleted = true;
            --count;
            return true;
        }
        return false;
    }

    int size() const { return count; }

    void print() const {
        for (int i = 0; i < capacity; ++i) {
            const Entry& e = table[i];
            cout << "  " << i << ": ";
            if (e.occupied && !e.deleted)      cout << "(" << e.key << ", " << e.value << ")";
            else if (e.deleted)                cout << "<deleted>";
            else                               cout << "-";
            cout << "\n";
        }
    }
};

void demoHashTable() {
    cout << "\n========== PART 2: Hash table (string->int, linear probing) ==========\n";

    HashTableLinear ht(11);

    ht.insert("apple", 10);
    ht.insert("banana", 20);
    ht.insert("cherry", 30);
    ht.insert("date", 40);
    ht.insert("elder", 50);

    cout << "\nInitial table:\n";
    ht.print();

    int v = 0;
    cout << "\nFind 'cherry': ";
    if (ht.find("cherry", v)) cout << v << "\n";
    else                      cout << "not found\n";

    cout << "Find 'kiwi'  : ";
    if (ht.find("kiwi", v))   cout << v << "\n";
    else                      cout << "not found\n";

    ht.remove("banana");
    cout << "\nAfter removing 'banana' (note <deleted>):\n";
    ht.print();
    cout << "\nElement 'cherry' is still accessible: ";
    if (ht.find("cherry", v)) cout << v << "\n";
    else                      cout << "ERROR\n";

    ht.insert("fig", 60);
    cout << "\nAfter inserting 'fig' (slot <deleted> is reused):\n";
    ht.print();
}


// PART 3:

int binarySearchString(const vector<string>& arr, const string& target) {
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else                   right = mid - 1;
    }
    return -1;
}

void benchmark() {
    cout << "\n========== PART 3: Benchmark ==========\n";

    const int N = 100000;
    const int Q = 5000;

    vector<string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) keys.push_back("key_" + to_string(i));

    vector<string> sortedArr = keys;
    sort(sortedArr.begin(), sortedArr.end());

    HashTableLinear ht(2 * N);
    for (int i = 0; i < N; ++i) ht.insert(keys[i], i);

    vector<string> queries;
    mt19937 rng(42);
    for (int i = 0; i < Q; ++i) {
        if (i % 2 == 0) queries.push_back(keys[rng() % N]);
        else            queries.push_back("missing_" + to_string(rng() % N));
    }

    volatile int sink = 0;

    auto t1 = high_resolution_clock::now();
    for (const auto& q : queries) sink += binarySearchString(sortedArr, q);
    auto t2 = high_resolution_clock::now();
    auto binUs = duration_cast<microseconds>(t2 - t1).count();

    auto t3 = high_resolution_clock::now();
    for (const auto& q : queries) {
        int val;
        if (ht.find(q, val)) sink += val;
    }
    auto t4 = high_resolution_clock::now();
    auto hashUs = duration_cast<microseconds>(t4 - t3).count();

    cout << "Array size:          " << N << "\n";
    cout << "Number of queries:   " << Q << "\n\n";
    cout << "Binary search O(log n): " << binUs << " us\n";
    cout << "Hash table O(1) avg:    " << hashUs << " us\n";

    if (hashUs > 0)
        cout << "Hash table is faster by " << fixed << setprecision(2)
        << (double)binUs / hashUs << " times.\n";
}

// MAIN

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    demoBinarySearchDouble();
    demoHashTable();
    benchmark();

    cout << "\nDone! Press Enter to exit...";
    cin.get();
    return 0;
}

Вывод:
========== PART 1: Binary search (double, EPS=1e-9) ==========
Array: 0.1 0.3 0.5 0.7 0.9 1.1 1.3 1.5

target              iter      rec       comment
------------------------------------------------------------
0.7                 3         3         found
1.3                 6         6         found
0.5                 2         2         found
0.7                 3         3         EPS saved!
2                   -1        -1        not found

========== PART 2: Hash table (string->int, linear probing) ==========

Initial table:
  0: -
  1: (banana, 20)
  2: -
  3: -
  4: -
  5: -
  6: (cherry, 30)
  7: (date, 40)
  8: (elder, 50)
  9: -
  10: (apple, 10)

Контрольные вопросы 
1. Какое условие необходимо для применения двоичного поиска?
Массив должен быть отсортирован (по возрастанию или убыванию). Также нужен произвольный доступ к элементам по индексу (например, массив или vector, но не связный список).

2. Какова сложность двоичного поиска? Почему она логарифмическая?
Сложность — O(log n). На каждом шаге алгоритм отбрасывает половину оставшихся элементов. Количество шагов — это степень, в которую нужно возвести 2, чтобы получить n (то есть логарифм по основанию 2).

3. В чём разница между итеративной и рекурсивной реализацией?
Итеративная использует цикл while и занимает O(1) дополнительной памяти. Рекурсивная вызывает саму себя и занимает O(log n) памяти под стек вызовов. Итеративная работает чуть быстрее за счет отсутствия накладных расходов на вызовы функций.

4. Чем опасен вариант mid = (left + right) / 2? Как правильно вычислять середину?
При очень больших значениях left и right (близких к максимуму типа int) их сумма может вызвать переполнение и стать отрицательной. Правильно: mid = left + (right - left) / 2.

5. Что такое хеш-таблица? Из каких компонентов она состоит?
Это структура данных для хранения пар «ключ-значение» с доступом в среднем за O(1). Состоит из:

Массива (бакетов).

Хеш-функции (преобразует ключ в индекс).

Механизма разрешения коллизий (цепочки или открытая адресация).

6. Какие требования предъявляются к хеш-функции?

Детерминированность (один и тот же ключ всегда дает один и тот же хеш).

Равномерное распределение (ключи должны распределяться по бакетам равномерно, чтобы избежать коллизий).

Быстрое вычисление (O(1), чтобы не убить всю скорость таблицы).

7. Что такое коллизия в хеш-таблице? Как с ней бороться?
Это ситуация, когда два разных ключа дают одинаковый хеш (и попадают в один индекс). Борются методами разрешения коллизий: метод цепочек (хранение списков в ячейках) и открытая адресация (поиск следующей свободной ячейки).

8. В чём отличие метода цепочек от открытой адресации?

Цепочки: в каждой ячейке массива хранится указатель на связный список. Элементы хранятся вне массива.

Открытая адресация: все элементы хранятся прямо в массиве. Если ячейка занята, алгоритм ищет следующую свободную по определенному правилу (пробированию).

9. Что такое первичная и вторичная кластеризация?

Первичная — возникает при линейном пробировании, когда занятые ячейки сливаются в длинные непрерывные блоки, из-за чего поиск и вставка сильно замедляются.

Вторичная — возникает при квадратичном пробировании, когда ключи с одинаковым начальным хешем образуют свои отдельные группы (кластеры).

10. Какова средняя и худшая сложность операций в хеш-таблице?

Средняя: Вставка, поиск и удаление — O(1).

Худшая: O(n). Это происходит, если все ключи попали в одну ячейку (плохая хеш-функция) или таблица переполнена.

11. Когда эффективнее использовать двоичный поиск, а когда - хеш-таблицу?

Двоичный поиск: когда данные статичны (редко меняются), уже отсортированы, и важна экономия памяти.

Хеш-таблица: когда нужны частые операции вставки, удаления и поиска, и скорость важнее дополнительных расходов памяти.

12. Что такое коэффициент загрузки хеш-таблицы? Почему он важен?
Это отношение количества элементов к размеру таблицы (load factor = n / capacity). Он показывает, насколько таблица заполнена. Если коэффициент слишком высокий (например, > 0.7 для открытой адресации), резко возрастает количество коллизий и время поиска, поэтому нужно проводить рехеширование.

13. Как работает рехеширование (динамическое расширение хеш-таблицы)?
Когда коэффициент загрузки превышает порог, создается новый массив большего размера (обычно в 2 раза). Затем все старые элементы по одному пересчитываются (по новой хеш-функции от нового размера) и вставляются в новую таблицу. Старая память очищается.

14. Как выбрать размер хеш-таблицы? Почему часто выбирают простое число?
Размер выбирают так, чтобы он был больше ожидаемого количества элементов (для низкого load factor). Часто берут простое число, чтобы минимизировать коллизии при использовании простой хеш-функции вида key % size (у простого числа меньше общих делителей с ключами).

15. Как реализовать хеш-функцию для строк?
Проще всего использовать полиномиальный хеш. Идем по строке, и на каждом шаге умножаем текущий хеш на некое простое число (например, 31) и прибавляем код текущего символа:
hash = (hash * 31 + c) % table_size;


