#pragma once 
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <limits> 
#include <algorithm>
#include <memory>            
#include "iterator.h"

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    Vector();
    Vector(const Vector& other); 
    Vector(Vector&& other) noexcept; 
    ~Vector();

    Vector& operator=(const Vector& other); 
    Vector& operator=(Vector&& other) noexcept;
    void PushBack(const T& value);
    void PushBack(T&& value);
    void PopBack();
    void Clear();

    size_t Size() const;
    size_t Capacity() const;
    void Resize(size_t n, const T& value = T());
    void Reserve(size_t n);

    T& Back();
    T* Data();
    T& Front();
    std::size_t Max_Size() const;
    void Swap(Vector& other);
    void Insert(size_t position, const T& value);
    void Erase(size_t position, size_t count = 1);

    Iterator<T> CBegin() const;
    Iterator<T> Begin();
    Iterator<T> End();
    T& At(size_t index);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    bool Empty() const;
    void Assign(const T* elements, size_t count);
    Iterator<T> RBegin();
    Iterator<T> REnd();

    template<typename... Args>
    T& Emplace(size_t position, Args&&... args);
    template<typename... Args>
    T& EmplaceBack(Args&&... args);
    template<typename... Args>
    T& Emplace(Iterator<T> position, Args&&... args);

private:
    void ReAlloc(size_t newCapacity);

private:


    Allocator m_Alloc;   
    T* m_Data = nullptr;   
    size_t m_Size = 0;     
    size_t m_Capacity = 0; 
};

///////////////////////////////////////////////////


template<typename T, typename Allocator>
Vector<T, Allocator>::Vector() {
    ReAlloc(2);
}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector& other)
    : m_Size(other.m_Size), m_Capacity(other.m_Capacity) {
    m_Data = m_Alloc.allocate(m_Capacity);

    for (size_t i = 0; i < m_Size; ++i) {
        m_Alloc.construct(&m_Data[i], other.m_Data[i]); 
     }
}
template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept
 : m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity){
    other.m_Data = nullptr;
    other.m_Size = 0;
    other.m_Capacity = 0;
 }

template<typename T, typename Allocator>
Vector<T, Allocator>::~Vector() {
    Clear();
    m_Alloc.deallocate(m_Data, m_Capacity);
}

template<typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& other) {
    if (this != &other){
        Clear();
        m_Alloc.deallocate(m_Data, m_Capacity);
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_Data = m_Alloc.allocate(m_Capacity);
        for (size_t i = 0; i < m_Size; ++i) {
            m_Alloc.construct(&m_Data[i], other.m_Data[i]); 
        }
    }
    return *this;
}

template<typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        Clear();
        m_Alloc.deallocate(m_Data, m_Capacity);

        m_Data = other.m_Data;
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        other.m_Data = nullptr; 
        other.m_Size = 0;
        other.m_Capacity = 0;
    }
    return *this;
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::ReAlloc(size_t newCapacity) {
    if (newCapacity <= m_Size)
        return;

    T* newBlock = m_Alloc.allocate(newCapacity);

    /*for (size_t i = 0; i < m_Size; ++i) {
        new(newBlock + i) T(std::move(m_Data[i])); 
        m_Data[i].~T(); 
    }*/
    try {
        for (size_t i = 0; i < m_Size; ++i) {
            m_Alloc.construct(&newBlock[i], std::move(m_Data[i]));
            m_Alloc.destroy(&m_Data[i]);
        }
    } catch (...) {
        m_Alloc.deallocate(newBlock, newCapacity);
        throw; 
    }

    m_Alloc.deallocate(m_Data, m_Capacity);

    m_Data = newBlock;
    m_Capacity = newCapacity;
}

// PushBack (копирование)
template<typename T, typename Allocator>
void Vector<T, Allocator>::PushBack(const T& value) {
    if (m_Size >= m_Capacity)
        ReAlloc(m_Capacity > 0 ? 2 * m_Capacity : 1);

    m_Alloc.construct(&m_Data[m_Size], value);
    /*Инициализация объекта с помощью placement-new: 
    Метод construct аллокатора вызывает placement-new для создания нового объекта в указанной памяти, 
    используя аргумент value в качестве параметра конструктора. То есть эта строка эквивалентна следующему вызову:

    эквивалентно
    new (&m_Data[i]) T(value);
    Использование copy-конструктора: При передаче value (если оно является lvalue и не преобразовано в rvalue с помощью std::move)
    будет вызван копирующий конструктор класса T. То есть создаваемый объект инициализируется копией 
    объекта value.
 */
    ++m_Size;
}

// PushBack (перемещение)
template<typename T, typename Allocator>
void Vector<T, Allocator>::PushBack(T&& value) {
    if (m_Size >= m_Capacity)
        ReAlloc(m_Capacity > 0 ? 2 * m_Capacity : 1);

    m_Alloc.construct(&m_Data[m_Size], std::move(value));
    ++m_Size;
}

// удаляет последний элемент
template<typename T, typename Allocator>
void Vector<T, Allocator>::PopBack() {
    if (m_Size > 0) {
        --m_Size;
        m_Alloc.destroy(&m_Data[m_Size]);
    }
}

//  очищает массив
template<typename T, typename Allocator>
void Vector<T, Allocator>::Clear() {
    for (size_t i = 0; i < m_Size; ++i) {
        m_Alloc.destroy(&m_Data[i]);
    }
    m_Size = 0;
}

//  возвращает количество элементов
template<typename T, typename Allocator>
size_t Vector<T, Allocator>::Size() const {
    return m_Size;
}

//  возвращает выделенную ёмкость
template<typename T, typename Allocator>
size_t Vector<T, Allocator>::Capacity() const {
    return m_Capacity;
}

// изменяет размер вектора, заполняя новыми значениями или уничтожая лишние элементы
template<typename T, typename Allocator>
void Vector<T, Allocator>::Resize(size_t n, const T& value) {
    if (n > m_Capacity) {
        Reserve(n);
    }

    if (n > m_Size) {
        for (size_t i = m_Size; i < n; ++i) {
            m_Alloc.construct(&m_Data[i], value);
        }
    } else {
        for (size_t i = n; i < m_Size; ++i) {
            m_Alloc.destroy(&m_Data[i]);
        }
    }

    m_Size = n;
}

// резервирует память для как минимум n элементов
template<typename T, typename Allocator>
void Vector<T, Allocator>::Reserve(size_t n) {
    if (n <= m_Capacity)
        return;

    T* new_Data = m_Alloc.allocate(n);

    for (size_t i = 0; i < m_Size; ++i) {
        m_Alloc.construct(&new_Data[i], std::move(m_Data[i])); // move rvalue без затрат на полное копирование с помощью конструктора
        m_Alloc.destroy(&m_Data[i]);
    }

    m_Alloc.deallocate(m_Data, m_Capacity);
    m_Data = new_Data;
    m_Capacity = n;
}

// возвращает ссылку на последний элемент
template<typename T, typename Allocator>
T& Vector<T, Allocator>::Back() {
    if (m_Size == 0)
        throw std::out_of_range("Vector is empty");
    return m_Data[m_Size - 1];
}

//  возвращает указатель на данные
template<typename T, typename Allocator>
T* Vector<T, Allocator>::Data() {
    if (m_Size == 0)
        throw std::out_of_range("Vector is empty");
    return m_Data;
}

// возвращает ссылку на первый элемент
template<typename T, typename Allocator>
T& Vector<T, Allocator>::Front() {
    if (m_Size == 0)
        throw std::out_of_range("Vector is empty");
    return m_Data[0];
}

// возвращает максимально возможное количество элементов
template<typename T, typename Allocator>
std::size_t Vector<T, Allocator>::Max_Size() const {
    return std::numeric_limits<std::size_t>::max() / sizeof(T);
}

//  копирует элементы из массива в вектор
template<typename T, typename Allocator>
void Vector<T, Allocator>::Assign(const T* elements, size_t count) {
    Clear();
    Resize(count);
    for (size_t i = 0; i < count; i++) {
        m_Alloc.construct(&m_Data[i], elements[i]);
    }
}

//  меняет содержимое двух векторов
template<typename T, typename Allocator>
void Vector<T, Allocator>::Swap(Vector& other) {
    std::swap(m_Data, other.m_Data);
    std::swap(m_Size, other.m_Size);
    std::swap(m_Capacity, other.m_Capacity);
}

// вставляет элемент на указанную позицию
template<typename T, typename Allocator>
void Vector<T, Allocator>::Insert(size_t position, const T& value) {
    if (position > m_Size)
        throw std::out_of_range("Invalid position for Insert");

    if (m_Size >= m_Capacity)
        ReAlloc(m_Capacity > 0 ? m_Capacity * 2 : 1);


    for (size_t i = m_Size; i > position; --i) {
        m_Alloc.construct(&m_Data[i], std::move(m_Data[i - 1]));
        m_Alloc.destroy(&m_Data[i - 1]);
    }

    m_Alloc.construct(&m_Data[position], value);
    ++m_Size;
}

// удаляет один или несколько элементов, начиная с позиции position
template<typename T, typename Allocator>
void Vector<T, Allocator>::Erase(size_t position, size_t count) {
    if (position + count > m_Size || position >= m_Size)
        throw std::out_of_range("Invalid range for Erase");


    for (size_t i = position; i < m_Size - count; ++i) {    
        m_Alloc.construct(&m_Data[i], std::move(m_Data[i + count]));
        m_Alloc.destroy(&m_Data[i + count]);
    }

    for (size_t i = m_Size - count; i < m_Size; ++i) {
        m_Alloc.destroy(&m_Data[i]);
    }
    m_Size -= count;
}

// возвращает итератор на первый элемент
template<typename T, typename Allocator>
Iterator<T> Vector<T, Allocator>::Begin() {
    return Iterator<T>(m_Data);
}

//  возвращает константный итератор на первый элемент
template<typename T, typename Allocator>
Iterator<T> Vector<T, Allocator>::CBegin() const {
    return Iterator<T>(m_Data);
}

// возвращает итератор на элемент, следующий за последним
template<typename T, typename Allocator>
Iterator<T> Vector<T, Allocator>::End() {
    if (m_Size == 0) {
        return Iterator<T>(m_Data); 
    }
    
    return Iterator<T>(m_Data + m_Size);
}

// возвращает итератор для обратного обхода (последний элемент)
template<typename T, typename Allocator>
Iterator<T> Vector<T, Allocator>::RBegin() {
    if (m_Size == 0) {
        return Iterator<T>(m_Data); 
    }
    return Iterator<T>(m_Data + m_Size - 1);
}

// возвращает итератор, указывающий перед первым элементом в обратном обходе
template<typename T, typename Allocator>
Iterator<T> Vector<T, Allocator>::REnd() {
    return Iterator<T>(m_Data - 1);
}

// проверяет, пуст ли вектор
template<typename T, typename Allocator>
bool Vector<T, Allocator>::Empty() const {
    return m_Size == 0;
}

// возвращает ссылку на элемент с проверкой границ
template<typename T, typename Allocator>
T& Vector<T, Allocator>::At(size_t index) {
    if (index >= m_Size)
        throw std::out_of_range("Index out of range");
    return m_Data[index];
}

// Оператор [] для доступа к элементу без проверки
template<typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](size_t index) {
    return m_Data[index];
}

template<typename T, typename Allocator>
const T& Vector<T, Allocator>::operator[](size_t index) const {
    return m_Data[index];
}

//  вставляет элемент, создавая его непосредственно в памяти
template<typename T, typename Allocator>
template<typename... Args>
T& Vector<T, Allocator>::Emplace(size_t position, Args&&... args) {
    if (position > m_Size)
        throw std::out_of_range("Invalid position for Emplace");

    if (m_Size >= m_Capacity)
        ReAlloc(m_Capacity > 0 ? m_Capacity * 2 : 1);   

    for (size_t i = m_Size; i > position; --i) {
        m_Alloc.construct(&m_Data[i], std::move(m_Data[i - 1]));
        m_Alloc.destroy(&m_Data[i - 1]);
    }

    m_Alloc.construct(&m_Data[position], std::forward<Args>(args)...);
    ++m_Size;
    return m_Data[position];
}
template<typename T, typename Allocator>
template<typename... Args>
T& Vector<T, Allocator>::Emplace(Iterator<T> position, Args&&... args) {
    size_t posIndex = position.base() - m_Data; 
    return Emplace(posIndex, std::forward<Args>(args)...);
}

//  создает новый элемент в конце вектора без лишнего копирования
template<typename T, typename Allocator>
template<typename... Args>
T& Vector<T, Allocator>::EmplaceBack(Args&&... args) {
    if (m_Size >= m_Capacity)
        ReAlloc(m_Capacity > 0 ? m_Capacity * 2 : 1);

    m_Alloc.construct(&m_Data[m_Size], std::forward<Args>(args)...);
    return m_Data[m_Size++];
}
