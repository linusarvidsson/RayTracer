//
//  Sphere.cpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-09-18.
//

#include "Sphere.hpp"

Sphere::Sphere()
{
    center = Vertex(0, 0, 0, 1);
    radius = 1;
    
    objectMaterial = DIFFUSE;
}

Sphere::Sphere(Vertex center_, double radius_, ColorDbl color_, Material material_)
{
    center = center_;
    radius = radius_;
    objectColor = color_;
    objectMaterial = material_;
}

bool Sphere::rayIntersection(Ray &ray)
{
    Vertex o = ray.start;
    Vector l = (ray.end - ray.start).normalize();
    double a = 1;
    double b = (l * 2).dot(o - center);
    double c = (o - center).dot(o - center) - radius * radius;
    
    double discriminant = (b*b/4) - a*c;
    if (discriminant > 0)
    {
        double d = - (b/2) - sqrt(discriminant);
        if (d > 0)
        {
            ray.intersection = ray.start + l * d;
            ray.normal = (ray.intersection - center) * (1 / radius);
            ray.intersection = ray.intersection + ray.normal * 0.0001;
            
            return true;
        }
    }
    
    return false;
    
//    if (discriminant < 0) {
//        return false;
//    }
    
    //double d = - (b/2) - sqrt(discriminant);
}
//
//bool Sphere::intersectionPoint(Ray &ray)
//{
//    Vertex o = ray.start;
//    Vector l = (ray.end - ray.start).normalize();
//    double a = 1;
//    double b = (l * 2).dot(o - center);
//    double c = (o - center).dot(o - center) - radius * radius;
//
//    double discriminant = (b*b/4) - a*c;
//    double d = - (b/2) - sqrt(discriminant);
//
//
//
//    return ;
//}


Material Sphere::material(){
    return objectMaterial;
}

ColorDbl Sphere::color(){
    return objectColor;
}
