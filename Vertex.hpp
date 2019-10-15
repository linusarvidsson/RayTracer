#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include "Vector.hpp"

class Vertex {
public:
    double x;
    double y;
    double z;
    double w;
    
    Vertex();
    Vertex(double x_, double y_, double z_, double w_);
    Vertex(Vector v);
    Vector operator-(Vertex rhs);
    Vertex operator+(Vector rhs);
    double magnitude();
};

#endif /* Vertex_hpp */
