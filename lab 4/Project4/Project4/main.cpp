#include <iostream>
#include <stdexcept>

// ==========================================
// ОДНОСВЯЗНЫЙ СПИСОК (SinglyLinkedList)
// ==========================================
template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int count;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~SinglyLinkedList() { clear(); }

    // Вставка в начало O(1)
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) tail = newNode;
        count++;
    }

    // Вставка в конец O(1) с tail
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    // Удаление из начала O(1)
    void pop_front() {
        if (head == nullptr) throw std::underflow_error("List is empty");
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        if (head == nullptr) tail = nullptr;
    }

    // Удаление из конца O(n)
    void pop_back() {
        if (head == nullptr) throw std::underflow_error("List is empty");
        if (head->next == nullptr) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        count--;
    }

    // Поиск O(n)
    Node* find(const T& value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    // Дополнительное задание (Вариант 6): Циклический сдвиг влево на k позиций O(n)
    void rotateLeft(int k) {
        if (head == nullptr || head->next == nullptr || k <= 0) return;

        k = k % count; // Если k больше размера списка
        if (k == 0) return;

        Node* current = head;
        int steps = 1;
        // Находим новый хвост (узел перед новым head)
        while (steps < k) {
            current = current->next;
            steps++;
        }

        Node* newHead = current->next;
        Node* newTail = current;

        // Замыкаем список в кольцо и разрываем в нужном месте
        newTail->next = nullptr;
        tail->next = head;
        head = newHead;
        tail = newTail;
    }

    void print() const {
        Node* current = head;
        std::cout << "[";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) std::cout << ", ";
            current = current->next;
        }
        std::cout << "]" << std::endl;
    }

    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }
};

// ==========================================
// ДВУСВЯЗНЫЙ СПИСОК (DoublyLinkedList)
// ==========================================
template <typename T>
class DoublyLinkedList {
private:
    struct DNode {
        T data;
        DNode* prev;
        DNode* next;
        DNode(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };
    DNode* head;
    DNode* tail;
    int count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~DoublyLinkedList() { clear(); }

    // Вставка в начало O(1)
    void push_front(const T& value) {
        DNode* newNode = new DNode(value);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    // Вставка в конец O(1)
    void push_back(const T& value) {
        DNode* newNode = new DNode(value);
        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    // Удаление из начала O(1)
    void pop_front() {
        if (head == nullptr) throw std::underflow_error("List is empty");
        DNode* temp = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
    }

    // Удаление из конца O(1)
    void pop_back() {
        if (tail == nullptr) throw std::underflow_error("List is empty");
        DNode* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
    }

    // Поиск O(n)
    DNode* find(const T& value) const {
        DNode* current = head;
        while (current != nullptr) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    // Дополнительное задание (Вариант 6): Циклический сдвиг влево на k позиций O(n)
    void rotateLeft(int k) {
        if (head == nullptr || head->next == nullptr || k <= 0) return;

        k = k % count;
        if (k == 0) return;

        DNode* current = head;
        int steps = 1;
        while (steps < k) {
            current = current->next;
            steps++;
        }

        DNode* newTail = current;
        DNode* newHead = current->next;

        // Обновляем указатели next
        newTail->next = nullptr;
        tail->next = head;

        // Обновляем указатели prev
        head->prev = tail;
        newHead->prev = nullptr;

        // Обновляем head и tail
        head = newHead;
        tail = newTail;
    }

    void print() const {
        DNode* current = head;
        std::cout << "[";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) std::cout << ", ";
            current = current->next;
        }
        std::cout << "]" << std::endl;
    }

    void printReverse() const {
        DNode* current = tail;
        std::cout << "[";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->prev != nullptr) std::cout << ", ";
            current = current->prev;
        }
        std::cout << "]" << std::endl;
    }

    void clear() {
        while (head != nullptr) {
            DNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }
};

// ==========================================
// ДЕМОНСТРАЦИОННАЯ ПРОГРАММА
// ==========================================
int main() {
    setlocale(LC_ALL, "ru");

    std::cout << "=== Односвязный список (SinglyLinkedList) ===" << std::endl;
    SinglyLinkedList<int> sList;
    sList.push_back(1);
    sList.push_back(2);
    sList.push_back(3);
    sList.push_back(4);
    sList.push_back(5);

    std::cout << "Исходный список: ";
    sList.print();

    int k_s = 2;
    std::cout << "После rotateLeft(" << k_s << "): ";
    sList.rotateLeft(k_s);
    sList.print();
    std::cout << std::endl;

    std::cout << "=== Двусвязный список (DoublyLinkedList) ===" << std::endl;
    DoublyLinkedList<int> dList;
    dList.push_back(10);
    dList.push_back(20);
    dList.push_back(30);
    dList.push_back(40);
    dList.push_back(50);

    std::cout << "Исходный список (прямой): ";
    dList.print();
    std::cout << "Исходный список (обратный): ";
    dList.printReverse();

    int k_d = 3;
    std::cout << "После rotateLeft(" << k_d << "): ";
    dList.rotateLeft(k_d);
    dList.print();

    std::cout << "Проверка обратного вывода после сдвига: ";
    dList.printReverse();

    return 0;
}