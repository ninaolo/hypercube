#include <iostream>
#include <initializer_list>
#include <stdexcept>

using namespace std;

#ifndef __SIMPLE_CONTAINER_H_INCLUDED__
#define __SIMPLE_CONTAINER_H_INCLUDED__

template <typename T>
class Vector {
    
private:
    size_t vectorsize; // The "user" size of the vector
    T* array;
    void checkRange(size_t i) const;
    size_t heap_size; // The "real" size of the vector on the heap
    
public:

    // Constructors + destructor
    explicit Vector();
    explicit Vector(size_t s);
    explicit Vector(const initializer_list<T>& l);
    explicit Vector(size_t s, T initvalue);
    ~Vector();
    
    // Copy constructor
    Vector(Vector const& src);
    
    //Copy assignment operator
    Vector& operator= (Vector const& src);
    
    // Move constructor
    Vector(Vector&& src);
    
    // Move assignment operator
    Vector& operator= (Vector&& src);
    
    // "Get" const index operator
    T const& operator[](size_t i) const;

    // "Set" index operator, not const
    T& operator[](size_t i);
    
    void clear();
    
    void reset();
    
    size_t size() const;
    
    void erase(size_t index);
    
    void push_back(T element);
    
    void insert(size_t index, T element);
    
    size_t capacity() const;
    
    void printvec() const;
    
    const T* begin() const;
    const T* end() const;
    const T* find(T const&) const;
    T* begin();
    T* end();
    T* find(T const&);

};

#endif

template <typename T>
Vector<T>::Vector(){
    static_assert(is_move_assignable<T>::value, "Type is not move assignable.");
    static_assert(is_move_constructible<T>::value, "Type is not move constructible."); // Can be constructed fr rvalue
    vectorsize = 0;
    heap_size = 0;
    array = new T[vectorsize];
}

template <typename T>
Vector<T>::Vector(size_t s) {
    static_assert(is_move_assignable<T>::value, "Type is not move assignable.");
    static_assert(is_move_constructible<T>::value, "Type is not move constructible."); // Can be constructed fr rvalue
    vectorsize = s;
    heap_size = s;
    array = new T[vectorsize];
    for (int i = 0; i < s; ++i) {
        array[i] = T();
    }
}

template <typename T>
Vector<T>::Vector(const initializer_list<T>& l) {
    static_assert(is_move_assignable<T>::value, "Type is not move assignable.");
    static_assert(is_move_constructible<T>::value, "Type is not move constructible."); // Can be constructed fr rvalue
    vectorsize = l.size();
    heap_size = l.size();
    array = new T[vectorsize];
    for (int i = 0; i < vectorsize; ++i) {
        array[i] = l.begin()[i];
    }
}

template <typename T>
Vector<T>::Vector(size_t s, T initvalue) {
    static_assert(is_move_assignable<T>::value, "Type is not move assignable.");
    static_assert(is_move_constructible<T>::value, "Type is not move constructible."); // Can be constructed fr rvalue
    vectorsize = s;
    heap_size = s;
    array = new T[vectorsize];
    for (int i = 0; i < s; ++i) {
        array[i] = initvalue;
    }
}

template <typename T>
Vector<T>::~Vector() {
    delete[] array;
}

template <typename T>
// Copy constructor
Vector<T>::Vector(Vector const& src) {
    vectorsize = src.vectorsize;
    heap_size = src.vectorsize;
    array = new T[src.vectorsize];
    for (int i = 0; i < src.vectorsize; ++i) {
        array[i] = src.array[i];
    }
}

template <typename T>
//Copy assignment operator
Vector<T>& Vector<T>::operator=(Vector const& src) {
    if (this == &src) {return *this;}
    vectorsize = src.vectorsize;
    heap_size = src.vectorsize;
    delete[] array; // Delete the old array since we get a new one, otherwise memory leak
    array = new T[src.vectorsize];
    for (int i = 0; i < src.vectorsize; ++i) {
        array[i] = src.array[i];
    }
    return *this; // c1  = c2 = c3 works because of this
}

template <typename T>
// Move constructor
Vector<T>::Vector(Vector&& src) {
    vectorsize = src.vectorsize;
    heap_size = src.vectorsize;
    array = src.array;
    src.vectorsize = 0;
    src.array = nullptr;
}

template <typename T>
// Move assignment operator
Vector<T>& Vector<T>::operator= (Vector&& src) {
    if (this != &src) {
        delete[] array; // Avoid memory-leak
        array = src.array;
        vectorsize = src.vectorsize;
        heap_size = src.vectorsize;
        src.vectorsize = 0;
        src.array = nullptr;
    }
    return *this;
}

template <typename T>
// "Get" const index operator
T const& Vector<T>::operator[](size_t i) const {
    //cout << "const";
    Vector<T>::checkRange(i);
    return array[i];
}

template <typename T>
// "Set" index operator, not const
T& Vector<T>::operator[](size_t i) {
    //cout << "ej const";
    Vector<T>::checkRange(i);
    return array[i];
}

template <typename T>
// Removes every element, making vectorsize = 0
void Vector<T>::clear() {
    delete [] array;
    vectorsize = 0;
    array = new T[heap_size];
}

template <typename T>
// Resets the elements to 0
void Vector<T>::reset() {
    for (int i = 0; i < vectorsize; ++i) {
        array[i] = T();
    }
}

template <typename T>
// Returns the number of elements in the container
size_t Vector<T>::size() const {
    return vectorsize;
}

template <typename T>
// Helper method for checking vector range
void Vector<T>::checkRange(size_t i) const {
    if (i > vectorsize-1 || vectorsize == 0) {
        throw out_of_range("");
    }
}

template <typename T>
// Removes the element at the index specified
void Vector<T>::erase(size_t index) {
    Vector<T>::checkRange(index);
    for (size_t i=index; i<vectorsize-1; i++) {
        array[i] = array[i+1];
    }
    array[vectorsize-1] = T();
    vectorsize = vectorsize -1;
}

template <typename T>
// Appends the given element to the end of the container with amortized constant time complexity
void Vector<T>::push_back(T element) {
    if (vectorsize == heap_size) {
        heap_size = vectorsize*2 + 1;
        T* temparray = new T[heap_size];
        for (size_t i=0; i<vectorsize; i++) {
            temparray[i] = array[i];
        }
        temparray[vectorsize] = element;
        vectorsize++;
        delete [] array;
        array = temparray;
    }
    else {
        array[vectorsize] = element;
        vectorsize++;
    }
}

template <typename T>
// Inserts the element value immediately before the index specified
void Vector<T>::insert(size_t index, T element) {
    if (index == vectorsize) {Vector<T>::push_back(element); return;}
    Vector<T>::checkRange(index);
    if (vectorsize == heap_size) {
        heap_size = vectorsize*2 + 1;
        T* temparray = new T[heap_size];
        for (size_t i=0; i<index; i++) {
            temparray[i] = array[i];
        }
        temparray[index] = element;
        for (size_t i=index+1; i<=vectorsize; i++) {
            temparray[i] = array[i-1];
        }
        vectorsize++;
        delete [] array;
        array = temparray;
    }
    else {
        for (size_t i=vectorsize; i>index; i--) {
            array[i] = array[i-1];
        }
        array[index] = element;
        vectorsize++;
    }
}

template <typename T>
// Returns the number of elements that can potentially be stored in the container
size_t Vector<T>::capacity() const {
    return heap_size;
}

template <typename T>
// Prints the vector
void Vector<T>::printvec() const {
    for (size_t i=0;i<vectorsize; i++) {
        cout << array[i] << " ";
    }
}

template <typename T>
// Returns a RandomAccessIterator to the first element of the range
const T* Vector<T>::begin() const {
    return &array[0];
}


template <typename T>
// Returns a RandomAccessIterator referring to the element right after the last element in the container
const T* Vector<T>::end() const {
    return &array[vectorsize];
}

template <typename T>
// Returns a RandomAccessIterator referring to the first element that compares equal to the argument, or end()
const T* Vector<T>::find(T const& element) const {
    for (size_t i = 0; i<vectorsize; i++) {
        if (array[i] == element) {
            return &array[i];
        }
    }
    return Vector<T>::end();
}

template <typename T>
// Returns a RandomAccessIterator to the first element of the range
T* Vector<T>::begin() {
    return &array[0];
}

template <typename T>
// Returns a RandomAccessIterator referring to the element right after the last element in the container
T* Vector<T>::end() {
    return &array[vectorsize];
}

template <typename T>
// Returns a RandomAccessIterator referring to the first element that compares equal to the argument, or end()
T* Vector<T>::find(T const& element) {
    for (size_t i = 0; i<vectorsize; i++) {
        if (array[i] == element) {
            return &array[i];
        }
    }
    return Vector<T>::end();
}

