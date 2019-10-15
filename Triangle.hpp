#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>

#include "Object.hpp"
#include "Vertex.hpp"
#include "ColorDbl.hpp"
#include "Ray.hpp"

class Ray;

class Triangle : public Object {
public:
    Vertex a;
    Vertex b;
    Vertex c;
    Vector normal;
    
    Triangle();
    Triangle(Vertex a_, Vertex b_, Vertex c_, ColorDbl color_, Material material_);
    bool rayIntersection(Ray &arg);
    
    Material material();
    ColorDbl color();
    
private:
    Material objectMaterial;
    ColorDbl objectColor;
};

#endif /* Triangle_hpp */
