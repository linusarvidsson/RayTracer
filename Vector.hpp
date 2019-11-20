#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>
#include <cmath>

class Vector {
public:
    double x, y, z;
    
    Vector();
    Vector(double x_, double y_, double z_);
    Vector cross(Vector rhs);
    double dot(Vector rhs);
    double magnitude();
    double magnitudeSquared();
    Vector normalize();
    
    Vector operator+(Vector rhs);
    Vector operator-(Vector rhs);
    Vector operator*(double rhs);
};

#endif /* Vector_hpp */
