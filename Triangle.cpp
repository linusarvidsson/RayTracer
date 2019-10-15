#include "Triangle.hpp"
#include <iostream>

Triangle::Triangle()
{
    a = b = c = Vertex();
    normal = Vector();
    objectColor = ColorDbl();
    objectMaterial = DIFFUSE;
}

Triangle::Triangle(Vertex a_, Vertex b_, Vertex c_, ColorDbl color_, Material material_)
{
    a = Vertex(a_);
    b = Vertex(b_);
    c = Vertex(c_);
    normal = (b_ - a_).cross(c_ - a_).normalize();
    objectColor = ColorDbl(color_);
    
    objectMaterial = material_;
}

bool Triangle::rayIntersection(Ray &ray)
{
    Vector T = ray.start - a;
    Vector E1 = b - a;
    Vector E2 = c - a;
    Vector D = ray.end - ray.start;
    Vector P = D.cross(E2);
    Vector Q = T.cross(E1);
    
    double t = Q.dot(E2) / P.dot(E1);
    double u = P.dot(T) / P.dot(E1);
    double v = Q.dot(D) / P.dot(E1);
    
    if (t > ray.t) return false;
    
    if ((t > 0) && (u <= 1) && (u >= 0) && (v <= 1) && (v >= 0) && (u + v <= 1))
    {
        ray.t = t;
        ray.objectNormal = normal;
        ray.intersection = a + E1 * u + E2 * v + normal * 0.01;
        return true;
    }

    return false;
    
};

Material Triangle::material(){
    return objectMaterial;
}

ColorDbl Triangle::color(){
    return objectColor;
}
