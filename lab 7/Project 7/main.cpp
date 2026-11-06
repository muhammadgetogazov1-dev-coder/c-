#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// Глобальные счетчики для сбора статистики
unsigned long long quick_comparisons = 0;
unsigned long long quick_swaps = 0;
unsigned long long heap_comparisons = 0;
unsigned long long heap_swaps = 0;


// 1. БЫСТРАЯ СОРТИРОВКА (Quick Sort)


// Функция разделения (partition)
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Опорный элемент - последний
    int i = (low - 1);     // Индекс меньшего элемента

    for (int j = low; j <= high - 1; j++) {
        quick_comparisons++; // Считаем сравнение
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            quick_swaps++;   // Считаем обмен
        }
    }
    swap(arr[i + 1], arr[high]);
    quick_swaps++;           // Считаем обмен опорного элемента
    return (i + 1);
}

// Основная функция быстрой сортировки
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);  // Левая часть
        quickSort(arr, pi + 1, high); // Правая часть
    }
}

// 2. ПИРАМИДАЛЬНАЯ СОРТИРОВКА (Heap Sort)


// Функция для восстановления свойств кучи (heapify)
void heapify(int arr[], int n, int i) {
    int largest = i;       // Инициализируем наибольший элемент как корень
    int left = 2 * i + 1;  // Левый потомок
    int right = 2 * i + 2; // Правый потомок

    // Если левый потомок больше корня
    if (left < n) {
        heap_comparisons++;
        if (arr[left] > arr[largest])
            largest = left;
    }

    // Если правый потомок больше, чем наибольший на данный момент
    if (right < n) {
        heap_comparisons++;
        if (arr[right] > arr[largest])
            largest = right;
    }

    // Если наибольший элемент не корень
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heap_swaps++; // Считаем обмен
        // Рекурсивно просеиваем вниз
        heapify(arr, n, largest);
    }
}

// Основная функция пирамидальной сортировки
void heapSort(int arr[], int n) {
    // Построение max-heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Извлечение элементов из кучи по одному
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heap_swaps++; // Считаем обмен
        heapify(arr, i, 0);
    }
}


// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ


// Копирование массива для честного тестирования
void copyArray(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// Вывод массива (для малых размеров)
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}


// ГЛАВНАЯ ФУНКЦИЯ

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    // Размеры массивов для тестирования
    int sizes[] = { 100, 1000, 10000 };
    int num_sizes = 3;

    cout << "=== Сравнение Quick Sort и Heap Sort (Средний уровень, Вариант 6) ===\n\n";
    cout << setw(10) << "Размер" << setw(15) << "Quick Comp" << setw(15) << "Quick Swap"
        << setw(15) << "Heap Comp" << setw(15) << "Heap Swap" << "\n";
    cout << string(70, '-') << "\n";

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int* original = new int[n];
        int* for_quick = new int[n];
        int* for_heap = new int[n];

        // Заполняем массив случайными числами
        for (int i = 0; i < n; i++) {
            original[i] = rand() % (n * 10); // Генерация в диапазоне
        }

        // Копируем для честного сравнения
        copyArray(original, for_quick, n);
        copyArray(original, for_heap, n);

        // Сбрасываем счетчики
        quick_comparisons = 0; quick_swaps = 0;
        heap_comparisons = 0; heap_swaps = 0;

        // Запуск Quick Sort
        quickSort(for_quick, 0, n - 1);

        // Запуск Heap Sort
        heapSort(for_heap, n);

        // Вывод статистики
        cout << setw(10) << n
            << setw(15) << quick_comparisons
            << setw(15) << quick_swaps
            << setw(15) << heap_comparisons
            << setw(15) << heap_swaps
            << "\n";

        delete[] original;
        delete[] for_quick;
        delete[] for_heap;
    }

    cout << "\nТестирование завершено.\n";
    return 0;
}