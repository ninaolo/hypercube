#include "container.hpp"

/*
 This is a hypercube which uses templates in C++ to create multiple dimensions.

 The hypercube uses private inheritance from the vector class so that no methods
 can be used. This makes sure that for example push_back() does not work, since
 it would change the dimension of the hypercube.
*/

template <typename T, size_t dim>
class Hypercube : private Vector< Hypercube<T, dim-1> > { // Goes "deeper" into the hypercube
public:
    Hypercube() : Vector< Hypercube<T, dim-1> >() {};
    Hypercube(size_t s) : Vector< Hypercube<T, dim-1> >(s, Hypercube<T, dim-1>(s)) {};
    using Vector< Hypercube<T, dim-1> >::operator[];
};

template <typename T> // Last dimension = Vector
class Hypercube<T, 1> : Vector<T> { // Special case
public:
    Hypercube() : Vector<T>() {};
    Hypercube(size_t s) : Vector<T>(s) {};
    using Vector<T>::operator[];
};