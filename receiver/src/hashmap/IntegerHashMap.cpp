#include "IntegerHashMap.h"

template class IntegerHashMap<void (*)(String)>;

template <typename K>
IntegerHashMap<K>::IntegerHashMap(int size) {
    this->size = size;
    this->elements = new K[size];
    for(int i = size-1; i>=0; i--)
        this->elements[i] = NULL;
}

template <typename K>
bool IntegerHashMap<K>::containsKey(int key) {
    return get(key) != NULL;
}

template <typename K>
void IntegerHashMap<K>::put(int key, K object) {
    this->elements[computeHash(key)] = object;
}

template <typename K>
K IntegerHashMap<K>::get(int key) {
    return this->elements[computeHash(key)];
}

template <typename K>
int IntegerHashMap<K>::computeHash(int key) {
    return key%(this->size);
}