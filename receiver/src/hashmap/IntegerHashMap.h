#include <Arduino.h>

/**
 * This is NOT a perfect hash function but it is a simplified version
 * to avoid useless massive performance requirement and too high complexities
 * for an embedded system purpose.
 * @tparam K
 */
template <typename K>
class IntegerHashMap{
public:
    IntegerHashMap(int size);

    bool containsKey(int key);

    void put(int key, K object);

    K get(int key);

private:
    int size;

    K* elements;

    int computeHash(int key);
};