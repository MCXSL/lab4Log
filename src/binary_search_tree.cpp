#include "binary_search_tree.hpp"

#include <ostream>
#include <string>
#include <utility>

namespace {

std::string nodeLabel(int value, std::size_t level) {
    return std::to_string(value) + " [" + std::to_string(level) + ']';
}

void printLineBetween(std::ostream& output, std::size_t start, std::size_t end) {
    for (std::size_t column = start + 1; column < end; ++column) {
        output << "─";
    }
}

} // namespace

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

TreeSearchResult BinarySearchTree::findMatches(int value) const {
    TreeSearchResult result;
    const Node* current = root_;
    while (current != nullptr) {
        if (value == current->data) {
            result.levels.push_back(result.directions.size());
        }
        const bool goLeft = value < current->data;
        current = goLeft ? current->left : current->right;
        result.directions += goLeft ? 'L' : 'R';
    }

    // Для вывода нужен только путь до последнего найденного узла.
    result.directions.resize(result.levels.empty() ? 0 : result.levels.back());
    return result;
}

void BinarySearchTree::print(std::ostream& output, TreeOrientation orientation) const {
    if (root_ == nullptr) {
        output << "(пусто)\n";
        return;
    }
    if (orientation == TreeOrientation::Vertical) {
        printVertical(output);
        return;
    }

    std::string prefix;
    printSubtree(root_->right, prefix, false, 1, output);
    output << nodeLabel(root_->data, 0) << '\n';
    printSubtree(root_->left, prefix, true, 1, output);
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
                                  bool leftChild, std::size_t level,
                                  std::ostream& output) {
    if (node == nullptr) {
        return;
    }
    const std::size_t prefixLength = prefix.size();
    if (node->right != nullptr) {
        prefix += leftChild ? "│   " : "    ";
        printSubtree(node->right, prefix, false, level + 1, output);
        prefix.resize(prefixLength);
    }

    output << prefix << (leftChild ? "└── " : "┌── ")
           << nodeLabel(node->data, level) << '\n';

    if (node->left != nullptr) {
        prefix += leftChild ? "    " : "│   ";
        printSubtree(node->left, prefix, true, level + 1, output);
        prefix.resize(prefixLength);
    }
}

void BinarySearchTree::printVertical(std::ostream& output) const {
    struct PositionedNode {
        std::string label;
        std::size_t start;
        std::size_t center;
        std::size_t left;
        std::size_t right;
    };

    std::vector<std::vector<PositionedNode>> rows;
    std::size_t nextColumn = 0;

    // Симметричный обход выделяет подписи слева направо без наложений.
    const auto place = [&](const auto& self, const Node* node, std::size_t level)
        -> std::size_t {
        if (level == rows.size()) {
            rows.emplace_back();
        }
        const auto left = node->left == nullptr ? std::string::npos
                                                : self(self, node->left, level + 1);
        auto label = nodeLabel(node->data, level);
        const auto start = nextColumn;
        const auto center = start + label.size() / 2;
        nextColumn += label.size() + 2;
        const auto right = node->right == nullptr ? std::string::npos
                                                  : self(self, node->right, level + 1);
        rows[level].push_back({std::move(label), start, center,
                               left == std::string::npos ? center : left,
                               right == std::string::npos ? center : right});
        return center;
    };
    place(place, root_, 0);

    for (const auto& row : rows) {
        std::size_t column = 0;
        for (const auto& node : row) {
            output << std::string(node.start - column, ' ') << node.label;
            column = node.start + node.label.size();
        }
        output << '\n';

        column = 0;
        for (const auto& node : row) {
            const bool hasLeft = node.left < node.center;
            const bool hasRight = node.right > node.center;
            if (!hasLeft && !hasRight) {
                continue;
            }
            output << std::string(node.left - column, ' ');
            if (hasLeft) {
                output << "┌";
                printLineBetween(output, node.left, node.center);
            }
            output << (hasLeft ? (hasRight ? "┴" : "┘") : "└");
            if (hasRight) {
                printLineBetween(output, node.center, node.right);
                output << "┐";
            }
            column = node.right + 1;
        }
        if (column != 0) {
            output << '\n';
        }
    }
}
