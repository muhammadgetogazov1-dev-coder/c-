#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm> 

const int MAX = 100; 


class Stack {
    int data[MAX], topIdx = -1;
public:
    void push(int val) { if (topIdx < MAX) data[++topIdx] = val; }
    int pop() {
        if (topIdx < 0) throw std::underflow_error("Stack is empty");
        return data[topIdx--];
    }
    int top() const { return data[topIdx]; }
    bool isEmpty() const { return topIdx == -1; }
};

void task1() {
    std::cout << "--- Задание 1: Сортировка стека ---\n";
    Stack s, tmp;
    s.push(3); s.push(1); s.push(4); s.push(2); 
    while (!s.isEmpty()) {
        int val = s.pop();
        while (!tmp.isEmpty() && tmp.top() > val) s.push(tmp.pop()); 
    }
    while (!tmp.isEmpty()) s.push(tmp.pop()); 

    std::cout << "Результат (верх->низ): ";
    while (!s.isEmpty()) std::cout << s.pop() << " ";
    std::cout << "\n\n";
}


class DualQueue {
    std::string priQueue[MAX], normQueue[MAX]; 
    int priFront = 0, priRear = 0, normFront = 0, normRear = 0;
public:
    void enqueue(const std::string& val, bool isPriority = false) {
        if (isPriority) priQueue[priRear++] = val;
        else normQueue[normRear++] = val;
    }
    std::string dequeue() {
        if (priFront < priRear) return priQueue[priFront++];
        if (normFront < normRear) return normQueue[normFront++];
        throw std::underflow_error("Queue is empty");
    }
};

void task2() {
    std::cout << "--- Задание 2: Очередь с приоритетом ---\n";
    DualQueue q;
    q.enqueue("Обычный 1");
    q.enqueue("Срочный 1", true);
    q.enqueue("Обычный 2");
    q.enqueue("Срочный 2", true);

    while (true) {
        try { std::cout << q.dequeue() << "\n"; }
        catch (...) { break; } 
    }
    std::cout << "\n";
}


class Deque {
    int data[MAX];
    int front = 0, rear = 0; 
public:
    void push_back(int val) { if (rear < MAX) data[rear++] = val; }
    void reverse() { std::reverse(data + front, data + rear); } 
    void print() {
        for (int i = front; i < rear; i++) std::cout << data[i] << " ";
        std::cout << "\n";
    }
};

void task3() {
    std::cout << "--- Задание 3: Дек с reverse() ---\n";
    Deque dq;
    dq.push_back(1); dq.push_back(2); dq.push_back(3);
    std::cout << "До reverse: "; dq.print();
    dq.reverse();
    std::cout << "После reverse: "; dq.print();
    std::cout << "\n";
}


class PriorityQueue {
    int heap[MAX], size = 0;

    void heapifyUp(int i) {
        while (i > 0 && heap[(i - 1) / 2] < heap[i]) {
            std::swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
    void heapifyDown(int i) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, largest = i;
            if (l < size && heap[l] > heap[largest]) largest = l;
            if (r < size && heap[r] > heap[largest]) largest = r;
            if (largest == i) break;
            std::swap(heap[i], heap[largest]);
            i = largest;
        }
    }
public:
    void push(int val) { heap[size] = val; heapifyUp(size++); }
    int pop() {
        if (size == 0) throw std::underflow_error("PQ is empty");
        int maxVal = heap[0];
        heap[0] = heap[--size];
        heapifyDown(0);
        return maxVal;
    }
    void changePriority(int oldVal, int newVal) {
        for (int i = 0; i < size; i++) {
            if (heap[i] == oldVal) {
                bool increased = (newVal > oldVal);
                heap[i] = newVal;
                increased ? heapifyUp(i) : heapifyDown(i);
                return;
            }
        }
    }
};

void task4() {
    std::cout << "--- Задание 4: Приоритетная очередь ---\n";
    PriorityQueue pq;
    pq.push(30); pq.push(10); pq.push(50); pq.push(20);

    std::cout << "Максимум: " << pq.pop() << "\n"; 

    std::cout << "Меняем приоритет 10 на 100...\n";
    pq.changePriority(10, 100);
    std::cout << "Новый максимум: " << pq.pop() << "\n"; 
}


int main() {
    setlocale(LC_ALL, "Russian");
    try {
        task1();
        task2();
        task3();
        task4();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
    return 0;
}