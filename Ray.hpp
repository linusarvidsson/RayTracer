#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include "ColorDbl.hpp"

class Triangle;

class Ray {
public:
    // Start and end point of ray
    Vertex start, end;
    
    // The point where the ray intersects an object
    Vertex intersection;
    // Normal of intersected object
    Vector objectNormal;
    // Color of intersected object
    int objectIndex;

    // Color information coming from the ray
    ColorDbl color;
    // Scalar t. Length of ray. Initialized as an arbitrarily large number
    double t = 100000000;
    
    double importance = 1.0;
    // Pointer to parent ray
    Ray *parent = nullptr;
    // Pointer to reflected ray
    Ray *R = nullptr;
    //Pointer to refracted ray
    Ray *T = nullptr;
   
    
    Ray(Vertex start_, Vertex end_);
};

#endif /* Ray_hpp */

