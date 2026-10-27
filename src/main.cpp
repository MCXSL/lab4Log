#include "binary_search_tree.hpp"

#include <iostream>
#include <new>
#include <sstream>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {

bool readInteger(const std::string& prompt, int& value) {
    std::string token;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> token)) {
            return false;
        }

        std::istringstream parser(token);
        char extra = '\0';
        if ((parser >> value) && !(parser >> extra)) {
            return true;
        }
        std::cout << "Ошибка: введите целое число в диапазоне int.\n";
    }
}

void printTrees(const BinarySearchTree& withDuplicates,
                const BinarySearchTree& withoutDuplicates) {
    std::cout << "\nДерево с повторами (узлов: " << withDuplicates.size() << "):\n";
    withDuplicates.print(std::cout);
    std::cout << "\nДерево без повторов (узлов: " << withoutDuplicates.size() << "):\n";
    withoutDuplicates.print(std::cout);
    std::cout << "\nКорень слева; правое поддерево сверху, левое снизу.\n";
}

void showSearch(const BinarySearchTree& tree, const std::string& label, int value) {
    std::cout << label << ": значение " << value
              << (tree.contains(value) ? " найдено.\n" : " не найдено.\n");
}

int run() {
    BinarySearchTree withDuplicates(DuplicatePolicy::Allow);
    BinarySearchTree withoutDuplicates(DuplicatePolicy::Reject);

    std::cout << "Лабораторная работа № 4. Бинарное дерево поиска\n"
              << "Введите целые числа. -1 завершает начальное построение.\n"
              << "Из одного ввода строятся деревья с повторами и без повторов.\n";

    int value = 0;
    while (true) {
        if (!readInteger("Число: ", value)) {
            std::cout << "\nВвод завершён.\n";
            printTrees(withDuplicates, withoutDuplicates);
            return 0;
        }
        if (value == -1) {
            break;
        }
        withDuplicates.insert(value);
        withoutDuplicates.insert(value);
    }

    printTrees(withDuplicates, withoutDuplicates);

    while (true) {
        std::cout << "\n1 — вывести деревья\n"
                  << "2 — найти значение\n"
                  << "3 — подсчитать вхождения\n"
                  << "4 — добавить число в оба дерева\n"
                  << "5 — очистить оба дерева\n"
                  << "0 — выход\n";

        int command = 0;
        if (!readInteger("Выберите действие: ", command) || command == 0) {
            std::cout << "\nРабота завершена.\n";
            return 0;
        }

        switch (command) {
        case 1:
            printTrees(withDuplicates, withoutDuplicates);
            break;
        case 2:
            if (!readInteger("Искомое значение: ", value)) {
                return 0;
            }
            showSearch(withDuplicates, "С повторами", value);
            showSearch(withoutDuplicates, "Без повторов", value);
            break;
        case 3:
            if (!readInteger("Значение для подсчёта: ", value)) {
                return 0;
            }
            std::cout << "С повторами: вхождений = " << withDuplicates.count(value) << '\n';
            std::cout << "Без повторов: вхождений = " << withoutDuplicates.count(value) << '\n';
            break;
        case 4:
            if (!readInteger("Добавляемое число (можно -1): ", value)) {
                return 0;
            }
            withDuplicates.insert(value);
            std::cout << "С повторами: число " << value << " добавлено.\n";
            if (withoutDuplicates.insert(value)) {
                std::cout << "Без повторов: число " << value << " добавлено.\n";
            } else {
                std::cout << "Без повторов: число " << value
                          << " уже есть, повтор не добавлен.\n";
            }
            break;
        case 5:
            withDuplicates.clear();
            withoutDuplicates.clear();
            std::cout << "Оба дерева очищены.\n";
            break;
        default:
            std::cout << "Нет такого пункта меню. Выберите число от 0 до 5.\n";
            break;
        }
    }
}

} // namespace

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    try {
        return run();
    } catch (const std::bad_alloc&) {
        std::cerr << "Недостаточно памяти для выполнения операции.\n";
        return 1;
    }
}
