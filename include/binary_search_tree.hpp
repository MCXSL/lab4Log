#pragma once

#include <cstddef>
#include <iosfwd>

enum class DuplicatePolicy {
    Allow,
    Reject
};

class BinarySearchTree {
public:
    explicit BinarySearchTree(DuplicatePolicy policy = DuplicatePolicy::Allow) noexcept;
    ~BinarySearchTree();

    // Каждый объект единолично владеет своими узлами.
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    // Возвращает false, если повтор запрещён и значение уже есть.
    bool insert(int value);
    [[nodiscard]] bool contains(int value) const noexcept;
    [[nodiscard]] std::size_t count(int value) const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;

    // Дерево выводится повёрнутым на 90 градусов, корень слева.
    void print(std::ostream& output) const;
    void clear() noexcept;

private:
    struct Node {
        int data;
        Node* left;
        Node* right;
    };

    static void destroySubtree(Node* node) noexcept;
    static void printSubtree(const Node* node, std::size_t depth, std::ostream& output);

    Node* root_ = nullptr;
    std::size_t size_ = 0;
    DuplicatePolicy policy_;
};
