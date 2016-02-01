# Hypercube

This is a multidimensional vector (or hypercube) written in C++ in collaboration with Nivin Fakih.

## Use the vector


Hypercube<int, 3> n(7);                         // an int hypercube with 7*7*7 elements
Hypercube<std::string, 6> m(5);                 // six dimensions with strings, 5*5*...*5 elements
m[1][3][2][1][4][0] = 7;						// set an element of the hypercube
`