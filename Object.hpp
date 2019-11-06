#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include "Ray.hpp"

enum Material{
    LAMBERTIAN,
    OREN_NAYAR,
    REFLECTIVE,
    TRANSPARENT,
    EMISSIVE
};

class Object{
public:
    virtual ~Object() = 0;
    virtual bool rayIntersection(Ray &ray) = 0;
    virtual Material material() = 0;
    virtual ColorDbl color() = 0;
    virtual Vertex point(double u, double v) = 0;
    virtual Vector getNormal() = 0;
};

#endif /* Object_hpp */
