#include "Triangle.hpp"
#include <iostream>

Triangle::Triangle()
{
    a = b = c = Vertex();
    normal = Vector();
    color = ColorDbl();
    objectMaterial = DIFFUSE;
}

Triangle::Triangle(Vertex a_, Vertex b_, Vertex c_, ColorDbl color_)
{
    a = Vertex(a_);
    b = Vertex(b_);
    c = Vertex(c_);
    normal = (b_ - a_).cross(c_ - a_).normalize();
    color = ColorDbl(color_);
    
    objectMaterial = DIFFUSE;
}

bool Triangle::rayIntersection(Ray &arg)
{
    Vector T = arg.start - a;
    Vector E1 = b - a;
    Vector E2 = c - a;
    Vector D = arg.end - arg.start;
    Vector P = D.cross(E2);
    Vector Q = T.cross(E1);
    
    double t = Q.dot(E2) / P.dot(E1);
    double u = P.dot(T) / P.dot(E1);
    double v = Q.dot(D) / P.dot(E1);
    
    
    if ((t > 0) && (u <= 1) && (u >= 0) && (v <= 1) && (v >= 0) && (u + v <= 1) && (t < arg.t))
    {
        arg.color = color;
        arg.normal = normal;
        arg.intersection = a + E1 * u + E2 * v + normal * 0.01;
        return true;
    }

    return false;
    
};

Material Triangle::material(){
    return objectMaterial;
}

//Vertex Triangle::intersectionPoint(Ray arg)
//{
//    Vector T = arg.start - a;
//    Vector E1 = b - a;
//    Vector E2 = c - a;
//    Vector D = arg.end - arg.start;
//    Vector P = D.cross(E2);
//    Vector Q = T.cross(E1);
//
//    double u = P.dot(T) / P.dot(E1);
//    double v = Q.dot(D) / P.dot(E1);
//
//    return
//}
