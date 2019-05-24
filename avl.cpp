#include <iostream>
#include <algorithm>

template <class T>
struct Node {
    Node* left; Node* right;
    T key;
    int height;
    int nodes;
    Node(T value) {
        left = right = nullptr;
        key = value;
        height = 1;
        nodes = 1;
    }
};

template <class T>
class AVL {
public:
    void insert(T value) { root = insert(root, value); }
    void remove(T value) { root = remove(root, value); }
    int orderStatistic(int k) { return orderStatistic(root, k, nodes(root)); }
    Node<T>* balance(Node<T>*& node);
    AVL() { root = nullptr; }
    ~AVL() { destroy(root); }
private:
    // Несколько вспомогательных функций
    Node<T>* removeMin(Node<T>*& root) {
        if (root->left == nullptr) {
            return root->right;
        }
        root->left = removeMin(root->left);
        return balance(root);
    }
    template <class T>
    Node<T>* findMin(Node<T>*& node) {
        return node->left ? findMin(node->left) : node;
    }
    // Хотим вызывать nodes и height от, возможно, нулвых указателей.
    template <class T>
    int nodes(Node<T>*& node) {
        return node ? node->nodes : 0;
    }

    template <class T>
    int height(Node<T>*& node) {
        return node ? node->height : 0;
    }

    // Узнать баланс-фактор в вершине
    template <class T>
    int heightDifference(Node<T>*& node) {
        return height(node->right) - height(node->left);
    }


    // Установить корректные значения полей nodes и height, с учетом, что левые и правые поддеревья правильные
    template <class T>
    void dataCorrect(Node<T>*& node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
        node->nodes = nodes(node->left) + nodes(node->right) + 1;
    }
    void destroy(Node<T>*& root) {
        if (root == nullptr) return;
        destroy(root->left);
        destroy(root->right);
        delete root;
    }
    Node<T>* root;
    Node<T>* rotateRight(Node<T>* root);
    Node<T>* rotateLeft(Node<T>*& root);
    Node<T>* insert(Node<T>*& root, T value);
    Node<T>* remove(Node<T>*& root, T value);
    int orderStatistic(Node<T>*& root, int k, int n);
};


template <class T>
Node<T>* AVL<T>::rotateRight(Node<T>* root) {
    Node<T>* newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    dataCorrect(root);
    dataCorrect(newRoot);
    return newRoot;
}

template <class T>
Node<T>* AVL<T>::rotateLeft(Node<T>*& root) {
    Node<T>* newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    dataCorrect(root);
    dataCorrect(newRoot);
    return newRoot;
}

template <class T>
Node<T>* AVL<T>::balance(Node<T>*& node) {
    dataCorrect(node);
    if (heightDifference(node) == 2) {
        if (heightDifference(node->right) < 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    if (heightDifference(node) == -2) {
        if (heightDifference(node->left) > 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    return node;
}

template <class T>
Node<T>* AVL<T>::insert(Node<T>*& root, T value) {
    if (root == nullptr)
        return new Node<T>(value);
    if (value < root->key)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);
    return balance(root);
}

template <class T>
Node<T>* AVL<T>::remove(Node<T>*& root, T value) {
    if (root == nullptr)
        return nullptr;
    if (value < root->key)
        root->left = remove(root->left, value);
    else if (value > root->key)
        root->right = remove(root->right, value);
    else {
        Node<T>* left = root->left;
        Node<T>* right = root->right;
        delete root;
        if (right == nullptr)
            return left;
        if (left == nullptr)
            return right;
        Node<T>* min = findMin(right);
        min->right = removeMin(right);
        min->left = left;
        return balance(min);
    }
    return balance(root);
}

template <class T>
int AVL<T>::orderStatistic(Node<T>*& root, int k, int n) {
    int lowerNodes = nodes(root->left) + 1;
    if (lowerNodes == k)
        return root->key;
    if (lowerNodes < k)
        return orderStatistic(root->right, k - lowerNodes, n - lowerNodes);
    else
        return orderStatistic(root->left, k, n - nodes(root->right) + 1);
}

int main() {
    AVL<int> AVL;
    int n; std::cin >> n;
    for (int i = 0; i < n; i++) {
        int value = 0;
        int order = 0;
        std::cin >> value >> order;
        if (value > 0)
            AVL.insert(value);
        else
            AVL.remove(std::abs(value));
        std::cout << AVL.orderStatistic(order + 1) << '\n';
    }
    return 0;
}
