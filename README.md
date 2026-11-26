МИНИCTEPCTBO НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ
Федеральное государственное автономное
образовательное учреждение высшего образования
«СЕВЕРО-КАВКАЗСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ»
Институт перспективной инженерии
Департамент цифровых, робототехнических систем и электроники
Межинститутская базовая кафедра 


Отчет по лабораторной работе № 9
Вариант 9
Тема: «Бинарное дерево. Обходы. BST»
Дисциплина: «Алгоритмы и структуры данных»


Выполнил: 
Студент группы ПИЖ-б-о-25-2 (1),
направление подготовки: 09.03.04 «Программная инженерия»
Гетогазов Мухаммад Умарович 
Проверил: 
Ассистент межинститутской базовой кафедры ДЦРСиЭ 
Белик.А.И





Ставрополь 2026

Условие     
Обязательные операции                      Дополнительное задание
preOrder, inOrder, postOrder,levelOrder  найти сумму всех элементов

2)insert, search, inOrder             Найти предшественника(предыдущий по порядку)


Код 
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// ==================== УЗЕЛ ====================
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// ==================== ЧАСТЬ 1: БИНАРНОЕ ДЕРЕВО ====================
class BinaryTree {
private:
    Node* root;

    void preOrder(Node* node) {
        if (!node) return;
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
    void inOrder(Node* node) {
        if (!node) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }
    void postOrder(Node* node) {
        if (!node) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }
    // Сумма всех элементов (вариант 6)
    int sum(Node* node) {
        if (!node) return 0;
        return node->data + sum(node->left) + sum(node->right);
    }
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { destroy(root); }

    void buildSample() {
        root = new Node(10);
        root->left = new Node(5);
        root->right = new Node(15);
        root->left->left = new Node(3);
        root->left->right = new Node(7);
        root->right->right = new Node(20);
        root->left->right->left = new Node(6);
        root->right->right->left = new Node(18);
    }

    void preOrder() { cout << "pre-order:  "; preOrder(root);  cout << endl; }
    void inOrder() { cout << "in-order:   "; inOrder(root);   cout << endl; }
    void postOrder() { cout << "post-order: "; postOrder(root); cout << endl; }

    void levelOrder() {
        if (!root) return;
        cout << "level-order: ";
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* cur = q.front(); q.pop();
            cout << cur->data << " ";
            if (cur->left)  q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        cout << endl;
    }

    int getSum() { return sum(root); }
};

// ==================== ЧАСТЬ 2: BST ====================
class BST {
private:
    Node* root;

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->data)      node->left = insert(node->left, key);
        else if (key > node->data) node->right = insert(node->right, key);
        return node;
    }

    Node* search(Node* node, int key) {
        if (!node || node->data == key) return node;
        if (key < node->data) return search(node->left, key);
        return search(node->right, key);
    }

    Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }
    Node* findMax(Node* node) {
        while (node && node->right) node = node->right;
        return node;
    }

    Node* remove(Node* node, int key) {
        if (!node) return nullptr;
        if (key < node->data)      node->left = remove(node->left, key);
        else if (key > node->data) node->right = remove(node->right, key);
        else {
            // Случай 1: лист
            if (!node->left && !node->right) { delete node; return nullptr; }
            // Случай 2: один потомок
            else if (!node->left) {
                Node* tmp = node->right; delete node; return tmp;
            }
            else if (!node->right) {
                Node* tmp = node->left; delete node; return tmp;
            }
            // Случай 3: два потомка
            else {
                Node* minNode = findMin(node->right);
                node->data = minNode->data;
                node->right = remove(node->right, minNode->data);
            }
        }
        return node;
    }

    void inOrder(Node* node) {
        if (!node) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    int height(Node* node) {
        if (!node) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    // Предшественник (вариант 6): наибольший ключ, меньший заданного
    Node* predecessor(Node* node, int key) {
        Node* pred = nullptr;
        while (node) {
            if (key > node->data) { pred = node; node = node->right; }
            else { node = node->left; }
        }
        return pred;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void insert(int key) { root = insert(root, key); }
    bool search(int key) { return search(root, key) != nullptr; }
    void remove(int key) { root = remove(root, key); }
    void print() { inOrder(root); cout << endl; }
    int getHeight() { return height(root); }

    int getMin() { Node* n = findMin(root); return n ? n->data : -1; }
    int getMax() { Node* n = findMax(root); return n ? n->data : -1; }

    // Доп. задание варианта 6
    int getPredecessor(int key) {
        Node* p = predecessor(root, key);
        return p ? p->data : -1;
    }
};

// ==================== ВИЗУАЛИЗАЦИЯ ====================
void printTree(Node* root, int space = 0, int indent = 5) {
    if (!root) return;
    space += indent;
    printTree(root->right, space);
    cout << endl;
    for (int i = indent; i < space; i++) cout << " ";
    cout << root->data << "\n";
    printTree(root->left, space);
}

// ==================== MAIN ====================
int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    cout << "========== ЧАСТЬ 1: БИНАРНОЕ ДЕРЕВО ==========\n";
    BinaryTree tree;
    tree.buildSample();
    tree.preOrder();
    tree.inOrder();
    tree.postOrder();
    tree.levelOrder();
    cout << "Сумма всех элементов: " << tree.getSum() << endl;

    cout << "\n========== ЧАСТЬ 2: BST + ПРЕДШЕСТВЕННИК ==========\n";
    BST bst;
    int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
    for (int k : keys) bst.insert(k);
    cout << "Симметричный обход BST: ";
    bst.print();
    cout << "Высота: " << bst.getHeight() << endl;
    cout << "Поиск 40: " << (bst.search(40) ? "найден" : "не найден") << endl;
    cout << "Поиск 100: " << (bst.search(100) ? "найден" : "не найден") << endl;
    cout << "Предшественник для 50: " << bst.getPredecessor(50) << endl;
    cout << "Предшественник для 30: " << bst.getPredecessor(30) << endl;
    cout << "Предшественник для 20: " << bst.getPredecessor(20)
        << " (нет, -1)" << endl;

    cout << "\n========== ЧАСТЬ 3: СЛУЧАЙНЫЙ BST (N=20) ==========\n";
    BST randomBST;
    vector<int> nums;
    cout << "Случайные числа: ";
    for (int i = 0; i < 20; i++) {
        int x = rand() % 100;
        nums.push_back(x);
        randomBST.insert(x);
        cout << x << " ";
    }
    cout << "\n\nОтсортированная последовательность (in-order): ";
    randomBST.print();
    cout << "Минимум: " << randomBST.getMin() << endl;
    cout << "Максимум: " << randomBST.getMax() << endl;
    cout << "Высота: " << randomBST.getHeight() << endl;

    int toDelete;
    cout << "\nВведите элемент для удаления: ";
    cin >> toDelete;
    randomBST.remove(toDelete);
    cout << "Дерево после удаления " << toDelete << ":\n";
    randomBST.print();
    cout << "\nВизуализация:\n";
    // Для визуализации нужен доступ к корню — добавим геттер или упростим
    return 0;
}

Вывод
========== ЧАСТЬ 1: БИНАРНОЕ ДЕРЕВО ==========
pre-order:  10 5 3 7 6 15 20 18
in-order:   3 5 6 7 10 15 18 20
post-order: 3 6 7 5 18 20 15 10
level-order: 10 5 15 3 7 20 6 18
Сумма всех элементов: 84

========== ЧАСТЬ 2: BST + ПРЕДШЕСТВЕННИК ==========
Симметричный обход BST: 20 30 40 50 60 70 80
Высота: 3
Поиск 40: найден
Поиск 100: не найден
Предшественник для 50: 40
Предшественник для 30: 20
Предшественник для 20: -1 (нет, -1)

========== ЧАСТЬ 3: СЛУЧАЙНЫЙ BST (N=20) ==========
Случайные числа: 21 29 36 89 89 61 62 7 10 33 0 79 34 91 34 18 83 47 69 39

Отсортированная последовательность (in-order): 0 7 10 18 21 29 33 34 36 39 47 61 62 69 79 83 89 91
Минимум: 0
Максимум: 91
Высота: 8

Контрольные вопросы
Что такое бинарное дерево? Из каких элементов оно состоит?

Бинарное дерево — это иерархическая структура данных, в которой каждый узел имеет не более двух потомков (левый и правый). Состоит из:

Узлов (Node) — хранят данные и указатели на левого/правого потомка;

Корня (root) — верхний узел, не имеющий родителя;

Листьев (leaf) — узлы без потомков;

Внутренних узлов — узлы, имеющие хотя бы одного потомка;

Рёбер — связи между узлами (указатели).

2. В чём отличие бинарного дерева от бинарного дерева поиска (BST)?

В обычном бинарном дереве порядок расположения элементов произвольный. В BST выполняется свойство упорядоченности: для каждого узла все ключи в левом поддереве меньше ключа узла, а все ключи в правом поддереве больше. Благодаря этому в BST возможен быстрый поиск, а in-order обход даёт отсортированную последовательность.

3. Какие существуют виды обхода бинарного дерева? В чём их особенности?

Прямой (pre-order): Корень → Левый → Правый. Используется для копирования дерева, построения префиксного выражения.

Симметричный (in-order): Левый → Корень → Правый. Для BST даёт отсортированную последовательность.

Обратный (post-order): Левый → Правый → Корень. Используется для удаления дерева, вычисления выражений.

Уровневый (level-order): По уровням слева направо, реализуется через очередь. Используется для поиска в ширину.

4. Какой обход даёт отсортированную последовательность для BST?

Симметричный (in-order) обход — посещение «левый → корень → правый» выдаёт элементы в возрастающем порядке.

5. Какова сложность операций поиска, вставки и удаления в BST? От чего она зависит?

В среднем — O(log n), в худшем — O(n). Сложность зависит от высоты дерева. Если дерево сбалансировано, высота ≈ log₂n, и операции быстрые. Если дерево вырождено в список, высота = n, и сложность становится линейной.

6. Что происходит с BST, если вставлять элементы в отсортированном порядке?

Дерево вырождается в связный список (каждый новый узел становится правым или левым потомком предыдущего). Высота становится равной n, и все операции деградируют до O(n). Это главный недостаток обычного BST.

7. Какие три случая нужно рассмотреть при удалении узла из BST?

Узел — лист (нет потомков): просто удаляем узел.

Узел имеет одного потомка: заменяем удаляемый узел его единственным потомком.

Узел имеет двух потомков: находим минимальный элемент в правом поддереве (или максимальный в левом), копируем его значение в удаляемый узел, затем рекурсивно удаляем этот минимальный узел.

8. Как найти наименьший элемент в BST? Наибольший?

Наименьший: идти от корня по левым потомкам до упора (while node->left != nullptr).

Наибольший: идти от корня по правым потомкам до упора (while node->right != nullptr).

Сложность — O(h).

9. Как вычислить высоту бинарного дерева рекурсивно?

cpp
int height(Node* node) {
    if (node == nullptr) return 0;
    return 1 + max(height(node->left), height(node->right));
}
Высота пустого дерева = 0, иначе 1 + максимум из высот поддеревьев. Сложность O(n).

10. Что такое наименьший общий предок (LCA) двух узлов в дереве?

LCA — это самый нижний (глубокий) узел, который является предком одновременно для обоих заданных узлов. В BST его можно найти за O(h): если оба ключа меньше текущего — идём влево, если оба больше — вправо, иначе текущий узел и есть LCA.

11. Как проверить, является ли бинарное дерево BST?

Два способа:

Через in-order обход: последовательность должна быть строго возрастающей.

Рекурсивно с границами: для каждого узла проверяем, что его ключ находится в допустимом диапазоне (min, max), передавая границы вниз по дереву.

cpp
bool isBST(Node* node, long minV = LONG_MIN, long maxV = LONG_MAX) {
    if (!node) return true;
    if (node->data <= minV || node->data >= maxV) return false;
    return isBST(node->left, minV, node->data) &&
           isBST(node->right, node->data, maxV);
}
12. Что такое сбалансированное дерево? Почему это важно для производительности?

Сбалансированное дерево — дерево, в котором разница высот левого и правого поддеревьев для каждого узла не превышает 1 (AVL-условие) или высота ограничена O(log n). Это важно, потому что гарантирует высоту O(log n), а значит операции поиска/вставки/удаления выполняются за O(log n), а не деградируют до O(n).

13. Как преобразовать несбалансированное BST в сбалансированное?

Выполнить in-order обход и сохранить элементы в отсортированный массив/вектор.

Рекурсивно построить новое дерево: средний элемент массива становится корнем, левая половина — левым поддеревом, правая — правым.

cpp
Node* buildBalanced(vector<int>& v, int l, int r) {
    if (l > r) return nullptr;
    int mid = (l + r) / 2;
    Node* root = new Node(v[mid]);
    root->left  = buildBalanced(v, l, mid - 1);
    root->right = buildBalanced(v, mid + 1, r);
    return root;
}
14. Как восстановить дерево по массиву обхода (in-order + pre-order)?

Первый элемент pre-order — корень.

Находим этот элемент в in-order — всё слева от него принадлежит левому поддереву, всё справа — правому.

Рекурсивно повторяем для левой и правой частей, последовательно беря элементы из pre-order.

cpp
Node* build(vector<int>& pre, vector<int>& ino, int& idx, int l, int r) {
    if (l > r) return nullptr;
    int val = pre[idx++];
    Node* root = new Node(val);
    int pos = find(ino.begin() + l, ino.begin() + r + 1, val) - ino.begin();
    root->left  = build(pre, ino, idx, l, pos - 1);
    root->right = build(pre, ino, idx, pos + 1, r);
    return root;
}
15. Как реализовать итеративный (не рекурсивный) обход бинарного дерева?

Используется стек (для pre/in/post-order) или очередь (для level-order).

Итеративный in-order (через стек):

cpp
void inOrderIterative(Node* root) {
    stack<Node*> st;
    Node* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }
        cur = st.top(); st.pop();
        cout << cur->data << " ";
        cur = cur->right;
    }
}
Итеративный pre-order:

cpp
void preOrderIterative(Node* root) {
    if (!root) return;
    stack<Node*> st;
    st.push(root);
    while (!st.empty()) {
        Node* cur = st.top(); st.pop();
        cout << cur->data << " ";
        if (cur->right) st.push(cur->right); // сначала правый
        if (cur->left)  st.push(cur->left);  // потом левый
    }