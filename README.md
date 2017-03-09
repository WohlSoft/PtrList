# PtrList
A vector-like container template which stores elements as array of pointers to each element

[![CircleCI](https://circleci.com/gh/WohlSoft/PtrList.svg?style=svg)](https://circleci.com/gh/WohlSoft/PtrList)

# Has two implementations
* vptrlist.h - implemented as inherence of std::vector<std::unique_ptr<T>> (works faster more stable)
* ptrlist.h - implemented from scratch (early implementation)

# How to use
Just include vptrlist.h into your project and use the class VPtrList by same way as well known std::vector. Useful in case when elements are must have persistent address to be able have an external pointers to each of them.

# Difference from regular std::vector
* Address of every element is persistent (until copy entire array)
* Has extra functions:
    * `bool contains(const T &item) const;` - to check existing of element by it's content
    * `ssize_t indexOf(const T &item) const;` - finds item at begin and returns index of it or -1 if item is not found
    * `iterator find(const T &item);` - finds item at begun and returns iterator of it or .end() if item is not found
    * `iterator find_last_of(const T &item)` - finds item at end and returns iterator of it or .rend() if item is not found
    * `void removeOne(const T &item)` - finds item by content and removes it and stops on first case
    * `void removeAll(const T &item)` - find all items by same content and remove all of them, and stop on reaching end
    * `void removeAt(size_t at)` - remove item by index
    * `void removeAt(size_t at, size_t num)` - remove range of items starts from index and to count of them
    * `void pop_front()` - removes first item
    * `void swap(size_t from, size_t to)` - internally swap two elements between their positions
    * `void move(size_t from, size_t to)` - internally move item from one position to another
    * `void push_front(const T &item)` - append item to begin
    * `T &first()` - equivalent of `front()`
    * `T &last()` - equivalent of `back()`

