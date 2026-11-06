#include <iostream>
#include <cstdlib>
#include <chrono>
#include <windows.h>
using namespace std;
using namespace chrono;

struct Stats {
    long long comparisons = 0; // сравнения
    long long swaps = 0;       // обмены
};

// ===== 1. Пузырьковая =====
void bubbleSort(int arr[], int n, Stats& st) {
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            ++st.comparisons;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                ++st.swaps;
                swapped = true;
            }
        }
        if (!swapped) break; // оптимизация
    }
}

// ===== 2. Вставками =====
void insertionSort(int arr[], int n, Stats& st) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            ++st.comparisons;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                ++st.swaps; // считаем сдвиг как перемещение
                --j;
            }
            else break;
        }
        arr[j + 1] = key;
    }
}

// ===== 3. Выбором =====
void selectionSort(int arr[], int n, Stats& st) {
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            ++st.comparisons;
            if (arr[j] < arr[minIndex]) minIndex = j;
        }
        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
            ++st.swaps;
        }
    }
}

// Вспомогательная функция для запуска и вывода статистики
void runTest(const char* name, void (*sortFunc)(int[], int, Stats&), int original[], int n) {
    int* copy = new int[n];
    for (int i = 0; i < n; ++i) copy[i] = original[i];

    Stats st;
    auto start = high_resolution_clock::now();
    sortFunc(copy, n, st);
    auto end = high_resolution_clock::now();
    long long us = duration_cast<microseconds>(end - start).count();

    cout << name
        << " | Сравнений: " << st.comparisons
        << " | Обменов: " << st.swaps
        << " | Время: " << us << " мкс\n";

    delete[] copy;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");

    const int N = 1000; // размер массива
    int* arr = new int[N];
    srand(42); // фиксируем генератор случайных чисел

    // Заполняем случайными числами
    for (int i = 0; i < N; ++i) arr[i] = rand() % N;

    cout << "Размер массива N = " << N << " (случайные числа)\n";
    cout << "----------------------------------------------------------\n";

    runTest("Пузырьковая", bubbleSort, arr, N);
    runTest("Вставками  ", insertionSort, arr, N);
    runTest("Выбором    ", selectionSort, arr, N);

    delete[] arr;
    return 0;
}