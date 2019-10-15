#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include <cmath>

#include "Object.hpp"
#include "Ray.hpp"
#include "ColorDbl.hpp"

class Vertex;

class Sphere : public Object {
public:
    Vertex center;
    
    Sphere();
    Sphere(Vertex center_, double radius_, ColorDbl color_, Material material_);
    bool rayIntersection(Ray &arg);
    Material material();
    ColorDbl color();
    
private:
    double radius;
    Material objectMaterial;
    ColorDbl objectColor;
};

#endif /* Sphere_hpp */
