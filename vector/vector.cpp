#include <iostream>
#include "vector.h" // Убедитесь, что путь к заголовочному файлу корректен
#include <vector>
struct MyType {
    MyType(int x, int y, int z) : a(x), b(y), c(z) {}
    int a, b, c;
};
int main() {
    Vector<int> vec;

    // Тестируем PushBack
    std::cout << "PushBack:" << std::endl;
    vec.PushBack(1);
    vec.PushBack(2);
    vec.PushBack(3);
    std::cout << "Размер вектора: " << vec.Size() << std::endl; // Ожидается 3
    std::cout << "Элементы вектора: ";
    for (size_t i = 0; i < vec.Size(); ++i) {
        std::cout << vec[i] << " "; // Ожидается 1 2 3
    }
    std::cout << std::endl;

    // Тестируем Emplace
    std::cout << "Emplace:" << std::endl;
    Vector<MyType> vec1;
    vec1.Emplace(vec1.Begin(), 1, 2, 3);

    // Тестируем PopBack
    std::cout << "PopBack:" << std::endl;
    vec.PopBack();
    std::cout << "Размер вектора после PopBack: " << vec.Size() << std::endl; // Ожидается 2
    std::cout << "Элементы вектора: ";
    for (size_t i = 0; i < vec.Size(); ++i) {
        std::cout << vec[i] << " "; // Ожидается 1 2
    }
    std::cout << std::endl;

    // Тестируем Insert
    std::cout << "Insert:" << std::endl;
    vec.Insert(1, 4);
    std::cout << "Элементы вектора после Insert: ";
    for (size_t i = 0; i < vec.Size(); ++i) {
        std::cout << vec[i] << " "; // Ожидается 1 4 2
    }
    std::cout << std::endl;

    // Тестируем Erase
    std::cout << "Erase:" << std::endl;
    vec.Erase(0); // Удаляем первый элемент
    std::cout << "Элементы вектора после Erase: ";
    for (size_t i = 0; i < vec.Size(); ++i) {
        std::cout << vec[i] << " "; // Ожидается 4 2
    }
    std::cout << std::endl;

    // Тестируем EmplaceBack
    std::cout << "EmplaceBack:" << std::endl;
    vec.EmplaceBack(5);
    std::cout << "Элементы вектора после EmplaceBack: ";
    for (size_t i = 0; i < vec.Size(); ++i) {
        std::cout << vec[i] << " "; // Ожидается 4 2 5
    }
    std::cout << std::endl;

    // Тестируем At
    std::cout << "At:" << std::endl;
    std::cout << "Элемент по индексу 1: " << vec.At(1) << std::endl; // Ожидается 2

    // Тестируем Clear
    std::cout << "Clear:" << std::endl;
    vec.Clear();
    std::cout << "Размер вектора после Clear: " << vec.Size() << std::endl; // Ожидается 0

    // Тестируем Empty
    std::cout << "Empty: " << (vec.Empty() ? "Да" : "Нет") << std::endl; // Ожидается "Да"
    std:: cout << "---------------------------------------------------------------" << std:: endl;
    std::vector<int> vector;
vector.emplace(vector.begin(), 42); // Вставляет число 42
for (size_t i = 0; i < vector.size(); ++i) {
    std::cout << vector[i] << " "; // Ожидается 4 2 5
}


    return 0;
}