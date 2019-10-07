//
//  Vector.hpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-09-13.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>
#include <cmath>

class Vector {
public:
    double x, y, z;
    
    Vector();
    Vector(double x_, double y_, double z_);
    Vector operator-(Vector rhs);
    Vector operator*(double rhs);
    Vector cross(Vector rhs);
    double dot(Vector rhs);
    double magnitude();
    
    Vector normalize();
};

#endif /* Vector_hpp */
