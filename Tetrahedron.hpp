#ifndef Tetrahedron_hpp
#define Tetrahedron_hpp

#include "Triangle.hpp"
#include <stdio.h>

class Tetrahedron : public Object {
public:
    
    Tetrahedron(Vertex a, Vertex b, Vertex c, Vertex d, ColorDbl color, Material material);
    bool rayIntersection(Ray &ray);
    Vertex point(double u, double v) {return Vertex();}
    Vector getNormal() {return Vector();}
    Triangle intersectedTriangle(Ray arg);
    Material material();
    ColorDbl color();
    
private:
    Triangle triangles[4];
    Material objectMaterial;
    ColorDbl objectColor;
};

#endif /* Tetrahedron_hpp */
