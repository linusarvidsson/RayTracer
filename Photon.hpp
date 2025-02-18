//
//  Photon.hpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-11-06.
//

#ifndef Photon_hpp
#define Photon_hpp

#include <stdio.h>
#include "ColorDbl.hpp"
#include "Vector.hpp"
#include "Vertex.hpp"

class Photon {
public:
    //Pointing towards position
    Vector direction;
    Vertex position;
    ColorDbl flux;
    bool isShadow = false;
    
    Photon();
    Photon(Vector dir, Vector pos, double f)
    : direction(dir), position(pos), flux(f) {}
    
private:
    
};

#endif /* Photon_hpp */
