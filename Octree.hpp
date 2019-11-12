//
//  Octree.hpp
//  RayTracer
//
//  Created by Ludvig Kratzert on 2019-11-06.
//

#ifndef Octree_hpp
#define Octree_hpp

#include <iostream>
#include <stdio.h>
#include "Photon.hpp"
#include <vector>

using namespace std;

struct Node {
    double xmin, xmax, ymin, ymax, zmin, zmax;
    int n = 0;
    Node* parent;
    Node* children[8];
    vector<shared_ptr<Photon>> photons;
    
    void setDimensions(double a, double b, double c, double d, double e, double f) {
        xmin = a; xmax = b;
        ymin = c; ymax = d;
        zmin = e; zmax = f;
    }
    
    void addPhoton(shared_ptr<Photon> photon) {
        photons.push_back(photon);
        n++;
    }
    
    // Will recursively split all children
    void split(int N0) {
        for (int i = 0; i < 8; i++) {
            children[i] = new Node;
            children[i]->parent = this;
        }
        double xmid = (xmin + xmax) / 2.0;
        double ymid = (ymin + ymax) / 2.0;
        double zmid = (zmin + zmax) / 2.0;
        
        children[0]->setDimensions(xmin, xmid, ymin, ymid, zmin, zmid);
        children[1]->setDimensions(xmin, xmid, ymin, ymid, zmid, zmax);
        children[2]->setDimensions(xmin, xmid, ymid, ymax, zmin, zmid);
        children[3]->setDimensions(xmin, xmid, ymid, ymax, zmid, zmax);
        children[4]->setDimensions(xmid, xmax, ymin, ymid, zmin, zmid);
        children[5]->setDimensions(xmid, xmax, ymin, ymid, zmid, zmax);
        children[6]->setDimensions(xmid, xmax, ymid, ymax, zmin, zmid);
        children[7]->setDimensions(xmid, xmax, ymid, ymax, zmid, zmax);
        
        //hög x? + 4
        //hög y? + 2
        //hög z? + 1
        
        for (int i = 0; i < n; i++) {
            int index = 0;
            if (photons[i]->position.x > xmid) index += 4;
            if (photons[i]->position.y > ymid) index += 2;
            if (photons[i]->position.z > zmid) index += 1;
            children[index]->addPhoton(photons[i]);
        }
        
        photons.clear();
        n = 0;
        /*
        for (int i = 0; i < 8; i++) {
            if (children[i]->n > N0) children[i]->split(N0);
        }*/
        if (children[0]->n > N0) children[0]->split(N0);
    }
};

class Octree {
public:
    int N0 = 1000;
    Node* root;
    
    Octree(int n0, vector<shared_ptr<Photon>> photons, double a, double b, double c, double d, double e, double f);
    
private:
    
};

#endif /* Octree_hpp */
