#include "binary_search_tree.hpp"

#include <ostream>
#include <string>

BinarySearchTree::BinarySearchTree(DuplicatePolicy policy) noexcept : policy_(policy) {}

BinarySearchTree::~BinarySearchTree() {
    clear();
}

bool BinarySearchTree::insert(int value) {
    // link указывает на указатель, в который нужно записать новый узел.
    Node** link = &root_;
    while (*link != nullptr) {
        if (value == (*link)->data && policy_ == DuplicatePolicy::Reject) {
            return false;
        }
        if (value < (*link)->data) {
            link = &(*link)->left;
        } else {
            // При разрешённых повторах равные значения направляются вправо.
            link = &(*link)->right;
        }
    }

    *link = new Node{value, nullptr, nullptr};
    ++size_;
    return true;
}

bool BinarySearchTree::contains(int value) const noexcept {
    const Node* current = root_;
    while (current != nullptr) {
        if (value == current->data) {
            return true;
        }
        current = value < current->data ? current->left : current->right;
    }
    return false;
}

std::size_t BinarySearchTree::count(int value) const noexcept {
    std::size_t occurrences = 0;
    const Node* current = root_;
    while (current != nullptr) {
        if (value < current->data) {
            current = current->left;
        } else {
            if (value == current->data) {
                ++occurrences;
            }
            // Слева значения строго меньше. Остальные повторы могут быть
            // только справа; при следующих сравнениях путь может идти и влево.
            current = current->right;
        }
    }
    return occurrences;
}

std::size_t BinarySearchTree::size() const noexcept {
    return size_;
}

void BinarySearchTree::print(std::ostream& output) const {
    if (root_ == nullptr) {
        output << "(пусто)\n";
        return;
    }
    std::string prefix;
    printSubtree(root_->right, prefix, false, output);
    output << root_->data << '\n';
    printSubtree(root_->left, prefix, true, output);
}

void BinarySearchTree::clear() noexcept {
    destroySubtree(root_);
    root_ = nullptr;
    size_ = 0;
}

void BinarySearchTree::destroySubtree(Node* node) noexcept {
    if (node == nullptr) {
        return;
    }
    destroySubtree(node->left);
    destroySubtree(node->right);
    delete node;
}

void BinarySearchTree::printSubtree(const Node* node, std::string& prefix,
                                  bool leftChild, std::ostream& output) {
    if (node == nullptr) {
        return;
    }
    const std::size_t prefixLength = prefix.size();
    if (node->right != nullptr) {
        prefix += leftChild ? "│   " : "    ";
        printSubtree(node->right, prefix, false, output);
        prefix.resize(prefixLength);
    }

    output << prefix << (leftChild ? "└── " : "┌── ") << node->data << '\n';

    if (node->left != nullptr) {
        prefix += leftChild ? "    " : "│   ";
        printSubtree(node->left, prefix, true, output);
        prefix.resize(prefixLength);
    }
}
