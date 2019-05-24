#include <iostream>


template <typename T>
class HashTable {
public:
    bool insert(const T& value);
    bool remove(const T& value);
    bool search(const T& value) const;

    HashTable(size_t size = 8, double fill_factor = 0.75) {
        max_size = size;
        current_size = 0;
        max_fill_factor = fill_factor;
        create_empty_data(size);
    }
    ~HashTable() { delete[] data; }

private:
    struct TableNode;

    size_t max_size;
    size_t current_size;
    TableNode** data;
    double max_fill_factor;

    int hash(const T& value) const;
    void rehash();
    double get_fill_factor() const { return static_cast<double>(current_size) / max_size; }
    void create_empty_data(size_t size) { data = new TableNode*[size]; for (size_t i = 0; i < size; ++i) data[i] = nullptr; }

};

template <typename T>
struct HashTable<T>::TableNode {
    bool deleted;
    T value;
    TableNode() { deleted = true; }
    TableNode(T val) { deleted = false; value = val; }
    ~TableNode() {};
};

template<typename T>
bool HashTable<T>::insert(const T & value) {
    if (search(value)) return false;
    if (get_fill_factor() >= max_fill_factor) rehash();
    int h = hash(value);
    for (size_t i = 0; i < max_size; ++i) {
        h = (h + i) % max_size;
        if (!data[h]) {
            data[h] = new TableNode(value);
            ++current_size;
            return true;
        }
        if (data[h]->value == value && data[h]->deleted) {
            data[h]->deleted = false;
            ++current_size;
            return true;
        }
    }
}

template<typename T>
bool HashTable<T>::remove(const T & value) {
    if (!search(value)) return false;
    int h = hash(value);
    for (size_t i = 0; i < max_size; ++i) {
        h = (h + i) % max_size;
        if (!data[h]) return false;
        if (data[h]->value == value && !data[h]->deleted) {
            data[h]->deleted = true;
            return true;
        }
    }
    return false;
}

template<typename T>
bool HashTable<T>::search(const T & value) const {
    int h = hash(value);
    for (size_t i = 0; i < max_size; ++i) {
        h = (h + i) % max_size;
        if (!data[h]) return false;
        if (data[h]->value == value && !data[h]->deleted) return true;
        if (data[h]->value == value && data[h]->deleted) return false;
    }
    return false;
}

template<typename T>
int HashTable<T>::hash(const T & value) const {
    int hash = 0;
    for (size_t i = 0; i < value.size(); i++) {
        hash = (hash * 257 + value[i]) % max_size;
    }
    return hash;
}

template<typename T>
void HashTable<T>::rehash() {
    TableNode** tmp = new TableNode*[max_size];
    for (size_t i = 0; i < max_size; ++i) {
        tmp[i] = data[i];
    }
    delete[] data;
    size_t previous_max_size = max_size;
    max_size *= 2;
    create_empty_data(max_size);
    for (size_t i = 0; i < previous_max_size; ++i) {
        if (tmp[i] && !tmp[i]->deleted)
            insert(tmp[i]->value);
    }
    delete[] tmp;
}
