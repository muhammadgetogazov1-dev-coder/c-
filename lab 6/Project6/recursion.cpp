#include <iostream>
#include <clocale>
using namespace std;

// ===== 1. Факториал =====
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// ===== 2. Числа Фибоначчи =====
long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

// ===== 3. Максимум в массиве =====
int findMax(int arr[], int n) {
    if (n == 1) return arr[0];
    int maxOfRest = findMax(arr, n - 1);
    return (arr[n - 1] > maxOfRest) ? arr[n - 1] : maxOfRest;
}

// ===== 4. Ханойские башни =====
long long hanoiMoves = 0;
void hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        cout << "Переместить диск 1 с " << from << " на " << to << "\n";
        ++hanoiMoves;
        return;
    }
    hanoi(n - 1, from, aux, to);
    cout << "Переместить диск " << n << " с " << from << " на " << to << "\n";
    ++hanoiMoves;
    hanoi(n - 1, aux, to, from);
}

int main() {
    setlocale(LC_ALL, "Russian"); // Включаем поддержку русского языка

    cout << "=== Задача 1: Факториал ===\n";
    for (int n = 0; n <= 10; ++n)
        cout << n << "! = " << factorial(n) << "\n";

    cout << "\n=== Задача 2: Фибоначчи ===\n";
    for (int n = 0; n <= 15; ++n)
        cout << "F(" << n << ") = " << fib(n) << "\n";

    cout << "\n=== Задача 3: Максимум в массиве ===\n";
    int arr[] = { 3, 7, -2, 15, 9, 4, 11 };
    int n = sizeof(arr) / sizeof(arr[0]);
    cout << "Массив: ";
    for (int i = 0; i < n; ++i) cout << arr[i] << " ";
    cout << "\nМаксимум = " << findMax(arr, n) << "\n";

    cout << "\n=== Задача 4: Ханойские башни (5 дисков) ===\n";
    hanoi(5, 'A', 'C', 'B');
    cout << "Всего ходов: " << hanoiMoves << " (теория: 2^5 - 1 = 31)\n";

    return 0;
}