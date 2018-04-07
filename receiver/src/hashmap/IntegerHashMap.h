#include <Arduino.h>
#ifndef INTEGERHASHMAP_H_
#define INTEGERHASHMAP_H_
/**
 * This is not a perfect hash map since it is a simplified version
 * to avoid useless massive performance requirement and too high complexities
 * for an embedded system purpose.
 * This simplification implies that the objects with the same key can't be stored,
 * consequently, before inserting an element, should be better controlling if its place
 * is free or taken.
 * @tparam K the storing object type
 */
template <typename K>
class IntegerHashMap{
public:
    /**
     * This method is the constructor.
     * @param size the map size
     */
    IntegerHashMap(int size);

    /**
     * This method verifies if an key is used or free.
     * @param key the key to control
     * @return if the cell is taken, true, else false
     */
    bool containsKey(int key);

    /**
     * This method is used to insert an element in the map.
     * This method doesn't control if the cell is free or not, consequently,
     * this method will overwrite an already present element.
     * @param key
     * @param object
     */
    void put(int key, K object);

    /**
     * This method is used to get a specific element given its key.
     * @param key the object key
     * @return the object if it is present, else NULL
     */
    K get(int key);

private:
    /**
     * The map size passed during the instantiation.
     */
    int size;

    /**
     * This is a pointer to the instanced array that contains the elements.
     */
    K* elements;

    /**
     * This method is used to compute the hash address given a key.
     * @param key the key used for this computation
     * @return the hashed key
     */
    int computeHash(int key);
};

#endif
