//
//  Octree.cpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-11-06.
//

#include "Octree.hpp"

Octree::Octree(int n0, vector<shared_ptr<Photon>> photons, double a, double b, double c, double d, double e, double f)
{
    N0 = n0;
    root = new Node;
    root->setDimensions(a, b, c, d, e, f);
    for (int i = 0; i < photons.size(); i++) {
        root->addPhoton(photons[i]);
    }
    
    if (root->n > N0) root->split(N0);
}
