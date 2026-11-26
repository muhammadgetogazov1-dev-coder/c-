МИНИCTEPCTBO НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ
Федеральное государственное автономное
образовательное учреждение высшего образования
«СЕВЕРО-КАВКАЗСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ»
Институт перспективной инженерии
Департамент цифровых, робототехнических систем и электроники
Межинститутская базовая кафедра 


Отчет по лабораторной работе № 10
Вариант 5
Тема: «Реализация класса «Динамический массив целых чисел» с конструкторами, деструктором и основными операциями»
Дисциплина: «Алгоритмы и структуры данных»


Выполнил: 
Студент группы ПИЖ-б-о-25-2 (1),
направление подготовки: 09.03.04 «Программная инженерия»
Гетогазов Мухаммад Умарович 
Проверил: 
Ассистент межинститутской базовой кафедры ДЦРСиЭ 
Белик.А.И





Ставрополь 2026


Код
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;

// ==================== AVL-ДЕРЕВО ====================
struct AVLNode {
    int key, height;
    AVLNode* left, * right;
    AVLNode(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
    AVLNode* root;
    int h(AVLNode* n) { return n ? n->height : 0; }
    int bf(AVLNode* n) { return n ? h(n->left) - h(n->right) : 0; }
    void upd(AVLNode* n) { if (n) n->height = 1 + max(h(n->left), h(n->right)); }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left; y->left = x->right; x->right = y;
        upd(y); upd(x); return x;
    }
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right; x->right = y->left; y->left = x;
        upd(x); upd(y); return y;
    }
    AVLNode* balance(AVLNode* n) {
        if (!n) return nullptr;
        upd(n);
        int b = bf(n);
        if (b > 1) {
            if (bf(n->left) < 0) n->left = leftRotate(n->left);
            return rightRotate(n);
        }
        if (b < -1) {
            if (bf(n->right) > 0) n->right = rightRotate(n->right);
            return leftRotate(n);
        }
        return n;
    }
    AVLNode* insert(AVLNode* n, int k) {
        if (!n) return new AVLNode(k);
        if (k < n->key) n->left = insert(n->left, k);
        else if (k > n->key) n->right = insert(n->right, k);
        else return n;
        return balance(n);
    }
    AVLNode* findMin(AVLNode* n) { while (n && n->left) n = n->left; return n; }

    // УДАЛЕНИЕ (вариант 6: все случаи, в т.ч. два потомка)
    AVLNode* remove(AVLNode* n, int k) {
        if (!n) return nullptr;
        if (k < n->key) n->left = remove(n->left, k);
        else if (k > n->key) n->right = remove(n->right, k);
        else {
            if (!n->left || !n->right) {
                AVLNode* t = n->left ? n->left : n->right;
                if (!t) { delete n; return nullptr; }
                *n = *t; delete t;
            }
            else {
                AVLNode* m = findMin(n->right);
                n->key = m->key;
                n->right = remove(n->right, m->key);
            }
        }
        return balance(n);
    }
    bool search(AVLNode* n, int k) {
        if (!n) return false;
        if (n->key == k) return true;
        return k < n->key ? search(n->left, k) : search(n->right, k);
    }
    void inOrder(AVLNode* n) {
        if (!n) return;
        inOrder(n->left); cout << n->key << " "; inOrder(n->right);
    }
    void print(AVLNode* n, int lvl = 0) {
        if (!n) return;
        print(n->right, lvl + 1);
        for (int i = 0; i < lvl; i++) cout << "    ";
        cout << n->key << " (h=" << n->height << ", bf=" << bf(n) << ")\n";
        print(n->left, lvl + 1);
    }
    void destroy(AVLNode* n) {
        if (!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }
    void insert(int k) { root = insert(root, k); }
    void remove(int k) { root = remove(root, k); }
    bool search(int k) { return search(root, k); }
    void print() { inOrder(root); cout << endl; }
    void printTree() { print(root); }
};

// ==================== TRIE ====================
class Trie {
    struct Node {
        Node* ch[26];
        bool end;
        Node() : end(false) { fill(ch, ch + 26, nullptr); }
    };
    Node* root;
    void destroy(Node* n) {
        if (!n) return;
        for (int i = 0; i < 26; i++) destroy(n->ch[i]);
        delete n;
    }
    void collect(Node* n, string cur, vector<string>& res) {
        if (n->end) res.push_back(cur);
        for (int i = 0; i < 26; i++)
            if (n->ch[i]) collect(n->ch[i], cur + char('a' + i), res);
    }
public:
    Trie() : root(new Node()) {}
    ~Trie() { destroy(root); }

    void insert(const string& w) {
        Node* c = root;
        for (char x : w) {
            int i = x - 'a';
            if (i < 0 || i >= 26) continue;
            if (!c->ch[i]) c->ch[i] = new Node();
            c = c->ch[i];
        }
        c->end = true;
    }
    bool search(const string& w) {
        Node* c = root;
        for (char x : w) {
            int i = x - 'a';
            if (i < 0 || i >= 26 || !c->ch[i]) return false;
            c = c->ch[i];
        }
        return c->end;
    }
    bool startsWith(const string& p) {
        Node* c = root;
        for (char x : p) {
            int i = x - 'a';
            if (i < 0 || i >= 26 || !c->ch[i]) return false;
            c = c->ch[i];
        }
        return true;
    }
    // ДОП. ЗАДАНИЕ варианта 6: самый длинный префикс-слово
    string longestPrefix(const string& w) {
        Node* c = root;
        int last = -1;
        for (int i = 0; i < (int)w.size(); i++) {
            int idx = w[i] - 'a';
            if (idx < 0 || idx >= 26 || !c->ch[idx]) break;
            c = c->ch[idx];
            if (c->end) last = i;
        }
        return last == -1 ? "" : w.substr(0, last + 1);
    }
    vector<string> allWords() {
        vector<string> r; collect(root, "", r); return r;
    }
};

// ==================== BST (для сравнения) ====================
struct BSTNode {
    int key; BSTNode* left, * right;
    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};
class BST {
    BSTNode* root;
    BSTNode* insert(BSTNode* n, int k) {
        if (!n) return new BSTNode(k);
        if (k < n->key) n->left = insert(n->left, k);
        else if (k > n->key) n->right = insert(n->right, k);
        return n;
    }
    void destroy(BSTNode* n) {
        if (!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }
public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }
    void insert(int k) { root = insert(root, k); }
};

// ==================== MAIN ====================
int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    // ---------- AVL ----------
    cout << "===== ЧАСТЬ 1: AVL =====" << endl;
    AVLTree avl;
    for (int k : {10, 20, 30, 40, 50, 25, 60, 70, 15, 35}) avl.insert(k);
    cout << "In-order: "; avl.print();
    avl.printTree();
    cout << "Поиск 25: " << (avl.search(25) ? "да" : "нет") << endl;

    cout << "\nУдаляем 20 (два потомка):" << endl;
    avl.remove(20);
    cout << "In-order: "; avl.print();
    avl.printTree();

    // ---------- TRIE ----------
    cout << "\n===== ЧАСТЬ 2: TRIE =====" << endl;
    Trie trie;
    for (auto w : { "cat", "car", "cart", "catfish", "dog" }) trie.insert(w);

    cout << "search(cat): " << (trie.search("cat") ? "да" : "нет") << endl;
    cout << "search(cattle): " << (trie.search("cattle") ? "да" : "нет") << endl;
    cout << "startsWith(ca): " << (trie.startsWith("ca") ? "да" : "нет") << endl;

    cout << "\nСамый длинный префикс (вариант 6):" << endl;
    for (auto w : { "cattle", "cartoon", "catfish", "doom", "xyz" })
        cout << "  \"" << w << "\" -> \"" << trie.longestPrefix(w) << "\"" << endl;

    cout << "\nВсе слова: ";
    for (auto& w : trie.allWords()) cout << w << " ";
    cout << endl;

    // ---------- СРАВНЕНИЕ ----------
    cout << "\n===== ЧАСТЬ 3: BST vs AVL =====" << endl;
    cout << "N\tТип\t\tBST(мс)\tAVL(мс)" << endl;
    for (int N : {1000, 5000, 10000}) {
        vector<int> rnd(N), srt(N);
        for (int i = 0; i < N; i++) rnd[i] = rand() % (N * 10);
        for (int i = 0; i < N; i++) srt[i] = i;

        for (int type = 0; type < 2; type++) {
            auto& data = type == 0 ? rnd : srt;
            BST bst; AVLTree avl2;

            auto t1 = chrono::high_resolution_clock::now();
            for (int x : data) bst.insert(x);
            auto t2 = chrono::high_resolution_clock::now();
            double bt = chrono::duration<double, milli>(t2 - t1).count();

            t1 = chrono::high_resolution_clock::now();
            for (int x : data) avl2.insert(x);
            t2 = chrono::high_resolution_clock::now();
            double at = chrono::duration<double, milli>(t2 - t1).count();

            cout << N << "\t" << (type ? "Отсортир." : "Случайные")
                << "\t" << bt << "\t" << at << endl;
        }
    }
    return 0;
}
Вывод
===== ЧАСТЬ 1: AVL =====
In-order: 10 15 20 25 30 35 40 50 60 70
            70 (h=1, bf=0)
        60 (h=2, bf=-1)
    50 (h=3, bf=0)
        40 (h=2, bf=1)
            35 (h=1, bf=0)
30 (h=4, bf=0)
        25 (h=1, bf=0)
    20 (h=3, bf=1)
            15 (h=1, bf=0)
        10 (h=2, bf=-1)
Поиск 25: да

Удаляем 20 (два потомка):
In-order: 10 15 25 30 35 40 50 60 70
            70 (h=1, bf=0)
        60 (h=2, bf=-1)
    50 (h=3, bf=0)
        40 (h=2, bf=1)
            35 (h=1, bf=0)
30 (h=4, bf=-1)
        25 (h=1, bf=0)
    15 (h=2, bf=0)
        10 (h=1, bf=0)

===== ЧАСТЬ 2: TRIE =====
search(cat): да
search(cattle): нет
startsWith(ca): да

Самый длинный префикс (вариант 6):
  "cattle" -> "cat"
  "cartoon" -> "cart"
  "catfish" -> "catfish"
  "doom" -> ""
  "xyz" -> ""


  Контрольные вопросы
  2. Что такое AVL-дерево? В чём его отличие от обычного BST?

AVL-дерево — это самобалансирующееся бинарное дерево поиска, в котором для каждого узла выполняется условие: |высота(левое) − высота(правое)| ≤ 1.

Отличие от BST: обычный BST может выродиться в связный список при вставке отсортированных данных (высота O(n), операции O(n)). AVL автоматически балансируется поворотами, гарантируя высоту O(log n) и операции O(log n) даже в худшем случае.

3. Что такое фактор баланса? Как он вычисляется?

Фактор баланса (balance factor) — разность высот левого и правого поддеревьев узла:

cpp
int balanceFactor(Node* node) {
    return height(node->left) - height(node->right);
}
Значение	Состояние	Действие
−1, 0, 1	Сбалансирован	Ничего
> 1	Левое поддерево выше	Правый поворот
< −1	Правое поддерево выше	Левый поворот
4. Какие существуют типы поворотов в AVL-дереве?

Четыре типа:

Правый поворот (Right Rotation) — LL-случай

Левый поворот (Left Rotation) — RR-случай

Левый-правый поворот (Left-Right) — LR-случай

Правый-левый поворот (Right-Left) — RL-случай

5. Когда применяется правый поворот? Левый поворот?

Правый поворот: фактор баланса узла > 1 (левое поддерево выше) и фактор баланса левого потомка ≥ 0 (LL-случай).

Левый поворот: фактор баланса узла < −1 (правое поддерево выше) и фактор баланса правого потомка ≤ 0 (RR-случай).

6. Когда применяется левый-правый поворот? Правый-левый?

Левый-правый (LR): фактор баланса > 1 и фактор баланса левого потомка < 0. Сначала левый поворот для левого потомка, затем правый для узла.

Правый-левый (RL): фактор баланса < −1 и фактор баланса правого потомка > 0. Сначала правый поворот для правого потомка, затем левый для узла.

7. Какова сложность операций в AVL-дереве?

Поиск, вставка, удаление — O(log n) в среднем и худшем случае. Это гарантируется тем, что высота AVL-дерева всегда O(log n).

8. Почему AVL-дерево называется самобалансирующимся?

Потому что после каждой вставки или удаления дерево автоматически проверяет факторы баланса и выполняет повороты, восстанавливая условие |BF| ≤ 1. Программисту не нужно балансировать вручную.

9. Как часто происходит балансировка при вставке?

Балансировка выполняется только при необходимости — когда после вставки фактор баланса какого-то узла выходит за пределы [−1, 1]. В среднем это происходит нечасто: один поворот (одинарный или двойной) исправляет дисбаланс. В худшем случае — O(log n) проверок на пути от корня к вставленному узлу, но поворотов — не более одного.

10. Чем AVL-дерево отличается от красно-чёрного дерева?

Критерий	AVL	Красно-чёрное
Баланс	Строгий (|BF| ≤ 1)	Ослабленный (высота ≤ 2log(n+1))
Высота	Ниже (быстрее поиск)	Выше
Вставка/удаление	Больше поворотов	Меньше поворотов (быстрее модификация)
Применение	Частый поиск	Частые вставки/удаления (std::map)
11. В каких случаях AVL-дерево эффективнее BST?

AVL эффективнее BST, когда:

Данные вставляются в отсортированном порядке (BST вырождается, AVL — нет)

Нужны гарантированные O(log n) операции

Преобладают операции поиска над вставкой/удалением

Префиксное дерево (Trie)
12/13. Что такое Trie? Для решения каких задач используется?

Trie (префиксное дерево) — структура данных для хранения строк, где каждый узел соответствует символу, а путь от корня до узла образует префикс.

Применение:

Автодополнение (подсказки при вводе)

Поиск по префиксу

Проверка орфографии

IP-маршрутизация (двоичный Trie)

Хранение словарей

14. Какова сложность поиска в Trie? От чего она зависит?

Сложность — O(L), где L — длина строки (ключа). Она не зависит от количества хранимых слов n. Поиск всегда пропорционален длине искомой строки.

15. Какая структура данных эффективнее для строк: Trie или хеш-таблица?

Критерий	Trie	Хеш-таблица
Точный поиск	O(L)	O(L) в среднем, O(n) в худшем
Поиск по префиксу	O(L)	O(n) — перебор всех
Автодополнение	Легко	Сложно
Память	Больше	Меньше
Упорядоченность	Есть	Нет
Вывод: Trie эффективнее для префиксных операций и автодополнения; хеш-таблица — для точного поиска при ограниченной памяти.

16. Как реализовать автодополнение с помощью Trie?

Найти узел, соответствующий введённому префиксу (спуск по Trie)

Запустить DFS от этого узла, собирая все слова (пути до узлов с isEndOfWord == true)

Вернуть найденные слова (можно ограничить топ-5/топ-10)

cpp
void collectWords(Node* node, string current, vector<string>& result) {
    if (node->isEndOfWord) result.push_back(current);
    for (int i = 0; i < 26; i++)
        if (node->children[i])
            collectWords(node->children[i], current + char('a' + i), result);
}
Сложность: O(L + K), где L — длина префикса, K — суммарная длина найденных слов.

17. Каковы недостатки Trie? Какой объём памяти он требует?

Недостатки:

Большой расход памяти: каждый узел хранит массив из 26 указателей (для английского алфавита) — даже если большинство пусты

Разреженность: большинство указателей — nullptr

Зависимость от алфавита: для Unicode (кириллица, иероглифы) массив детей становится огромным

Объём памяти: ≈ O(26 × L × n) в худшем случае, где n — число слов, L — средняя длина.