#pragma once

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

enum class DuplicatePolicy {
    Allow,
    Reject
};

enum class TreeOrientation {
    Horizontal,
    Vertical
};

struct TreeSearchResult {
    std::string directions; // Путь от корня: L — влево, R — вправо.
    std::vector<std::size_t> levels;
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
    [[nodiscard]] TreeSearchResult findMatches(int value) const;
    [[nodiscard]] std::size_t size() const noexcept;

    void print(std::ostream& output,
               TreeOrientation orientation = TreeOrientation::Horizontal) const;
    void clear() noexcept;

private:
    struct Node {
        int data;
        Node* left;
        Node* right;
    };

    static void destroySubtree(Node* node) noexcept;
    static void printSubtree(const Node* node, std::string& prefix, bool leftChild,
                             std::size_t level, std::ostream& output);
    void printVertical(std::ostream& output) const;

    Node* root_ = nullptr;
    std::size_t size_ = 0;
    DuplicatePolicy policy_;
};
