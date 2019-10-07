//
//  Sphere.hpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-09-18.
//

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
    ColorDbl color;
    
    Sphere();
    Sphere(Vertex center_, double radius_, ColorDbl color_);
    bool rayIntersection(Ray &arg);
    //Vertex intersectionPoint(Ray &arg);
    Material material();
    
private:
    double radius;
    Material objectMaterial;
};

#endif /* Sphere_hpp */
