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
    ColorDbl color;
    
    Triangle();
    Triangle(Vertex a_, Vertex b_, Vertex c_, ColorDbl color_);
    bool rayIntersection(Ray &arg);
    //Vertex intersectionPoint(Ray &arg);
    
    Material material();
    
private:
    Material objectMaterial;
};

#endif /* Triangle_hpp */
