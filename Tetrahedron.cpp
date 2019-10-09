//
//  Tetrahedron.cpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-09-17.
//

#include "Tetrahedron.hpp"
#include <iostream>

Tetrahedron::Tetrahedron(Vertex a, Vertex b, Vertex c, Vertex d, ColorDbl color, Material material){
    objectColor = color;
    objectMaterial = material;
    
    triangles[0] = Triangle(a, b, c, objectColor, objectMaterial);
    triangles[1] = Triangle(a, c, d, objectColor, objectMaterial);
    triangles[2] = Triangle(b, d, c, objectColor, objectMaterial);
    triangles[3] = Triangle(a, d, b, objectColor, objectMaterial);
    
}

bool Tetrahedron::rayIntersection(Ray &ray){
    // Loop through the tetrahedrons triangles and check if the ray intersects with any of them
    
    bool found = false;
    
    for(int i = 0; i < 4; i++){
        found = triangles[i].rayIntersection(ray) ? true : found;
    }
    
    return found;
}

Material Tetrahedron::material(){
    return objectMaterial;
}

ColorDbl Tetrahedron::color(){
    return objectColor;
}

Triangle Tetrahedron::intersectedTriangle(Ray ray){
    // Arbitrary large number for the comparison where we want to find the intersected triangle with the smallest t
    double min_t = 100000;
    Triangle intersectedTriangle;
    
    for (int i = 0; i < 4; i++)
    {
        Vector T = ray.start - triangles[i].a;
        Vector E1 = triangles[i].b - triangles[i].a;
        Vector E2 = triangles[i].c - triangles[i].a;
        Vector D = ray.end - ray.start;
        Vector P = D.cross(E2);
        Vector Q = T.cross(E1);
        
        double t = Q.dot(E2) / P.dot(E1);
        
        // The triangle with the smallest t scalar is the triangle closest to the ray source
        if (t < min_t)
        {
            min_t = t;
            intersectedTriangle = triangles[i];
        }
    }
    
    return intersectedTriangle;
}



