//
//  Photon.cpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-11-06.
//

#include "Photon.hpp"

Photon::Photon() {
    position = Vertex();
    direction = Vector();
    flux = ColorDbl();
}
