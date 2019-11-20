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
    Node* children[8] = {nullptr};
    vector<shared_ptr<Photon>> photons;
    
    Node* findChild(double x, double y, double z) {
        double xmid = (xmin + xmax) / 2.0;
        double ymid = (ymin + ymax) / 2.0;
        double zmid = (zmin + zmax) / 2.0;
        
        if (children[0] != nullptr) {
            int index = 0;
            if (x > xmid) index += 4;
            if (y > ymid) index += 2;
            if (z > zmid) index += 1;
            return children[index]->findChild(x,y,z);
        }
        
        return this;
    }
    
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
        
        for (int i = 0; i < 8; i++) {
            if (children[i]->n > N0) children[i]->split(N0);
        }
    }
};

class Octree {
public:
    int N0 = 1000;
    Node* root;
    
    Octree();
    Octree(int n0, vector<shared_ptr<Photon>> photons, double a, double b, double c, double d, double e, double f);
    
    bool findPhotons(double radius, Vertex intersection, vector<shared_ptr<Photon>> &photonsFound){
        
        Node* node = this->root->findChild(intersection.x, intersection.y, intersection.z);
        double radiusSquared = radius*radius;
        
        int intersections = 0;
        int intersectedBox = -1;
        
        // Check if search radius is inside the box
        if( abs(node->xmin - intersection.x) < radius) {
            intersections++;
            intersectedBox = 0;
        }
        if( abs(node->xmax - intersection.x) < radius) {
            intersections++;
            intersectedBox = 1;
        }
        if( abs(node->ymin - intersection.y) < radius) {
            intersections++;
            intersectedBox = 2;
        }
        if( abs(node->ymax - intersection.y) < radius) {
            intersections++;
            intersectedBox = 3;
        }
        if( abs(node->zmin - intersection.z) < radius) {
            intersections++;
            intersectedBox = 4;
        }
        if( abs(node->zmax - intersection.z) < radius) {
            intersections++;
            intersectedBox = 5;
        }
        
        // Return false if intersection is in the corner of the box
        if (intersections > 1) return false;
        
        if (intersections == 1){
            Node* neighborNode = nullptr;
            
            if(intersectedBox == 0) neighborNode = this->root->findChild(intersection.x - radius, intersection.y, intersection.z);
            if(intersectedBox == 1) neighborNode = this->root->findChild(intersection.x + radius, intersection.y, intersection.z);
            if(intersectedBox == 2) neighborNode = this->root->findChild(intersection.x, intersection.y - radius, intersection.z);
            if(intersectedBox == 3) neighborNode = this->root->findChild(intersection.x, intersection.y + radius, intersection.z);
            if(intersectedBox == 4) neighborNode = this->root->findChild(intersection.x, intersection.y, intersection.z - radius);
            if(intersectedBox == 5) neighborNode = this->root->findChild(intersection.x, intersection.y, intersection.z + radius);
            
            if(neighborNode != nullptr){
                for (int i = 0; i < neighborNode->n; i++){
                    
                    Vertex photonPosition = neighborNode->photons[i]->position;
                    double distance = (photonPosition - intersection).magnitudeSquared();
                    
                    if (distance < radiusSquared) {
                        //cout << "Add photon\n";
                        photonsFound.push_back(neighborNode->photons[i]);
                    }
                    
                }
            }
        }
        
        for (int i = 0; i < node->n; i++) {
            Vertex photonPosition = node->photons[i]->position;
            double distance = (photonPosition - intersection).magnitudeSquared();
            
            if (distance < radiusSquared) {
                //cout << "Add photon\n";
                photonsFound.push_back(node->photons[i]);
            }
        }
        
        return true;
    }
    
private:
    
};

#endif /* Octree_hpp */
