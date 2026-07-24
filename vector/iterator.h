template<typename T>
class Iterator {
private:
    T* cur;
public:
    Iterator(T* first) : cur(first) {}

    // Префиксный ++
    Iterator& operator++() {
        ++cur;
        return *this;
    }

    // Постфиксный ++
    Iterator operator++(int) {
        Iterator temp = *this;
        ++cur;
        return temp;
    }

    // Префиксный --
    Iterator& operator--() {
        --cur;
        return *this;
    }

    // Постфиксный --
    Iterator operator--(int) {
        Iterator temp = *this;
        --cur;
        return temp;
    }

    // Оператор + возвращает новый итератор
    Iterator operator+(int n) const {
        return Iterator(cur + n);
    }

    // Оператор - возвращает новый итератор
    Iterator operator-(int n) const {
        return Iterator(cur - n);
    }

    // Разыменование
    T& operator*() const {
        return *cur;
    }

    // Операторы сравнения
    bool operator!=(const Iterator& it) const {
        return cur != it.cur;
    }

    bool operator==(const Iterator& it) const {
        return cur == it.cur;
    }

    // Возвращает базовый указатель
    T* base() const {
        return cur;
    }

    ~Iterator() {}
};