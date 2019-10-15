#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include "Ray.hpp"

enum Material{
    DIFFUSE,
    REFLECTIVE,
    TRANSPARENT
};

class Object{
public:
    virtual ~Object() = 0;
    virtual bool rayIntersection(Ray &ray) = 0;
    virtual Material material() = 0;
    virtual ColorDbl color() = 0;
};

#endif /* Object_hpp */
